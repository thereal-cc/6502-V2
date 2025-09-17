# 6502-V2

A simple 6502 Emulator written in C. This implements all 6502 instructions, addressing modes, stack operations, and interrupt handling, and can load raw binary programs into memory for execution.  

## Building

Follow these steps in order to build the emulator:

```bash
git clone 
cd 6502-V2
make clean & make
```

## Usage

By default, the program will load a Fibonnaci program into address 0x0000. If you want to load your own program, you can pass it in as a command-line argument, such as:

```bash
./bin/6502 <path to bin file>
```

## Future Plans

Ideally, I'd like to incorporate this into a variety of other emulators, such as the Apple 1, Commodore Pet, etc. I also plan on improving accuracy, specifically pausing emulation for the appropriate amount of cycles. Any feedback would be greatly appreciated, so [use this link](https://carson-cassidy.netlify.app/contact) to contact me.
