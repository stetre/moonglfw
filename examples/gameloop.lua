#!/usr/bin/env lua
-- Script: gameloop.lua

-- A game loop example. Shows how GLFW can be used to implement a game loop from which
-- the multiple tasks that make up the game (graphics rendering, physics updates, AI
-- updates, etc) are executed at different rates.

local glfw = require("moonglfw")

-- Create the window where the game will run:
local window = glfw.create_window(320, 240, "Game loop skeleton")

local function update_counter(ups)
-- Returns a function n=f(dt) that, given a variable delta time dt, computes how many
-- updates must be performed in order to have ups updates per second. Any reminder time
-- is accounted for in the subsequent call(s).
   local fixed_dt = 1/ups
   local n, tot_dt, remainder = 0, 0, 0
   return function(dt)
      tot_dt = dt + remainder
      n, remainder = tot_dt//fixed_dt, tot_dt%fixed_dt
      return n
   end
end

-- Create a separate update counter for each game task, specifying the rate at
-- which we want the task to execute:
local n_graphics_updates = update_counter(30) -- render 30 frames per second
local n_physics_updates = update_counter(180) -- update physics 180 times per second
local n_ai_updates = update_counter(60)       -- update AI 60 times per second

local last_t, current_t, dt
last_t = glfw.get_time()

-- Enter the game loop:
while not glfw.window_should_close(window) do
   -- 1) Compute the time elapsed since the last iteration (the 'delta time' dt).
   -- This dt is variable. That is, it differs from iteration to iteration because it depends
   -- both on how much computation we do, and on the scheduling of our application from the OS.
   current_t = glfw.get_time()
   dt = current_t - last_t
   last_t = current_t
   -- print(string.format("dt = %.3f ms", dt*1000))

   -- 2) Poll for any input event (from keyboard, mouse, joysticks, gamepads, etc)
   -- that may have occurred since the last iteration.
   -- This just records the input events, which we will process later at step 3
   -- (unless we registered event callbacks, in which case they are executed now).
   glfw.poll_events()
   
   -- 3) Process input. Here we just check if the user pressed ESC, but of course a real
   -- game will have much more inputs. Note that here we do not use an update counter,
   -- so input processing will occur at the maximum (and variable) possible rate.
   if glfw.get_key(window, 'escape')=='press' then
      glfw.set_window_should_close(window, true)
   end
   -- ... process any other input ...
   
   -- 4) Perform the game tasks (here we just print that we are performing them).
   -- We perform each task as many times as needed to keep up with the desired rate.
   for i=1, n_physics_updates(dt) do
      print(current_t, "updating physics")
   end
   for i=1, n_ai_updates(dt) do
      print(current_t, "updating AI")
   end
   for i=1, n_graphics_updates(dt) do
      print(current_t, "rendering frame")
   end
   -- ... whatever else ...
end

