# PBRenderer 1.0.1

####  Physically Based Renderer

Application for creating physically based renderings. The whole project is divided into two main parts; rasterizer and pbrenderer. The first part implements a few basic GLSL shaders for visualising normals with flat and smooth shading, additionally, mouse picking and normal ray casting is added. A user can fly around the scene freely using the following buttons: W, S, D, A and left mouse button. Finally, place the camera in a selected position in the world space and render an image using R or C button. The former has to be pressed to physically based render a scene and the latter to ray cast normals. The biggest part of this project is pbrenderer with the following functions:

##### How to run:

To run the project Visual Studio is needed. Render images are saved in the folder output which is created in the repository root automatically. The example of a configuraiton file is in the folder with the same name. Default.json is out of the box file for demonstration.

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

![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/output_1.jpg)
> The image was genereated using 1500 samples per pixel and it took 8 minutes [8 core CPU].

#### TODO:

- [ ] Implement more materials and example scenes
- [ ] Add BRDFs for rasterizer shaders
- [ ] Improve render speed (patches)

#### References:
[0] [pbr-book (3ed-2018)](http://www.pbr-book.org/3ed-2018/contents.html)