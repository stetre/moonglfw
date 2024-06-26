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
static int Version;
static int Initialized = 0;

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
        if(Initialized)
            { glfw.Terminate(); Initialized = 0; }
        enums_free_all(moonglfw_L);
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
 | Re-initialization (needed only to set init hints)                            |
 *------------------------------------------------------------------------------*/

static int Init(lua_State *L)
    {
    if(Initialized) return 0;
    if(glfw.Init() != GL_TRUE)
        {
        const char *descr;
        glfw.GetError(&descr);
        if(descr) return luaL_error(L, descr);
        return luaL_error(L, "glfwInit() failed");
        }
    glfw.SetErrorCallback(errorCallback);
    Initialized = 1;
    return 0;
    }

static int Boolean(lua_State *L, int hint)
    {
    int value = checkboolean(L, 2);
    glfw.InitHint(hint, value);
    return 0;
    }


#define ENUM(L, hint, checkfunc) do {       \
    int value = checkfunc((L), 2);          \
    glfw.InitHint((hint), value);           \
    return 0;                               \
} while(0)


static int InitHint(lua_State *L)
    {
    int hint;
    if(Initialized)
        { glfw.Terminate(); Initialized = 0; }
    CheckPfn(L, InitHint, 3, 3, 0);
    hint = checkinithint(L, 1);
    switch(hint)
        {
        case GLFW_JOYSTICK_HAT_BUTTONS:
        case GLFW_COCOA_CHDIR_RESOURCES:
        case GLFW_COCOA_MENUBAR:
        case GLFW_X11_XCB_VULKAN_SURFACE: return Boolean(L, hint);
        case GLFW_ANGLE_PLATFORM_TYPE: ENUM(L, hint, checkangleplatformtype);
        case GLFW_PLATFORM: ENUM(L, hint, checkplatform);
        case GLFW_WAYLAND_LIBDECOR: ENUM(L, hint, checkwaylandlibdecor);
        default:
            return luaL_error(L, "invalid init hint '%s'", lua_tostring(L, 1));
        }
    return 0;
    }
#undef ENUM

static int GetPlatform(lua_State *L)
    {
    int platform;
    CheckPfn(L, GetPlatform, 3, 4, 0);
    platform = glfw.GetPlatform();
    pushplatform(L, platform);
    return 1;
    }


static int PlatformSupported(lua_State *L)
    {
    int platform;
    CheckPfn(L, PlatformSupported, 3, 4, 0);
    platform = checkplatform(L, 1);
    lua_pushboolean(L, glfw.PlatformSupported(platform));
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "init", Init },
        { "init_hint", InitHint },
        { "get_platform", GetPlatform },
        { "platform_supported", PlatformSupported },
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
    moonglfw_open_enums(L);
    moonglfw_open_getproc(L);
    AddVersions(L);

    /* Do not include hats in glfwGetJoystickButtons() if version >= 3.3.0 */
    if(checkminversion(3, 3, 0))
        glfw.InitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);

    Init(L);
    atexit(AtExit);

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

