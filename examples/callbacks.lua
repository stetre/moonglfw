#!/usr/bin/env lua
-- Script: callbacks.lua

glfw = require("moonglfw")

glfw.set_error_callback(function (ec, descr)
   print("error", ec, descr)
   os.exit()
end)

w = glfw.create_window(640, 480, "Callbacks example")
glfw.set_window_pos_callback(w, function(...) print("WindowPos", table.unpack({...})) end)
glfw.set_window_size_callback(w, function(...) print("WindowSize", table.unpack({...})) end)
glfw.set_window_close_callback(w, function(...) print("WindowClose", table.unpack({...})) end)
glfw.set_window_refresh_callback(w, function(...) print("WindowRefresh", table.unpack({...})) end)
glfw.set_window_focus_callback(w, function(...) print("WindowFocus", table.unpack({...})) end)
glfw.set_window_iconify_callback(w, function(...) print("WindowIconify", table.unpack({...})) end)
glfw.set_framebuffer_size_callback(w, function(...) print("FramebufferSize", table.unpack({...})) end)
glfw.set_key_callback(w, function(...) print("Key", table.unpack({...})) end)
glfw.set_char_callback(w, function(...) print("Char", table.unpack({...})) end)
glfw.set_char_mods_callback(w, function(...) print("CharMods", table.unpack({...})) end)
glfw.set_mouse_button_callback(w, function(...) print("MouseButton", table.unpack({...})) end)
glfw.set_cursor_pos_callback(w, function(...) print("CursorPos", table.unpack({...})) end)
glfw.set_cursor_enter_callback(w, function(...) print("CursorEnter", table.unpack({...})) end)
glfw.set_scroll_callback(w, function(...) print("Scroll", table.unpack({...})) end)
glfw.set_drop_callback(w, function(...) print("Drop", table.unpack({...})) end)

while not glfw.window_should_close(w) do
   glfw.poll_events()
end
