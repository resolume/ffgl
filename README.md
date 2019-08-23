This is the Resolume fork of the FFGL repository. It is up to date and has Visual Studio and Xcode projects to compile 32 bit and 64 bit plugins.

The master branch is used for continued development. It will contain the latest features, fixes and bugs. Plugins compiled with the master branch will work in Resolume 7.
If you do not want to be affected by the latest bugs you can use a different branch. eg the FFGL 2.0 branch, which is the FFGL SDK as it was when we just released Resolume 7: https://github.com/resolume/ffgl/tree/ffgl2.0.  
See the changes below for information on what has changed since the FFGL 2.0 branch.

To compile plugins for hosts that support FFGL 1.6 (Resolume 6, VDMX, COGE, Isadora, Magic Music Visuals), switch to the FFGL1.6 branch: https://github.com/resolume/ffgl/tree/ffgl1.6

You can find some help to get started with FFGL plugin development on the [wiki](https://github.com/resolume/ffgl/wiki).

Also more examples are available on this [repo](https://github.com/flyingrub/ffgl/tree/more/).

## Changes since FFGL 2.0
- Reintroduction of Scoped bindings. (Removed unsafe resetOpenGLState function, use the ScopedBinding classes instead)
- The Module Definition File is no longer required on Windows.
- The SDK no longer adjusts any textures to use GL_CLAMP_TO_BORDER.
- Fixed some internal SDK warnings, convoluted implementations and inconsistencies in namings. (Most notably the quickstart module now uses the same conventions as the ffgl and ffglex modules)
