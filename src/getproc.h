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

#ifndef getprocDEFINED
#define getprocDEFINED

#include "pfn.h"

#define CheckPfn(L, pfn, major, minor, patch) do {          \
    if(glfw.pfn==NULL)                                      \
        {                                                   \
        if(!checkminversion((major), (minor), (patch)))     \
            return luaL_error((L),                          \
                "glfw"#pfn" requires GLFW version >= %d.%d.%d", (major), (minor), (patch));\
        else                                                \
            return luaL_error((L), "glfw"#pfn" address not found");  \
        }                                                   \
} while(0)

#define CheckVulkanPfn(L, pfn) CheckPfn(L, pfn, 3, 2, 0)

#define CheckNativePfn(L, pfn) do {                             \
    if(glfw.pfn==NULL)                                          \
        return luaL_error((L), "glfw"#pfn" address not found"); \
} while(0)


/* Dispatch tables */

/* Global functions */
typedef struct {
    PFN_glfwInit Init;
    PFN_glfwTerminate Terminate;
    PFN_glfwGetVersion GetVersion;
    PFN_glfwGetVersionString GetVersionString;
    PFN_glfwSetErrorCallback SetErrorCallback;
    PFN_glfwGetMonitors GetMonitors;
    PFN_glfwGetPrimaryMonitor GetPrimaryMonitor;
    PFN_glfwGetMonitorPos GetMonitorPos;
    PFN_glfwGetMonitorPhysicalSize GetMonitorPhysicalSize;
    PFN_glfwGetMonitorName GetMonitorName;
    PFN_glfwSetMonitorCallback SetMonitorCallback;
    PFN_glfwGetVideoModes GetVideoModes;
    PFN_glfwGetVideoMode GetVideoMode;
    PFN_glfwSetGamma SetGamma;
    PFN_glfwGetGammaRamp GetGammaRamp;
    PFN_glfwSetGammaRamp SetGammaRamp;
    PFN_glfwDefaultWindowHints DefaultWindowHints;
    PFN_glfwWindowHint WindowHint;
    PFN_glfwCreateWindow CreateWindow;
    PFN_glfwDestroyWindow DestroyWindow;
    PFN_glfwWindowShouldClose WindowShouldClose;
    PFN_glfwSetWindowShouldClose SetWindowShouldClose;
    PFN_glfwSetWindowTitle SetWindowTitle;
    PFN_glfwSetWindowIcon SetWindowIcon;
    PFN_glfwGetWindowPos GetWindowPos;
    PFN_glfwSetWindowPos SetWindowPos;
    PFN_glfwGetWindowSize GetWindowSize;
    PFN_glfwSetWindowSizeLimits SetWindowSizeLimits;
    PFN_glfwSetWindowAspectRatio SetWindowAspectRatio;
    PFN_glfwSetWindowSize SetWindowSize;
    PFN_glfwGetFramebufferSize GetFramebufferSize;
    PFN_glfwGetWindowFrameSize GetWindowFrameSize;
    PFN_glfwIconifyWindow IconifyWindow;
    PFN_glfwRestoreWindow RestoreWindow;
    PFN_glfwMaximizeWindow MaximizeWindow;
    PFN_glfwShowWindow ShowWindow;
    PFN_glfwHideWindow HideWindow;
    PFN_glfwFocusWindow FocusWindow;
    PFN_glfwGetWindowMonitor GetWindowMonitor;
    PFN_glfwSetWindowMonitor SetWindowMonitor;
    PFN_glfwGetWindowAttrib GetWindowAttrib;
    PFN_glfwSetWindowUserPointer SetWindowUserPointer;
    PFN_glfwGetWindowUserPointer GetWindowUserPointer;
    PFN_glfwSetWindowPosCallback SetWindowPosCallback;
    PFN_glfwSetWindowSizeCallback SetWindowSizeCallback;
    PFN_glfwSetWindowCloseCallback SetWindowCloseCallback;
    PFN_glfwSetWindowRefreshCallback SetWindowRefreshCallback;
    PFN_glfwSetWindowFocusCallback SetWindowFocusCallback;
    PFN_glfwSetWindowIconifyCallback SetWindowIconifyCallback;
    PFN_glfwSetFramebufferSizeCallback SetFramebufferSizeCallback;
    PFN_glfwPollEvents PollEvents;
    PFN_glfwWaitEvents WaitEvents;
    PFN_glfwWaitEventsTimeout WaitEventsTimeout;
    PFN_glfwPostEmptyEvent PostEmptyEvent;
    PFN_glfwGetInputMode GetInputMode;
    PFN_glfwSetInputMode SetInputMode;
    PFN_glfwGetKeyName GetKeyName;
    PFN_glfwGetKey GetKey;
    PFN_glfwGetMouseButton GetMouseButton;
    PFN_glfwGetCursorPos GetCursorPos;
    PFN_glfwSetCursorPos SetCursorPos;
    PFN_glfwCreateCursor CreateCursor;
    PFN_glfwCreateStandardCursor CreateStandardCursor;
    PFN_glfwDestroyCursor DestroyCursor;
    PFN_glfwSetCursor SetCursor;
    PFN_glfwSetKeyCallback SetKeyCallback;
    PFN_glfwSetCharCallback SetCharCallback;
    PFN_glfwSetCharModsCallback SetCharModsCallback;
    PFN_glfwSetMouseButtonCallback SetMouseButtonCallback;
    PFN_glfwSetCursorPosCallback SetCursorPosCallback;
    PFN_glfwSetCursorEnterCallback SetCursorEnterCallback;
    PFN_glfwSetScrollCallback SetScrollCallback;
    PFN_glfwSetDropCallback SetDropCallback;
    PFN_glfwJoystickPresent JoystickPresent;
    PFN_glfwGetJoystickAxes GetJoystickAxes;
    PFN_glfwGetJoystickButtons GetJoystickButtons;
    PFN_glfwGetJoystickName GetJoystickName;
    PFN_glfwSetJoystickCallback SetJoystickCallback;
    PFN_glfwSetClipboardString SetClipboardString;
    PFN_glfwGetClipboardString GetClipboardString;
    PFN_glfwGetTime GetTime;
    PFN_glfwSetTime SetTime;
    PFN_glfwGetTimerValue GetTimerValue;
    PFN_glfwGetTimerFrequency GetTimerFrequency;
    PFN_glfwMakeContextCurrent MakeContextCurrent;
    PFN_glfwGetCurrentContext GetCurrentContext;
    PFN_glfwSwapBuffers SwapBuffers;
    PFN_glfwSwapInterval SwapInterval;
    PFN_glfwExtensionSupported ExtensionSupported;
//#ifdef VULKAN
    PFN_glfwGetProcAddress GetProcAddress;
    PFN_glfwVulkanSupported VulkanSupported;
    PFN_glfwGetRequiredInstanceExtensions GetRequiredInstanceExtensions;
    PFN_glfwGetInstanceProcAddress GetInstanceProcAddress;
    PFN_glfwGetPhysicalDevicePresentationSupport GetPhysicalDevicePresentationSupport;
    PFN_glfwCreateWindowSurface CreateWindowSurface;
//#endif
//  Native access functions
    PFN_glfwGetWin32Adapter GetWin32Adapter;
    PFN_glfwGetWin32Monitor GetWin32Monitor;
    PFN_glfwGetWin32Window GetWin32Window;
    PFN_glfwGetWGLContext GetWGLContext;
//  PFN_glfwGetCocoaMonitor GetCocoaMonitor;
//  PFN_glfwGetCocoaWindow GetCocoaWindow;
//  PFN_glfwGetNSGLContext GetNSGLContext;
    PFN_glfwGetX11Display GetX11Display;
    PFN_glfwGetX11Adapter GetX11Adapter;
    PFN_glfwGetX11Monitor GetX11Monitor;
    PFN_glfwGetX11Window GetX11Window;
    PFN_glfwGetGLXContext GetGLXContext;
    PFN_glfwGetGLXWindow GetGLXWindow;
    PFN_glfwGetWaylandDisplay GetWaylandDisplay;
    PFN_glfwGetWaylandMonitor GetWaylandMonitor;
    PFN_glfwGetWaylandWindow GetWaylandWindow;
    PFN_glfwGetMirDisplay GetMirDisplay;
    PFN_glfwGetMirMonitor GetMirMonitor;
    PFN_glfwGetMirWindow GetMirWindow;
    PFN_glfwGetEGLDisplay GetEGLDisplay;
    PFN_glfwGetEGLContext GetEGLContext;
    PFN_glfwGetEGLSurface GetEGLSurface;
} moonglfw_dt_t;

#define glfw moonglfw_glfw
extern moonglfw_dt_t glfw;

#endif /* getprocDEFINED */




