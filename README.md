# karateka-decompiled

Greetings retrocoders fellows !

This repository is the result of a decompilation process I did a few years ago.
It allows you to build the executable `KARATEKA.EXE`, from the MSDOS version of the legendary game KARATEKA by Jordan Mechner.
Jordan [stated](https://x.com/jmechner/status/1795693958950993951) that he was "deeply disappointed" with this version, and I totally understand why.

Anyway; this version is part asm routines, part C source code.
The C part was compiled with the Lattice C compiler version 2.15; it seems that there are at least two versions of this compiler that shares the version number 2.15 and I cannot tell what are the differences. We would need a Lattice C expert to explain that to us.
I use masm (microsoft macro assembler) version 1.25 to build the asm part but I guess it might work with other versions too.

I created a batch file `DOIT.BAT` which allows to build the executable; you may want to modify it to fit your own configuration.

File names are not explicit, they are the segment in memory at which the modules are loaded(in my configuration) and it allows you to see in which order they are linked in the final executable.
Same for the variables/functions names; most of the time I name them with their offset in memory ... which doesn't help the comprehension if I must agree.

Anyway, we have the algorithms and the structures, and that  might be enough for thoses who want to have an insight of game programming back in the 80s.
In Karateka's case, the most interesting part, in my opinion, is the script engine that manages the cutscene animation. By mastering it you might even be able to creta you own animations, why not ?

## SDL joystick controls

The SDL 1.2 port supports the first connected joystick or gamepad. Use the D-pad
or left stick to move (left/right/up/down map to the arrow-key controls).
Joystick buttons 0-8 map to Space, A, Z, X, W, S, Q, B, and 0, respectively.
Button numbering follows SDL 1.2's joystick API and may vary by device.

I whish you a lot of decompiling fun !  
2024/05/30 ergonomy_joe
