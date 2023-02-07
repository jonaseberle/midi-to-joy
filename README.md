# midi-to-joy

A user-space application that creates a standard Linux joystick via `/dev/uinput` and drives its 
axii and buttons from MIDI device inputs.

It is currently not very configurable and just creates 24 buttons and 32 axii for a 
Launchcontrol XL.

## Usage

```
./midi-to-joy </dev/midiX>
  /dev/midiX    The port where the LaunchControl XL is connected
```

## Build

```
mkdir -p build/ && cd build/
cmake .. && make
```
