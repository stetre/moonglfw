#!/usr/bin/env lua
-- MoonGLFW example: fullscreen.lua
-- Shows how to toggle a window to fullscreen and viceversa.

local gl = require("moongl")
local glfw = require("moonglfw")

local W, H = 640, 480 -- current window width and height

glfw.version_hint(3, 3, 'core')
local window = glfw.create_window(W, H, "Toggle Full Screen Example")
glfw.make_context_current(window)
gl.init()

local fullscreen = false -- when in fullscreen, will hold the restore parameters

local function toggle_fullscreen()
   if not fullscreen then
      -- Get the current window position and size parameters, and save
      -- them for the next fullscreen toggle:
      local x, y = glfw.get_window_pos(window)
      local w, h = glfw.get_window_size(window)
      fullscreen = { x, y, w, h }
      -- Get the primary monitor and its video mode parameters:
      local monitor = glfw.get_primary_monitor()
      local vmode = glfw.get_video_mode(monitor)
      -- Eventually go full screen:
      glfw.set_window_monitor(window, monitor, 0, 0, vmode.width, vmode.height)
   else
      -- Restore the window with the saved parameters:
      local x, y, w, h = table.unpack(fullscreen)
      fullscreen = false
      glfw.set_window_monitor(window, nil, x, y, w, h)
   end
end

glfw.set_window_size_callback(window, function(window, width, height)
   W, H = width, height
   print("window resized to "..W.."x"..H..(fullscreen and " (full screen)" or ""))
end)

glfw.set_key_callback(window, function(window, key, scancode, action)
   if key == 'escape' and action == 'press' then
      glfw.set_window_should_close(window, true)
   elseif key == 'f11' and action == 'press' then
      toggle_fullscreen()
   end
end)

print("\nPress F11 to toggle fullscreen, or ESC to exit.\n")

while not glfw.window_should_close(window) do
   glfw.wait_events_timeout(1/60)
   gl.clear_color(0.6, 0, 0, 1)
   gl.clear('color')
   glfw.swap_buffers(window)
end

