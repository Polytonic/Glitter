# [Glitter](https://github.com/Polytonic/Glitter/) :: [Samples](https://github.com/Polytonic/Glitter/tree/master/Samples)

If you're new to OpenGL, it's almost a rite of passage to write your own shader class and model loader. To help you out, I ripped these directly from my own renderer (`Mirage`), so you'll see a bunch of references to it in this sample code. I wrote this when I was still in college, so there are *definitely* some warts in my code.

### Shader

If you're following along with a tutorial, you'll want to read through this one first. The [shader class](https://github.com/Polytonic/Glitter/blob/master/Samples/shader.hpp) covers how to load a GLSL shader source file and turn it into a usable OpenGL shader object. I wrote it in a fashion that allows you to "chain" attach calls, like in JavaScript.

```cpp
Shader shader;
shader.attach("main.vert")
      .attach("main.frag")
      ... // and so on ...
```

There is some basic error handling to help you out if you get stuck.

### Mesh

Model loading is a bit harder. Most standard models are actually comprised of multiple, "sub-models" (or sub-meshes). For example, a character model in a video game might have a "torso" section, a "left arm" and a "right arm" section, and so on, all inside the same model file. Here I provide a sample [mesh class](https://github.com/Polytonic/Glitter/blob/master/Samples/mesh.hpp) that will handle multi-meshes; the screenshot on the main page is one of them!

Most OpenGL tutorials will guide you through writing a standard "Mesh" class, which involves writing a standard tree containing a set of nodes. This entails a containing "tree" class, and a "node" class containing data. As an alternative, I wrote an intrusive tree implementation, which stores the tree relation directly inside the nodes. This [Quora post](http://qr.ae/RFzeSU) might be helpful in understanding what an intrusive data structure is, and why they are used.
