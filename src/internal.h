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

/********************************************************************************
 * internal common header                                                       *
 ********************************************************************************/

#ifndef internalDEFINED
#define internalDEFINED

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "moonglfw.h"
#include "tree.h"

/* Note: all the dynamic symbols of this library (should) start with 'moonglfw_' .
 * The only exception is the luaopen_moonglfw() function, which is searched for
 * with that name by Lua.
 * MoonGLFW's string references on the Lua registry also start with 'moonglfw_'.
 */

#include "structs.h"
#include "enum.h"

#define TOSTR_(x) #x
#define TOSTR(x) TOSTR_(x)

#include "getproc.h"

extern lua_State *moonglfw_L; /* the global Lua state */

#if 0
/* .c */
#define  moonglfw_
#endif

/* utils.c */
#define noprintf moonglfw_noprintf
int noprintf(const char *fmt, ...);
#define notavailable moonglfw_notavailable
int notavailable(lua_State *L, ...);
#define now moonglfw_now
double now(void);
#define since(t) (now() - (t))
#define sleeep moonglfw_sleeep
void sleeep(double seconds);
#define checklightuserdata moonglfw_checklightuserdata
void *checklightuserdata(lua_State *L, int arg);
#define optlightuserdata moonglfw_optlightuserdata
void *optlightuserdata(lua_State *L, int arg);
#define checkoption_hint moonglfw_checkoption_hint 
int checkoption_hint(lua_State *L, int arg, const char *def, const char *const lst[]);
#define checkboolean moonglfw_checkboolean
GLboolean checkboolean(lua_State *L, int arg);
#define testwindow moonglfw_testwindow
int testwindow(lua_State *L, int arg, win_t **win_);
#define checkwindow moonglfw_checkwindow
win_t *checkwindow(lua_State *L, int arg);
#define testmonitor moonglfw_testmonitor
int testmonitor(lua_State *L, int arg, mon_t **mon_);
#define checkmonitor moonglfw_checkmonitor
mon_t *checkmonitor(lua_State *L, int arg);
#define testcursor moonglfw_testcursor
int testcursor(lua_State *L, int arg, cur_t **cur_);
#define checkcursor moonglfw_checkcursor
cur_t *checkcursor(lua_State *L, int arg);
#define pushvidmode moonglfw_pushvidmode
int pushvidmode(lua_State *L, const GLFWvidmode *mode);
#define pushgammaramp moonglfw_pushgammaramp
int pushgammaramp(lua_State *L, const GLFWgammaramp *ramp);
#define checkgammaramp moonglfw_checkgammaramp
int checkgammaramp(lua_State *L, int arg, GLFWgammaramp *ramp);

/* id.c */
#define id_new moonglfw_id_new
int id_new(void);
#define id_free moonglfw_id_free
void id_free(int key);
#define id_free_all moonglfw_id_free_all
void id_free_all(void);

/* mon.c */
#define mon_search moonglfw_mon_search
mon_t *mon_search(int id);
#define mon_first moonglfw_mon_first
mon_t *mon_first(int id);
#define mon_next moonglfw_mon_next
mon_t *mon_next(mon_t *mon);
#define mon_new moonglfw_mon_new
mon_t *mon_new(void);
#define mon_free moonglfw_mon_free
void mon_free(mon_t* mon);
#define mon_free_all moonglfw_mon_free_all
void mon_free_all(void);
#define mon_check moonglfw_mon_check
mon_t* mon_check(lua_State *L, int arg);


/* win.c */
#define win_search moonglfw_win_search
win_t *win_search(int id);
#define win_first moonglfw_win_first
win_t *win_first(int id);
#define win_next moonglfw_win_next
win_t *win_next(win_t *win);
#define win_new moonglfw_win_new
win_t *win_new(void);
#define win_free moonglfw_win_free
void win_free(win_t* win);
#define win_free_all moonglfw_win_free_all
void win_free_all(void);
#define win_check moonglfw_win_check
win_t* win_check(lua_State *L, int arg);

/* cur.c */
#define cur_search moonglfw_cur_search
cur_t *cur_search(int id);
#define cur_first moonglfw_cur_first
cur_t *cur_first(int id);
#define cur_next moonglfw_cur_next
cur_t *cur_next(cur_t *cur);
#define cur_new moonglfw_cur_new
cur_t *cur_new(void);
#define cur_free moonglfw_cur_free
void cur_free(cur_t* cur);
#define cur_free_all moonglfw_cur_free_all
void cur_free_all(void);
#define cur_check moonglfw_cur_check
cur_t* cur_check(lua_State *L, int arg);


/* monitor.c */
#define monitorAdd moonglfw_monitorAdd
int monitorAdd(lua_State *L, GLFWmonitor *monitor);
#define monitorCallback moonglfw_monitorCallback
void monitorCallback(GLFWmonitor *monitor, int event);


/* main.c */
#define checkminversion moonglfw_checkminversion
int checkminversion(int major, int minor, int rev);
#define errorCallback moonglfw_errorCallback
void errorCallback(int ec, const char *descr);
int luaopen_moonglfw(lua_State *L);
void moonglfw_utils_init(lua_State *L);
int moonglfw_open_getproc(lua_State *L);
void moonglfw_atexit_getproc(void);
void moonglfw_open_window(lua_State *L);
void moonglfw_open_hint(lua_State *L);
void moonglfw_open_monitor(lua_State *L);
void moonglfw_open_callbacks(lua_State *L);
void moonglfw_open_input(lua_State *L);
void moonglfw_open_context(lua_State *L);
void moonglfw_open_vulkan(lua_State *L);
void moonglfw_open_native(lua_State *L);

#define CheckJoystick(L, arg) luaL_checkinteger((L), (arg)) - 1 /* 1,2, ..., 16 */
#define PushJoystick(L, joy) lua_pushinteger((L), (joy) + 1)

/*------------------------------------------------------------------------------*
 | Debug and other utilities                                                    |
 *------------------------------------------------------------------------------*/

/* If this is printed, it denotes a suspect bug: */
#define UNEXPECTED_ERROR "unexpected error (%s, %d)", __FILE__, __LINE__
#define unexpected(L) luaL_error((L), UNEXPECTED_ERROR)
#define NOT_AVAILABLE do { return luaL_error(L, "function not available"); } while(0)

#define NOT_IMPLEMENTED(func)               \
static int func(lua_State *L)               \
    {                                       \
    luaL_error(L, "function "#func" is not implemented");   \
    return 0;                           \
    }

#define NOT_SUPPORTED(func)                 \
static int func(lua_State *L)               \
    {                                       \
    luaL_error(L, "function "#func" is not supported"); \
    return 0;                           \
    }

#if defined(DEBUG)

#define checkoption checkoption_hint
#define DBG printf

#define TR() do {                                           \
    printf("trace %s %d\n",__FILE__,__LINE__);              \
} while(0)

#define BK() do {                                           \
    printf("break %s %d\n",__FILE__,__LINE__);              \
    getchar();                                              \
} while(0)

#else 

#define checkoption luaL_checkoption
#define DBG noprintf
#define TR()
#define BK()

#endif /* DEBUG */

/*------------------------------------------------------------------------------*
 | Code generators                                                              |
 *------------------------------------------------------------------------------*/

#define CALLBACK_UNREF(cb) do { \
        if((cb) != LUA_NOREF)   { \
            luaL_unref(moonglfw_L, LUA_REGISTRYINDEX, (cb));    \
            (cb) = LUA_NOREF;   } \
} while(0)

#define VOID_FUNC(func) \
    static int func(lua_State *L) { (void)L; glfw##func(); return 0; }


#endif /* internalDEFINED */
