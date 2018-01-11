## MoonGLFW: Lua bindings for GLFW

MoonGLFW is a Lua binding library for [GLFW](http://www.glfw.org/).

It runs on GNU/Linux and on Windows (MSYS2/MinGW) and requires 
[Lua](http://www.lua.org/) (>=5.3)
and [GLFW](http://www.glfw.org/download.html) (>=3.1).

MoonGLFW is part of the [MoonLibs](https://github.com/stetre/moonlibs) collection
of Lua libraries for graphics and audio programming.

_Author:_ _[Stefano Trettel](https://www.linkedin.com/in/stetre)_

[![Lua logo](./doc/powered-by-lua.gif)](http://www.lua.org/)

#### License

MIT/X11 license (same as Lua). See [LICENSE](./LICENSE).

#### Documentation

See the [Reference Manual](https://stetre.github.io/moonglfw/doc/index.html).

#### Getting and installing

Setup the build environment as described [here](https://github.com/stetre/moonlibs), then:

```sh
$ git clone https://github.com/stetre/moonglfw
$ cd moonglfw
moonglfw$ make
moonglfw$ make install # or 'sudo make install' (Ubuntu)
```

NOTE: Vulkan support requires GLFW version >= 3.2, and the Vulkan loader (libvulkan.so)
to be in the linker's search path at runtime
(see [MoonVulkan](https://github.com/stetre/moonvulkan#getting-and-installing)'s installation 
instructions for more details).

#### Example

```lua
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
```

The script can be executed at the shell prompt with the standard Lua interpreter:

```shell
$ lua hello.lua
```

Other examples can be found in the **examples/** directory contained in the release package,
and in the [MoonLibs](https://github.com/stetre/moonlibs) repository.


