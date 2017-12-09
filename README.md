Swanson OS
==========

<p align="center">
  <img src="media/logo.svg" width="200px" height="200px"/>
</p>

[![Build Status](https://travis-ci.org/tay10r/swanson_os.svg?branch=master)](https://travis-ci.org/tay10r/swanson_os)
[![Build status](https://ci.appveyor.com/api/projects/status/9ydfcp7s3t6pp4u8/branch/master?svg=true)](https://ci.appveyor.com/project/tholberton/swanson-os/branch/master)

Swanson is an operating system built for virtualization.

It is intended to be a POSIX-complaint operating system that can run entirely in user space.

The idea is to have an operating system that can run on top of another operating system without much overhead.

### Motivation

It can be difficult using an operating system on a PC other than the one that was factory shipped.
Sometimes, installing the operating system may wreck the one that was factory shipped.
Other times, it may install successfully but the drivers for it won't work correctly.
And even if it does work okay, in a lot of cases it voids the warranty for the computer.

This is the case where virtualization comes in handy.
With technology like QEMU or VirtualBox, it's easy to run an operating system without wrecking the one that came with the computer.
But there's one more problem with this kind of virtualization, it's kind of slow.

That's where Swanson OS comes into play.
It's an experimental operating system design with a focus in compartmentalization and abstraction.

### Building

CMake is the primary build system.

Build the project by doing this:

```shell-script
mkdir build && cd build
cmake ..
cmake --build .
```

If you're developing the project in the terminal, QMake may be useful as well.
It generates Makefiles recursively, and creates a target for the test program.
This makes it easy to work inside the source directory, and run the test program as changes are made.

Use it like so:

```shell-script
qmake
cd src
# Edit stuff
make
make check
```

### Dependencies

Here's a list of dependencies for building the project.

| Name     | Comment                            | Required |
|----------|------------------------------------|----------|
| cmake    | Used for building the project.     | Yes      |
| doxygen  | Used for generating documentation. | No       |
| graphviz | Used for generating documentation. | No       |

It's assumed you will have either GCC, Clang or Microsoft Visual Studio already installed.

