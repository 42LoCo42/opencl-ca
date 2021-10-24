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
