
## How to install MoonGLFW

### On Ubuntu

Install Lua as described in the installation instructions for 
[MoonFLTK](https://github.com/stetre/moonfltk/).

Install GLFW:

```sh
$ sudo apt-get install libglfw3-dev
```

Download MoonGLFW's [latest release](https://github.com/stetre/moonglfw/releases)
(e.g. moonglfw-0.1.tar.gz).

```sh
$ tar -zxpvf moonglfw-0.1.tar.gz
$ cd moonglfw-0.1
moonglfw-0.1$ make
moonglfw-0.1$ sudo make install
```

### On Windows with MSYS2/MinGW

Set up the MSYS2/MinGW environment and install Lua as described in the installation 
instructions for [MoonFLTK](https://github.com/stetre/moonfltk/).

Install GLFW:

```sh
$ pacman -S mingw-w64-i686-glfw      # (32-bit)
$ pacman -S mingw-w64-x86_64-glfw    # (64-bit)
```

Download MoonGLFW's [latest release](https://github.com/stetre/moonglfw/releases)
(e.g. moonglfw-0.1.tar.gz).

```sh
$ tar -zxpvf moonglfw-0.1.tar.gz
$ cd moonglfw-0.1
moonglfw-0.1$ make platform=mingw
moonglfw-0.1$ make install platform=mingw
```

