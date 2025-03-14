### README
___
### Description

A simple Chip-8 interpreter written as an introduction to coding emulators. Graphics are displayed using SDL.

The emulator supports toggling certain behaviors for ambiguous instructions, but doing so can cause certain roms to behave unexpectedly, or crash.
It also supports pausing, changing speed, and toggling debug mode. All of this can be done during runtime.

I am almost certain that not everything works as expected, and I am trying to fix bugs and improve poor implementations as I spot them.

##### To-do

- [ ] Add sound
- [ ] Improve terminal output
- [ ] Add additional customizations
- [ ] Improve comments

___
### Usage

The program is compiled using `make`. It will compile into an executable called `run`. To run the emulator on Linux, use `./run <PATH_TO_ROM> <DEBUG_FLAG>`.
It is not necessary to provide a debug flag. The program will run with only the file path, with debug disabled by default.

##### Controls

- `SPACE`:   Pause/Resume
- `ENTER`:   Restart
- `ESCAPE`:  Exit
- `F1`:      Toggle debug
- `F10`:     Toggle shift behavior
- `F11`:     Toggle jump with offset behavior
- `F12`:     Toggle store / load behavior
- `MINUS`:   Decrease speed
- `PLUS`:    Increase speed

Control of the keypad defaults to using 1-4, Q-R, A-F, and Z-V. Support for different control schemes is not currently planned.
