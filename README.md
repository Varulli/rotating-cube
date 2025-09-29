# rotating-cube

A render of a cube rotating in space with a light source. (Note: resizing console window may be required to avoid jittering)

Download the [latest release](https://github.com/Varulli/rotating-cube/releases/latest) or [build it yourself](https://github.com/Varulli/rotating-cube#build-instructions).

![rotating-cube-demo](https://github.com/user-attachments/assets/0fa36bd2-5856-481d-a5e7-215bd7238720)

## Build Instructions

### Requirements
- C compiler
- CMake 3.5+

### Steps
```
git clone https://github.com/Varulli/rotating-cube.git
cd rotating-cube
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
Run executable directly, or:
```
./rotating-cube [seconds]  # Runs for some time (default: 10)
```
