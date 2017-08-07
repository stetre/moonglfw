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

/****************************************************************************
 * Callback functions                                                       *
 ****************************************************************************/
#include "internal.h"

static int PushMods(lua_State *L, int mods)
    {
    lua_pushboolean(L, mods & GLFW_MOD_SHIFT);
    lua_pushboolean(L, mods & GLFW_MOD_CONTROL);
    lua_pushboolean(L, mods & GLFW_MOD_ALT);
    lua_pushboolean(L, mods & GLFW_MOD_SUPER);
    return 4;
    }

static int MonitorId(GLFWmonitor *monitor)
    {
    mon_t *mon = mon_first(0);
    while(mon)
        {
        if(mon->monitor == monitor)
            return mon->id;
        mon = mon_next(mon);
        }
    return -1; /* should not happen */
    }


/*--------------------------------------------------------------------------*
 | Global callbacks execution                                               |
 *--------------------------------------------------------------------------*/

#define L moonglfw_L

#define BEGIN(cb)  /* retrieve the callback */                          \
    int top = lua_gettop(L);                                            \
do {                                                                    \
    if(lua_rawgeti(L, LUA_REGISTRYINDEX, cb) != LUA_TFUNCTION)          \
        { luaL_error(L, UNEXPECTED_ERROR); return; }                    \
} while(0)

#define EXEC(nargs)  do {                                               \
    if(lua_pcall(L, (nargs), 0, 0) != LUA_OK) { lua_error(L); return; } \
    lua_settop(L, top);                                                 \
} while(0)

static int Error = LUA_NOREF;
static void ErrorCallback(int ec, const char* descr)
    {
    BEGIN(Error);
    lua_pushinteger(L, ec); /* @@ farne un enum? */ 
    lua_pushstring(L, descr);
    EXEC(2);
    }

//typedef void (* GLFWmonitorfun)(GLFWmonitor*,int);
//GLFWmonitorfun glfw.SetMonitorCallback(GLFWmonitorfun cbfun);
static int Monitor = LUA_NOREF;
static void MonitorCallback(GLFWmonitor *monitor, int event)
    {
    BEGIN(Monitor);
    lua_pushinteger(L, MonitorId(monitor));
    lua_pushstring(L, event==GLFW_CONNECTED ? "connected" : "disconnected");
    EXEC(2);
    /* always call the default callback: */
    monitorCallback(monitor, event);
    }


static void joystickCallback(int joy, int event)
    {
    (void)joy;
    (void)event;
    }

//typedef void (* GLFWjoystickfun)(int joy, int event);
//GLFWjoystickfun glfw.SetJoystickCallback(GLFWjoystickfun cbfun);
static int Joystick = LUA_NOREF;
static void JoystickCallback(int joy, int event)
    {
    BEGIN(Joystick);
    PushJoystick(L, joy);
    lua_pushstring(L, event==GLFW_CONNECTED ? "connected" : "disconnected");
    EXEC(2);
   /* always call the default callback: */
    joystickCallback(joy, event);
    }


#undef BEGIN
#undef EXEC
#undef L

/*--------------------------------------------------------------------------*
 | Global callbacks registration                                            |
 *--------------------------------------------------------------------------*/

#define REGISTER_FUNC(cb, func_index, defaultcb, major, minor, rev) \
static int Set##cb##Callback(lua_State *L)                          \
    {                                                               \
    CheckPfn(L, Set##cb##Callback, major, minor, rev);              \
    if(cb != LUA_NOREF)                                             \
        { luaL_unref(L, LUA_REGISTRYINDEX, cb); cb = LUA_NOREF; }   \
    if(lua_isnoneornil(L, func_index))                              \
        { glfw.Set##cb##Callback(defaultcb); return 0; }            \
    if(!lua_isfunction(L, func_index))                              \
        return luaL_argerror(L, func_index, "function expected");   \
    lua_pushvalue(L, func_index);                                   \
    cb = luaL_ref(L, LUA_REGISTRYINDEX);                            \
    glfw.Set##cb##Callback(cb##Callback);                           \
    return 0;                                                       \
    }

REGISTER_FUNC(Error, 1, errorCallback, 3, 1, 0)
REGISTER_FUNC(Monitor, 1, monitorCallback, 3, 1, 0)
REGISTER_FUNC(Joystick, 1, joystickCallback, 3, 2, 0)

#undef REGISTER_FUNC

/*--------------------------------------------------------------------------*
 | Window-specific callbacks execution                                      |
 *--------------------------------------------------------------------------*/

#define L moonglfw_L

#define BEGIN(cb)  /* retrieve the callback */                      \
    int top = lua_gettop(L);                                        \
do {                                                                \
    int id = (intptr_t)glfw.GetWindowUserPointer(window);            \
    win_t *win = win_search(id);                                    \
    if((!win) || (lua_rawgeti(L, LUA_REGISTRYINDEX, win->cb) != LUA_TFUNCTION)) \
        { unexpected(L); return; }                                  \
    lua_pushinteger(L, id);                                         \
} while(0)

#define EXEC(nargs)  do {                                               \
    if(lua_pcall(L, (nargs) + 1, 0, 0) != LUA_OK) { lua_error(L); return; } \
    lua_settop(L, top);                                                 \
} while(0)

#define RETURN do {                                                     \
    lua_settop(L, top); return;                                         \
} while(0)

static void WindowPosCallback(GLFWwindow *window, int xpos, int ypos)
    {
    BEGIN(WindowPos);
    lua_pushinteger(L, xpos);
    lua_pushinteger(L, ypos);
    EXEC(2);
    }

static void WindowSizeCallback(GLFWwindow *window, int width, int height)
    {
    BEGIN(WindowSize);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    EXEC(2);
    }

static void WindowCloseCallback(GLFWwindow *window)
    {
    BEGIN(WindowClose);
    EXEC(0);
    }

static void WindowRefreshCallback(GLFWwindow *window)
    {
    BEGIN(WindowRefresh);
    EXEC(0);
    }

static void WindowFocusCallback(GLFWwindow *window, int focused)
    {
    BEGIN(WindowFocus);
    lua_pushboolean(L, focused);
    EXEC(1);
    }

static void WindowIconifyCallback(GLFWwindow *window, int iconified)
    {
    BEGIN(WindowIconify);
    lua_pushboolean(L, iconified);
    EXEC(1);
    }


static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
    BEGIN(FramebufferSize);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    EXEC(2);
    }

//typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
//GLFWkeyfun glfw.SetKeyCallback(GLFWwindow* window, GLFWkeyfun cbfun);
static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
    int n = 3;
    BEGIN(Key);
    enumPush(L, key, enumKey());
    lua_pushinteger(L, scancode);
    enumPush(L, action, enumAction());
    n += PushMods(L, mods);
    EXEC(n);
    }


//typedef void (* GLFWcharfun)(GLFWwindow*,unsigned int);
//GLFWcharfun glfw.SetCharCallback(GLFWwindow* window, GLFWcharfun cbfun);
static void CharCallback(GLFWwindow *window, unsigned int codepoint)
    {
    BEGIN(Char);
    lua_pushinteger(L, codepoint);
    EXEC(1);
    }


//typedef void (* GLFWcharmodsfun)(GLFWwindow*,unsigned int,int);
//GLFWcharmodsfun glfw.SetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun cbfun);
static void CharModsCallback(GLFWwindow *window, unsigned int codepoint, int mods)
    {
    int n = 1;
    BEGIN(CharMods);
    lua_pushinteger(L, codepoint);
    n += PushMods(L, mods);
    EXEC(n);
    }


//typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
//GLFWmousebuttonfun glfw.SetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun cbfun);
static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
    int n = 2;
    BEGIN(MouseButton);
    enumPush(L, button, enumButton());
    enumPush(L, action, enumAction());
    n += PushMods(L, mods);
    EXEC(n);
    }


//typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
//GLFWcursorposfun glfw.SetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cbfun);
static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
    {
    BEGIN(CursorPos);
    lua_pushnumber(L, xpos);
    lua_pushnumber(L, ypos);
    EXEC(2);
    }


//typedef void (* GLFWcursorenterfun)(GLFWwindow*,int);
//GLFWcursorenterfun glfw.SetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun cbfun);
static void CursorEnterCallback(GLFWwindow *window, int entered)
    {
    BEGIN(CursorEnter);
    lua_pushboolean(L, entered);
    EXEC(1);
    }


//typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
//GLFWscrollfun glfw.SetScrollCallback(GLFWwindow* window, GLFWscrollfun cbfun);
static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
    BEGIN(Scroll);
    lua_pushnumber(L, xoffset);
    lua_pushnumber(L, yoffset);
    EXEC(2);
    }


//typedef void (* GLFWdropfun)(GLFWwindow*,int,const char**);
//GLFWdropfun glfw.SetDropCallback(GLFWwindow* window, GLFWdropfun cbfun);
static void DropCallback(GLFWwindow *window, int count, const char** paths)
    {
    int i;
    BEGIN(Drop);
    for(i = 0; i<count; i++)
        lua_pushstring(L, paths[i]);
    EXEC(count);
    }


#undef BEGIN
#undef EXEC
#undef L

/*--------------------------------------------------------------------------*
 | Window-specific callbacks registration                                   |
 *--------------------------------------------------------------------------*/

#define REGISTER_FUNC(cb)           \
static int Set##cb##Callback(lua_State *L)                       \
    {                                                   \
    win_t *win = checkwindow(L, 1);                     \
    if(0)                                               \
        { /* unregister callback */                     \
        if(win->cb == LUA_NOREF) return 0; /* nothing to do */  \
        CALLBACK_UNREF(win->cb);                        \
        glfw.Set##cb##Callback(win->window, NULL);       \
        return 0;                                       \
        }                                               \
    if(!lua_isfunction(L, 2))                           \
        return luaL_argerror(L, 2, "function expected");\
    if(win->cb != LUA_NOREF)                            \
        luaL_unref(L, LUA_REGISTRYINDEX, win->cb);      \
    lua_pushvalue(L, 2);                                \
    win->cb = luaL_ref(L, LUA_REGISTRYINDEX);           \
    glfw.Set##cb##Callback(win->window, cb##Callback);   \
    return 0;                                           \
    }

REGISTER_FUNC(WindowPos)
REGISTER_FUNC(WindowSize)
REGISTER_FUNC(WindowClose)
REGISTER_FUNC(WindowRefresh)
REGISTER_FUNC(WindowFocus)
REGISTER_FUNC(WindowIconify)
REGISTER_FUNC(FramebufferSize)
REGISTER_FUNC(Key)
REGISTER_FUNC(Char)
REGISTER_FUNC(CharMods)
REGISTER_FUNC(MouseButton)
REGISTER_FUNC(CursorPos)
REGISTER_FUNC(CursorEnter)
REGISTER_FUNC(Scroll)
REGISTER_FUNC(Drop)

#undef REGISTER_FUNC

/*--------------------------------------------------------------------------*
 | Functions registration                                                   |
 *--------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        /* global */
        { "set_error_callback", SetErrorCallback },
        /* window specific */
        { "set_window_pos_callback", SetWindowPosCallback },
        { "set_window_size_callback", SetWindowSizeCallback },
        { "set_window_close_callback", SetWindowCloseCallback },
        { "set_window_refresh_callback", SetWindowRefreshCallback },
        { "set_window_focus_callback", SetWindowFocusCallback },
        { "set_window_iconify_callback", SetWindowIconifyCallback },
        { "set_framebuffer_size_callback", SetFramebufferSizeCallback },
        { "set_key_callback", SetKeyCallback },
        { "set_char_callback", SetCharCallback },
        { "set_char_mods_callback", SetCharModsCallback },
        { "set_mouse_button_callback", SetMouseButtonCallback },
        { "set_cursor_pos_callback", SetCursorPosCallback },
        { "set_cursor_enter_callback", SetCursorEnterCallback },
        { "set_scroll_callback", SetScrollCallback },
        { "set_drop_callback", SetDropCallback },
        { "set_monitor_callback", SetMonitorCallback },
        { "set_joystick_callback", SetJoystickCallback },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_callbacks(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }


