# CCHIP-8

⚠️Work in progress⚠️

My other CHIP-8 emulator project (in Rust): https://github.com/mezei-peter/RCHIP-8

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

I included a version of Pong. The frame rate is not controlled yet, so it might run too quickly. Control with the **1, Q** and the **4, R** keys.
```
make
make run ARGS=roms/Pong-Paul_Vervalin-1990.ch8
```
