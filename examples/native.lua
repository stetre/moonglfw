#!/usr/bin/env lua
-- MoonGLFW example: native.lua
-- Shows the usage of native access functions.

glfw = require("moonglfw")

print(glfw._VERSION)
print(glfw._GLFW_VERSION)
print(glfw.get_version())
print(glfw.get_version_string())

monitor = glfw.get_primary_monitor()
window = glfw.create_window(200,100)

print("EGL supported:", glfw.NATIVE_EGL)
print("get_win32_adapter", pcall(glfw.get_win32_adapter, monitor))
print("get_win32_monitor", pcall(glfw.get_win32_monitor, monitor))
print("get_win32_window", pcall(glfw.get_win32_window, window))

print("WGL supported:", glfw.NATIVE_WGL)
print("get_wgl_context", pcall(glfw.get_wgl_context, window))

print("X11 supported:", glfw.NATIVE_X11)
print("get_x11_display", pcall(glfw.get_x11_display))
print("get_x11_adapter", pcall(glfw.get_x11_adapter, monitor))
print("get_x11_monitor", pcall(glfw.get_x11_monitor, monitor))
print("get_x11_window", pcall(glfw.get_x11_window, window))

print("GLX supported:", glfw.NATIVE_GLX)
print("get_glx_context", pcall(glfw.get_glx_context, window))
print("get_glx_window", pcall(glfw.get_glx_window, window))

print("WAYLAND supported:", glfw.NATIVE_WAYLAND)
print("get_wayland_display", pcall(glfw.get_wayland_display))
print("get_wayland_monitor", pcall(glfw.get_wayland_monitor, monitor))
print("get_wayland_window", pcall(glfw.get_wayland_window, window))

print("MIR supported:", glfw.NATIVE_MIR)
print("get_mir_display", pcall(glfw.get_mir_display))
print("get_mir_monitor", pcall(glfw.get_mir_monitor, monitor))
print("get_mir_window", pcall(glfw.get_mir_window, window))

print("EGL supported:", glfw.NATIVE_EGL)
print("get_egl_display", pcall(glfw.get_egl_display))
print("get_egl_context", pcall(glfw.get_egl_context, window))
print("get_egl_surface", pcall(glfw.get_egl_surface, window))

