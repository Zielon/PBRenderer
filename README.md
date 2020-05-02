# PBRenderer 1.0.1

####  Physically Based Renderer

GLTF application for creating physically based renderings. The whole project is divided into two main parts; rasterizer and pbrenderer. The first path implements a few basic GLSL shaders for visualising normals with flat and smooth shading, additionally mouse picking and normal ray castring. The biggest part is pbrenderer with the following functions:

#### Available integrators:
```
1) Whitted
2) Direct light
3) Path tracer with importance sampling
```

####  Available materials:
```
1) Lambertian with cosine weighted sampling 
2) Perfect specular
3) Perfect transmit

(Based on microfacet distributions)
4) Plastic
5) Glass
6) Metal/Gold
```

![input_1](https://github.com/Zielon/PBRenderer/blob/master/samples/input_1.jpg)
![output_1](https://github.com/Zielon/PBRenderer/blob/master/samples/output_1.jpg)


#### References:
[pbr-book (3ed-2018)](http://www.pbr-book.org/3ed-2018/contents.html)