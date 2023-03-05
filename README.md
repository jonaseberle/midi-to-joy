# midi-to-joy

A user-space application that creates standard Linux joysticks via `/dev/uinput` and drives its 
axii and buttons from a MIDI device.

It is currently not very configurable and just creates 2 virtual joysticks for a LaunchControl XL. It expects the "Factory Preset 1" to be active.
* joystick 1 with 24 buttons and 8 axii (the lower sliders)
* joystick 2 with 24 axii (the upper turn dials)

## Usage

```
./midi-to-joy </dev/snd/midiC?D?>
  /dev/snd/midiC?D?    The port where the LaunchControl XL is connected (there are 2 - in my case the first one is correct)
```

## Build

```
mkdir -p build/ && cd build/
cmake .. && make
```
