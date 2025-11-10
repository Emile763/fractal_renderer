# Global Description
Project from 2021.\
This small app lets you explore the fascinating world of two famous fractals: the Mandelbrot set and the Julia set.

A fractal found in the Julia set:
<img width="1920" height="1080" alt="Julia Screen" src="https://github.com/user-attachments/assets/fbf1d9c5-ca69-4758-9e72-1100a8fcbf04" />
A fractal found in the Mandelbrot set:
<img width="1920" height="1080" alt="Mandelbrot Screen" src="https://github.com/user-attachments/assets/382459cf-701c-42f9-8fff-4fcecbc0b683" />

# How To Compile
At the root of the repository:
```console
mkdir build
cd build
cmake ..
cmake --build .
# To launch
./RenderApp
```

# How To Use

<kbd>Z</kbd><kbd>Q</kbd><kbd>S</kbd><kbd>D</kbd> : Move around\
<kbd>&uarr;</kbd><kbd>&darr;</kbd> : Zoom in and out\
<kbd>&rarr;</kbd><kbd>&larr;</kbd> : Refine/Coarsen the rendering (be careful when refining, as it can lead to performance issues)\
<kbd>Right Click</kbd> : Change the colors based on the cursor position\
<kbd>Space</kbd> : Switch to the other fractal set\
<kbd>A</kbd><kbd>E</kbd> : Change the power of the Mandelbrot fractal\
<kbd>Left Click</kbd> : Change the Julia fractal based on the cursor position

