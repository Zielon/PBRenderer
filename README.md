# PBRenderer 2.0.0

####  Physically Based Renderer

Application for generating physically based renderings. The whole project is divided into two main parts; rasterizer and pbrenderer. The first part implements a few basic GLSL shaders for visualising normals with flat and smooth shading, additionally, mouse picking and normal ray casting is added. A user can fly around the scene freely using the following buttons: W, S, D, A and left mouse button. Finally, place the camera in a selected position in the world space and render an image using R or C button. The former has to be pressed to physically based render a scene and the latter to ray cast normals. The biggest part of this project is pbrenderer.

#### How to run:

To run the project Visual Studio is needed. Render images are saved in the folder output which is created in the repository root automatically. The example of a configuraiton file is in the folder with the same name.

Use [vcpkg](https://github.com/Microsoft/vcpkg) to install packages:
```
1) glad_x64-windows
2) glfw3_x64-windows
3) glm_x64-windows
4) stb_x64-windows
5) zlib_x64-windows
6) rapidjson_x64-windows
```

#### Available integrators:

```
1) Whitted
2) Direct light
3) Path tracer with MIS
```

####  Available materials:

```
1) Lambertian
2) Perfect specular
3) Perfect transmit
4) Plastic
5) Glass
6) Metal
```

####  Results:

It took around 10-15 minutes per image [8 threads].

![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/ajax_gold.jpg)
```
Material: gold
Distribuion: GGX with alpha 0.15
Resolution: [900x700]
SPP: 1536
Rays max depth: 10
```

![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/ajax_plastic.jpg)
```
Material: plastic
Distribuion: GGX with alpha 0.55
Lambertian material + Microfacets reflection
Resolution: [900x700]
SPP: 1536
Rays max depth: 10
```

![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/ajax_glass.jpg)
```
Material: glass
IOR 1.5
Resolution: [900x700]
SPP: 3000
Rays max depth: 10
```

![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/bunny.jpg)
```
Material: plastic and glass sphere
IOR 1.5
Resolution: [900x700]
SPP: 1536
Rays max depth: 10
```

#### References:
[0] [pbr-book (3ed-2018)](http://www.pbr-book.org/3ed-2018/contents.html)