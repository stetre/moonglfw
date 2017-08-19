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


int checkoption_hint(lua_State *L, int arg, const char *def, const char *const lst[])
/* Variant of luaL_checkoption(), with an added hint in the error message */
    {
    const char *hint = NULL;
    const char *name = (def) ? luaL_optstring(L, arg, def) : luaL_checkstring(L, arg);
    int i;
    for (i=0; lst[i]; i++)
        if (strcmp(lst[i], name) == 0)  return i;

    if(lua_checkstack(L, i*2))
        {
        for(i=0; lst[i]; i++)
            {
            lua_pushfstring(L, "'%s'", lst[i]);
            lua_pushstring(L, "|");
            }
        i = i*2;
        if(i>0)
            {
            lua_pop(L, 1); /* the last separator */
            lua_concat(L, i-1);
            hint = lua_tostring(L, -1); 
            }
        }
    if(hint)
        return luaL_argerror(L, arg, lua_pushfstring(L, 
                    "invalid option '%s', valid options are: %s", name, hint));
    return luaL_argerror(L, arg, lua_pushfstring(L, "invalid option '%s'", name));
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
 | Inits                                                                        |
 *------------------------------------------------------------------------------*/

void moonglfw_utils_init(lua_State *L)
    {
    time_init(L);
    }

