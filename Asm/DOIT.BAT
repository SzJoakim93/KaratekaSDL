@echo off
set OLDPATH=%PATH%

set LC=\TOOLS\LC215E
set PATH=%LC%;%PATH%
set MODEL=S
set LIB=%LC%\%MODEL%
REM option -I doesn't work?
REM set INCLUDE=%LC%

REM goto TOLINK

REM -n is for "39 long symbol names"
REM -v is for "no stack check"

REM it seems that this version of Lattice wants
REM the include files to be in the current directory
copy %LC%\STDIO.H .
copy %LC%\FCNTL.H .

LC -n main.c
\TOOLS\MASM125 __0671.asm;
LC -n C_0fbb.c
\TOOLS\MASM125 C_1705.asm;
LC -n C_19F5.c
\TOOLS\MASM125 C_2329.asm;
LC -n C_268a.c
\TOOLS\MASM125 C_30A1.asm;
LC -n C_3233.c
\TOOLS\MASM125 C_3C41.asm;
LC -n -v C_414a.c
\TOOLS\MASM125 C_41BE.asm;

:TOLINK
\tools\link306 /MAP @karateka.rsp

set PATH=%OLDPATH%
set OLDPATH=
