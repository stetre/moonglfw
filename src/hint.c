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


ENUM_STRINGS(TargetStrings) = {
    "resizable",
    "visible",
    "decorated",
    "focused",
    "auto iconify",
    "floating",
    "red bits",
    "green bits",
    "blue bits",
    "alpha bits",
    "depth bits",
    "stencil bits",
    "accum red bits",
    "accum green bits",
    "accum blue bits",
    "accum alpha bits",
    "aux buffers",
    "samples",
    "refresh rate",
    "stereo",
    "srgb capable",
    "doublebuffer",
    "client api",
    "context version major",
    "context version minor",
    "context robustness",
    "context release behavior",
    "opengl forward compat",
    "opengl debug context",
    "opengl profile",
    /* attribute only */
    "iconified",
    "context revision",
    NULL
};
ENUM_CODES(TargetCodes) = {
    GLFW_RESIZABLE,
    GLFW_VISIBLE,
    GLFW_DECORATED,
    GLFW_FOCUSED,
    GLFW_AUTO_ICONIFY,
    GLFW_FLOATING,
    GLFW_RED_BITS,
    GLFW_GREEN_BITS,
    GLFW_BLUE_BITS,
    GLFW_ALPHA_BITS,
    GLFW_DEPTH_BITS,
    GLFW_STENCIL_BITS,
    GLFW_ACCUM_RED_BITS,
    GLFW_ACCUM_GREEN_BITS,
    GLFW_ACCUM_BLUE_BITS,
    GLFW_ACCUM_ALPHA_BITS,
    GLFW_AUX_BUFFERS,
    GLFW_SAMPLES,
    GLFW_REFRESH_RATE,
    GLFW_STEREO,
    GLFW_SRGB_CAPABLE,
    GLFW_DOUBLEBUFFER,
    GLFW_CLIENT_API,
    GLFW_CONTEXT_VERSION_MAJOR,
    GLFW_CONTEXT_VERSION_MINOR,
    GLFW_CONTEXT_ROBUSTNESS,
    GLFW_CONTEXT_RELEASE_BEHAVIOR,
    GLFW_OPENGL_FORWARD_COMPAT,
    GLFW_OPENGL_DEBUG_CONTEXT,
    GLFW_OPENGL_PROFILE,
    /* attribute only */
    GLFW_ICONIFIED,
    GLFW_CONTEXT_REVISION,
};
ENUM_T(TargetEnum, TargetStrings, TargetCodes)
#define CheckTarget(L, arg) enumCheck((L), (arg), &TargetEnum)
#define PushTarget(L, code) enumPush((L), (code), &TargetEnum)
#define CheckAttrib CheckTarget


ENUM_STRINGS(ApiStrings) = {
    "no api",
    "opengl",
    "opengl es",
    NULL
};
ENUM_CODES(ApiCodes) = {
    GLFW_NO_API,
    GLFW_OPENGL_API,
    GLFW_OPENGL_ES_API,
};
ENUM_T(ApiEnum, ApiStrings, ApiCodes)
#define CheckApi(L, arg) enumCheck((L), (arg), &ApiEnum)
#define PushApi(L, code) enumPush((L), (code), &ApiEnum)


ENUM_STRINGS(RobustnessStrings) = {
    "no robustness",
    "no reset notification",
    "lose context on reset",
    NULL
};
ENUM_CODES(RobustnessCodes) = {
    GLFW_NO_ROBUSTNESS,
    GLFW_NO_RESET_NOTIFICATION,
    GLFW_LOSE_CONTEXT_ON_RESET,
};
ENUM_T(RobustnessEnum, RobustnessStrings, RobustnessCodes)
#define CheckRobustness(L, arg) enumCheck((L), (arg), &RobustnessEnum)
#define PushRobustness(L, code) enumPush((L), (code), &RobustnessEnum)


ENUM_STRINGS(ReleaseBehaviorStrings) = {
    "any",
    "flush",
    "none",
    NULL
};
ENUM_CODES(ReleaseBehaviorCodes) = {
    GLFW_ANY_RELEASE_BEHAVIOR,
    GLFW_RELEASE_BEHAVIOR_FLUSH,
    GLFW_RELEASE_BEHAVIOR_NONE,
};
ENUM_T(ReleaseBehaviorEnum, ReleaseBehaviorStrings, ReleaseBehaviorCodes)
#define CheckReleaseBehavior(L, arg) enumCheck((L), (arg), &ReleaseBehaviorEnum)
#define PushReleaseBehavior(L, code) enumPush((L), (code), &ReleaseBehaviorEnum)

ENUM_STRINGS(ProfileStrings) = {
    "any",
    "core",
    "compat",
    NULL
};
ENUM_CODES(ProfileCodes) = {
    GLFW_OPENGL_ANY_PROFILE,
    GLFW_OPENGL_CORE_PROFILE,
    GLFW_OPENGL_COMPAT_PROFILE,
};
ENUM_T(ProfileEnum, ProfileStrings, ProfileCodes)
#define CheckProfile(L, arg) enumCheck((L), (arg), &ProfileEnum)
#define PushProfile(L, code) enumPush((L), (code), &ProfileEnum)


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

static int Enum(lua_State *L, int target, enum_t *e)
    {
    int hint = enumCheck(L, 2, e);
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


static int WindowHint(lua_State *L)
    {
    int target = CheckTarget(L, 1);
    switch(target)
        {
        case GLFW_RESIZABLE:
        case GLFW_VISIBLE:
        case GLFW_DECORATED:
        case GLFW_FOCUSED:
        case GLFW_AUTO_ICONIFY:
        case GLFW_FLOATING: return Boolean(L, target);
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
        case GLFW_CLIENT_API: return Enum(L, target, &ApiEnum);
        case GLFW_CONTEXT_VERSION_MAJOR:
        case GLFW_CONTEXT_VERSION_MINOR: return Integer(L, target);
        case GLFW_CONTEXT_ROBUSTNESS: return Enum(L, target, &RobustnessEnum);
        case GLFW_CONTEXT_RELEASE_BEHAVIOR: return Enum(L, target, &ReleaseBehaviorEnum);
        case GLFW_OPENGL_FORWARD_COMPAT:
        case GLFW_OPENGL_DEBUG_CONTEXT: return Boolean(L, target);
        case GLFW_OPENGL_PROFILE: return Enum(L, target, &ProfileEnum);
        default:
            return luaL_error(L, "invalid target '%s'", lua_tostring(L, 1));
        }
    return 0;
    }

static int VersionHint(lua_State *L) /* NONGLFW */
    {
    int major = luaL_checkinteger(L, 1);
    int minor = luaL_checkinteger(L, 2);
    int profile = enumCheck(L, 3, &ProfileEnum);
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

static int GetEnum(lua_State *L, GLFWwindow* window, int attrib, enum_t *e)
    {
    int val = glfw.GetWindowAttrib(window, attrib);
    enumPush(L, val, e);
    return 1;
    }


static int GetWindowAttrib(lua_State *L)
    {
    win_t *win = checkwindow(L, 1);
    int attrib = CheckAttrib(L, 2);
    
    switch(attrib)
        {
        case GLFW_FOCUSED:
        case GLFW_ICONIFIED:
        case GLFW_VISIBLE:
        case GLFW_RESIZABLE:
        case GLFW_DECORATED:
        case GLFW_FLOATING:
            return GetBoolean(L, win->window, attrib);
        case GLFW_CLIENT_API:
            return GetEnum(L, win->window, attrib, &ApiEnum);
        case GLFW_CONTEXT_VERSION_MAJOR:
        case GLFW_CONTEXT_VERSION_MINOR:
        case GLFW_CONTEXT_REVISION:
            return GetInteger(L, win->window, attrib);
        case GLFW_OPENGL_FORWARD_COMPAT:
        case GLFW_OPENGL_DEBUG_CONTEXT:
            return GetBoolean(L, win->window, attrib);
        case GLFW_OPENGL_PROFILE:
            return GetEnum(L, win->window, attrib, &ProfileEnum);
        case GLFW_CONTEXT_ROBUSTNESS:
            return GetEnum(L, win->window, attrib, &RobustnessEnum);
        case GLFW_CONTEXT_RELEASE_BEHAVIOR:
            return GetEnum(L, win->window, attrib, &ReleaseBehaviorEnum);
        default:
            return luaL_error(L, "invalid attribute '%s'", lua_tostring(L, 2));
        }
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "default_window_hints", DefaultWindowHints },
        { "window_hint", WindowHint },
        { "version_hint", VersionHint }, 
        { "get_window_attrib", GetWindowAttrib },
        { NULL, NULL } /* sentinel */
    };

void moonglfw_open_hint(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

