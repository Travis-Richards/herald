Building on Windows
===================

Building on Windows requires a little bit more space than other systems.
You'll have to download and install the following software.

 - [CMake](https://cmake.org/download/)
 - [Visual Studio](https://visualstudio.microsoft.com/downloads/)
 - [Qt](https://www.qt.io/download)

Note that there are official pre-compiled downloads of this project that eliminate the need to download all the dependencies.

If you'd still like to build from scratch, install the above software and entire the project directory in the command line.

Also ensure that, before opening the command line, that you have `cmake.exe` in your PATH environment variable.

Create a subdirectory in the project called "build" and enter it.

```
mkdir build
cd build
```

Use CMake to generate the files for Visual Studio

```
cmake ..
```

Then, build the project.

```
cmake --build . --config Release
```

If that command completes successfully, then you'll have `herald.exe` in `Release\herald.exe`.
