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

static int CreateWindow(lua_State *L)
    {
    win_t *win, *share_win=NULL;
    mon_t *mon;
    intptr_t id;
    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    const char* title = luaL_optstring(L, 3, "");
    GLFWmonitor* monitor = testmonitor(L, 4, &mon) ? mon->monitor : NULL;
    GLFWwindow* share = testwindow(L, 5, &share_win) ? share_win->window : NULL;

    win = win_new();
    if(!win)
        return luaL_error(L, "cannot create window");
    
    win->window = glfw.CreateWindow(width, height, title, monitor, share);
    if(win->window == NULL)
        {
        win_free(win);
        return luaL_error(L, "cannot create window");
        }
    /* this will allow us to retrieve the win_t* from the GLFWwindow*: */
    id = win->id;
    win->share_id =  share_win ? share_win->id : -1;
    glfw.SetWindowUserPointer(win->window, (void*)id);
    lua_pushinteger(L, win->id);
    return 1;
    }

static int DestroyWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfw.DestroyWindow(win->window);
    win_free(win);
    return 0;
    }


static int FocusWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    CheckPfn(L, FocusWindow, 3, 2, 0);
    glfw.FocusWindow(win->window);
    return 0;
    }

static int SetWindowAspectRatio(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int numer = luaL_optinteger(L, 2, GLFW_DONT_CARE);
    int denom = luaL_optinteger(L, 3, GLFW_DONT_CARE);
    CheckPfn(L, SetWindowAspectRatio, 3, 2, 0);
    glfw.SetWindowAspectRatio(win->window, numer, denom);
    return 0;
    }

static int SetWindowSizeLimits(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int minwidth = luaL_optinteger(L, 2, GLFW_DONT_CARE);
    int minheight = luaL_optinteger(L, 3, GLFW_DONT_CARE);
    int maxwidth = luaL_optinteger(L, 4, GLFW_DONT_CARE);
    int maxheight = luaL_optinteger(L, 5, GLFW_DONT_CARE);
    CheckPfn(L, SetWindowSizeLimits, 3, 2, 0);
    glfw.SetWindowSizeLimits(win->window, minwidth, minheight, maxwidth, maxheight);
    return 0;
    }

static int SetWindowIcon(lua_State *L)
    {
    size_t len;
    int count, arg;
    GLFWimage images[32];
    win_t *win = checkwindow(L, 1);
    CheckPfn(L, SetWindowIcon, 3, 2, 0);
    count = 0;
    arg = 2;
    while(!lua_isnoneornil(L, arg))
        {
        images[count].width = luaL_checkinteger(L, arg++);
        images[count].height = luaL_checkinteger(L, arg++);
        images[count].pixels = (unsigned char*)luaL_checklstring(L, arg++, &len);
        count++;
        }
    if(count==0)
        luaL_checkinteger(L, arg); /* raises an error */
    glfw.SetWindowIcon(win->window, count, images);
    return 0;
    }

static int SetWindowMonitor(lua_State *L)
    {
    mon_t *mon;
    win_t *win = checkwindow(L, 1);
    GLFWmonitor* monitor = testmonitor(L, 2, &mon) ? mon->monitor : NULL;
    int xpos = luaL_checkinteger(L, 3);
    int ypos = luaL_checkinteger(L, 4);
    int width = luaL_checkinteger(L, 5);
    int height = luaL_checkinteger(L, 6);
    int refreshRate = luaL_optinteger(L, 7, GLFW_DONT_CARE);
    CheckPfn(L, SetWindowMonitor, 3, 2, 0);
    glfw.SetWindowMonitor(win->window, monitor, xpos, ypos, width, height, refreshRate);
    return 0;
    }



static int PollEvents(lua_State *L)
    {
    (void)L;
    glfw.PollEvents();
    return 0;
    }

static int WaitEvents(lua_State *L)
    {
    (void)L;
    glfw.WaitEvents();
    return 0;
    }

static int WaitEventsTimeout(lua_State *L)
    {
    double timeout = luaL_checknumber(L, 1);
    CheckPfn(L, WaitEventsTimeout, 3, 2, 0);
    glfw.WaitEventsTimeout(timeout);
    return 0;
    }

static int PostEmptyEvent(lua_State *L)
    {
    (void)L;
    glfw.PostEmptyEvent();
    return 0;
    }

static int SwapBuffers(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfw.SwapBuffers(win->window);
    return 0;
    }

static int WindowShouldClose(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    lua_pushboolean(L, glfw.WindowShouldClose(win->window));
    return 1;
    }


static int SetWindowShouldClose(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int value = checkboolean(L, 2);
    glfw.SetWindowShouldClose(win->window, value);
    return 0;
    }

static int SetWindowTitle(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    const char *title = luaL_optstring(L, 2, "");
    glfw.SetWindowTitle(win->window, title);
    return 0;
    }

static int GetWindowPos(lua_State *L)
    {
    int xpos, ypos;
    win_t *win = checkwindow(L, 1);
    glfw.GetWindowPos(win->window, &xpos, &ypos);
    lua_pushinteger(L, xpos);
    lua_pushinteger(L, ypos);
    return 2;
    }

static int SetWindowPos(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int xpos = luaL_checkinteger(L, 2);
    int ypos = luaL_checkinteger(L, 3);
    glfw.SetWindowPos(win->window, xpos, ypos);
    return 0;
    }

static int GetWindowSize(lua_State *L)
    {
    int width, height;
    win_t *win = checkwindow(L, 1);
    glfw.GetWindowSize(win->window, &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
    }

static int SetWindowSize(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    glfw.SetWindowSize(win->window, width, height);
    return 0;
    }

static int GetFramebufferSize(lua_State *L)
    {
    int width, height;
    win_t *win = checkwindow(L, 1);
    glfw.GetFramebufferSize(win->window, &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
    }

static int GetWindowFrameSize(lua_State *L)
    {
    int left, top, right, bottom;
    win_t *win = checkwindow(L, 1);
    (void)L;
    glfw.GetWindowFrameSize(win->window, &left, &top, &right, &bottom);
    lua_pushinteger(L, left);
    lua_pushinteger(L, top);
    lua_pushinteger(L, right);
    lua_pushinteger(L, bottom);
    return 4;
    }

static int IconifyWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfw.IconifyWindow(win->window);
    return 0;
    }

static int RestoreWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfw.RestoreWindow(win->window);
    return 0;
    }

static int MaximizeWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    CheckPfn(L, MaximizeWindow, 3, 2, 0);
    glfw.MaximizeWindow(win->window);
    return 0;
    }

static int ShowWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfw.ShowWindow(win->window);
    return 0;
    }

static int HideWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfw.HideWindow(win->window);
    return 0;
    }

static int GetWindowMonitor(lua_State *L)
    {
    int id;
    win_t *win = checkwindow(L, 1);
    GLFWmonitor *monitor = glfw.GetWindowMonitor(win->window);
    if(!monitor)
        return 0;
    id = monitorAdd(L, monitor);
    lua_pushinteger(L, id); 
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "create_window", CreateWindow },
        { "destroy_window", DestroyWindow },
        { "focus_window", FocusWindow },
        { "set_window_aspect_ratio", SetWindowAspectRatio },
        { "set_window_size_limits", SetWindowSizeLimits },
        { "set_window_icon", SetWindowIcon },
        { "set_window_monitor", SetWindowMonitor },
        { "poll_events", PollEvents },
        { "wait_events", WaitEvents },
        { "wait_events_timeout", WaitEventsTimeout },
        { "post_empty_event", PostEmptyEvent },
        { "swap_buffers", SwapBuffers },
        { "window_should_close", WindowShouldClose },
        { "set_window_should_close", SetWindowShouldClose },
        { "set_window_title", SetWindowTitle },
        { "get_window_pos", GetWindowPos },
        { "set_window_pos", SetWindowPos },
        { "get_window_size", GetWindowSize },
        { "set_window_size", SetWindowSize },
        { "get_framebuffer_size", GetFramebufferSize },
        { "get_window_frame_size", GetWindowFrameSize },
        { "iconify_window", IconifyWindow },
        { "restore_window", RestoreWindow },
        { "maximize_window", MaximizeWindow },
        { "show_window", ShowWindow },
        { "hide_window", HideWindow },
        { "get_window_monitor", GetWindowMonitor },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_window(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

