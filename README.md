# Vult for Daisy Seed
An example of using Vult-generated code with the Daisy Seed, based on the [Teensy Example](https://github.com/modlfo/teensy-vult-example).

## How to use

### Configure:
```
$ cmake -B build -G Ninja -D FETCHCONTENT_QUIET=OFF -D CMAKE_BUILD_TYPE=Debug
```

### Build:
```
$ cmake --build build --target vult_demo-bin
```

### Upload
The output binary is at `build/vult_demo.bin`. It can be uploaded to the Seed using the [Daisy Web Programmer](https://electro-smith.github.io/Programmer/)

The available USB-MIDI messages are:
- MIDI note on/off in any channel to control the pitch
- Control Change (CC31) to control the detune parameter of the oscillator

The source of the oscillator is the file phasedist.vult.
