/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Stefano Trettel
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

moonglfw_dt_t glfw;   /* sndfile.h dispatch table */

#define FP(f) *(void**)(&(f))
/* Cast to silent compiler warnings without giving up the -Wpedantic flag.
 *("ISO C forbids conversion of function pointer to object pointer type")
 */

#ifdef LINUX
/*----------------------------------------------------------------------------------*
 | Linux                                                                            |
 *----------------------------------------------------------------------------------*/

#define LIBNAME "libglfw.so"

#include <dlfcn.h>
static int Init(lua_State *L)
    {
    char *err;
    void *handle = dlopen(LIBNAME, RTLD_LAZY | RTLD_LOCAL);
    if(!handle)
        {
        err = dlerror();
        return luaL_error(L, err != NULL ? err : "cannot load "LIBNAME);
        }

    /* Fill the global dispatch table */
#define GET(fn) do {                                                \
    FP(glfw.fn) = dlsym(handle, "glfw"#fn);                         \
    if(!glfw.fn) return luaL_error(L, "cannot find glfw"#fn);       \
} while(0)
#define OPT(fn) do {    /* optional */                              \
    FP(glfw.fn) = dlsym(handle, "glfw"#fn);                         \
} while(0)
    /* If MoonGLFW loads successfully, these function pointers are guaranteed
     * to be valid so they need not be checked before using them.
     */
    GET(Init);
    GET(Terminate);
    GET(GetVersion);
    GET(GetVersionString);
    GET(SetErrorCallback);
    GET(GetMonitors);
    GET(GetPrimaryMonitor);
    GET(GetMonitorPos);
    GET(GetMonitorPhysicalSize);
    GET(GetMonitorName);
    GET(SetMonitorCallback);
    GET(GetVideoModes);
    GET(GetVideoMode);
    GET(SetGamma);
    GET(GetGammaRamp);
    GET(SetGammaRamp);
    GET(DefaultWindowHints);
    GET(WindowHint);
    GET(CreateWindow);
    GET(DestroyWindow);
    GET(WindowShouldClose);
    GET(SetWindowShouldClose);
    GET(SetWindowTitle);
    GET(GetWindowPos);
    GET(SetWindowPos);
    GET(GetWindowSize);
    GET(SetWindowSize);
    GET(GetFramebufferSize);
    GET(GetWindowFrameSize);
    GET(IconifyWindow);
    GET(RestoreWindow);
    GET(MaximizeWindow);
    GET(ShowWindow);
    GET(HideWindow);
    GET(GetWindowMonitor);
    GET(GetWindowAttrib);
    GET(SetWindowUserPointer);
    GET(GetWindowUserPointer);
    GET(SetWindowPosCallback);
    GET(SetWindowSizeCallback);
    GET(SetWindowCloseCallback);
    GET(SetWindowRefreshCallback);
    GET(SetWindowFocusCallback);
    GET(SetWindowIconifyCallback);
    GET(SetFramebufferSizeCallback);
    GET(PollEvents);
    GET(WaitEvents);
    GET(PostEmptyEvent);
    GET(GetInputMode);
    GET(SetInputMode);
    GET(GetKey);
    GET(GetMouseButton);
    GET(GetCursorPos);
    GET(SetCursorPos);
    GET(CreateCursor);
    GET(CreateStandardCursor);
    GET(DestroyCursor);
    GET(SetCursor);
    GET(SetKeyCallback);
    GET(SetCharCallback);
    GET(SetCharModsCallback);
    GET(SetMouseButtonCallback);
    GET(SetCursorPosCallback);
    GET(SetCursorEnterCallback);
    GET(SetScrollCallback);
    GET(SetDropCallback);
    GET(JoystickPresent);
    GET(GetJoystickAxes);
    GET(GetJoystickButtons);
    GET(GetJoystickName);
    GET(SetClipboardString);
    GET(GetClipboardString);
    GET(GetTime);
    GET(SetTime);
    GET(MakeContextCurrent);
    GET(GetCurrentContext);
    GET(SwapBuffers);
    GET(SwapInterval);
    GET(ExtensionSupported);
    /* Optional functions, i.e. functions from GLFW > 3.1
     * These functions pointers may be NULL, so check before using them.
     */
    // GLFW ver 3.2.0:
    OPT(GetKeyName);
    OPT(GetTimerFrequency);
    OPT(GetTimerValue);
    OPT(FocusWindow);
    OPT(SetWindowAspectRatio);
    OPT(SetWindowSizeLimits);
    OPT(SetWindowIcon);
    OPT(SetWindowMonitor);
    OPT(WaitEventsTimeout);
    OPT(SetJoystickCallback);
//#ifdef VULKAN requires GLFW version >= 3.20
    OPT(GetProcAddress);
    OPT(VulkanSupported);
    OPT(GetRequiredInstanceExtensions);
    OPT(GetInstanceProcAddress);
    OPT(GetPhysicalDevicePresentationSupport);
    OPT(CreateWindowSurface);
//#endif
#undef GET
#undef OPT
    return 0;
    }

#else
/*----------------------------------------------------------------------------------*
 | @@ Other platforms (MINGW, WIN32, ecc) 
 *----------------------------------------------------------------------------------*/
static int Init(lua_State *L)
    {
    return luaL_error(L, "platform not supported");
    return 0;
    }

#endif

/*---------------------------------------------------------------------------*/

int moonglfw_open_getproc(lua_State *L)
    {
    Init(L);
    return 0;
    }

