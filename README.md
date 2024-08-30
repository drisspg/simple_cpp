# Simple CPP

![surfing](https://github.com/drisspg/simple_cuda/assets/32754868/4f8d27e2-dcc2-40a3-9aff-a814340878e4)


Rando c++ stuff


## Structure

Code goes into 2 places. I plan on figuring out what utilities and generals things I want to that are agnostic to individual examples. These will go into src/.
They will go into one of two places, currently this header only so I plan on putting stuff src/include. But I want to learn more about libraries so I might put some stuff in src/ and create a library that the examples will depend on.

The examples will go into examples/. I plan on using the CMakeLists.txt in the root to build all the examples. By default cmake will build every example, which is fine for now. I might add a way to build a specific example later.


## Building

```Shell
# Build all examples in debug
make debug

# Builds all in release
make release
```

This will build all the examples and put the executables in build/bin.

### Example E2E run for the tiled_mm example in chapter5
```Shell
make debug

./build/bin/hello_world

```
### Dependencies 
- Cmake
- Ninja
- c++=std23 compilier
