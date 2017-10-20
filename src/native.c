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

//const char* glfwGetWin32Adapter(GLFWmonitor* monitor);
static int GetWin32Adapter(lua_State *L)
    {
    const char *what;
    mon_t *mon = checkmonitor(L, 1);
    CheckNativePfn(L, GetWin32Adapter);
    what = glfw.GetWin32Adapter(mon->monitor);
    if(!what) return 0;
    lua_pushstring(L, what);
    return 1;
    }

//const char* glfwGetWin32Monitor(GLFWmonitor* monitor);
static int GetWin32Monitor(lua_State *L)
    {
    const char *what;
    mon_t *mon = checkmonitor(L, 1);
    CheckNativePfn(L, GetWin32Monitor);
    what = glfw.GetWin32Monitor(mon->monitor);
    if(!what) return 0;
    lua_pushstring(L, what);
    return 1;
    }

//void* /* HWND */ glfwGetWin32Window(GLFWwindow* window);
static int GetWin32Window(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetWin32Window);
    what = glfw.GetWin32Window(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* HGLRC */ glfwGetWGLContext(GLFWwindow* window);
static int GetWGLContext(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetWGLContext);
    what = glfw.GetWGLContext(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//CGDirectDisplayID glfwGetCocoaMonitor(GLFWmonitor* monitor);
//id glfwGetCocoaWindow(GLFWwindow* window);
//id glfwGetNSGLContext(GLFWwindow* window);

//void* /* Display* */ glfwGetX11Display(void);
static int GetX11Display(lua_State *L)
    {
    void *what;
    CheckNativePfn(L, GetX11Display);
    what = glfw.GetX11Display();
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//unsigned long /* RRCrtc */ glfwGetX11Adapter(GLFWmonitor* monitor);
static int GetX11Adapter(lua_State *L)
    {
    unsigned long what;
    mon_t *mon = checkmonitor(L, 1);
    CheckNativePfn(L, GetX11Adapter);
    what = glfw.GetX11Adapter(mon->monitor);
//  if(!what) return 0;
    lua_pushinteger(L, what);
    return 1;
    }

//unsigned long /* RROutput */ glfwGetX11Monitor(GLFWmonitor* monitor);
static int GetX11Monitor(lua_State *L)
    {
    unsigned long what;
    mon_t *mon = checkmonitor(L, 1);
    CheckNativePfn(L, GetX11Monitor);
    what = glfw.GetX11Monitor(mon->monitor);
//  if(!what) return 0;
    lua_pushinteger(L, what);
    return 1;
    }

//unsigned long /* Window */ glfwGetX11Window(GLFWwindow* window);
static int GetX11Window(lua_State *L)
    {
    unsigned long what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetX11Window);
    what = glfw.GetX11Window(win->window);
//  if(!what) return 0;
    lua_pushinteger(L, what);
    return 1;
    }

//void* /* GLXContext */ glfwGetGLXContext(GLFWwindow* window);
static int GetGLXContext(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetGLXContext);
    what = glfw.GetGLXContext(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//unsigned long /* GLXWindow */ glfwGetGLXWindow(GLFWwindow* window);
static int GetGLXWindow(lua_State *L)
    {
    unsigned long what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetGLXWindow);
    what = glfw.GetGLXWindow(win->window);
//  if(!what) return 0;
    lua_pushinteger(L, what);
    return 1;
    }

//void* /* struct wl_display* */ glfwGetWaylandDisplay(void);
static int GetWaylandDisplay(lua_State *L)
    {
    void *what;
    CheckNativePfn(L, GetWaylandDisplay);
    what = glfw.GetWaylandDisplay();
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* struct wl_output* */ glfwGetWaylandMonitor(GLFWmonitor* monitor);
static int GetWaylandMonitor(lua_State *L)
    {
    void *what;
    mon_t *mon = checkmonitor(L, 1);
    CheckNativePfn(L, GetWaylandMonitor);
    what = glfw.GetWaylandMonitor(mon->monitor);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* struct wl_surface* */ glfwGetWaylandWindow(GLFWwindow* window);
static int GetWaylandWindow(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetWaylandWindow);
    what = glfw.GetWaylandWindow(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* MirConnection* */ glfwGetMirDisplay(void);
static int GetMirDisplay(lua_State *L)
    {
    void *what;
    CheckNativePfn(L, GetMirDisplay);
    what = glfw.GetMirDisplay();
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//int glfwGetMirMonitor(GLFWmonitor* monitor);
static int GetMirMonitor(lua_State *L)
    {
    int what;
    mon_t *mon = checkmonitor(L, 1);
    CheckNativePfn(L, GetMirMonitor);
    what = glfw.GetMirMonitor(mon->monitor);
//  if(!what) return 0;
    lua_pushinteger(L, what);
    return 1;
    }

//void* /* MirSurface* */ glfwGetMirWindow(GLFWwindow* window);
static int GetMirWindow(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetMirWindow);
    what = glfw.GetMirWindow(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* EGLDisplay */ glfwGetEGLDisplay(void);
static int GetEGLDisplay(lua_State *L)
    {
    void *what;
    CheckNativePfn(L, GetEGLDisplay);
    what = glfw.GetEGLDisplay();
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* EGLContext */ glfwGetEGLContext(GLFWwindow* window);
static int GetEGLContext(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetEGLContext);
    what = glfw.GetEGLContext(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }

//void* /* EGLSurface */ glfwGetEGLSurface(GLFWwindow* window);
static int GetEGLSurface(lua_State *L)
    {
    void *what;
    win_t *win = checkwindow(L, 1);
    CheckNativePfn(L, GetEGLSurface);
    what = glfw.GetEGLSurface(win->window);
    if(!what) return 0;
    lua_pushlightuserdata(L, what);
    return 1;
    }


static int GetContext(lua_State *L)
    {
    void *what = NULL;
    win_t *win = checkwindow(L, 1);

    if(glfw.GetWGLContext)
        {
        what = glfw.GetWGLContext(win->window);
        if(what)
            {
            lua_pushlightuserdata(L, what);
            lua_pushstring(L, "wgl");
            }
        }

    if(!what && glfw.GetGLXContext)
        {
        what = glfw.GetGLXContext(win->window);
        if(what)
            {
            lua_pushlightuserdata(L, what);
            lua_pushstring(L, "glx");
            }
        }

    if(!what && glfw.GetEGLContext)
        {
        what = glfw.GetEGLContext(win->window);
        if(what)
            {
            lua_pushlightuserdata(L, what);
            lua_pushstring(L, "egl");
            }
        }

    if(!what) return 0;
    return 2;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "get_win32_adapter", GetWin32Adapter },
        { "get_win32_monitor", GetWin32Monitor },
        { "get_win32_window", GetWin32Window },
        { "get_wgl_context", GetWGLContext },
        { "get_x11_display", GetX11Display },
        { "get_x11_adapter", GetX11Adapter },
        { "get_x11_monitor", GetX11Monitor },
        { "get_x11_window", GetX11Window },
        { "get_glx_context", GetGLXContext },
        { "get_glx_window", GetGLXWindow },
        { "get_wayland_display", GetWaylandDisplay },
        { "get_wayland_monitor", GetWaylandMonitor },
        { "get_wayland_window", GetWaylandWindow },
        { "get_mir_display", GetMirDisplay },
        { "get_mir_monitor", GetMirMonitor },
        { "get_mir_window", GetMirWindow },
        { "get_egl_display", GetEGLDisplay },
        { "get_egl_context", GetEGLContext },
        { "get_egl_surface", GetEGLSurface },
        { "get_context", GetContext },
        { NULL, NULL } /* sentinel */
    };

#define SetNative(what, supported) do {     \
    lua_pushboolean(L, (supported));        \
    lua_setfield(L, -2, "NATIVE_"#what);    \
} while (0)

void moonglfw_open_native(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    SetNative(WIN32, glfw.GetWin32Adapter && glfw.GetWin32Monitor && glfw.GetWin32Window);
    SetNative(WGL, glfw.GetWGLContext != NULL);
    SetNative(COCOA, 0); //glfw.GetCocoaMonitor && glfw.GetCocoaWindow);
    SetNative(NSGL, 0); //glfw.GetNSGLContext != NULL);
    SetNative(X11, glfw.GetX11Display && glfw.GetX11Adapter && glfw.GetX11Monitor && glfw.GetX11Window);
    SetNative(GLX, glfw.GetGLXContext && glfw.GetGLXWindow);
    SetNative(WAYLAND, glfw.GetWaylandDisplay && glfw.GetWaylandMonitor && glfw.GetWaylandWindow);
    SetNative(MIR, glfw.GetMirDisplay && glfw.GetMirMonitor && glfw.GetMirWindow);
    SetNative(EGL, glfw.GetEGLDisplay && glfw.GetEGLContext && glfw.GetEGLSurface);
    }

