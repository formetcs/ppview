# Building PlanPro Viewer

PlanPro Viewer can be built on Windows, Linux and macOS platforms.
Because automated installation scripts are still missing, some steps have to be done manually.


## Requirements

To build PlanPro Viewer, you will need:

- Qt 6.4.0 or higher
  - On Windows and macOS it is easiest to download the Online Installer for Qt Open Source from [https://www.qt.io](https://www.qt.io) (Qt Account required).
  - On Linux use the Qt version from the Package Manager of your distribution.
  - Make sure that all required modules (Core, GUI, XML, Widgets and Print Support) and the tools (like qmake, moc, uic, rcc) are installed.
- A C++ compiler and toolchain supported by your Qt version
  - On Windows it is recommended to use [Microsoft Visual Studio](https://visualstudio.microsoft.com) (Community Edition is sufficient).
  - On macOS, use [Apple Xcode](https://developer.apple.com).
  - On Linux, use GCC or Clang provided by your Package Manager.

Make sure that all tools are in your system's PATH environment variable.


## Compiling the Sources

Open a command line, navigate to the `src` subdirectory inside your project's root directory and enter `qmake`.
This will generate the Makefiles for your system.
After that, enter `make` on Linux/macOS or `nmake` on Windows.


## Generation of Translation Files

PlanPro Viewer uses dynamically loadable translation files.
To generate them, enter `lrelease ppview.pro` (still in the `src` subdirectory).
This will create a QM file for each translation (`ppview_*.qm`).


## Creating Deployable Distributions

To get PlanPro Viewer working properly and allow distribution to computers without the build environment, it is necessary to include the runtime libraries and resources into an application bundle.

On Windows, create an empty directory and copy the executable (`ppview.exe`) into it.
Open a command line, navigate to this directory and enter `windeployqt ppview.exe`.
This will copy the Qt and compiler runtimes into this directory.
After that, copy the QM files created in the previous step into the `translations` subdirectory.
Finally, copy the files `LICENSE`, `README.md` and `third-party-licenses.txt` from the project root directory into the distribution root directory (where `ppview.exe` is).
The whole directory can now be deployed to other machines.
Double-click on `ppview.exe` to run PlanPro Viewer.
If you get a message that the Visual Studio runtimes are missing, run the installation program which has also been copied into your distribution directory.

On macOS, open a command line, navigate to the directory where the application (`PlanPro Viewer.app`) has been created and enter `macdeployqt PlanPro Viewer.app`.
This will copy the Qt and compiler runtimes into this application bundle.
In contrast to Windows, the system translation files are not copied.
Locate the directory where the translations are stored in your Qt installation (usually a `translations` subdirectory, containing `qt_*.qm` files).
Copy the whole `translations` subdirectory into `PlanPro Viewer.app/Contents/Resources/translation`.
After that, copy your QM files created in a previous step into the `PlanPro Viewer.app/Contents/Resources/translation` subdirectory.
Finally, copy the files `LICENSE`, `README.md` and `third-party-licenses.txt` from the project root directory into `PlanPro Viewer.app/Contents/Resources`.
The bundle (`PlanPro Viewer.app`) can now be deployed to other machines.
Double-click on the icon to run PlanPro Viewer.

On Linux, it is common practice to install the runtime libraries system-wide.
Use the Package manager to install the Qt runtimes for each used module (Core, GUI, XML, Widgets and Print Support) and the Qt system translation files.
The created binary (`ppview`) can be installed in any directory.
The files `LICENSE`, `README.md` and `third-party-licenses.txt` must be in the same directory as `ppview`, and the created QM files in a `translations` subdirectory.
Please note that the PlanPro Viewer expects the Qt system translation files in `/usr/share/qt6/translations`.
To run PlanPro Viewer, open a command line, navigate to the directory where you have installed it and enter `./ppview`.
