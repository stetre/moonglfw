#!/usr/bin/env lua
-- MoonGLFW example: simple.lua
--
-- Derived from the simple.c example that comes with GLFW 3.2.1 (http://www.glfw.org).
--
-- Requires also MoonGL and MoonGLMATH (https://github.com/stetre/moonlibs).

local glfw = require('moonglfw')
local gl = require('moongl')         -- OpenGL bindings
local glmath = require('moonglmath') -- graphics math library

local vertex_shader_text = [[
uniform mat4 MVP;
attribute vec3 vCol;
attribute vec2 vPos;
varying vec3 color;
void main()
   {
   gl_Position = MVP * vec4(vPos, 0.0, 1.0);
   color = vCol;
   }
]]

local fragment_shader_text = [[
varying vec3 color;
void main()
   {
   gl_FragColor = vec4(color, 1.0);
   }
]]

local vertices = {
   --   x    y     r    g    b
    { -0.6, -0.4, 1.0, 0.0, 0.0 },
    {  0.6, -0.4, 0.0, 1.0, 0.0 },
    {  0.0,  0.6, 0.0, 0.0, 1.0 }
}

local function error_callback(ec, description)
   io.stderr:write(string.format("Error: %s (%d)\n", description, ec))
end

local function key_callback(window, key, scancode, action)
   print(window, key, scancode, action)
   if key == 'escape' and action == 'press' then
      glfw.set_window_should_close(window, true)
   end
end

glfw.set_error_callback(error_callback)
glfw.window_hint('context version major', 2)
glfw.window_hint('context version minor', 0)

local window = glfw.create_window(640, 480, "Simple example")
glfw.set_key_callback(window, key_callback)
glfw.make_context_current(window)
gl.init()
glfw.swap_interval(1)

local vertex_buffer = gl.new_buffer('array')
gl.buffer_data('array', gl.pack('float', vertices), 'static draw')

local program, vertex_shader, fragment_shader = 
   gl.make_program_s('vertex', vertex_shader_text, 'fragment', fragment_shader_text)

local mvp_location = gl.get_uniform_location(program, "MVP")
local vpos_location = gl.get_attrib_location(program, "vPos")
local vcol_location = gl.get_attrib_location(program, "vCol")

local floatsz = gl.sizeof('float')
gl.enable_vertex_attrib_array(vpos_location)
gl.vertex_attrib_pointer(vpos_location, 2, 'float', false, floatsz*5, 0)
gl.enable_vertex_attrib_array(vcol_location)
gl.vertex_attrib_pointer(vcol_location, 3, 'float', false, floatsz*5, floatsz*2)

collectgarbage()
collectgarbage('stop')
while not glfw.window_should_close(window) do
   local width, height = glfw.get_framebuffer_size(window)
   local ratio = width/height

   gl.viewport(0, 0, width, height)
   gl.clear('color')

   local mvp = glmath.ortho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0) 
               * glmath.rotate_z(glfw.get_time())
        
   gl.use_program(program)
   gl.uniform_matrix4f(mvp_location, true, gl.flatten(mvp))
   gl.draw_arrays('triangles', 0, 3)
   gl.use_program(0)

   glfw.swap_buffers(window)
   glfw.poll_events()
   collectgarbage('step')
end

gl.delete_program(program)
glfw.destroy_window(window)

