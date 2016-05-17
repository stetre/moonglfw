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
    
    win->window = glfwCreateWindow(width, height, title, monitor, share);
    if(win->window == NULL)
        {
        win_free(win);
        return luaL_error(L, "cannot create window");
        }
    /* this will allow us to retrieve the win_t* from the GLFWwindow*: */
    id = win->id;
    win->share_id =  share_win ? share_win->id : -1;
    glfwSetWindowUserPointer(win->window, (void*)id);
    lua_pushinteger(L, win->id);
    return 1;
    }

static int DestroyWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfwDestroyWindow(win->window);
    win_free(win);
    return 0;
    }

static int PollEvents(lua_State *L)
    {
    (void)L;
    glfwPollEvents();
    return 0;
    }

static int WaitEvents(lua_State *L)
    {
    (void)L;
    glfwWaitEvents();
    return 0;
    }

static int PostEmptyEvent(lua_State *L)
    {
    (void)L;
    glfwPostEmptyEvent();
    return 0;
    }

static int SwapBuffers(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfwSwapBuffers(win->window);
    return 0;
    }

static int WindowShouldClose(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    lua_pushboolean(L, glfwWindowShouldClose(win->window));
    return 1;
    }


static int SetWindowShouldClose(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int value = checkboolean(L, 2);
    glfwSetWindowShouldClose(win->window, value);
    return 0;
    }

static int SetWindowTitle(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    const char *title = luaL_optstring(L, 2, "");
    glfwSetWindowTitle(win->window, title);
    return 0;
    }

static int GetWindowPos(lua_State *L)
    {
    int xpos, ypos;
    win_t *win = checkwindow(L, 1);
    glfwGetWindowPos(win->window, &xpos, &ypos);
    lua_pushinteger(L, xpos);
    lua_pushinteger(L, ypos);
    return 2;
    }

static int SetWindowPos(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int xpos = luaL_checkinteger(L, 2);
    int ypos = luaL_checkinteger(L, 3);
    glfwSetWindowPos(win->window, xpos, ypos);
    return 0;
    }

static int GetWindowSize(lua_State *L)
    {
    int width, height;
    win_t *win = checkwindow(L, 1);
    glfwGetWindowSize(win->window, &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
    }

static int SetWindowSize(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    glfwSetWindowSize(win->window, width, height);
    return 0;
    }

static int GetFramebufferSize(lua_State *L)
    {
    int width, height;
    win_t *win = checkwindow(L, 1);
    glfwGetFramebufferSize(win->window, &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
    }

static int GetWindowFrameSize(lua_State *L)
    {
    int left, top, right, bottom;
    win_t *win = checkwindow(L, 1);
    (void)L;
    glfwGetWindowFrameSize(win->window, &left, &top, &right, &bottom);
    lua_pushinteger(L, left);
    lua_pushinteger(L, top);
    lua_pushinteger(L, right);
    lua_pushinteger(L, bottom);
    return 4;
    }

static int IconifyWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfwIconifyWindow(win->window);
    return 0;
    }

static int RestoreWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfwRestoreWindow(win->window);
    return 0;
    }

static int ShowWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfwShowWindow(win->window);
    return 0;
    }

static int HideWindow(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    glfwHideWindow(win->window);
    return 0;
    }

static int GetWindowMonitor(lua_State *L)
    {
    int id;
    win_t *win = checkwindow(L, 1);
    GLFWmonitor *monitor = glfwGetWindowMonitor(win->window);
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
        { "poll_events", PollEvents },
        { "wait_events", WaitEvents },
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
        { "show_window", ShowWindow },
        { "hide_window", HideWindow },
        { "get_window_monitor", GetWindowMonitor },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_window(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

