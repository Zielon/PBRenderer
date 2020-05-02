# PBRenderer 1.0.1

####  Physically Based Renderer

Application for creating physically based renderings. The whole project is divided into two main parts; rasterizer and pbrenderer. The first part implements a few basic GLSL shaders for visualising normals with flat and smooth shading, additionally, mouse picking and normal ray casting is added. A user can fly around the scene freely using the following buttons: W, S, D, A and left mouse button. Finally, place the camera in a selected position in the world space and render an image using R or C button. The former has to be pressed to physically based render a scene and the latter to ray cast normals. The biggest part of this project is pbrenderer with the following functions:

To run the project Visual Studio is needed. Render images are saved in the folder output which is created in the repository root automatically.

#### Available integrators:
```
1) Whitted
2) Direct light
3) Path tracer with MIS
```

####  Available BRDFs materials:
```
1) Lambertian
2) Perfect specular
3) Perfect transmit
4) Plastic
5) Glass
6) Metal/Gold
```

####  Results:

The renderd images are in 900x700 resolution.

![input_1](https://github.com/Zielon/PBRenderer/blob/master/samples/input_1.jpg)
![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/output_1.jpg)

> The Stanford Bunny mesh in the picture above is made of plastic material, the smaller sphere is made of glass and the bigger one in the right corner is mirror.
> The image was genereated using 2048 samples per pixel and it took 35 minutes [8 core CPU].

![output_2](https://github.com/Zielon/PBRenderer/blob/master/samples/output_2.jpg)

> The image was genereated using 1024 samples per pixel and it took 25 minutes [8 core CPU].

#### TODO:

- [ ] Implement more materials and example scenes
- [ ] Add BRDFs for rasterizer shaders
- [ ] Improve render speed (patches)

#### References:
[0] [pbr-book (3ed-2018)](http://www.pbr-book.org/3ed-2018/contents.html)