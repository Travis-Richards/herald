Building on Ubuntu
==================

To first ensure that all the dependencies are on the system, run the following commands:

```
sudo apt update
sudo apt install qt5-default g++ cmake make
```

Once those commands complete, enter the project directory and create a subdirectory called "build" and enter it.

```
mkdir build
cd build
```

Now, you'll use CMake to do some configuration and ensure everything on the system is present.

```
cmake ..
```

Once that's done, you should see some new files and folders in your current directory.
One of those files should be called "Makefile", which means everything completed successfully.
Then, you can compile all the source files by running this command:

```
make
```

When that's over, there should be an executable called "herald" in your current directory.
You can run it from there or install it to the system using this command:

```
sudo make install
```
