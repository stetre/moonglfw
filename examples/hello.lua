#!/usr/bin/env lua
-- Script: hello.lua

glfw = require("moonglfw")

-- Create a window:
window = glfw.create_window(640, 480, "Hello world!")

function my_callback(w, x, y) 
   assert(w == window)
   print("cursor position:", x, y) 
end

-- Register a callback to track the cursor's position:
glfw.set_cursor_pos_callback(window, my_callback)

-- Repeatedly poll for events:
while not glfw.window_should_close(window) do
   glfw.poll_events()
end

