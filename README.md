# opencl-ca
Run cellular automatons as OpenCL kernels

## Usage
1. Install dependencies: `sudo pacman -S opencl-driver opencl-headers ocl-icd`
2. Edit `kernel.cl` to select a CA and space configuration. Example:
```opencl
// at line 4:
#define at(x, y) torus(board, board_width, x, y) // e.g. change torus to closed
#include "ca/life.cl" // e.g. change life.cl to majority.cl
```
3. Run with `make`
4. For the best experience, set your terminal font to [this](https://strlen.com/files/square.ttf)

## Controls
- q to quit
- Return to toggle automatic mode
- + and * to increase/double speed
- - and _ to decrease/halve speed
- Space to do one step in non-automatic mode

## References
- [clErrorString](./clutils.c#L3): from [this gist](https://gist.github.com/gavinb/5728887)
- [This example](https://www.olcf.ornl.gov/tutorials/cpu-vector-addition/) and its [associated repo](https://github.com/olcf/vector_addition_tutorials)
