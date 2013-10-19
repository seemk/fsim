## 2D Fluid rendering


Fluid rendering testbed

Tools required to build:

[cmake 2.8+](http://www.cmake.org/cmake/resources/software.html)
### Building
#### Windows. Using Visual C++, required compiler version VC12:

<pre>
mkdir build && cd build  
</pre>

32 bit: 
<pre>
cmake fsim_root_directory
</pre>

64 bit:
<pre>
cmake -G "Visual Studio 12 Win64" fsim_root_directory
</pre>

Use the generated Visual Studio project to build the executables.

**NOTE:** The debugger path doesn't contain the shader folder, run it from the bin/ directory.

#### Linux. GCC 4.7+, Clang (untested):
<pre>
mkdir build && cd build
cmake fsim_root_directory
make
</pre>

The resulting binary after building is in the build/bin/ folder.
