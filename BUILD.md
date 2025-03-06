# Building PlanPro Viewer

PlanPro Viewer can be built on Windows, Linux and macOS platforms.


## Requirements

To build PlanPro Viewer, you will need:

- Qt 6.4.0 or higher
  - On Windows and macOS it is easiest to download the Online Installer for Qt Open Source from [https://www.qt.io](https://www.qt.io) (Qt Account required).
  - On Linux use the Qt version from the Package Manager of your distribution.
  - Make sure that all required modules (Core, GUI, XML, Widgets, SVG and Print Support) and the tools (like qmake, moc, uic, rcc) are installed.
- A C++ compiler and toolchain supported by your Qt version
  - On Windows it is recommended to use [Microsoft Visual Studio](https://visualstudio.microsoft.com) (Community Edition is sufficient).
  - On macOS, use [Apple Xcode](https://developer.apple.com).
  - On Linux, use GCC or Clang provided by your Package Manager.

Make sure that all tools are in your system's PATH environment variable.

To create the installation program on Windows, you additionally need [Inno Setup](https://jrsoftware.org/isinfo.php) version 6.4.0 or higher.


## Compiling the Sources

Open a command line, navigate to the `src` subdirectory inside your project's root directory and enter `qmake`.
This will generate the Makefiles for your system.
After that, enter `make` on Linux/macOS or `nmake` on Windows.



## Installation and Deployment

To get PlanPro Viewer working properly and allow distribution to computers without the build environment, it is necessary to include the runtime libraries and resources into an application bundle.
The needed steps are different depending on your platform.

### Windows

After you have compiled the sources, enter `nmake release-windeployqt` (still in the `src` subdirectory).
This will copy all needed resource files and runtime DLLs into the `release` subdirectory.

To simplify installation and distribution, we have created a script to build an installation program using the Inno Setup Compiler.
The script is named `ppview.iss` and is located in the the `install/windows` subdirectory inside your project's root directory.
Open the Inno Setup IDE, load the script and compile it.
The resulting installation program is located in the `build` subdirectory and named `ppview-x.y.z-setup.exe` (where x.y.z are the version numbers).

On the target machine, the setup program installs all needed files, creates start menu entries and associates the *.ppxml file extension with PlanPro Viewer.
The Visual C++ Runtime is installed globally, therefore the setup program needs to be executed with administrator privileges.

### macOS

After you have compiled the sources, enter `make macdeploy` (still in the `src` subdirectory).
This will copy all needed files into the application bundle.
The bundle (`PlanPro Viewer.app`) can now be deployed to other machines.
Double-click on the icon to run PlanPro Viewer.

### Linux

On Linux, it is common practice to install the runtime libraries system-wide.

To install PlanPro Viewer on your development machine, enter `make install` (still in the `src` subdirectory).
This will install PlanPro Viewer and all needed resources in `/usr/local/ppview`.
To install to a different location, you have to set the variable `PPVIEW_INSTALL_PATH` during the invocation of `qmake`.
For example, to install to `/opt/var/ppview`, you have to enter `qmake "PPVIEW_INSTALL_PATH=/opt/var/ppview"`.
After that, `make install` copies the files to the new location.

Please not that `make install` needs superuser rights when installing to a system location (outside of your home directory).

To deploy it to another machine, you have to copy the `ppview` directory to the target machine.
It can be installed in any directory.
Use the Package manager to install the Qt runtimes for each used module (Core, GUI, XML, Widgets, SVG and Print Support) and the Qt system translation files.
Please note that PlanPro Viewer expects the Qt system translation files in `/usr/share/qt6/translations` (which is the case on most Linux distributions).

To run PlanPro Viewer, open a command line, navigate to the directory where you have installed it and enter `./ppview`.
It is recommended to create a symbolic link in one of your system's `bin` directories.
