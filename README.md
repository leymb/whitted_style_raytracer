This repository contains my very first and very limited Whitted-style raytracer made for the second Block of my first year (2020) in the CMGT program at BUAS, by Melvin Rother.
It can render simple primitives and basic refractive & reflective Materials and uses interlacing to improve performance. It is, however, limited by a static camera, no caustics and no anti-aliasing.

![image](https://github.com/user-attachments/assets/1546ae61-9a36-4507-8b2c-34c722c333b8)

While limited, my ray tracer can visualize basic primitives like planes and spheres and factors simple light models like Blinn-Phong shading in. It also employs a simple 2D screenspace grid as an acceleration structure and uses OpenMP multithreading to increase performance. 
Since this is an entirely CPU based ray tracer, I employ another technique to accelerate the rendering process: Interlacing. While this would ruin the image quality in a ray tracer with a movable camera, it is optimal for improving the performance of a ray tracer with stationary cameras. The Idea behind interlacing is not to render/trace every line of pixels every frame, meaning that depending on the stride, the workload the CPU has to carry can be drastically reduced. However, this also means that some pixels are not updated every frame, resulting in artefacts of previous frames being shown when moving the camera in real-time.
