This is the Resolume fork of the FFGL repository. It is up to date and has Visual Studio and Xcode projects to compile 32 bit and 64 bit plugins.

This FFGL 2.1 branch contains the 2.1 version of the FFGL SDK. It contains some often requested new features and fixes to issues present in the FFGL 2.0 branch. The new features introduced in FFGL 2.1 require support by the host application. If you're using Resolume you will need version 7.0.3 to get host support for these new features.. If you want to use the latest development version with the most recent fixes and features you should use the master branch.

To compile plugins for hosts that support FFGL 1.6 (Resolume 6, VDMX, COGE, Isadora, Magic Music Visuals), switch to the FFGL1.6 branch: https://github.com/resolume/ffgl/tree/ffgl1.6

You can find some help to get started with FFGL plugin development on the [wiki](https://github.com/resolume/ffgl/wiki).

Also more examples are available on this [repo](https://github.com/flyingrub/ffgl/tree/more/).

## What's changed in FFGL 2.1
- Added support for embedding a thumbnail inside the plugin. See the "CustomThumbnail" example on how to expose your thumbnail.
- Added support for File parameters. The plugin can create a file param and specify what extensions it supports, the host then has enough information to show a file chooser dialog with active extension filtering.
- Added support for toggling parameter visibility.
- Added support for parameter visibility events. (A plugin can now toggle visibility of a parameter on the fly)
- Reintroduction of Scoped bindings. (Removed unsafe resetOpenGLState function, use the ScopedBinding classes instead)
- The Module Definition File is no longer required on Windows.
- The SDK no longer adjusts any textures to use GL_CLAMP_TO_BORDER.
- Renamed CFreeFrameGLPlugin class to CFFGLPlugin. Plugins now need to inherit from the CFFGLPlugin class.
- Fixed param value being normalized when ranges are involved. Values going between host and plugins are now the actual values, not the phase within that parameter's range.
- Fixed some internal SDK warnings, convoluted implementations and inconsistencies in namings. (Most notably the quickstart module now uses the same conventions as the ffgl and ffglex modules)
