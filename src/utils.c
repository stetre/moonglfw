/* The MIT License (MIT)
 *
 * Copyright (c) 2016 Stefano Trettel
 *
 * Software repository: MoonGLFW, https://github.com/stetre/moonglfw
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

int noprintf(const char *fmt, ...) 
    { (void)fmt; return 0; }


int notavailable(lua_State *L, ...) 
    { NOT_AVAILABLE; }

/*------------------------------------------------------------------------------*
 | Malloc                                                                       |
 *------------------------------------------------------------------------------*/

/* We do not use malloc(), free() etc directly. Instead, we inherit the memory
 * allocator from the main Lua state instead (see lua_getallocf in the Lua manual)
 * and use that.
 *
 * By doing so, we can use an alternative malloc() implementation without recompiling
 * this library (we have needs to recompile lua only, or execute it with LD_PRELOAD
 * set to the path to the malloc library we want to use).
 */
static lua_Alloc Alloc = NULL;
static void* AllocUd = NULL;

static void malloc_init(lua_State *L)
    {
    if(Alloc) unexpected(L);
    Alloc = lua_getallocf(L, &AllocUd);
    }

static void* Malloc_(size_t size)
    { return Alloc ? Alloc(AllocUd, NULL, 0, size) : NULL; }

static void Free_(void *ptr)
    { if(Alloc) Alloc(AllocUd, ptr, 0, 0); }

void *Malloc(lua_State *L, size_t size)
    {
    void *ptr;
    if(size == 0)
        { luaL_error(L, errstring(ERR_MALLOC_ZERO)); return NULL; }
    ptr = Malloc_(size);
    if(ptr==NULL)
        { luaL_error(L, errstring(ERR_MEMORY)); return NULL; }
    memset(ptr, 0, size);
    //DBG("Malloc %p\n", ptr);
    return ptr;
    }

void *MallocNoErr(lua_State *L, size_t size) /* do not raise errors (check the retval) */
    {
    void *ptr = Malloc_(size);
    (void)L;
    if(ptr==NULL)
        return NULL;
    memset(ptr, 0, size);
    //DBG("MallocNoErr %p\n", ptr);
    return ptr;
    }

char *Strdup(lua_State *L, const char *s)
    {
    size_t len = strnlen(s, 256);
    char *ptr = (char*)Malloc(L, len + 1);
    if(len>0)
        memcpy(ptr, s, len);
    ptr[len]='\0';
    return ptr;
    }


void Free(lua_State *L, void *ptr)
    {
    (void)L;
    //DBG("Free %p\n", ptr);
    if(ptr) Free_(ptr);
    }


/*------------------------------------------------------------------------------*
 | Custom luaL_checkxxx() style functions                                       |
 *------------------------------------------------------------------------------*/

void *checklightuserdata(lua_State *L, int arg)
    {
    if(lua_type(L, arg) != LUA_TLIGHTUSERDATA)
        { luaL_argerror(L, arg, "expected lightuserdata"); return NULL; }
    return lua_touserdata(L, arg);
    }
    
void *optlightuserdata(lua_State *L, int arg)
    {
    if(lua_isnoneornil(L, arg))
        return NULL;
    return checklightuserdata(L, arg);
    }

GLboolean checkboolean(lua_State *L, int arg)
    {
    if(!lua_isboolean(L, arg))
        return (GLboolean)luaL_argerror(L, arg, "boolean expected");
    return lua_toboolean(L, arg) ? GL_TRUE : GL_FALSE;
    }

int testwindow(lua_State *L, int arg, win_t **win_)
    {
    int id;
    win_t *win;
    if(lua_isnoneornil(L, arg))
        return 0;
    id = luaL_checkinteger(L, arg);
    win = win_search(id);
    if(!win)
        luaL_error(L, "invalid window id");
    if(win_) *win_ = win;
    return 1;
    }

win_t *checkwindow(lua_State *L, int arg)
    {
    int id = luaL_checkinteger(L, arg);
    win_t *win = win_search(id);
    if(!win)
        luaL_error(L, "invalid window id");
    return win;
    }

int testmonitor(lua_State *L, int arg, mon_t **mon_)
    {
    int id;
    mon_t *mon;
    if(lua_isnoneornil(L, arg))
        return 0;
    id = luaL_checkinteger(L, arg);
    mon = mon_search(id);
    if(!mon)
        luaL_error(L, "invalid monitor id");
    if(mon_) *mon_ = mon;
    return 1;
    }


mon_t *checkmonitor(lua_State *L, int arg)
    {
    int id = luaL_checkinteger(L, arg);
    mon_t *mon = mon_search(id);
    if(!mon)
        luaL_error(L, "invalid monitor id");
    return mon;
    }


int testcursor(lua_State *L, int arg, cur_t **cur_)
    {
    int id;
    cur_t *cur;
    if(lua_isnoneornil(L, arg))
        return 0;
    id = luaL_checkinteger(L, arg);
    cur = cur_search(id);
    if(!cur)
        luaL_error(L, "invalid cursor id");
    if(cur_) *cur_ = cur;
    return 1;
    }


cur_t *checkcursor(lua_State *L, int arg)
    {
    int id = luaL_checkinteger(L, arg);
    cur_t *cur = cur_search(id);
    if(!cur)
        luaL_error(L, "invalid cursor id");
    return cur;
    }

int pushgamepadbuttons(lua_State *L, const GLFWgamepadstate *state)
    {
    lua_newtable(L);
#define Setfield(x, name) do {  \
    lua_pushstring(L, state->buttons[GLFW_GAMEPAD_BUTTON_##x]==GLFW_PRESS ? "press" : "release"); \
    lua_setfield(L, -2, name);  \
} while(0)
    Setfield(A, "a");
    Setfield(A, "a");
    Setfield(B, "b");
    Setfield(X, "x");
    Setfield(Y, "y");
    Setfield(LEFT_BUMPER, "left_bumper");
    Setfield(RIGHT_BUMPER, "right_bumper");
    Setfield(BACK, "back");
    Setfield(START, "start");
    Setfield(GUIDE, "guide");
    Setfield(LEFT_THUMB, "left_thumb");
    Setfield(RIGHT_THUMB, "right_thumb");
    Setfield(DPAD_UP, "dpad_up");
    Setfield(DPAD_RIGHT, "dpad_right");
    Setfield(DPAD_DOWN, "dpad_down");
    Setfield(DPAD_LEFT, "dpad_left");
#undef Setfield
    return 1;
    }

int pushgamepadaxes(lua_State *L, const GLFWgamepadstate *state)
    {
    lua_newtable(L);
#define Setfield(x, name) do {  \
    lua_pushnumber(L, state->axes[GLFW_GAMEPAD_AXIS_##x]); \
    lua_setfield(L, -2, name);  \
} while(0)
    Setfield(LEFT_X, "left_x");
    Setfield(LEFT_Y, "left_y");
    Setfield(RIGHT_X, "right_x");
    Setfield(RIGHT_Y, "right_y");
    Setfield(LEFT_TRIGGER, "left_trigger");
    Setfield(RIGHT_TRIGGER, "right_trigger");
#undef Setfield
    return 1;
    }

int pushvidmode(lua_State *L, const GLFWvidmode *mode)
    {
    int table;
    lua_newtable(L);
    table = lua_gettop(L);
#define Setfield(x, name) \
        do { lua_pushinteger(L, mode->x); lua_setfield(L, table, name); } while(0)
    Setfield(width, "width");
    Setfield(height, "height");
    Setfield(redBits,"red_bits");
    Setfield(greenBits,"green_bits");
    Setfield(blueBits,"blue_bits");
    Setfield(refreshRate,"refresh_rate");
#undef Setfield
    return 1;
    }

int pushgammaramp(lua_State *L, const GLFWgammaramp *ramp)
    {
    unsigned int i;
    int table;
#define Setfield(x, name)   do {            \
        lua_newtable(L);                    \
        table = lua_gettop(L);              \
        for(i=0; i<ramp->size; i++)         \
            {                               \
            lua_pushinteger(L, ramp->x[i]); \
            lua_seti(L, table, i+1);        \
            }                               \
} while(0)
    Setfield(red,"red");
    Setfield(green,"green");
    Setfield(blue,"blue");
#undef Setfield
    return 3;
    }

int checkgammaramp(lua_State *L, int arg, GLFWgammaramp *ramp)
    {
    unsigned short i;
#define R arg
#define G arg+1
#define B arg+2
    if(!lua_istable(L,R))
        return luaL_argerror(L, R, "table expected");
    if(!lua_istable(L,G))
        return luaL_argerror(L, G, "table expected");
    if(!lua_istable(L,B))
        return luaL_argerror(L, B, "table expected");
    ramp->size = luaL_len(L, R);
    if(ramp->size == 0)
        return luaL_argerror(L, R, "empty array");
    if( (luaL_len(L, G) != ramp->size) || (luaL_len(L, B) != ramp->size))
        return luaL_argerror(L, R, "red, green and blue arrays do not have the same length");
    ramp->red = (unsigned short*)malloc(ramp->size * sizeof(unsigned short));
    ramp->green = (unsigned short*)malloc(ramp->size * sizeof(unsigned short));
    ramp->blue = (unsigned short*)malloc(ramp->size * sizeof(unsigned short));
    for(i=0; i < ramp->size; i++)
        {
        lua_geti(L, R, i+1); ramp->red[i] = luaL_checkinteger(L, -1); lua_pop(L, 1);
        lua_geti(L, G, i+1); ramp->green[i] = luaL_checkinteger(L, -1); lua_pop(L, 1);
        lua_geti(L, B, i+1); ramp->blue[i] = luaL_checkinteger(L, -1); lua_pop(L, 1);
        }
    return 0;
#undef R
#undef G
#undef B
    }



/*------------------------------------------------------------------------------*
 | Time utilities                                                               |
 *------------------------------------------------------------------------------*/

#if defined(LINUX)
#include <unistd.h> /* for usleep */

#if 0
static double tstosec(const struct timespec *ts)
    {
    return ts->tv_sec*1.0+ts->tv_nsec*1.0e-9;
    }
#endif

static void sectots(struct timespec *ts, double seconds)
    {
    ts->tv_sec=(time_t)seconds;
    ts->tv_nsec=(long)((seconds-((double)ts->tv_sec))*1.0e9);
    }

double now(void)
    {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC,&ts)!=0)
        { printf("clock_gettime error\n"); return -1; }
    return ts.tv_sec + ts.tv_nsec*1.0e-9;
#else
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
        { printf("gettimeofday error\n"); return -1; }
    return tv.tv_sec + tv.tv_usec*1.0e-6;
#endif
    }

void sleeep(double seconds)
    {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts, ts1;
    struct timespec *req, *rem, *tmp;
    sectots(&ts, seconds);
    req = &ts;
    rem = &ts1;
    while(1)
        {
        if(nanosleep(req, rem) == 0)
            return;
        tmp = req;
        req = rem;
        rem = tmp;
        }
#else
    usleep((useconds_t)(seconds*1.0e6));
#endif
    }

#define time_init(L) do { (void)L; /* do nothing */ } while(0)

#elif defined(MINGW)

#include <windows.h>

static LARGE_INTEGER Frequency;
double now(void)
    {
    LARGE_INTEGER ts;
    QueryPerformanceCounter(&ts);
    return ((double)(ts.QuadPart))/Frequency.QuadPart;
    }

void sleeep(double seconds)
    {
    DWORD msec = (DWORD)seconds * 1000;
    //if(msec < 0) return;  DWORD seems to be unsigned
    Sleep(msec);
    }

static void time_init(lua_State *L)
    {
    (void)L;
    QueryPerformanceFrequency(&Frequency);
    }

#endif

/*------------------------------------------------------------------------------*
 | Internal error codes                                                         |
 *------------------------------------------------------------------------------*/

const char* errstring(int err)
    {
    switch(err)
        {
        case 0: return "success";
        case ERR_GENERIC: return "generic error";
        case ERR_TABLE: return "not a table";
        case ERR_EMPTY: return "empty list";
        case ERR_TYPE: return "invalid type";
        case ERR_VALUE: return "invalid value";
        case ERR_NOTPRESENT: return "missing";
        case ERR_MEMORY: return "out of memory";
        case ERR_MALLOC_ZERO: return "zero bytes malloc";
        case ERR_LENGTH: return "invalid length";
        case ERR_POOL: return "elements are not from the same pool";
        case ERR_BOUNDARIES: return "invalid boundaries";
        case ERR_UNKNOWN: return "unknown field name";
        default:
            return "???";
        }
    return NULL; /* unreachable */
    }
/*------------------------------------------------------------------------------*
 | Inits                                                                        |
 *------------------------------------------------------------------------------*/

void moonglfw_utils_init(lua_State *L)
    {
    malloc_init(L);
    time_init(L);
    }

