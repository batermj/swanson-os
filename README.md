Swanson OS
==========

Swanson is an operating system built for virtualization.

It is intended to be a POSIX-complaint operating system that can run entirely in user space.

The idea is to have an operating system that can run on top of another operating system without much overhead.

### Building

CMake is the primary build system.

Build the project by doing this:

```
mkdir build && cd build
cmake ..
cmake --build .
```

### Dependencies

Here's a list of dependencies for building the project.

| Name     | Comment                            | Required |
|----------|------------------------------------|----------|
| cmake    | Used for building the project.     | Yes      |
| doxygen  | Used for generating documentation. | No       |
| graphviz | Used for generating documentation. | No       |

It's assumed you will have either GCC, Clang or Microsoft Visual Studio already installed.

