;========================================
;	KARATEKA
;	Copyright 1986 Jordan Mechner
;	IBM version by The Connelley Group
;	reverse-coded by ergonomy_joe 2022
;========================================
TITLE DrawMeters

EXTRN D_00F8:WORD
EXTRN k_StrR:WORD	;0114
EXTRN k_StrL:WORD	;0116
EXTRN D_0118:WORD
EXTRN D_011A:WORD
EXTRN D_0170:WORD
EXTRN cga_buffer:BYTE

DGROUP	GROUP DATA
;########################################
DATA	SEGMENT WORD PUBLIC 'DATA'
;----------------------------------------
PUBLIC D_D4E0
D_D4E0	dw 0	;previous k_StrR
PUBLIC D_D4E2
D_D4E2	dw 0	;previous k_StrL

;triangle_1
;x.x.............
;x.x.x...........
;x.x.x.x.........
;x.x.x.x.x.......
;x.x.x.x.........
;x.x.x...........
;x.x.............
D_D4E4	db 0A0h,000h
	db 0A8h,000h
	db 0AAh,000h
	db 0AAh,080h
	db 0AAh,000h
	db 0A8h,000h
	db 0A0h,000h

;triangle_2
;.............x.x
;...........x.x.x
;.........x.x.x.x
;.......x.x.x.x.x
;.........x.x.x.x
;...........x.x.x
;.............x.x
D_D4F2	db 00h,05h
	db 00h,15h
	db 00h,55h
	db 01h,55h
	db 00h,55h
	db 00h,15h
	db 00h,05h

D_D500	db 0
D_D501	db 0
;----------------------------------------
DATA	ENDS
;########################################
PROG	SEGMENT PUBLIC BYTE 'PROG'
	ASSUME CS:PROG,DS:DGROUP
;----------------------------------------30A1
PUBLIC DrMeters
DrMeters:
	CMP	D_00F8,0
	JZ	C_30AE
	MOV	k_StrL,0
C_30AE:
	;-- backup values --
	PUSH	k_StrL
	PUSH	k_StrR
	;-- --
	CMP	D_0170,0
	JZ	C_30C3
	MOV	k_StrL,0
C_30C3:
	;-- --
	CMP	D_0118,0
	JZ	C_30D3
	MOV	k_StrR,0
	JMP	C_314F	;(+nop)
C_30D3:
	;-- --
	CMP	D_011A,0
	JZ	C_30E3
	MOV	k_StrL,0
	JMP	C_314F	;(+nop)
C_30E3:
	;------------
	;-- [Left] --
	MOV	AX,k_StrL
	SUB	AX,D_D4E2
	CMP	AX,1
	JZ	C_3114
	JG	C_314F
	CMP	AX,0FFFFh
	;%%%% skip code because buggy? %%%%
	JMP	C_314F	;(+nop)
	;%%%% %%%%
__30F7:
	JL	C_314F
	JNZ	C_311B
	MOV	BX,k_StrL
	INC	BX
	MOV	AX,BX
	SHL	AX,1
	ADD	AX,BX
	SUB	AX,2
	LEA	DI,cga_buffer[3C50h]
	ADD	DI,AX
	CALL	C_31DF	;clear 8x8 @ DI

	JMP	SHORT C_311B
C_3114:
	MOV	BX,k_StrL
	CALL	C_3208	;put 1 triangle(for Left)
C_311B:
	;-------------
	;-- [Right] --
	MOV	AX,k_StrR
	SUB	AX,D_D4E0
	CMP	AX,1
	JZ	C_3146
	JG	C_314F
	CMP	AX,0FFFFh
	JL	C_314F
	JNZ	C_3170
	MOV	BX,k_StrR
	INC	BX
	MOV	AX,BX
	SHL	AX,1
	ADD	AX,BX
	LEA	DI,cga_buffer[3CA0h]
	SUB	DI,AX
	CALL	C_31DF	;clear 8x8 @ DI

	JMP	SHORT C_3170
C_3146:
	MOV	BX,k_StrR
	CALL	C_321F	;put 1 triangle(for Right)

	JMP	SHORT C_3170
	;--   --
	;-------
C_314F:
	CALL	ClearBottom
	;------------
	;-- [Left] --
	MOV	BX,k_StrL
	CMP	BX,0
	JZ	C_3161
C_315B:
	CALL	C_3208	;put 1 triangle(for Left)
	DEC	BX
	JG	C_315B
C_3161:
	;-------------
	;-- [Right] --
	MOV	BX,k_StrR
	CMP	BX,0
	JZ	C_3170
C_316A:
	CALL	C_321F	;put 1 triangle(for Right)
	DEC	BX
	JG	C_316A
	;--   --
	;-------
C_3170:
	MOV	AX,k_StrL
	MOV	D_D4E2,AX
	MOV	AX,k_StrR
	MOV	D_D4E0,AX
	;--------------------------------
	;-- [Left]Blink when 2 or less --
	CMP	D_D500,0
	JZ	C_3189
	DEC	D_D500
	JMP	SHORT C_31A9
C_3189:
	CMP	k_StrL,2
	JG	C_31A9
	MOV	D_D500,1
	LEA	DI,cga_buffer[3C51h]
	CALL	C_31DF	;clear 8x8 @ DI
	LEA	DI,cga_buffer[3C54h]
	CALL	C_31DF	;clear 8x8 @ DI
	MOV	D_D4E2,0
C_31A9:
	;---------------------------------
	;-- [Right]Blink when 2 or less --
	CMP	D_D501,0
	JZ	C_31B6
	DEC	D_D501
	JMP	SHORT C_31D6
C_31B6:
	CMP	k_StrR,2
	JG	C_31D6
	MOV	D_D501,1
	LEA	DI,cga_buffer[3C9Dh]
	CALL	C_31DF	;clear 8x8 @ DI
	LEA	DI,cga_buffer[3C9Ah]
	CALL	C_31DF	;clear 8x8 @ DI
	MOV	D_D4E0,0
C_31D6:
	;--   --
	;-------
	;-- restore values --
	POP	k_StrR
	POP	k_StrL
	;-- --
	RET	
;----------------------------------------
;clear 8x8 @ DI
C_31DF:
	MOV	CX,7
C_31E2:
	MOV	WORD PTR [DI],0
	ADD	DI,50h
	LOOP	C_31E2

	RET	
;----------------------------------------31EC
PUBLIC ClearBottom
ClearBottom:
	LEA	DI,cga_buffer[3980h]
	XOR	AX,AX
	MOV	CX,280h
	REPZ	STOSW

	RET
;----------------------------------------
;put 1 triangle(DI)
C_31F8:
	MOV	CX,7
C_31FB:
	MOV	AX,[SI]
	MOV	[DI],AX
	ADD	SI,2
	ADD	DI,50h
	LOOP	C_31FB

	RET
;----------------------------------------
;put 1 triangle(for Left)
C_3208:
	MOV	AX,BX
	SHL	AX,1
	ADD	AX,BX
	SUB	AX,2
	LEA	DI,cga_buffer[3C50h]
	ADD	DI,AX
	LEA	SI,[D_D4E4]
	CALL	C_31F8	;put 1 triangle(DI)

	RET	
;----------------------------------------
;put 1 triangle(for Right)
C_321F:
	MOV	AX,BX
	SHL	AX,1
	ADD	AX,BX
	;
	LEA	DI,cga_buffer[3CA0h]
	SUB	DI,AX
	LEA	SI,[D_D4F2]
	CALL	C_31F8	;put 1 triangle(DI)

	RET	
;----------------------------------------
PROG	ENDS
;########################################
END

