This is the Resolume fork of the FFGL repository. It is up to date and has Visual Studio and Xcode projects to compile 32 bit and 64 bit plugins.

The master branch is used for continued development. It will contain the latest features, fixes and bugs. Plugins compiled with the master branch will work in Resolume 7.0.3 and up.
If you do not want to be affected by the latest bugs you can use a different branch. eg the FFGL 2.1 branch, which is the most recent released version of the sdk: https://github.com/resolume/ffgl/tree/ffgl2.1. To compile plugins for resolume 7.0.0/7.0.1/7.0.2 you will need to use the ffgl2.0 sdk because these hosts do not support the new features introduced in FFGL 2.1.

To compile plugins for hosts that support FFGL 1.6 (Resolume 6, VDMX, COGE, Isadora, Magic Music Visuals), switch to the FFGL1.6 branch: https://github.com/resolume/ffgl/tree/ffgl1.6

You can find some help to get started with FFGL plugin development on the [wiki](https://github.com/resolume/ffgl/wiki).

Also more examples are available on this [repo](https://github.com/flyingrub/ffgl/tree/more/).

## Changes since FFGL 2.1
- Added context state validation in debug builds. This provides plugin developers hints on which context state they need to restore.
- Removed default DllMain implementation so that plugins may implement it without changing the ffgl library.
- File parameters now accept an initial value just like text parameters. (This requires Resolume 7.2 for it to be picked up)


## Quickstart

Below are the first steps you need to create and test an FFGL plugin for Resolume. This assumes you have experience with git and C++ development.

### Mac

- Go to `<repo>/build/osx`, open `FFGLPlugins.xcodeproj`
- Create a compilation target for your plugin:
	- Select the Xcode project (top of the tree)
	- Duplicate a target and rename it
	- Remove the old plugin-specific files under Build Phases > Compile Sources (e.g. if you duplicated Gradients, remove `FFGLGradients.cpp`)
	- Duplicating a target in Xcode creates and assigns a new `xx copy-Info.plist` file, but we don't want that. Go to Build Settings > Packaging > Info.plist and change the file name to `FFGLPlugin-Info.plist`.  
	- Find the reference to the newly created `xx copy-Info.plist` file in the Xcode Project Navigator (probably all the way down the panel) and remove it there. When asked, choose Move to Trash.
- In Finder, duplicate a plugin folder and rename the files. Choose a corresponding plugin type, e.g. copy `AddSubtract` if you want to build an Effect plugin or `Gradients` if you want to build a Source plugin.
- Drag the new folder into the Xcode project. You will be asked to which target you want to add them, add them to your new target.
- Go to the target's Build Phases again and make sure there are no resources under the Copy Bundle Resources phase.
- Replace the class names to match your new plugin name and rename the elements in the PluginInfo struct
- Fix up the Build scheme:
	- When duplicating a target, a Build Scheme was also created. Next to the play and stop buttons, click the schemes dropdown and select Manage Schemes. 
	- Rename the scheme that was auto-created (e.g. "Gradient copy")
	- Select it in the scheme drop down.
- Press play (Cmd+B) to compile.
- Copy the resulting `.bundle` file from `<repo>/binaries/debug` to `~/Documents/Resolume/Extra Effects` and start Arena to test it.

### Windows 

This assumes you use Visual Studio 2017

- Go to `<repo>/build/windows`, duplicate a `.vcxproj` and the corresponding `.vcxproj.filters` file, and rename them.
- Open `FFGLPlugins.sln`. Then right-click the Solution in the solution explorer (top of the tree), and choose Add > Existing Project and select the new file.
- Remove the original `.cpp` and `.h` source files from the newly added project, i.e. if you duplicated `Gradient.vcxproj`, remove `FFGLGradients.h` and `FFGLGradients.cpp`
- In Explorer, go to `<repo>/source/`, duplicate a plugin folder and rename the files. Choose a corresponding plugin type, i.e. copy `AddSubtract` if you want to build an Effect plugin or `Gradients` if you want to build a Source plugin.
- Add the new source files to the project by dragging them into Visual Studio, onto your new project.
- If you want to start the build with Visual Studio's Build command (F5), right-click the project and select Set as Startup Project. Altenatively, you can right-click the project and select Build.
- After building, find the resulting `.dll` file in `\binaries\x64\Debug`. Copy it to `<user folder>/Documents/Resolume/Extra Effects`

## Convenience tips

- Create a symlink from the build binaries folder to Resolume's Extra Effects folder. This way, new plugins will be available as soon as you start Arena after compiling without the need to manually drag them over.
	1. Remove the existing _~/Documents/Resolume Arena/Extra Effects_ (perhaps check that it is empty first).
	2. Create the symlink: 
		- **Mac**: `ln -s <repo>/binaries/debug ~/Documents/Resolume\ Arena/Extra\ Effects`. 
		- **Windows 10**: `mklink /J "<user folder>\Documents\Resolume Arena\Extra Effects" <repo>\binaries\x64\Debug`

- Automatically start Arena after a successfull compile so that pressing Cmd + R (Mac) or F5 (Windows) compiles your plugin and starts Arena immediately.

	**Mac**
	
	- With your scheme selected in the scheme drop down, click it again and select Edit scheme.
	- Under Run > Executabe, browse and select Arena.app.
	- Disable Debug executable.
	- If you want to disable the splash screen when starting Arena, go to the Arguments tab, under Arguments Passed On Launch, press the + and add `--nosplash`

	**Windows**
	
	- Right click the project > properties > Debugging
	- For Command, fill the path to Arena, usually `C:\Program Files\Resolume Arena\Arena.exe`
	- For Working Directory, fill in the program folder, usually `C:\Program Files\Resolume Arena\`
	- If you want to disable the splash screen when starting Arena, for Command Arguments, add `--nosplash`
	
- In Arena, save a composition with a single layer and a single source that has your plugin applied so that it is loaded on launch and is ready to test right away.

## Logs

- Check if the plugin is loaded correctly in Resolume by going to Preferences > Feedback > View Log
- Viewing GLSL error logs is only possible if Arena is started by your IDE after compiling (see Convenience tips).
	- **Mac**: GLSL errors will be printed to the Xcode output window when the plugin is used after Arena has launched.
	- **Windows**: GLSL errors will be printed to the Visual Studio output window when the plugin is used after Arena has launched. 
