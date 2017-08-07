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


static int MakeContextCurrent(lua_State *L)
    {
    win_t * win = checkwindow(L, 1);
    glfw.MakeContextCurrent(win->window);
    return 0;
    }

static int GetCurrentContext(lua_State *L)
    {
    GLFWwindow* window = glfw.GetCurrentContext();
    lua_pushinteger(L, (intptr_t)glfw.GetWindowUserPointer(window));
    return 1;
    }

static int SwapInterval(lua_State *L)
    {
    int interval = luaL_checkinteger(L, 1);
    glfw.SwapInterval(interval);
    return 0;
    }

static int ExtensionSupported(lua_State *L)
    {
    const char* extension = luaL_checkstring(L, 1);
    lua_pushboolean(L, glfw.ExtensionSupported(extension));
    return 1;
    }

#if 0
        { "get_proc_address", GetProcAddress },
static int GetProcAddress(lua_State *L) //@@ 
    {
//GLFWglproc glfw.GetProcAddress(const char* procname);
    (void)L;
    return 0;
    }
#endif


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/


static const struct luaL_Reg Functions[] = 
    {
        { "make_context_current", MakeContextCurrent },
        { "get_current_context", GetCurrentContext },
        { "swap_interval", SwapInterval },
        { "extension_supported", ExtensionSupported },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_context(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

