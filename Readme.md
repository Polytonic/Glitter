# [Glitter](https://github.com/Polytonic/Glitter)
![Screenshot](http://i.imgur.com/MDo2rsy.jpg)

## Summary
Glitter is a dead-simple boilerplate for OpenGL. Many people tend to overlook how frustrating it is to install dependencies, especially in environments lacking package managers or when your account lacks administrative privileges. Instead, Glitter compiles and statically links every required dependency. This wastes a ton of space and a bit of time upfront, but it vastly simplifies what most people probably want: how to get started with OpenGL.

## Getting Started
Glitter has a single dependency: [cmake](http://www.cmake.org/download/), which is used to generate platform-specific makefiles or project files. Start by cloning this repository, making sure to pass the `--recursive` flag to grab all the dependencies. If you forgot, then you can `git submodule update --init` instead.

```bash
git clone --recursive https://github.com/Polytonic/Glitter
mkdir -p Build && cd Build
```

Now generate a project file or makefile for your platform. If you want to use a particular IDE, make sure it is installed; don't forget to set the Start-Up Project in Visual Studio or the Target in Xcode.

```bash
# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 12" ..
cmake -G "Visual Studio 12 Win64" ..
...
```

## Documentation
Glitter provides the most basic windowing example. It is a starting point, and tries very hard not to enforce any sort of directory structure. Feel free to edit the include paths in `CMakeLists.txt`. Glitter bundles most of the dependencies needed to implement a basic rendering engine. This includes:

 - [assimp](https://github.com/assimp/assimp)
 - [glad](https://github.com/Dav1dde/glad)
 - [glfw](https://github.com/glfw/glfw)
 - [glm](https://github.com/g-truc/glm)
 - [stb](https://github.com/nothings/stb)

You can use Glitter as a starting point for the tutorials on [learnopengl.com](http://learnopengl.com/). I have provided sample implementations of an intrusive tree mesh and shader class. These were used to generate the screenshot above, but will not compile out-of-the-box. I leave that exercise to the reader. :smiley:

## License
>The MIT License (MIT)

>Copyright (c) 2015 Kevin Fung

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
