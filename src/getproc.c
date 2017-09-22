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

moonglfw_dt_t glfw;   /* dispatch table */

#define FP(f) *(void**)(&(f))
/* Cast to silent compiler warnings without giving up the -Wpedantic flag.
 *("ISO C forbids conversion of function pointer to object pointer type")
 */

#if defined(LINUX)
#include <dlfcn.h>
#define LIBNAME "libglfw.so"
static void *Handle = NULL;

#elif defined(MINGW)
#include <windows.h>
#define LIBNAME "glfw3.dll"
#define LLIBNAME L"glfw3.dll"
static HMODULE Handle = NULL;
#ifdef CreateWindow /* damn MS... */
#undef CreateWindow
#endif

#else
#error "Cannot determine platform"
#endif



static int Init(lua_State *L)
    {
#if defined(LINUX)
    char *err;
    Handle = dlopen(LIBNAME, RTLD_LAZY | RTLD_LOCAL);
    if(!Handle)
        {
        err = dlerror();
        return luaL_error(L, err != NULL ? err : "cannot load "LIBNAME);
        }
#define GET(fn) do {                                                \
    FP(glfw.fn) = dlsym(Handle, "glfw"#fn);                         \
    if(!glfw.fn) return luaL_error(L, "cannot find glfw"#fn);       \
} while(0)
#define OPT(fn) do {    /* optional */                              \
    FP(glfw.fn) = dlsym(Handle, "glfw"#fn);                         \
} while(0)

#elif defined(MINGW)
    Handle = LoadLibraryW(LLIBNAME);
    if(!Handle)
        return luaL_error(L, "cannot load "LIBNAME);
#define GET(fn) do {                                                \
    glfw.fn = (PFN_glfw##fn)GetProcAddress(Handle, "glfw"#fn);      \
    if(!glfw.fn) return luaL_error(L, "cannot find glfw"#fn);       \
} while(0)
#define OPT(fn) do {    /* optional */                              \
    glfw.fn = (PFN_glfw##fn)GetProcAddress(Handle, "glfw"#fn);      \
} while(0)
#endif

    /* Fill the global dispatch table */

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
    OPT(MaximizeWindow);
//#ifdef VULKAN requires GLFW version >= 3.20
    OPT(GetProcAddress);
    OPT(VulkanSupported);
    OPT(GetRequiredInstanceExtensions);
    OPT(GetInstanceProcAddress);
    OPT(GetPhysicalDevicePresentationSupport);
    OPT(CreateWindowSurface);
//#endif
    /*  Native access functions */
    OPT(GetWin32Adapter);
    OPT(GetWin32Monitor);
    OPT(GetWin32Window);
    OPT(GetWGLContext);
//  OPT(GetCocoaMonitor);
//  OPT(GetCocoaWindow);
//  OPT(GetNSGLContext);
    OPT(GetX11Display);
    OPT(GetX11Adapter);
    OPT(GetX11Monitor);
    OPT(GetX11Window);
    OPT(GetGLXContext);
    OPT(GetGLXWindow);
    OPT(GetWaylandDisplay);
    OPT(GetWaylandMonitor);
    OPT(GetWaylandWindow);
    OPT(GetMirDisplay);
    OPT(GetMirMonitor);
    OPT(GetMirWindow);
    OPT(GetEGLDisplay);
    OPT(GetEGLContext);
    OPT(GetEGLSurface);
#undef GET
#undef OPT
    return 0;
    }


void moonglfw_atexit_getproc(void)
    {
#if defined(LINUX)
    if(Handle) dlclose(Handle);
#elif defined(MINGW)
    if(Handle) FreeLibrary(Handle);
#endif
    }

/*---------------------------------------------------------------------------*/

int moonglfw_open_getproc(lua_State *L)
    {
    Init(L);
    return 0;
    }

