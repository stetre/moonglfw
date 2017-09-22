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

#define MAKE_VERSION(major, minor, rev) (((major) << 22) | ((minor) << 12) | (rev))
static int Version ;

int checkminversion(int major, int minor, int rev)
/* Checks that libglfw.so Version is at least major.minor.ver */
    {
    return (Version >= MAKE_VERSION(major, minor, rev));
    }

static void AddVersions(lua_State *L)
/* Add version strings to the glfw table */
    {
    int major, minor, rev;
    lua_pushstring(L, "_VERSION");
    lua_pushstring(L, "MoonGLFW "MOONGLFW_VERSION);
    lua_settable(L, -3);

    lua_pushstring(L, "_GLFW_VERSION");
    glfw.GetVersion(&major, &minor, &rev);
    lua_pushfstring(L, "GLFW %d.%d.%d", major, minor, rev);
#if 0
    lua_pushfstring(L, "GLFW %d.%d.%d release %d",
            GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION, GLFW_RELEASE);
#endif
    lua_settable(L, -3);

    Version = MAKE_VERSION(major, minor, rev);
    if(!checkminversion(3, 1, 0))
        {
        lua_pushfstring(L, "MoonGLFW requires GLFW version >= 3.1 (found version %d.%d.%d)",
                            major, minor, rev);
        lua_error(L);
        }
    }

static int GetVersion(lua_State *L) 
    {
    int major, minor, rev;
    glfw.GetVersion(&major, &minor, &rev);
    lua_pushinteger(L, major);
    lua_pushinteger(L, minor);
    lua_pushinteger(L, rev);
    return 3;
    }

static int GetVersionString(lua_State *L) 
    {
    lua_pushstring(L, glfw.GetVersionString());
    return 1;
    }


lua_State *moonglfw_L = NULL;

static void AtExit(void)
    {
    if(moonglfw_L)
        {
        cur_free_all();
        mon_free_all();
        win_free_all();
        id_free_all();
        glfw.Terminate();
        moonglfw_atexit_getproc();
        moonglfw_L = NULL;
        }
    }

void errorCallback(int ec, const char *descr)
    {
    if(moonglfw_L)
        luaL_error(moonglfw_L, "GLFW error: '%s' (code = %d)", descr, ec);
    else
        printf("GLFW error: '%s' (code = %d)\n", descr, ec);
    }

static int Now(lua_State *L)
    {
    lua_pushnumber(L, now());
    return 1;
    }

static int Since(lua_State *L)
    {
    double t = luaL_checknumber(L, 1);
    lua_pushnumber(L, since(t));
    return 1;
    }

static int Sleep(lua_State *L)
    {
    double seconds = luaL_checknumber(L, 1);
    sleeep(seconds);
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "get_version", GetVersion },
        { "get_version_string", GetVersionString },
        { "now", Now },
        { "since", Since },
        { "sleep", Sleep },
        { NULL, NULL } /* sentinel */
    };


int luaopen_moonglfw(lua_State *L)
/* Lua calls this function to load the module */
    {
    moonglfw_L = L;

    moonglfw_utils_init(L);

    lua_newtable(L); /* the glfw table */
    moonglfw_open_getproc(L);
    AddVersions(L);


    if(glfw.Init() != GL_TRUE)
        return luaL_error(L, "glfwInit() failed");
    atexit(AtExit);
    glfw.SetErrorCallback(errorCallback);

    /* add glfw functions: */
    luaL_setfuncs(L, Functions, 0);
    moonglfw_open_window(L);
    moonglfw_open_hint(L);
    moonglfw_open_monitor(L);
    moonglfw_open_callbacks(L);
    moonglfw_open_input(L);
    moonglfw_open_context(L);
    moonglfw_open_vulkan(L);
    moonglfw_open_native(L);

    return 1;
    }

