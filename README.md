# Karateka SDL

**2024/05/30 — ergonomy_joe**

Karateka SDL is an SDL port based on the decompilation of the MS-DOS version of **KARATEKA** by Jordan Mechner, created by **ergonomy_joe**.

The game uses an SDL-based bridge to replace the original DOS routines. All assembly code has been rewritten in C. The original assembly source files are preserved for reference only and are no longer required to compile the game.

## Status

* The game is 100% playable.
* All known original gameplay features from the DOS version are working.
* Pressing **Escape** opens a pause menu with Resume, Settings, and Quit.
* The original PC speaker sounds are redirected to the sound card.
* Keyboard and gamepad input are supported through SDL.

## Usage

### Run the game

Before running the game, copy the following asset files from the original game into the root folder:

* `ALLAL`
* `ALLCAL`
* `ALLGAL`
* `ALLPAL`
* `ALLVAL`
* `BAL**` files
* `CAL**` files
* `CASTLE.BCG`
* `FUJI.BCG`
* `KM*.DAT` files
* `KM*.IND` files
* `KMI*.DAT` files
* `KMI*.IND` files
* `KMJ*.DAT` files
* `KMJ*.IND` files
* `KS*.DAT` files
* `KS*.IND` files
* `KSI*.DAT` files
* `KSI*.IND` files
* `KSJ*.DAT` files
* `KSJ*.IND` files
* `PRNGAL`
* `TITLE.BCG`

Then start the game:

* **Windows:** Run `Karateka-SDL.exe`
* **Linux:** Run `./Karateka-SDL`

### Controls

The game supports both keyboard and gamepad input.

| Keyboard     | Gamepad            | Action                                                           |
| ------------ | ------------------ | ---------------------------------------------------------------- |
| Space        | Button A           | Start a new game (menu); switch between attack and running modes |
| Left / Right | D-pad / left stick | Move left / right                                                |
| X            | Button X           | Kick                                                             |
| Y            | Button Y           | Hit                                                              |
| Escape       | Start              | Open the pause menu                                              |

> **Note:** Gamepad button names may vary depending on the device. See the SDL joystick mapping below for the exact button numbers.

In the pause and settings menus, use **Up/Down** or the gamepad direction controls to navigate. Use **Space/Enter** or gamepad button A to select. In Settings, use **Left/Right** to change the selected value; Escape returns to the pause menu. Available resolutions cycle through common window sizes, and changed settings are saved to `settings.ini`.

## Building

### Windows

1. Clone the repository.
2. Download a 32-bit MinGW toolchain:

   * [WinLibs](https://winlibs.com/#download-release)
   * Choose the MSVCRT runtime.
   * For retro computers, use MinGW 4.7.1:
     [MinGW 4.7.1](https://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.7.1/32-bit/threads-win32/dwarf/)

     This is the last version that supports Windows 95.
3. Extract MinGW.
4. Download the SDL 1.2 development package:
   [SDL 1.2.15 for Windows](https://libsdl.org/release/SDL-1.2.15-win32.zip)
5. Copy the `include/SDL` folder into `MinGW/include` (copy the entire `SDL` folder, not only its contents).
6. Copy the contents of the SDL `lib` directory into `MinGW/lib`.
7. Copy `bin/SDL.dll` into the root directory of the repository.
8. Open a command prompt in the repository root.
9. Run:

```bash
mingw32-make
```

10. Start the game with:

```text
Karateka-SDL.exe
```

### Linux

1. Clone the repository.
2. Install GCC if it is not already installed:

```bash
sudo apt-get update
sudo apt install gcc
```

3. Install the SDL 1.2 development libraries:

```bash
sudo apt install libsdl1.2-dev
```

4. Navigate to the repository root.
5. Build the game:

```bash
make
```

6. Run the game:

```bash
./Karateka-SDL
```

## SDL Joystick Controls

The SDL 1.2 port uses the first connected joystick or gamepad.

The D-pad or left analog stick can be used to control movement. These inputs are mapped to the corresponding arrow-key controls.

Joystick buttons are mapped as follows:

| Button | Keyboard |
| ------ | -------- |
| 0      | Space    |
| 1      | A        |
| 2      | Z        |
| 3      | X        |
| 4      | W        |
| 5      | S        |
| 6      | Q        |
| 7      | B        |
| 8      | 0        |

Button numbering follows the SDL 1.2 joystick API and may vary depending on the device and driver.

## Original Game Assets

The original game assets are **not included** in this repository. You must provide them from your own copy of the original MS-DOS game.

Please do not redistribute copyrighted game assets with this project.
