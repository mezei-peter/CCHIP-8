# CCHIP-8

⚠️Work in progress⚠️

My other CHIP-8 emulator project (in Rust): https://github.com/mezei-peter/RCHIP-8

## Introduction

The aim of this personal project is to create an interpreter and an emulated system so that binary .ch8 files turn into runnable programs.

CHIP-8 is an interpreted programming language. It is compiled into binary .ch8 files containing 2-byte long instructions that get interpreted by the CHIP-8 interpreter which gives instructions to the computer's processor. The instructions are in big-endian byte order. Some examples:

- 00E0 - Clear screen
- 1NNN - Set the program counter(16-bit register) to NNN(12-bit memory address)
- 6XNN - Set the xth variable register(an 8-bit register) to NN(8-bit value)
- 7XNN - Add to the xth variable register(an 8-bit register) the value NN(8-bit value)
- ANNN - Set the index register(16-bit register) to NNN(12-bit memory address)
- DXYN - Draw to the screen an N byte long(N pixel tall) sprite starting from the memory address stored in the index register(16-bit register) at coordinates stored by the xth and yth variable registers(8-bit registers). The screen is 64*32 pixels, so the coordinates fit in 1 byte each.

Currently, the main part of the emulator is finished and it runs programs. However, the frame rate is not controlled correctly yet, so some games might run too quickly or too slowly.

## Demo

Pong.

![screenshot of RCHIP-8 running Pong](https://i.imgur.com/xuzAkDZ.png)

corax89's test ROM extended by Timendus.

![screenshot of running a test rom](https://i.imgur.com/w40W7T7.png)


## Requirements
The project is mainly designed for a GNU/Linux system. It requires **gcc**, **make** and the **sdl2** library. On Ubuntu run these commands to install them:
```
sudo apt update
sudo apt install build-essential
sudo apt install make
sudo apt install libsdl2-dev
```

## Usage
The program can be built and run with **make**. It requires a .ch8 **file path** as an argument in *ARGS*. I provided some example ROMS in the *roms/* directory.
```
make
make run ARGS=roms/1-chip8-logo.ch8
```

I included a version of Pong. Control with the **1, Q** and the **4, R** keys.
```
make
make run ARGS=roms/Pong-Paul_Vervalin-1990.ch8
```
