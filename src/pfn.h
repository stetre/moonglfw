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

#ifndef pfnDEFINED
#define pfnDEFINED

#include "moonglfw.h"

typedef int (*PFN_glfwInit)(void);
typedef void (*PFN_glfwTerminate)(void);
typedef void (*PFN_glfwGetVersion)(int* major, int* minor, int* rev);
typedef const char* (*PFN_glfwGetVersionString)(void);
typedef GLFWerrorfun (*PFN_glfwSetErrorCallback)(GLFWerrorfun cbfun);
typedef GLFWmonitor** (*PFN_glfwGetMonitors)(int* count);
typedef GLFWmonitor* (*PFN_glfwGetPrimaryMonitor)(void);
typedef void (*PFN_glfwGetMonitorPos)(GLFWmonitor* monitor, int* xpos, int* ypos);
typedef void (*PFN_glfwGetMonitorPhysicalSize)(GLFWmonitor* monitor, int* widthMM, int* heightMM);
typedef const char* (*PFN_glfwGetMonitorName)(GLFWmonitor* monitor);
typedef GLFWmonitorfun (*PFN_glfwSetMonitorCallback)(GLFWmonitorfun cbfun);
typedef const GLFWvidmode* (*PFN_glfwGetVideoModes)(GLFWmonitor* monitor, int* count);
typedef const GLFWvidmode* (*PFN_glfwGetVideoMode)(GLFWmonitor* monitor);
typedef void (*PFN_glfwSetGamma)(GLFWmonitor* monitor, float gamma);
typedef const GLFWgammaramp* (*PFN_glfwGetGammaRamp)(GLFWmonitor* monitor);
typedef void (*PFN_glfwSetGammaRamp)(GLFWmonitor* monitor, const GLFWgammaramp* ramp);
typedef void (*PFN_glfwDefaultWindowHints)(void);
typedef void (*PFN_glfwWindowHint)(int hint, int value);
typedef GLFWwindow* (*PFN_glfwCreateWindow)(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
typedef void (*PFN_glfwDestroyWindow)(GLFWwindow* window);
typedef int (*PFN_glfwWindowShouldClose)(GLFWwindow* window);
typedef void (*PFN_glfwSetWindowShouldClose)(GLFWwindow* window, int value);
typedef void (*PFN_glfwSetWindowTitle)(GLFWwindow* window, const char* title);
typedef void (*PFN_glfwSetWindowIcon)(GLFWwindow* window, int count, const GLFWimage* images);
typedef void (*PFN_glfwGetWindowPos)(GLFWwindow* window, int* xpos, int* ypos);
typedef void (*PFN_glfwSetWindowPos)(GLFWwindow* window, int xpos, int ypos);
typedef void (*PFN_glfwGetWindowSize)(GLFWwindow* window, int* width, int* height);
typedef void (*PFN_glfwSetWindowSizeLimits)(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight);
typedef void (*PFN_glfwSetWindowAspectRatio)(GLFWwindow* window, int numer, int denom);
typedef void (*PFN_glfwSetWindowSize)(GLFWwindow* window, int width, int height);
typedef void (*PFN_glfwGetFramebufferSize)(GLFWwindow* window, int* width, int* height);
typedef void (*PFN_glfwGetWindowFrameSize)(GLFWwindow* window, int* left, int* top, int* right, int* bottom);
typedef void (*PFN_glfwIconifyWindow)(GLFWwindow* window);
typedef void (*PFN_glfwRestoreWindow)(GLFWwindow* window);
typedef void (*PFN_glfwMaximizeWindow)(GLFWwindow* window);
typedef void (*PFN_glfwShowWindow)(GLFWwindow* window);
typedef void (*PFN_glfwHideWindow)(GLFWwindow* window);
typedef void (*PFN_glfwFocusWindow)(GLFWwindow* window);
typedef GLFWmonitor* (*PFN_glfwGetWindowMonitor)(GLFWwindow* window);
typedef void (*PFN_glfwSetWindowMonitor)(GLFWwindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);
typedef int (*PFN_glfwGetWindowAttrib)(GLFWwindow* window, int attrib);
typedef void (*PFN_glfwSetWindowUserPointer)(GLFWwindow* window, void* pointer);
typedef void* (*PFN_glfwGetWindowUserPointer)(GLFWwindow* window);
typedef GLFWwindowposfun (*PFN_glfwSetWindowPosCallback)(GLFWwindow* window, GLFWwindowposfun cbfun);
typedef GLFWwindowsizefun (*PFN_glfwSetWindowSizeCallback)(GLFWwindow* window, GLFWwindowsizefun cbfun);
typedef GLFWwindowclosefun (*PFN_glfwSetWindowCloseCallback)(GLFWwindow* window, GLFWwindowclosefun cbfun);
typedef GLFWwindowrefreshfun (*PFN_glfwSetWindowRefreshCallback)(GLFWwindow* window, GLFWwindowrefreshfun cbfun);
typedef GLFWwindowfocusfun (*PFN_glfwSetWindowFocusCallback)(GLFWwindow* window, GLFWwindowfocusfun cbfun);
typedef GLFWwindowiconifyfun (*PFN_glfwSetWindowIconifyCallback)(GLFWwindow* window, GLFWwindowiconifyfun cbfun);
typedef GLFWframebuffersizefun (*PFN_glfwSetFramebufferSizeCallback)(GLFWwindow* window, GLFWframebuffersizefun cbfun);
typedef void (*PFN_glfwPollEvents)(void);
typedef void (*PFN_glfwWaitEvents)(void);
typedef void (*PFN_glfwWaitEventsTimeout)(double timeout);
typedef void (*PFN_glfwPostEmptyEvent)(void);
typedef int (*PFN_glfwGetInputMode)(GLFWwindow* window, int mode);
typedef void (*PFN_glfwSetInputMode)(GLFWwindow* window, int mode, int value);
typedef const char* (*PFN_glfwGetKeyName)(int key, int scancode);
typedef int (*PFN_glfwGetKey)(GLFWwindow* window, int key);
typedef int (*PFN_glfwGetMouseButton)(GLFWwindow* window, int button);
typedef void (*PFN_glfwGetCursorPos)(GLFWwindow* window, double* xpos, double* ypos);
typedef void (*PFN_glfwSetCursorPos)(GLFWwindow* window, double xpos, double ypos);
typedef GLFWcursor* (*PFN_glfwCreateCursor)(const GLFWimage* image, int xhot, int yhot);
typedef GLFWcursor* (*PFN_glfwCreateStandardCursor)(int shape);
typedef void (*PFN_glfwDestroyCursor)(GLFWcursor* cursor);
typedef void (*PFN_glfwSetCursor)(GLFWwindow* window, GLFWcursor* cursor);
typedef GLFWkeyfun (*PFN_glfwSetKeyCallback)(GLFWwindow* window, GLFWkeyfun cbfun);
typedef GLFWcharfun (*PFN_glfwSetCharCallback)(GLFWwindow* window, GLFWcharfun cbfun);
typedef GLFWcharmodsfun (*PFN_glfwSetCharModsCallback)(GLFWwindow* window, GLFWcharmodsfun cbfun);
typedef GLFWmousebuttonfun (*PFN_glfwSetMouseButtonCallback)(GLFWwindow* window, GLFWmousebuttonfun cbfun);
typedef GLFWcursorposfun (*PFN_glfwSetCursorPosCallback)(GLFWwindow* window, GLFWcursorposfun cbfun);
typedef GLFWcursorenterfun (*PFN_glfwSetCursorEnterCallback)(GLFWwindow* window, GLFWcursorenterfun cbfun);
typedef GLFWscrollfun (*PFN_glfwSetScrollCallback)(GLFWwindow* window, GLFWscrollfun cbfun);
typedef GLFWdropfun (*PFN_glfwSetDropCallback)(GLFWwindow* window, GLFWdropfun cbfun);
typedef int (*PFN_glfwJoystickPresent)(int joy);
typedef const float* (*PFN_glfwGetJoystickAxes)(int joy, int* count);
typedef const unsigned char* (*PFN_glfwGetJoystickButtons)(int joy, int* count);
typedef const char* (*PFN_glfwGetJoystickName)(int joy);
typedef GLFWjoystickfun (*PFN_glfwSetJoystickCallback)(GLFWjoystickfun cbfun);
typedef void (*PFN_glfwSetClipboardString)(GLFWwindow* window, const char* string);
typedef const char* (*PFN_glfwGetClipboardString)(GLFWwindow* window);
typedef double (*PFN_glfwGetTime)(void);
typedef void (*PFN_glfwSetTime)(double time);
typedef uint64_t (*PFN_glfwGetTimerValue)(void);
typedef uint64_t (*PFN_glfwGetTimerFrequency)(void);
typedef void (*PFN_glfwMakeContextCurrent)(GLFWwindow* window);
typedef GLFWwindow* (*PFN_glfwGetCurrentContext)(void);
typedef void (*PFN_glfwSwapBuffers)(GLFWwindow* window);
typedef void (*PFN_glfwSwapInterval)(int interval);
typedef int (*PFN_glfwExtensionSupported)(const char* extension);
//#ifdef VULKAN
typedef GLFWglproc (*PFN_glfwGetProcAddress)(const char* procname);
typedef int (*PFN_glfwVulkanSupported)(void);
typedef const char** (*PFN_glfwGetRequiredInstanceExtensions)(uint32_t* count);
typedef GLFWvkproc (*PFN_glfwGetInstanceProcAddress)(VkInstance instance, const char* procname);
typedef int (*PFN_glfwGetPhysicalDevicePresentationSupport)(VkInstance instance, VkPhysicalDevice device, uint32_t queuefamily);
typedef VkResult (*PFN_glfwCreateWindowSurface)(VkInstance instance, GLFWwindow* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);
//#endif


/* Typedefs for glfw2native.h functions (to avoid including native headers)
 */
typedef const char* (*PFN_glfwGetWin32Adapter)(GLFWmonitor* monitor);
typedef const char* (*PFN_glfwGetWin32Monitor)(GLFWmonitor* monitor);
typedef void* /* HWND */ (*PFN_glfwGetWin32Window)(GLFWwindow* window);
typedef void* /* HGLRC */ (*PFN_glfwGetWGLContext)(GLFWwindow* window);
//typedef CGDirectDisplayID (*PFN_glfwGetCocoaMonitor)(GLFWmonitor* monitor);
//typedef id (*PFN_glfwGetCocoaWindow)(GLFWwindow* window);
//typedef id (*PFN_glfwGetNSGLContext)(GLFWwindow* window);
typedef void* /* Display* */ (*PFN_glfwGetX11Display)(void);
typedef unsigned long /* RRCrtc */ (*PFN_glfwGetX11Adapter)(GLFWmonitor* monitor);
typedef unsigned long /* RROutput */ (*PFN_glfwGetX11Monitor)(GLFWmonitor* monitor);
typedef unsigned long /* Window */ (*PFN_glfwGetX11Window)(GLFWwindow* window);
typedef void* /* GLXContext */ (*PFN_glfwGetGLXContext)(GLFWwindow* window);
typedef unsigned long /* GLXWindow */ (*PFN_glfwGetGLXWindow)(GLFWwindow* window);
typedef void* /* struct wl_display* */ (*PFN_glfwGetWaylandDisplay)(void);
typedef void* /* struct wl_output* */ (*PFN_glfwGetWaylandMonitor)(GLFWmonitor* monitor);
typedef void* /* struct wl_surface* */ (*PFN_glfwGetWaylandWindow)(GLFWwindow* window);
typedef void* /* MirConnection* */ (*PFN_glfwGetMirDisplay)(void);
typedef int (*PFN_glfwGetMirMonitor)(GLFWmonitor* monitor);
typedef void* /* MirSurface* */ (*PFN_glfwGetMirWindow)(GLFWwindow* window);
typedef void* /* EGLDisplay */ (*PFN_glfwGetEGLDisplay)(void);
typedef void* /* EGLContext */ (*PFN_glfwGetEGLContext)(GLFWwindow* window);
typedef void* /* EGLSurface */ (*PFN_glfwGetEGLSurface)(GLFWwindow* window);

#endif /* pfnDEFINED */
