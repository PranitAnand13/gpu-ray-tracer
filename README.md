WEEK 1 tasks completed.
WEEK 3 tasks completed.
Till now i have learned basic gpu ray tracing, able to generate ppm files involving one or more spheres of different colors given by or calculated by various parameters, some new concepts i learnt worth mentioning are :
Move Semantics and Smart Pointers:
 a.Introduction to smart pointers and move semantics
 b.R-value references
 c.Move constructors and move assignment
 d.std::move
 e.std::unique_ptr
 f.std::shared_ptr
Virtual Functions:
 a.Pointers and references to the base class of derived objects
 b.Virtual functions and polymorphism
 c.The override and final specifiers, and covariant return types
 d.Virtual destructors, virtual assignment, and overriding virtualization
 e.Early binding and late binding
 f.etc
Also i have learnt basic sycl , through which i am able optimize the process of the generation of the ppm file.
Thos is the link to the GitHub repository on which we are given tasks and i have completed all tasks upto now: https://github.com/KeyaanKR/SOC25-GPU-based-Ray-Tracer
## After Mid-Term Checkpoint
WEEK 5 tasks completed.
Final-Ray-Tracer completed.
After the mid-term checkpoint, I added several features and improvements to the ray tracer.

I included a `Camera` class (which I later modified to support dynamic orientation and position). This helped in better controlling the view of the scene. I also added antialiasing, which smoothens out the edges and removes the sharp pixelated look.

Then, I learnt about and developed a `Material` class which could be used to create various materials like Lambertian, Metal, and Dielectric.

For the **Lambertian** material, I made several improvements such as:
- removing shadow acne,
- adding true Lambertian reflection,
- limiting the number of child rays or bounces to reduce unnecessary computation,
- and applying gamma correction to the output `.ppm` files.

Then I learnt about and developed the **Metal** material, which allowed the rendering of shiny metal spheres in different colors. I also added a **fuzz** feature to control how blurry or sharp the reflections appear.

After that, I learnt about and developed the **Dielectric** material, which enabled the rendering of glass spheres, hollow glass spheres, and even an air bubble in water. This involved implementing concepts like Snell’s Law, total internal reflection, and the Schlick Approximation for reflectivity.

I also added a **defocus blur** feature to simulate depth of field in the rendered image.

Finally, I generated a final scene that includes many spheres with various colors and material types.

Parallelly, I also developed a **GPU version** of the ray tracer using SYCL. This version included the same features (up to the dielectric material), although some parts were implemented differently due to differences in file structure. For example, the GPU version doesn’t have a separate `Material` class — the logic is integrated within the `sphere` and `ray_color` files.

