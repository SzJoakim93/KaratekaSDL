;========================================
;	KARATEKA
;	Copyright 1986 Jordan Mechner
;	IBM version by The Connelley Group
;	reverse-coded by ergonomy_joe 2022
;========================================
INCLUDE COMMON.MAC

EXTRN D_00F2:WORD
EXTRN D_00FA:WORD
EXTRN D_010C:WORD
EXTRN enemyClamp:WORD
EXTRN D_0130:WORD
EXTRN D_0168:WORD	;allows CTRL+V?
EXTRN D_016A:WORD	;allows CTRL+R/'J'/F1?
EXTRN cga_buffer:BYTE

EXTRN D_B9BE:WORD
EXTRN D_B9C0:BYTE

EXTRN playerPosClamp:WORD

EXTRN D_D43A:WORD
EXTRN D_D442:WORD

EXTRN D_D54C:BYTE	;sound "conf"
EXTRN D_D59E:BYTE	;TODO:should consider D_D59E like words
EXTRN D_D5A8:WORD

EXTRN BB_flip:NEAR	;0DD1
EXTRN C_1F43:NEAR
EXTRN C_1F86:NEAR
EXTRN sound:NEAR	;3C41
EXTRN k_puts:NEAR	;3EA9
EXTRN C_402A:NEAR

PORT_201	EQU 201h

DGROUP	GROUP DATA
;########################################
DATA	SEGMENT PUBLIC BYTE 'DATA'
;----------------------------------------
PUBLIC pressedKey
pressedKey	db 0
PUBLIC isKeyPending
isKeyPending	db 0
D_DE6A	db "v i{i",0	;version string
PUBLIC D_DE70
D_DE70	dw 0
PUBLIC D_DE72
D_DE72	dw 0
D_DE74	dw 0B800h
D_DE76	db 0
D_DE77	db 0	;original video mode
D_DE78	db 0	;defaultDrive
D_DE79	db 35h,2Dh,2Eh,7,5Bh,2,57h,57h,2,3,0,0	;port values for monochrome mode?
;scanlines offsets
PUBLIC D_DE85
D_DE85	dw 200 dup(0)
D_E015	dw 0	;mode is monochrome?
__E017	db 0
;-- --
D_E018	db 10h,10h,10h,10h,10h,10h,10h,0Ch,10h,10h,10h,0Ch,10h,10h,0Ch,10h,10h,10h,10h
	db 10h,10h,0Ch,10h,0Ch,10h,0Ch
D_E032	db 0,1,2,3,4,5,6,7,6,8,9,7,9,9,7,9,9,9,9,6,0,8,0,6,0,6
D_E04C	db 3,3,0,0,0,3,3,0,3,0
D_E056	db 3,3,3,0,0,3,3,3,3,3
D_E060	db 2,2,0,0,0,2,2,2,0,2
D_E06A	db 2,0,2,0,0,2,2,2,0,2
D_E074	db 0,0,0,0,0,0,0,0,2,0

D_E07E	dw 28h,2Ah,26h,22h,22h,1Ch
	dw -4,-4,-4,-14h,-14h,-14h
	dw -4,-4,-4,-14h,-14h,-14h
D_E0A2	db 82h,8Ch,96h,8Dh,92h,9Eh
	db 82h,8Ch,96h,8Bh,90h,9Eh
	db 82h,8Ch,96h,84h,90h,9Eh

__E0B4	db 0,0
D_E0B6	db 0
D_E0B7	dw 0,0
D_E0BB	dw 0,0
D_E0BF	dw 0	;joystick mask?
D_E0C1	db 0	;joystick direction flags?
D_E0C2	db 0	;joystick direction flags?
D_E0C3	db 0	;joystick direction flags?
D_E0C4	db 0	;joystick direction flags?
D_E0C5	db 0
D_E0C6	db 0
D_E0C7	dw 0
__E0C9	db 0,0 
PUBLIC D_E0CB
D_E0CB	db 0
PUBLIC D_E0CC
D_E0CC	db 0
PUBLIC D_E0CD
D_E0CD	db 0
PUBLIC D_E0CE
D_E0CE	db 0
__E0CF	db 0	;padding?
;----------------------------------------
DATA	ENDS
;########################################
PROG	SEGMENT PUBLIC BYTE 'PROG'
	ASSUME CS:PROG,DS:DGROUP
;----------------------------------------41BE
PUBLIC DoInput
DoInput:
	PUSH	BP
	MOV	BP,SP
	;-- --
	MOV	DL,0FFh	;console input request
	MOV	AH,6	;direct console I/O
	INT	21h
	JNZ	C_41EC
	;-- no key --
	;-- joystick? --
	CMP	D_DE72,0
	JNZ	C_41DE
	;--
	XOR	AX,AX
	PUSH	AX
	CALL	C_4586	;joystick to key?
	POP	BX
	CMP	AL,0
	JZ	C_41DE
	JMP	C_4287
C_41DE:	;-- --
	CMP	isKeyPending,0
	JNZ	C_41E8
	;--
	CBW
	POP	BP
	RET
C_41E8:	;--
	MOV	AL,0FFh
	POP	BP
	RET
C_41EC:	;-- has key --
	CMP	AL,1Bh	;ESC
	JNZ	C_41F3
	;-- --
	JMP	C_4293
C_41F3:	;-- --
	CMP	D_016A,1
	JZ	C_4203
	CMP	AL,12h	;CTRL+R?
	JNZ	C_4203
	;-- --
	CALL	C_1F86	;"CTRL+R menu"?
	JMP	SHORT C_423D
C_4203:
	CMP	AL,13h	;CTRL+S
	JNZ	C_421C
	;-- --
	CALL	Beep
	INC	D_D54C
	CMP	D_D54C,3
	JB	C_421A
	MOV	D_D54C,0
C_421A:	;-- --
	JMP	SHORT C_423D
C_421C:
	CMP	D_016A,1
	JZ	C_4241
	CMP	AL,'j'	;6Ah
	JZ	C_422B
	CMP	AL,'J'	;4Ah
	JNZ	C_4230
C_422B:	;-- 'j' 'J' --
	CALL	C_1F43
	;-- --
	JMP	SHORT C_423D
C_4230:	;-- --
	CMP	AL,'k'	;6Bh
	JNZ	C_4241
	;-- 'k' --
	CALL	Beep
	;--
	MOV	D_DE72,1
C_423D:	;-- --
	XOR	AX,AX
	POP	BP
	RET
C_4241:	;--
	CMP	D_0168,1
	JNZ	C_425F
	CMP	AL,16h	;CTRL+V?
	JNZ	C_425F
	;-- version string --
	CALL	Beep
	LEA	DI,cga_buffer[3AE4h]
	LEA	SI,[D_DE6A]	;"v i{i"
	CALL	k_puts
	CALL	BB_flip
	;-- --
	JMP	C_423D
C_425F:	;-- --
	CMP	AL,0
	JNZ	C_4287
	;-- extended code --
	MOV	AH,7	;direct console input without echo
	INT	21h
	;-- --
	CMP	D_016A,1
	JNZ	C_4277
	CMP	AL,3Bh	;<F1>
	JNZ	C_4277
	CALL	C_4385	;<F1>monochrome mode?
	JMP	C_423D
C_4277:
	CMP	AL,4Bh	;<left>
	JNZ	C_427F
	MOV	AL,'4'
	JMP	SHORT C_4287
C_427F:
	CMP	AL,4Dh	;<right>
	JZ	C_4285
	JMP	C_423D
C_4285:
	MOV	AL,'6'
C_4287:
	MOV	pressedKey,AL
	MOV	isKeyPending,1
	;--
	MOV	AL,0FFh
	POP	BP
	RET
C_4293:	;-- pause:wait key? --
	MOV	AH,7	;direct console input without echo
	INT	21h
	JMP	C_423D
;----------------------------------------4299
PUBLIC GetKey
GetKey:
X_4299:
	MOV	AL,pressedKey
	;-- --
	CMP	isKeyPending,0
	JNZ	C_42A7
	MOV	AH,7	;direct console input without echo
	INT	21h
C_42A7:	;-- --
	MOV	AH,0

	MOV	isKeyPending,0
	;-- is extended code ? --
	CMP	AL,0
	JZ	C_42B4
	JMP	SHORT C_42CA
C_42B4:	;-- extended code --
	MOV	AH,7	;direct console input without echo
	INT	21h

	CMP	AL,4Bh	;<left>
	JNZ	C_42C2
	MOV	AL,'4'	;34h
	MOV	AH,0
	JMP	SHORT C_42CA
C_42C2:
	CMP	AL,4Dh	;<right>
	JNZ	X_4299
	MOV	AL,'6'	;36h
	MOV	AH,0
C_42CA:	;-- --
	PUSH	AX
	CALL	WaitNoKey
	POP	AX

	RET
;----------------------------------------42D0
PUBLIC WaitNoKey
WaitNoKey:
X_42D0:	;-- wait while key --
	MOV	DL,0FFh	;console input request
	MOV	AH,6	;direct console I/O
	INT	21h
	JNZ	X_42D0
	;-- --
	MOV	isKeyPending,0

	RET
;----------------------------------------42DE
PUBLIC Beep
Beep:
	PUSH	WORD PTR D_D54C
	MOV	D_D54C,0
	;--
	MOV	AX,5
	PUSH	AX
	CALL	sound
	POP	AX
	;--
	POP	WORD PTR D_D54C
	RET
;----------------------------------------
;selectDisk_0
PUBLIC C_42F4
C_42F4:
	MOV	DL,0
	MOV	AH,0Eh
	INT	21h

	RET
;----------------------------------------42FB
PUBLIC WaitKey
WaitKey:
	MOV	AH,7	;direct console input without echo
	INT	21h

	RET
;----------------------------------------
PUBLIC C_4300
C_4300:
	MOV	AH,0Fh
	INT	10h
	MOV	D_DE77,AL
	XOR	AH,AH
	RET
;----------------------------------------
;getDefaultDrive
PUBLIC C_430A
C_430A:
	MOV	AH,19h	;getCurrentDefaultDrive
	INT	21h
	MOV	D_DE78,AL

	RET
;----------------------------------------
C_4312:
	IN	AL,DX
	AND	AL,BL
	XCHG	AH,AL
	MOV	CX,30000
C_431A:
	IN	AL,DX
	AND	AL,BL
	CMP	AH,AL
	JNZ	C_4325
	LOOP	C_431A
	STC
	RET
C_4325:
	CLC
	RET
;----------------------------------------
C_4327:
	MOV	DX,3BFh
	MOV	AL,0
	OUT	DX,AL
	;-- 0040:0010 --
	PUSH	DS
	MOV	AX,40h
	MOV	DS,AX
	MOV	SI,10h
	MOV	AL,[SI]
	AND	AL,0CFh
	OR	AL,20h
	MOV	[SI],AL
	POP	DS
	;--
	MOV	AH,0
	MOV	AL,4
	INT	10h
	;--
	PUSH	ES
	MOV	ES,D_DE74
	MOV	DI,1F40h
	MOV	DX,DI
	MOV	CX,5Ah
	MOV	BX,CX
	CLD
	MOV	AX,55AAh
	REPZ	STOSW
	MOV	DI,DX
	MOV	CX,BX
	REPZ	SCASW
	POP	ES
	JCXZ	C_4365
	;-- --
	STC
	RET
C_4365:	;-- color mode --
	MOV	D_E015,0
	;-- compute scanline offsets --
	MOV	AX,0
	MOV	DX,2000h
	MOV	CX,200
	LEA	BX,[D_DE85]
C_4378:
	MOV	[BX],AX
	ADD	BX,2
	ADD	AX,50h
	XCHG	DX,AX
	LOOP	C_4378
	;-- --
	CLC
	RET
;----------------------------------------
;<F1>monochrome mode?
C_4385:
	CMP	D_E015,1
	JNZ	C_438D
	RET
;----
;monochrome mode?
C_438D:
	MOV	DX,3BFh
	MOV	AL,3
	OUT	DX,AL
	;-- 0040:0010 --
	MOV	BL,D_DE76
	PUSH	DS
	MOV	AX,40h
	MOV	DS,AX
	MOV	SI,10h
	MOV	[SI],BL
	POP	DS
	;-- --
	MOV	DX,3BAh
	MOV	BL,80h
	CALL	C_4312
	JNB	C_43AF
	;-- --
	STC
	RET
C_43AF:	;-- init monochrome mode? --
	MOV	D_E015,1
	;-- --
	PUSH	SI
	PUSH	DI
	;-- compute scanline offsets --
	MOV	CX,200 / 4
	LEA	BX,[D_DE85]
	MOV	SI,0658h
	MOV	DI,2658h
	MOV	AX,4658h
	MOV	DX,6658h
C_43CA:
	MOV	[BX],SI
	MOV	[BX+02],DI
	MOV	[BX+04],AX
	MOV	[BX+06],DX
	ADD	SI,5Ah
	ADD	DI,5Ah
	ADD	AX,5Ah
	ADD	DX,5Ah
	ADD	BX,8
	LOOP	C_43CA
	;-- --
	CALL	C_4449	;clear VRAM?
	;--
	LEA	SI,[D_DE79]
	CALL	C_43FA
	;--
	MOV	DX,3B8h
	MOV	AL,8Ah
	OUT	DX,AL
	;--
	POP	DI
	POP	SI
	;-- --
	CLC
	RET
;----------------------------------------
C_43FA:
	MOV	DX,3B4h
	MOV	CX,0Ch
	XOR	AH,AH
C_4402:
	MOV	AL,AH
	OUT	DX,AL
	INC	DX
	LODSB
	OUT	DX,AL
	INC	AH
	DEC	DX
	LOOP	C_4402

	RET
;----------------------------------------
PUBLIC C_440E
C_440E:
	INT	11h
	MOV	D_DE76,AL
	;--
	CALL	C_4327
	JNB	C_441D
	CALL	C_438D	;monochrome mode?
	JB	C_4420
C_441D:
	MOV	AL,1
	RET
C_4420:
	CALL	C_4426
	XOR	AX,AX
	RET
;----------------------------------------
PUBLIC C_4426
C_4426:
	CALL	C_4449	;clear VRAM?
	;-- 0040:0010 --
	MOV	BL,D_DE76
	PUSH	DS
	MOV	AX,40h
	MOV	DS,AX
	MOV	SI,10h
	MOV	[SI],BL
	POP	DS
	;-- restore screen mode --
	MOV	AH,0
	MOV	AL,D_DE77
	INT	10h
	;-- restore default drive --
	MOV	DL,D_DE78
	MOV	AH,0Eh
	INT	21h
	;-- --
	RET
;----------------------------------------
;clear VRAM?
C_4449:
	PUSH	DI
	PUSH	ES
	;-- --
	MOV	ES,D_DE74
	MOV	CX,4000h
	MOV	AX,0
	MOV	DI,AX
	CLD
	REPZ	STOSW
	;-- --
	POP	ES
	POP	DI
	RET
;----------------------------------------
;collision related?
PUBLIC C_445D
C_445D:
	PUSH	BP
	MOV	BP,SP

	CMP	BYTE PTR [BP+04],2
	JG	C_446C
	ADD	BYTE PTR [BP+04],3
	JMP	SHORT C_447C
C_446C:
	SUB	BYTE PTR [BP+04],3
	JZ	C_447C
	SUB	WORD PTR [BP+06],4
	CMP	BYTE PTR [BP+04],1
	JZ	C_447C
C_447C:
	;-- --
	MOV	SI,[BP+08]
	AND	WORD PTR [BP+06],0FFFCh
	XOR	BX,BX
	XOR	DX,DX
	XOR	AX,AX
	MOV	BL,D_E032[SI]
	MOV	DL,D_E018[SI]
	CMP	[BP+06],DX
	JL	C_44F0
	JNZ	C_44B5
C_4499:
	CMP	WORD PTR [BP+04],4
	JLE	C_44A5
	MOV	AL,D_E06A[BX]
	JMP	SHORT C_44F2
C_44A5:
	CMP	WORD PTR [BP+04],3
	JL	C_44B1
	MOV	AL,D_E060[BX]
	JMP	SHORT C_44F2
C_44B1:
	MOV	AL,3
	JMP	SHORT C_44F2
C_44B5:
	ADD	DX,11h
	CMP	[BP+06],DX
	JG	C_44F2
	CMP	WORD PTR [BP+04],1
	JNZ	C_44C9
	MOV	AL,D_E056[BX]
	JMP	SHORT C_44F2
C_44C9:
	CMP	WORD PTR [BP+04],2
	JG	C_44E2
	MOV	AL,D_E04C[BX]
	CMP	BL,1
	JNZ	C_44F2
	CMP	BYTE PTR [BP+04],2
	JNZ	C_44F2
	MOV	AL,0
	JMP	SHORT C_44F2
C_44E2:
	SUB	DX,4
	CMP	[BP+06],DX
	JL	C_4499
	MOV	AL,D_E074[BX]
	JMP	SHORT C_44F2
C_44F0:
	MOV	AL,4
C_44F2:	;-- --
	POP	BP
	RET
;----------------------------------------
;damage related?
PUBLIC C_44F4
C_44F4:
	PUSH	BP
	MOV	BP,SP
	;-- --
	MOV	BX,[BP+06]
	SHL	BX,1
	;-- --
	CMP	D_00F2,1
	JNZ	C_450B
	CMP	BX,0Ch
	JL	C_450B
	ADD	BX,0Ch
C_450B:
	;-- --
	CMP	WORD PTR [BP+04],0
	JNZ	C_4520
	;-- bp04 is 0 --
	MOV	AX,playerPosClamp
	MOV	CL,61h
	CMP	WORD PTR [BP+06],3
	JGE	C_4536
	INC	CL
	JMP	SHORT C_4536
C_4520:	;-- bp04 is not 0 --
	MOV	AX,enemyClamp
	MOV	CL,63h
	CMP	D_D43A,0
	JZ	C_452F
	MOV	BX,0Eh
C_452F:
	CMP	BX,12h
	JGE	C_4536
	INC	CL
C_4536:	;-- --
	MOV	SI,D_B9BE
	MOV	DI,SI
	ADD	DI,4
C_453F:
	MOV	DL,D_B9C0[SI]
	MOV	D_B9C0[DI],DL
	DEC	SI
	DEC	DI
	CMP	SI,D_D442
	JNB	C_453F
	INC	SI
	ADD	AX,D_E07E[BX]
	CMP	D_0130,7
	JNZ	C_455E
	SUB	AX,23h
C_455E:
	MOV	WORD PTR D_B9C0[SI].f_01,AX

	MOV	D_B9C0[SI].f_00,CL

	SHR	BX,1
	MOV	CL,D_E0A2[BX]
	CMP	D_0130,7
	JNZ	C_4576
	ADD	CL,8
C_4576:
	MOV	D_B9C0[SI].f_03,CL
	;-- --
	ADD	D_B9BE,4
	ADD	D_D442,4
	;-- --
	POP	BP
	RET
;----------------------------------------
;joystick to key?
C_4586:
	CALL	C_480D
	CMP	WORD PTR [BP+04],0
	JZ	C_4590
	RET
C_4590:
	CMP	AL,0
	JNZ	C_45A2
	CMP	D_010C,40h
	JNB	C_459E
	XOR	AX,AX
	RET
C_459E:
	MOV	AX,' '
	RET
C_45A2:
	CMP	D_E0CB,0
	JZ	C_45B3
	MOV	AL,D_E0C1
	XCHG	AL,D_E0C2
	MOV	D_E0C1,AL
C_45B3:
	CMP	D_E0CC,0
	JZ	C_45C4
	MOV	AL,D_E0C3
	XCHG	AL,D_E0C4
	MOV	D_E0C3,AL
C_45C4:
	CMP	D_E0CD,0
	JZ	C_45DF
	MOV	AL,D_E0C1
	XCHG	AL,D_E0C3
	MOV	D_E0C1,AL
	MOV	AL,D_E0C2
	XCHG	AL,D_E0C4
	MOV	D_E0C2,AL
C_45DF:
	CMP	D_E0CE,0
	JZ	C_45F0
	MOV	AL,D_E0C5
	XCHG	AL,D_E0C6
	MOV	D_E0C5,AL
C_45F0:
	CMP	D_E0C2,1
	JNZ	C_4620
	CMP	D_E0C3,1
	JNZ	C_4620
	CMP	D_00FA,0
	JNZ	C_461C
	CMP	D_D43A,0
	JNZ	C_4619
	MOV	DX,enemyClamp
	SUB	DX,playerPosClamp
	CMP	DX,64h
	JNB	C_461C
C_4619:
	JMP	C_46C9
C_461C:
	MOV	AX,'&'	;26h
	RET
C_4620:
	CMP	D_010C,3Fh
	JLE	C_4632
	CMP	D_E0C6,1
	JNZ	C_4632
	MOV	AX,'b'
	RET
C_4632:
	CMP	D_E0C1,1
	JNZ	C_463D
	MOV	AX,'4'
	RET
C_463D:
	CMP	D_E0C2,1
	JNZ	C_4648
	MOV	AX,'6'
	RET
C_4648:
	CMP	D_E0C5,1
	JNZ	C_4669
	CMP	D_E0C3,1
	JNZ	C_465A
	MOV	AX,'w'
	RET
C_465A:
	CMP	D_E0C4,1
	JNZ	C_4665
	MOV	AX,'x'
	RET
C_4665:
	MOV	AX,'s'
	RET
C_4669:
	CMP	D_E0C6,1
	JNZ	C_468A
	CMP	D_E0C3,1
	JNZ	C_467B
	MOV	AX,'q'
	RET
C_467B:
	CMP	D_E0C4,1
	JNZ	C_4686
	MOV	AX,'z'
	RET
C_4686:
	MOV	AX,'a'
	RET
C_468A:
	CMP	D_E0C3,0
	JZ	C_46C9
	CMP	D_010C,41h
	JZ	C_46C9
	CMP	D_010C,42h
	JZ	C_46C9
	CMP	D_010C,46h
	JNZ	C_46AA
	MOV	AX,'0'
	RET
C_46AA:
	CMP	D_D43A,0
	JNZ	C_46C9
	CMP	D_00FA,0
	JNZ	C_46C5
	MOV	DX,enemyClamp
	SUB	DX,playerPosClamp
	CMP	DX,64h
	JB	C_46C9
C_46C5:
	MOV	AX,' '
	RET
C_46C9:
	XOR	AX,AX
	RET
;----------------------------------------
PUBLIC C_46CC
C_46CC:
	PUSH	DI
	PUSH	SI

	MOV	D_E0B6,0

	MOV	DX,PORT_201
	OUT	DX,AL
	IN	AL,DX
	AND	AL,3
	CMP	AL,3
	JNZ	C_46FF
	;-- tempo --
	MOV	CX,0
	LOOP	$
	;-- --
	IN	AL,DX
	AND	AL,3
	JNZ	C_46FF

	MOV	D_E0B6,1
	;--
	MOV	AH,1
	LEA	DI,[D_E0B7]
	CALL	C_4707
	;--
	MOV	AH,2
	LEA	DI,[D_E0BB]
	CALL	C_4707
C_46FF:
	POP	SI
	POP	DI

	MOV	AL,D_E0B6
	MOV	AH,0
	RET
;----------------------------------------
C_4707:
	CALL	C_48A5
	PUSH	AX
	MOV	BX,AX
	SHR	BX,1
	MOV	[DI],BX
	POP	AX
	ADD	AX,BX
	MOV	[DI+02],AX

	RET
;----------------------------------------
PUBLIC C_4718
C_4718:
	;-- compute speed --
	PUSH	BP
	PUSH	DS
	XOR	AX,AX
	MOV	DS,AX
	MOV	DI,AX
	MOV	BP,7
	MOV	CX,1F40h
	MOV	DX,PORT_201
	MOV	BX,46Ch
	MOV	SI,[BX]	;0040:006C
	INC	SI
	ADD	BP,SI
C_4731:
	CMP	SI,[BX]
	JNZ	C_4731
C_4735:
	IN	AL,DX
	INC	CX
	LOOPNZ	$ + 2
	IN	AL,DX
	INC	CX
	LOOPNZ	$ + 2
	MOV	AX,[BX]
	INC	DI
	CMP	AX,BP
	JNZ	C_4735
	POP	DS
	MOV	D_E0C7,DI
	POP	BP
	;-- --
	CMP	DI,11000
	JA	C_4783
	;-- "speed" < 11000 --
	MOV	BX,4	;TODO:should consider D_D59E like words
C_4753:
	INC	D_D59E[BX]
	SHR	D_D59E[BX],1
	DEC	BX
	JGE	C_4753
	;-- --
	MOV	BX,1Ch
C_4761:
	MOV	SI,D_D5A8[BX]
	SHR	BYTE PTR [SI],1
	CMP	BX,6
	JB	C_477C
	CMP	BX,0Eh
	JA	C_4775
	INC	BYTE PTR [SI]
	JMP	SHORT C_477C
C_4775:
	CMP	BX,10h
	JNZ	C_477C
	DEC	BYTE PTR [SI]
C_477C:
	SUB	BX,2
	JGE	C_4761
	;-- --
	JMP	SHORT C_47C3
C_4783:
	CMP	DI,13500
	JA	C_47A5
	;-- "speed" < 13500 --
	MOV	BX,1Ch
C_478C:
	MOV	SI,D_D5A8[BX]
	CMP	BYTE PTR [SI],6
	JB	C_4797
	DEC	BYTE PTR [SI]
C_4797:
	CMP	BYTE PTR [SI],8
	JA	C_479E
	DEC	BYTE PTR [SI]
C_479E:
	SUB	BX,2
	JGE	C_478C
	;-- --
	JMP	SHORT C_47C3
C_47A5:
	CMP	DI,20000
	JB	C_47C3
	;-- "speed" > 20000 --
	MOV	BX,4	;TODO:should consider D_D59E like words
C_47AE:
	SHL	D_D59E[BX],1
	DEC	BX
	JGE	C_47AE
	;-- --
	MOV	BX,1Ch
C_47B8:
	MOV	SI,D_D5A8[BX]
	SHL	BYTE PTR [SI],1
	SUB	BX,2
	JGE	C_47B8
C_47C3:	;-- --
	RET
;----------------------------------------
C_47C4:
	MOV	D_E0BF,0
	MOV	D_E0C5,0
	MOV	D_E0C6,0

	CMP	D_E0B6,0
	JZ	C_4809
	MOV	DX,PORT_201
	IN	AL,DX
	MOV	AH,AL
	AND	AH,30h
	XOR	AH,30h
	JZ	C_4809
	TEST	AH,10h
	JNZ	C_47F9
	OR	D_E0BF,20h
	MOV	D_E0C6,1
C_47F9:
	TEST	AH,20h
	JNZ	C_4809
	OR	D_E0BF,10h
	MOV	D_E0C5,1
C_4809:
	MOV	AX,D_E0BF
	RET
;----------------------------------------
C_480D:
	PUSH	DI
	PUSH	SI

	MOV	D_E0BF,0
	MOV	WORD PTR D_E0C1,0
	MOV	WORD PTR D_E0C3,0

	CMP	D_E0B6,0
	JZ	C_4890
	CALL	C_47C4
	CMP	WORD PTR [BP+04],0
	JNZ	C_489B
	MOV	DX,PORT_201
	MOV	AH,1
	CALL	C_48A5
	CMP	AX,0
	JZ	C_4862
	CMP	AX,D_E0B7[0]
	JG	C_4851

	OR	D_E0BF,1
	MOV	D_E0C1,1

	JMP	SHORT C_4862
C_4851:
	CMP	AX,D_E0B7[2]
	JL	C_4862

	OR	D_E0BF,2
	MOV	D_E0C2,1
C_4862:
	MOV	AH,2
	CALL	C_48A5
	CMP	AX,0
	JZ	C_4890
	CMP	AX,D_E0BB[0]
	JG	C_487F

	OR	D_E0BF,4
	MOV	D_E0C3,1

	JMP	SHORT C_4890
C_487F:
	CMP	AX,D_E0BB[2]
	JL	C_4890

	OR	D_E0BF,8
	MOV	D_E0C4,1
C_4890:
	MOV	AX,D_E0BF
	TEST	AX,0Fh
	JZ	C_4898
C_4898:
	POP	SI
	POP	DI
	RET
C_489B:
	CMP	AX,0
	JZ	C_4898
	MOV	AX,0Dh
	JMP	C_4898
;----------------------------------------
C_48A5:
	MOV	SI,0
	MOV	BX,4
C_48AB:
	CALL	C_48BD
	ADD	SI,CX
	DEC	BX
	JNZ	C_48AB
	MOV	CL,2
	SHR	SI,CL
	MOV	AX,310h
	SUB	AX,SI
	RET
;----------------------------------------
C_48BD:
	MOV	CX,310h
	OUT	DX,AL
C_48C1:
	IN	AL,DX
	TEST	AL,AH
	LOOPNZ	C_48C1

	RET
;----------------------------------------
;new int 24h
C_FAR_48C7:
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	BP
	PUSH	SI
	PUSH	DI
	PUSH	DS
	PUSH	ES
	;--
	MOV	AX,DGROUP	;3C92
	MOV	DS,AX
	CALL	C_402A	;int24h handler
	MOV	AL,1
	;--
	POP	ES
	POP	DS
	POP	DI
	POP	SI
	POP	BP
	POP	DX
	POP	CX
	POP	BX
	IRET
;----------------------------------------
PUBLIC C_48E2
C_48E2:
	PUSH	DS
	;--
	MOV	AX,PROG	;35BC
	MOV	DS,AX
	MOV	AX,2524h
	LEA	DX,WORD PTR [C_FAR_48C7]
	INT	21h
	;--
	POP	DS
	RET
;----------------------------------------
PROG	ENDS
END
