;========================================
;	KARATEKA
;	Copyright 1986 Jordan Mechner
;	IBM version by The Connelley Group
;	reverse-coded by ergonomy_joe 2022
;========================================
INCLUDE COMMON.MAC

EXTRN D_00E4:WORD
EXTRN D_00EA:WORD
EXTRN D_00EE:WORD

EXTRN D_0156:WORD

EXTRN D_B9BA:WORD
EXTRN D_B9BE:WORD
EXTRN D_B9C0:BYTE

EXTRN D_C264:BYTE
EXTRN D_C2B8:BYTE
EXTRN D_CC7A:BYTE
EXTRN D_CCCE:BYTE

EXTRN D_D4E2:WORD

EXTRN BB_clear:NEAR	;0BB7
EXTRN render:NEAR	;0C32
EXTRN load_sprite_assets:NEAR	;load ks/km files
EXTRN load_animation_script:NEAR	;12E0
EXTRN DoInput:NEAR	;41BE
EXTRN GetKey:NEAR	;4299

DGROUP	GROUP DATA
;########################################
DATA	SEGMENT WORD PUBLIC 'DATA'
;----------------------------------------
;-- for anti-copy protection --
D_BB34	db "Karateka Copyright 1986 Broderbund Software",0
;-- --
PUBLIC D_BB60
D_BB60	db 0	;hide life bars
D_BB61	dw 0	;timer related
__BB63	dw 1000h
PUBLIC D_BB65
D_BB65	dw 0
D_BB67	db 0
D_BB68	dw 0	;debug flag:"frame by frame"?
D_BB6A	dw 0	;backup for D_00EA?
D_BB6C	dw 0	;backup for D_00E4?
D_BB6E	dw 0	;backup for [bp+02]?
D_BB70	dw 0	;backup for BP?
D_BB72	dw 0	;timer related [for "SCRIPT_18/loop"]
D_BB74	dw 0
D_BB76	dw 0	;script index
D_BB78	dw OFFSET PROG:C_17CD	;SCRIPT_00/set_tune
	dw OFFSET PROG:C_17DA	;SCRIPT_02/set_bg
	dw OFFSET PROG:C_17E7	;SCRIPT_04/set_fig
	dw OFFSET PROG:C_1822	;SCRIPT_06/chg_fig
	dw OFFSET PROG:C_1842	;SCRIPT_08/do_scr
	dw OFFSET PROG:C_1854	;SCRIPT_0a/del_fig
	dw OFFSET PROG:C_1889	;SCRIPT_0c/set_wipe
	dw OFFSET PROG:C_1893	;SCRIPT_0e/set_nowipe
	dw OFFSET PROG:C_189D	;SCRIPT_10/wait
	dw OFFSET PROG:C_18B3	;SCRIPT_12/init_sal
	dw OFFSET PROG:C_18DA	;SCRIPT_14/set_pos
	dw OFFSET PROG:C_18F7	;SCRIPT_16/inc_x
	dw OFFSET PROG:C_1906	;SCRIPT_18/loop
PUBLIC D_BB92
D_BB92	dw 0	;last index in text script[unused]
;-- script data --
PUBLIC D_BB94
D_BB94	db 04h,02,64h,00,64h	;set_fig
	db 04h,01,96h,00,64h	;set_fig
	db 08h	;do_scr
	db 10h,5Ah	;wait
	db 06h,01,02,96h,00,14h	;chg_fig
	db 08h	;do_scr
	db 0FFh	;end_animation
__BBA9	db 3e9h dup(0)
;-- --
;----------------------------------------
DATA	ENDS
;########################################
PROG	SEGMENT PUBLIC BYTE 'PROG'
	ASSUME CS:PROG,DS:DGROUP
;----------------------------------------
PUBLIC C_1705
;check copy
C_1705:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	MOV	AX,1	;(b80100)       ;hack?
;	RET			;(c3)           ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	PUSH	BP	;(55)           ;original?
	MOV	BP,SP	;(88EC)         ;
	PUSH	ES	;(06)           ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	PUSH	DS
	;-- to F800:0000 --
	MOV	DI,7	;7 tries
C_170D:
	MOV	AX,206h
	MOV	BX,0F800h
	MOV	ES,BX
	XOR	BX,BX
	MOV	CH,0Ah
	MOV	CL,1
	MOV	DX,0
	INT	13h
	STI
	DEC	DI
	JZ	C_1770	;fail
	JB	C_170D
	;-- to DS:[bp+04] --
	MOV	DI,4	;4 tries
C_1729:
	MOV	AX,201h
	MOV	BX,[BP+04]
	MOV	CH,0Ah
	MOV	CL,7
	MOV	DX,0
	PUSH	DS
	POP	ES
	NOP	;???
	INT	13h
	STI
	DEC	DI
	JZ	C_1770	;fail
	JB	C_1729
	;-- to DS:[bp+04] --
	MOV	DI,4	;4 tries
C_1744:
	MOV	AX,201h
	MOV	BX,[BP+04]
	MOV	CH,0Ah
	MOV	CL,0F1h
	MOV	DX,0
	INT	13h
	STI
	DEC	DI
	JZ	C_1770	;fail
	JNB	C_1744
	CMP	AH,10h
	JNZ	C_1744
	;-- --
	CLD
	MOV	SI,OFFSET DGROUP:D_BB34
	MOV	CX,2Ch	;string length
	MOV	DI,[BP+04]
	ADD	DI,8Ch
	REPZ	CMPSB
	JZ	C_1774
C_1770:	;-- fail --
	XOR	AX,AX
	JZ	C_1777
C_1774:	;-- success --
	MOV	AX,1
C_1777:	;-- --
	POP	DS
	POP	ES
	POP	BP
	RET
;----------------------------------------
;
;----------------------------------------
PUBLIC C_177B
;execute script/animation?
C_177B:
	MOV	D_BB60,1
	CALL	C_18B3	;SCRIPT_12/init_sal
	MOV	D_BB76,0
	MOV	D_BB67,0
C_178E:
	MOV	SI,D_BB76
	MOV	BL,D_BB94[SI]
	CMP	BL,0FFh
	JZ	C_17C4
	MOV	BH,0
	PUSH	D_BB76
	CALL	D_BB78[BX]
	;-- --
	CMP	D_0156,0
	JZ	C_17C1
	CALL	DoInput
	CMP	AX,0
	JZ	C_17C1
	CALL	GetKey

	POP	D_BB76

	MOV	D_BB60,0
	RET	
C_17C1:
	POP	AX
	JMP	C_178E
C_17C4:
	MOV	AX,0
	MOV	D_BB60,0
	RET	
;----------------------------------------
;SCRIPT_00/set_tune
C_17CD:
	MOV	AL,D_BB94[SI+1]
	MOV	D_B9C0[0],AL
	ADD	D_BB76,2

	RET	
;----------------------------------------
;SCRIPT_02/set_bg
C_17DA:
	MOV	AL,D_BB94[SI+1]
	MOV	D_B9C0[2],AL
	ADD	D_BB76,2

	RET	
;----------------------------------------
;SCRIPT_04/set_fig
C_17E7:
	MOV	DI,D_B9BE
	MOV	AL,D_BB94[SI+1]
	MOV	D_B9C0[DI].f_00,AL
	MOV	AX,WORD PTR D_BB94[SI+2]
	CMP	D_BB67,0
	JZ	C_1806
	DEC	D_BB67
	ADD	AX,D_BB65
C_1806:
	MOV	WORD PTR D_B9C0[DI].f_01,AX
	MOV	AL,D_BB94[SI+4]
	MOV	D_B9C0[DI].f_03,AL
	MOV	BYTE PTR D_B9C0[DI+4].f_00,0FFh
	ADD	D_B9BE,4
	ADD	D_BB76,5

	RET	
;----------------------------------------
;SCRIPT_06/chg_fig
C_1822:
	MOV	BL,D_BB94[SI+1]
	MOV	BH,0
	SHL	BX,1
	SHL	BX,1
	MOV	AX,WORD PTR D_BB94[SI+2]
	MOV	WORD PTR D_B9C0[BX].f_03,AX
	MOV	AX,WORD PTR D_BB94[SI+4]
	MOV	WORD PTR D_B9C0[BX+4].f_01,AX
	ADD	D_BB76,6

	RET	
;----------------------------------------
;SCRIPT_08/do_scr
C_1842:
	CALL	render
	INC	D_BB76
	;-- --
	CMP	D_BB68,1
	JNZ	C_1853
	CALL	GetKey
C_1853:	;-- --
	RET	
;----------------------------------------
;SCRIPT_0a/del_fig
C_1854:
	MOV	BL,[SI+01]
	MOV	BH,0
	SHL	BX,1
	SHL	BX,1
	ADD	BX,3
	SUB	D_B9BE,4
C_1865:
	;-- copy struct --
	MOV	AX,WORD PTR D_B9C0[BX+4]
	MOV	WORD PTR D_B9C0[BX],AX
	MOV	AX,WORD PTR D_B9C0[BX+4+2]
	MOV	WORD PTR D_B9C0[BX+2],AX
	;-- --
	ADD	BX,4
	CMP	BX,D_B9BE
	JL	C_1865
	MOV	BYTE PTR D_B9C0[BX].f_00,0FFh
	ADD	D_BB76,2

	RET	
;----------------------------------------
;SCRIPT_0c/set_wipe
C_1889:
	MOV	BYTE PTR D_B9C0[1],1
	INC	D_BB76

	RET	
;----------------------------------------
;SCRIPT_0e/set_nowipe
C_1893:
	MOV	BYTE PTR D_B9C0[1],0
	INC	D_BB76

	RET
;----------------------------------------
;SCRIPT_10/wait
C_189D:
	CMP	D_BB68,0
	JNZ	C_18AD

	MOV	AL,D_BB94[SI+1]
	MOV	AH,0
	CALL	C_1906	;SCRIPT_18/loop
C_18AD:
	ADD	D_BB76,2

	RET	
;----------------------------------------
;SCRIPT_12/init_sal
C_18B3:
	MOV	WORD PTR [D_B9BA],0FFFFh
	MOV	D_00EE,1
	;-- --
	MOV	BYTE PTR D_B9C0[0],0
	MOV	WORD PTR D_B9C0[1],0
	;-- --
	MOV	BYTE PTR D_B9C0[3].f_00,0FFh
	MOV	D_B9BE,3
	INC	D_BB76

	RET	
;----------------------------------------
;SCRIPT_14/set_pos
C_18DA:
	CMP	D_BB68,1
	JNZ	C_18EC

	CALL	C_1842	;SCRIPT_08/do_scr
	CALL	C_18B3	;SCRIPT_12/init_sal
	SUB	D_BB76,2
C_18EC:
	MOV	D_BB67,2
	ADD	D_BB76,3

	RET	
;----------------------------------------
;SCRIPT_16/inc_x
C_18F7:
	MOV	AL,D_BB94[SI+1]
	CBW	
	ADD	D_BB65,AX
	ADD	D_BB76,2

	RET	
;----------------------------------------
;SCRIPT_18/loop
PUBLIC C_1906
C_1906:
	MOV	D_BB72,AX
	MOV	AH,0	;read system clock counter
	INT	1Ah
	ADD	D_BB72,DX
C_1911:
	MOV	AH,0	;read system clock counter
	INT	1Ah
	CMP	DX,D_BB72
	JNZ	C_1911
	;-- --
	RET
;----------------------------------------
;delay+control?
PUBLIC C_191C
C_191C:
	SHR	AX,1
	MOV	D_BB74,AX
C_1921:
	MOV	AX,2
	CALL	C_1906	;SCRIPT_18/loop
	CALL	DoInput
	CMP	AX,0
	JNZ	C_1935
	DEC	D_BB74
	JNZ	C_1921
C_1935:
	RET
;----------------------------------------1936
PUBLIC tim_strt
tim_strt:
	MOV	AH,0
	INT	1Ah
	MOV	D_BB61,DX

	RET
;----------------------------------------193F
PUBLIC tim_wait
tim_wait:
X_193F:
	MOV	AH,0
	INT	1Ah
	MOV	AX,DX
	SUB	DX,D_BB61
	CMP	DX,3
	JB	X_193F
	MOV	D_BB61,AX

	RET
;----------------------------------------1952
PUBLIC Cutscene
Cutscene:
	PUSH	BP
	MOV	BP,SP
	;-- backup some values --
	MOV	AX,[BP+02]	;return address
	MOV	D_BB6E,AX
	MOV	AX,D_00EA
	MOV	D_BB6A,AX
	MOV	AX,D_00E4
	MOV	D_BB6C,AX
	;-- --
	LEA	AX,[D_BB94]
	MOV	D_BB70,BP	;backup BP
	PUSH	AX
	PUSH	[BP+04]
	CALL	load_animation_script
	ADD	SP,4
	MOV	BP,D_BB70	;restore BP

	PUSH	[BP+06]
	CALL	load_sprite_assets	;load ks/km files
	ADD	SP,2
	MOV	BP,D_BB70	;restore BP

	MOV	D_00E4,0
	CALL	BB_clear
	MOV	D_00EE,1
	;-- --
	MOV	D_00EA,4
	;-- --
	CALL	C_177B	;execute script/animation?
	MOV	D_D4E2,0FFFFh
	;-- restore some values --
	MOV	AX,D_BB6A
	MOV	D_00EA,AX
	MOV	AX,D_BB6C
	MOV	D_00E4,AX
	MOV	BX,D_BB6E
	MOV	[BP+02],BX
	;-- --
	POP	BP
	RET	
;----------------------------------------
PUBLIC C_19BD
;make indexes for D_C2B8
C_19BD:
	LEA	BX,[D_C2B8]
	LEA	DI,[D_C264]
;----
C_19C5:
	MOV	SI,0
	MOV	CL,2Ah
C_19CA:
	MOV	[DI],SI
	ADD	DI,2
C_19CF:
	CMP	BYTE PTR [BX+SI],0FFh
	JZ	C_19E3
	CMP	BYTE PTR [BX+SI],18h
	JNZ	C_19DE
	ADD	SI,2
	JMP	C_19CF
C_19DE:
	ADD	SI,11h
	JMP	C_19CF
C_19E3:
	INC	SI
	DEC	CL
	JNZ	C_19CA

	RET	
;----
PUBLIC C_19E9
;make indexes for D_CCCE
C_19E9:
	LEA	BX,[D_CCCE]
	LEA	DI,[D_CC7A]
	CALL	C_19C5

	RET
;----------------------------------------
PROG	ENDS
;########################################
END
