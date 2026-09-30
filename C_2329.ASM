;========================================
;	KARATEKA
;	Copyright 1986 Jordan Mechner
;	IBM version by The Connelley Group
;	reverse-coded by ergonomy_joe 2022
;========================================
INCLUDE COMMON.MAC

EXTRN cameraClamp:WORD
EXTRN D_00EA:WORD
EXTRN D_00EC:WORD
EXTRN D_00F0:WORD
EXTRN D_00F2:WORD
EXTRN D_00F8:WORD
EXTRN D_00FA:WORD

EXTRN D_0100:WORD
EXTRN min_boundary:WORD
EXTRN max_boundary:WORD
EXTRN minCameraScroll:WORD
EXTRN maxCameraScroll:WORD
EXTRN D_010C:WORD
EXTRN enemyClamp:WORD
EXTRN D_0110:WORD
EXTRN D_0112:WORD

EXTRN D_0118:WORD
EXTRN D_011A:WORD

EXTRN D_012E:WORD

EXTRN D_0156:WORD
EXTRN D_0158:BYTE

EXTRN D_B9BE:WORD
EXTRN D_B9C0:BYTE

EXTRN playerPosClamp:WORD

EXTRN D_C230:WORD
EXTRN D_C232:WORD
EXTRN D_C234:WORD

EXTRN D_C262:WORD
EXTRN D_C264:WORD

EXTRN D_C2B8:BYTE

EXTRN D_CC78:WORD
EXTRN D_CC7A:WORD

EXTRN D_CCCE:BYTE

EXTRN D_D43A:WORD
EXTRN D_D43C:BYTE
EXTRN D_D43E:WORD
EXTRN D_D440:WORD
EXTRN D_D442:WORD

EXTRN C_20E3:NEAR
EXTRN C_268A:NEAR
EXTRN C_2ACE:NEAR

DGROUP	GROUP DATA
;########################################
DATA	SEGMENT WORD PUBLIC 'DATA'
;----------------------------------------
D_D434	dw 0
__D436	dw 0
D_D438	dw 0
;----------------------------------------
DATA	ENDS
;########################################
PROG	SEGMENT PUBLIC BYTE 'PROG'
	ASSUME CS:PROG,DS:DGROUP
;----------------------------------------
;local memcpy?
C_2329:
	PUSH	BP
	MOV	BP,SP

	MOV	DI,[BP+04]	;dst
	MOV	SI,[BP+06]	;src
	MOV	AX,[BP+08]	;src_end
	SUB	AX,SI
	MOV	CX,AX
	PUSH	ES
	PUSH	DS
	POP	ES
	REPZ	MOVSB	
	POP	ES

	POP	BP
	RET	
;----------------------------------------
C_2341:
	PUSH	BP
	MOV	BP,SP
	;-- --
	MOV	DI,[BP+04]
	MOV	SI,[BP+06]
	MOV	BX,3
C_234D:
	MOV	AL,[BX+SI+1]
	MOV	[BX+DI],AL
	MOV	AL,[BX+SI+6]
	MOV	[BX+DI+4],AL
	DEC	BX
	JGE	C_234D
	;-- --
	MOV	AX,[BP+08]
	ADD	[DI].f_01,AX
	ADD	[DI+4].f_01,AX
	;-- --
	POP	BP
	RET	
;----------------------------------------
PUBLIC C_2366
C_2366:
	;-- --
	MOV	BYTE PTR D_B9C0[0],0
	MOV	WORD PTR D_B9C0[1],0
	;-- --
	MOV	AX,playerPosClamp
	ADD	AX,3
	AND	AX,0FFFCh
	MOV	playerPosClamp,AX
	
	MOV	AX,enemyClamp
	ADD	AX,3
	AND	AX,0FFFCh
	MOV	enemyClamp,AX

	PUSH	D_C232
	PUSH	D_C230
	LEA	AX,D_B9C0[3]
	PUSH	AX
	CALL	C_2329	;local memcpy?
	ADD	SP,6
	MOV	D_D434,AX

	CMP	D_00F8,1
	JLE	C_23A9
	JMP	C_24E6
C_23A9:
	;-- SCRIPT 1 --
	MOV	BX,D_C262
C_23AD:
	CMP	D_C2B8[BX],0FFh
	JNZ	C_23E1
	CMP	D_0156,0
	JZ	C_23C0
	CALL	C_2ACE
	JMP	SHORT C_23C3
C_23C0:
	CALL	C_20E3
C_23C3:
	CMP	AX,2Ah
	JB	C_23CA
	XOR	AX,AX
C_23CA:
	SHL	AX,1
	MOV	BX,AX
	MOV	AX,D_C264[BX]
	MOV	D_C262,AX
	MOV	BX,AX
	CMP	D_00F8,1
	JLE	C_23AD

	JMP	C_24E6
C_23E1:
	MOV	SI,D_C262
	MOV	AH,0
	MOV	AL,D_C2B8[SI+1]
	MOV	D_010C,AX
	MOV	AL,D_C2B8[SI+2]
	MOV	CL,AL
	AND	AL,1
	SHR	CL,1
	MOV	BYTE PTR D_D43E,CL
	MOV	D_00EC,AX
	MOV	CX,playerPosClamp
	CMP	D_011A,0
	JNZ	C_2433
	MOV	AL,D_C2B8[SI+4]
	CBW	
	ADD	playerPosClamp,AX
	MOV	CX,playerPosClamp
	CMP	CX,min_boundary
	JG	C_2425
	MOV	CX,min_boundary
	MOV	playerPosClamp,CX
C_2425:
	CMP	CX,max_boundary
	JL	C_2433
	MOV	CX,max_boundary
	MOV	playerPosClamp,CX
C_2433:
	CMP	D_00FA,0
	JG	C_2477
	ADD	CX,8
	CMP	D_D43A,0
	JZ	C_2447
	SUB	CX,4
C_2447:
	CMP	D_010C,0Bh
	JNZ	C_2467
	PUSH	CX
	SUB	CX,enemyClamp
	CMP	CX,-10h
	POP	CX
	JL	C_2477
	MOV	D_010C,0
	MOV	DX,D_C264[0]
	MOV	D_C262,DX
C_2467:
	CMP	CX,enemyClamp
	JLE	C_2477
	SUB	CX,enemyClamp
	SUB	playerPosClamp,CX
	SUB	AX,CX
C_2477:
	;-- --
	CMP	D_011A,0
	JZ	C_248C

	MOV	CX,enemyClamp
	SUB	CX,170
	MOV	cameraClamp,CX

	JMP	SHORT C_2498
C_248C:
	MOV	CX,playerPosClamp
	SUB	CX,96h
	MOV	cameraClamp,CX
C_2498:	;-- --
	CMP	CX,minCameraScroll
	JG	C_24A6
	MOV	CX,minCameraScroll
	MOV	cameraClamp,CX
C_24A6:
	CMP	CX,maxCameraScroll
	JL	C_24B4
	MOV	CX,maxCameraScroll
	MOV	cameraClamp,CX
C_24B4:
	CMP	D_011A,0
	JNZ	C_24C2
	MOV	AL,D_C2B8[SI+6]
	MOV	D_B9C0[0],AL
C_24C2:
	;-- --
	PUSH	playerPosClamp
	LEA	AX,D_C2B8[7]
	ADD	AX,SI
	PUSH	AX
	LEA	AX,D_B9C0[3]
	ADD	AX,D_D434
	PUSH	AX
	CALL	C_2341
	ADD	SP,6
	;-- --
	ADD	D_D434,8
	ADD	D_C262,11h
C_24E6:
	;-- --
	CMP	D_00FA,1
	JLE	C_24F0
	JMP	C_2652
C_24F0:
	;-- SCRIPT 2 --
	MOV	SI,D_CC78
	CMP	BYTE PTR D_CCCE[SI],0FFh
	JNZ	C_2516
	CALL	C_268A
	CMP	D_00FA,1
	JLE	C_2508
	JMP	C_2652
C_2508:
	MOV	BX,AX
	SHL	BX,1
	MOV	SI,D_CC7A[BX]
	MOV	D_CC78,SI

	JMP	C_24F0
C_2516:
	MOV	AH,0
	MOV	AL,D_CCCE[SI+1]
	MOV	D_0110,AX
	MOV	AL,D_CCCE[SI+2]
	MOV	CL,AL
	SHR	CL,1
	MOV	BYTE PTR [D_D440],CL
	AND	AL,1
	MOV	D_0112,AX
	CMP	D_0118,0
	JZ	C_253A
	JMP	C_25BD
C_253A:
	MOV	AL,D_CCCE[SI+4]
	CBW	
	ADD	enemyClamp,AX
	CMP	D_012E,0
	JNZ	C_2564
	CMP	D_00F0,0
	JNZ	C_2564
	CMP	D_D43A,0
	JNZ	C_2564
	MOV	AX,max_boundary
	CMP	AX,enemyClamp
	JGE	C_2564
	MOV	enemyClamp,AX
C_2564:
	CMP	D_00FA,0
	JG	C_25A6
	MOV	AX,playerPosClamp
	ADD	AX,8
	CMP	D_D43A,0
	JZ	C_257D
	SUB	AX,4
	JMP	SHORT C_259D
C_257D:
	CMP	D_0110,0Ah
	JNZ	C_259D
	MOV	DX,enemyClamp
	SUB	DX,AX
	CMP	DX,10h
	JG	C_25A6
	MOV	D_0110,0
	MOV	DX,D_CC7A
	MOV	D_CC78,DX
C_259D:
	CMP	enemyClamp,AX
	JGE	C_25A6
	MOV	enemyClamp,AX
C_25A6:
	MOV	AX,D_0100
	CMP	enemyClamp,AX
	JGE	C_25B2
	MOV	enemyClamp,AX
C_25B2:
	MOV	AL,D_CCCE[SI+6]
	CMP	AL,0
	JZ	C_25BD
	MOV	D_B9C0[0],AL
C_25BD:
	MOV	DI,D_D434
	MOV	AL,D_CCCE[SI+8]
	MOV	D_B9C0[DI+3].f_00,AL

	MOV	AX,WORD PTR D_CCCE[SI+9]
	CMP	D_00F2,0
	JZ	C_25D7
	ADD	AX,4
C_25D7:
	CMP	D_D43A,0
	JZ	C_25E1
	ADD	AX,0Ch
C_25E1:
	ADD	AX,enemyClamp
	CMP	D_00F2,0
	JNZ	C_25F6
	CMP	D_D43A,0
	JNZ	C_25F6
	OR	AX,4000h
C_25F6:
	MOV	WORD PTR D_B9C0[DI+3].f_01,AX

	MOV	AL,D_CCCE[SI+0Bh]
	CMP	D_D43A,0
	JZ	C_2609
	ADD	AL,D_D43C
C_2609:
	MOV	D_B9C0[DI+3].f_03,AL
	MOV	AL,D_CCCE[SI+0Dh]
	CMP	D_D43A,0
	JNZ	C_261C
	ADD	AL,[D_0158]
C_261C:
	MOV	D_B9C0[DI+3+4].f_00,AL
	MOV	AX,WORD PTR D_CCCE[SI+0Eh]
	CMP	D_00F2,0
	JZ	C_262E
	ADD	AX,4
C_262E:
	CMP	D_D43A,0
	JZ	C_2638
	ADD	AX,0Ch
C_2638:
	ADD	AX,enemyClamp
	MOV	WORD PTR D_B9C0[DI+3+4].f_01,AX
	MOV	AL,D_CCCE[SI+10h]
	MOV	D_B9C0[DI+3+4].f_03,AL
	ADD	D_D434,8
	ADD	D_CC78,11h
C_2652:	;-- --
	PUSH	D_C234
	PUSH	D_C232

	MOV	AX,D_D434
	ADD	AX,3
	MOV	D_D442,AX

	LEA	AX,D_B9C0[3]
	ADD	AX,D_D434
	PUSH	AX
	MOV	D_D438,AX
	CALL	C_2329	;local memcpy?
	ADD	SP,6
	ADD	D_D434,AX

	MOV	SI,D_D434
	MOV	BYTE PTR D_B9C0[SI+3].f_00,0FFh
	ADD	SI,3
	MOV	D_B9BE,SI

	RET	
;----------------------------------------
PROG	ENDS
;########################################
END
