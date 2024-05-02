#!/usr/bin/env lua

glfw = require("moonglfw")

print(glfw._VERSION)
print(glfw._GLFW_VERSION)
print(glfw.get_version())
print(glfw.get_version_string())

--[[
print('platform: ' .. glfw.get_platform())
local platforms = glfw.enum('platform')
print(table.concat(platforms, ', '))
for _, p in ipairs(platforms) do
   if p ~= 'any' then
      print(p, glfw.platform_supported(p))
   end
end
--]]
