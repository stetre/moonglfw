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

/*------------------------------------------------------------------------------*
 | Hints                                                                        |
 *------------------------------------------------------------------------------*/

static int DefaultWindowHints(lua_State *L)
    {
    (void)L;
    glfw.DefaultWindowHints();
    return 0;
    }

static int Boolean(lua_State *L, int target)
    {
    int hint = checkboolean(L, 2);
    glfw.WindowHint(target, hint);
    return 0;
    }

static int Integer(lua_State *L, int target)
    {
    int hint = GLFW_DONT_CARE;
    const char *s;
    if(lua_type(L, 2) == LUA_TSTRING)
        {
        s = lua_tostring(L, 2);
        if(strncmp(s, "don't care", strlen(s)) != 0)
            return luaL_argerror(L, 2, "expected integer or \"don't care\"");
        }
    else
        {
        hint = luaL_checkinteger(L, 2);
#if 0
        if(hint < -1)
            return luaL_argerror(L, 2, "invalid value");
#endif
        }
    glfw.WindowHint( target, hint);
    return 0;
    }

static int String(lua_State *L, int target)
    {
    const char* hint = luaL_checkstring(L, 2);
    CheckPfn(L, WindowHintString, 3, 3, 0);
    glfw.WindowHintString(target, hint);
    return 0;
    }

#define ENUM(L, target, checkfunc) do { \
    int hint = checkfunc((L), 2);       \
    glfw.WindowHint((target), hint);    \
    return 0;                           \
} while(0)

static int WindowHint(lua_State *L)
    {
    int target = checktarget(L, 1);
    switch(target)
        {
        case GLFW_RESIZABLE:
        case GLFW_VISIBLE:
        case GLFW_DECORATED:
        case GLFW_FOCUSED:
        case GLFW_AUTO_ICONIFY:
        case GLFW_FLOATING:
        case GLFW_MAXIMIZED:
        case GLFW_CENTER_CURSOR:
        case GLFW_TRANSPARENT_FRAMEBUFFER:
        case GLFW_HOVERED:
        case GLFW_FOCUS_ON_SHOW:
        case GLFW_SCALE_TO_MONITOR: return Boolean(L, target);
        case GLFW_RED_BITS:
        case GLFW_GREEN_BITS:
        case GLFW_BLUE_BITS:
        case GLFW_ALPHA_BITS:
        case GLFW_DEPTH_BITS:
        case GLFW_STENCIL_BITS:
        case GLFW_ACCUM_RED_BITS:
        case GLFW_ACCUM_GREEN_BITS:
        case GLFW_ACCUM_BLUE_BITS:
        case GLFW_ACCUM_ALPHA_BITS:
        case GLFW_AUX_BUFFERS:
        case GLFW_SAMPLES:
        case GLFW_REFRESH_RATE: return Integer(L, target);
        case GLFW_STEREO:
        case GLFW_SRGB_CAPABLE:
        case GLFW_DOUBLEBUFFER: return Boolean(L, target);
        case GLFW_CLIENT_API: ENUM(L, target, checkapi);
        case GLFW_CONTEXT_CREATION_API: ENUM(L, target, checkcontextcreationapi);
        case GLFW_CONTEXT_VERSION_MAJOR:
        case GLFW_CONTEXT_VERSION_MINOR: return Integer(L, target);
        case GLFW_CONTEXT_ROBUSTNESS: ENUM(L, target, checkrobustness);
        case GLFW_CONTEXT_RELEASE_BEHAVIOR: ENUM(L, target, checkreleasebehavior);
        case GLFW_OPENGL_FORWARD_COMPAT:
        case GLFW_OPENGL_DEBUG_CONTEXT: return Boolean(L, target);
        case GLFW_OPENGL_PROFILE: ENUM(L, target, checkprofile);
        case GLFW_X11_CLASS_NAME:
        case GLFW_X11_INSTANCE_NAME: return String(L, target);
        default:
            return luaL_error(L, "invalid target '%s'", lua_tostring(L, 1));
        }
    return 0;
    }

#undef ENUM

static int VersionHint(lua_State *L) /* NONGLFW */
    {
    int major = luaL_checkinteger(L, 1);
    int minor = luaL_checkinteger(L, 2);
    int profile = checkprofile(L, 3);
    glfw.WindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfw.WindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfw.WindowHint(GLFW_OPENGL_PROFILE, profile);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Get window attributes                                                        |
 *------------------------------------------------------------------------------*/

static int GetInt_(lua_State *L, GLFWwindow* window, int attrib, int boolean)
#define GetInteger(L, w, a) GetInt_((L), (w), (a), 0)
#define GetBoolean(L, w, a) GetInt_((L), (w), (a), 1)
    {
    int val = glfw.GetWindowAttrib(window, attrib);
    if(boolean)
        lua_pushboolean(L, val);
    else
        lua_pushinteger(L, val);
    return 1;
    }

#define GET_ENUM(L, window, attrib, pushfunc)   do {    \
    int val = glfw.GetWindowAttrib((window), (attrib)); \
    pushfunc((L), val);                                 \
    return 1;                                           \
} while(0);

static int GetWindowAttrib(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int attrib = checktarget(L, 2);
    
    switch(attrib)
        {
        case GLFW_FOCUSED:
        case GLFW_ICONIFIED:
        case GLFW_VISIBLE:
        case GLFW_RESIZABLE:
        case GLFW_DECORATED:
        case GLFW_FLOATING:
        case GLFW_MAXIMIZED:
        case GLFW_CENTER_CURSOR:
        case GLFW_TRANSPARENT_FRAMEBUFFER:
        case GLFW_HOVERED:
        case GLFW_FOCUS_ON_SHOW:
        case GLFW_SCALE_TO_MONITOR:
            return GetBoolean(L, win->window, attrib);
        case GLFW_CLIENT_API: GET_ENUM(L, win->window, attrib, pushapi);
        case GLFW_CONTEXT_CREATION_API: GET_ENUM(L, win->window, attrib, pushcontextcreationapi);
        case GLFW_CONTEXT_VERSION_MAJOR:
        case GLFW_CONTEXT_VERSION_MINOR:
        case GLFW_CONTEXT_REVISION:
            return GetInteger(L, win->window, attrib);
        case GLFW_OPENGL_FORWARD_COMPAT:
        case GLFW_OPENGL_DEBUG_CONTEXT:
            return GetBoolean(L, win->window, attrib);
        case GLFW_OPENGL_PROFILE: GET_ENUM(L, win->window, attrib, pushprofile);
        case GLFW_CONTEXT_ROBUSTNESS: GET_ENUM(L, win->window, attrib, pushrobustness);
        case GLFW_CONTEXT_RELEASE_BEHAVIOR: GET_ENUM(L, win->window, attrib, pushreleasebehavior);
        default:
            return luaL_error(L, "invalid attribute '%s'", lua_tostring(L, 2));
        }
    return 0;
    }

#undef GET_ENUM

/*------------------------------------------------------------------------------*
 | Set window attributes                                                        |
 *------------------------------------------------------------------------------*/

static int SetBoolean(lua_State *L, GLFWwindow* window, int attrib)
    {
    int value = checkboolean(L, 3);
    CheckPfn(L, SetWindowAttrib, 3, 3, 0);
    glfw.SetWindowAttrib(window, attrib, value);
    return 0;
    }

static int SetWindowAttrib(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int attrib = checktarget(L, 2);
    switch(attrib)
        {
        case GLFW_RESIZABLE:
        case GLFW_DECORATED:
        case GLFW_FLOATING:
        case GLFW_AUTO_ICONIFY:
        case GLFW_FOCUS_ON_SHOW:
            return SetBoolean(L, win->window, attrib);
        default:
            return luaL_error(L, "invalid attribute '%s'", lua_tostring(L, 2));
        }
    return 0;
    }

#undef GET_ENUM


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "default_window_hints", DefaultWindowHints },
        { "window_hint", WindowHint },
        { "version_hint", VersionHint }, 
        { "get_window_attrib", GetWindowAttrib },
        { "set_window_attrib", SetWindowAttrib },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_hint(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

