# Tin Soldier Sample Code

## Build instructions

Navigate to the `TinSoldier/build` directory and build in command line:

`cl ../src/*.cpp -I../glm -I../include -o TinSoldier`

## Some notes

This sample contains the most basic functionality to demonstrate the ray marching rendering of a hardcoded scene and export the resulting image to a `.ppm` file. This was the state of the code at the time when I shared the smaller sample with you and I found it most appropriate to share the rest 'as it was'.

Since then I ported the ray marching functionality from this project into the framework I am also using to implement ray tracing and path tracing. With that some things have changed compared to this 'older' version, to better accomodate these methods:

1. `Ray` is now a seperate class, instead of a child of `Camera`. This is naturally more appropriate as the `Ray` object is more universal than just the primary camera ray.
2. The `Camera` class has been considerably refactored; having isolated the computation of the axes for `Camera` movement and ray casting, the `Camera` object is now easily configurable at runtime and the implementation is more readable. Also it now has support for anti-aliasing and depth of field + auto-focusing at runtime.
3. The struct `SDFQuery` is now generalized as `Query`, a child of the `Scene` class, as this struct holds data shared between the different rendering algorithms.
4. All PBR properties of `Components` are now refactored into a `Material` class. For most primitives both an intersection function and a Signed Distance Field exist, so only one implementation of `Components` suffices.

Also because of this shift in focus for the project the documentation for some functionality is not as complete.

I hope you find this helpful, and again thank you all for your time.

Best,

Martijn