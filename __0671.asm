;========================================
;	KARATEKA
;	Copyright 1986 Jordan Mechner
;	IBM version by The Connelley Group
;	reverse-coded by ergonomy_joe 2022
;========================================
INCLUDE COMMON.MAC

EXTRN cameraClamp:WORD
EXTRN D_00EA:WORD
EXTRN D_00EE:WORD
EXTRN D_0168:WORD	;allows CTRL+V?
EXTRN cga_buffer:BYTE

EXTRN D_BB60:BYTE
EXTRN D_DE85:WORD

EXTRN C_191C:NEAR	;delay+control?
EXTRN DrMeters:NEAR	;30A1
EXTRN sound:NEAR	;3C41
EXTRN C_414A:NEAR

GR_SEG	EQU 0B800h

DGROUP	GROUP DATA

;########################################
DATA	SEGMENT WORD PUBLIC 'DATA'
;----------------------------------------
D_42A8	dw 0
D_42AA	dw 0
figDataIndex	dw 0	;42AC
figMaskIndex	dw 0	;42AE
fig_stride	db 0	;42B0
fig_height	db 0	;42B1
fig_width	dw 0	;42B2
D_42B4	db 0
D_42B5	db 0
D_42B6	dw 0
D_42B8	db 0
D_42B9	db 50h	;# bytes per scanline
rleDataCount	db 0	;42BA
rleDataByte	db 0	;42BB
rleMaskCount	db 0	;42BC
rleMaskByte	db 0	;42BD
v_offset	dw 0	;42BE
D_42C0	dw 0
D_42C2	dw 0FF00h,3FC0h,0FF0h,03FCh
;-- --
PUBLIC ks_index
ks_index	dw 100h dup(0)	;42CA
PUBLIC ks_data
ks_data	db 41A0h dup(0)	;44CA
PUBLIC km_index
km_index	dw 100h dup(0)	;866A
PUBLIC km_data
km_data	db 1D9Ch dup(0)	;886A
PUBLIC D_A606
D_A606	db 10F4h dup(0)
;-- --
PUBLIC D_B6FA
;font data?
D_B6FA	dw 050h,064h,078h,08Ch,0A0h,0B4h,0CCh,0E4h
	dw 24Eh,258h,120h,134h,13Eh,152h,166h,17Ah
	dw 192h,1AAh,1BEh,1D2h,1E6h,1FAh,20Eh,222h
	dw 236h,0F8h,264h,26Eh,27Ah,286h,286h,292h
	dw 292h,292h,292h,292h,292h,292h,292h,292h
;B74A
	db 000h,01Ch,038h,030h,07Ch,067h,063h,073h,08Eh,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,07Ah,0CEh,0C6h,07Eh,0C6h,086h,086h,0FEh,000h,000h,000h
	db 000h,000h,000h,000h,001h,001h,000h,000h,000h,07Ch,0C7h,043h,003h,003h,083h,087h
	db 07Eh,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,00Ch,01Eh,038h,07Ch,066h
	db 0C3h,0C3h,0C3h,0C7h,07Ch,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,07Ch,083h,003h,03Fh,003h,003h,087h,07Ch,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,07Ch,066h,006h,006h,006h,006h,006h,03Eh,006h,006h,007h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,03Ch,0E3h,043h
	db 003h,0E3h,083h,087h,0FCh,080h,060h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,007h,007h,006h,0FEh,08Eh,086h,086h,086h,0C6h,0C6h,000h,000h,000h
	db 000h,001h,001h,001h,001h,000h,001h,000h,000h,0FFh,0E3h,070h,078h,03Ch,01Eh,00Fh
	db 0FFh,000h,000h,001h,000h,000h,000h,000h,000h,003h,003h,000h,000h,0FFh,0E3h,070h
	db 078h,03Ch,01Eh,00Fh,0FFh,000h,000h,001h,000h,000h,000h,000h,000h,003h,003h,007h
	db 007h,086h,0C6h,066h,036h,03Eh,066h,0C6h,086h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,001h,007h,007h,006h,006h,006h,006h,006h,006h,006h,03Eh,000h,000h,076h
	db 0EEh,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,000h,000h,00Fh,01Dh,038h,038h,038h,038h,01Ch
	db 03Ch,000h,000h,0E6h,09Eh,086h,086h,086h,086h,0C6h,0C6h,000h,000h,000h,001h,001h
	db 001h,001h,001h,000h,001h,000h,000h,07Eh,0C3h,083h,083h,083h,083h,087h,078h,000h
	db 000h,000h,000h,001h,001h,001h,001h,000h,000h,000h,000h,0F6h,08Eh,086h,086h,086h
	db 086h,08Eh,0F6h,006h,007h,000h,000h,000h,001h,003h,003h,003h,003h,001h,000h,000h
	db 000h,000h,000h,0FEh,0C2h,082h,082h,082h,082h,0C6h,0BEh,080h,080h,000h,000h,001h
	db 001h,001h,001h,001h,001h,001h,001h,001h,001h,000h,000h,0F6h,08Eh,086h,086h,086h
	db 07Eh,0E6h,086h,000h,000h,000h,003h,003h,003h,001h,000h,000h,003h,000h,000h,038h
	db 067h,007h,01Ch,038h,060h,067h,03Ch,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,00Ch,09Ch,0FFh,01Dh,01Ch,01Ch,01Ch,01Ch,09Ch,0F8h,000h,001h,001h,000h,000h
	db 000h,000h,000h,001h,000h,000h,000h,0C3h,0C3h,0C3h,0C3h,0C3h,0C3h,0E3h,09Eh,000h
	db 000h,000h,000h,000h,000h,000h,000h,001h,000h,000h,000h,087h,087h,08Eh,08Ch,05Ch
	db 078h,070h,070h,000h,000h,003h,001h,000h,000h,000h,000h,000h,000h,000h,000h,0C7h
	db 087h,08Eh,08Eh,05Ch,07Ch,038h,038h,000h,000h,0C1h,041h,021h,023h,036h,03Ch,01Ch
	db 01Ch,000h,000h,043h,067h,03Eh,01Ch,03Ch,076h,063h,0C1h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,007h,00Eh,09Ch,09Ch,0B8h,078h,040h,060h,06Ch
	db 038h,000h,000h,007h,003h,001h,001h,000h,000h,000h,000h,000h,000h,000h,000h,006h
	db 006h,006h,006h,006h,006h,006h,006h,000h,000h,01Eh,01Ch,01Ch,01Ch,01Ch,01Ch,01Ch
	db 01Ch,01Ch,00Fh,000h,000h,000h,000h,000h,000h,000h,000h,003h,003h,000h,000h,000h
	db 000h,000h,000h,000h,000h,003h,003h,003h,001h,000h,000h,000h,000h,003h,003h,000h
	db 003h,003h,000h,000h,000h,000h,000h,000h,000h,000h,03Fh,000h,000h,000h,000h,000h
	db 000h,040h,040h,0C0h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,0E0h,0E0h,000h,0E0h,0E0h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,0FEh,000h,000h
;-- --
PUBLIC D_B9BA
D_B9BA	dw 0	;prev cameraClamp?
__B9BC	db 0,0
PUBLIC D_B9BE	;index in D_B9C0?
D_B9BE	dw 0
PUBLIC D_B9C0
D_B9C0	db 0	;music
	db 0	;wipe
	db 0	;bg
	;-- --
	s_Fig<1, 20, 40>
	s_Fig<1, 40, 60>
	s_Fig<1, 60, 80>
	s_Fig<2,150,100>
	s_Fig<2,160,105>
	s_Fig<2,170,110>
__B9DB	s_Fig<-1,0,0>
__B9DF  db 4Ah dup(0,0,0,0)
__BB07	db 0
;-- --
;----------------------------------------
DATA	ENDS
;########################################
PROG	SEGMENT PUBLIC BYTE 'PROG'
	ASSUME CS:PROG,DS:DGROUP
;----------------------------------------
;uncalled?
__0671:
	PUSH	BP
	MOV	BP,SP
	
	MOV	AX,[BP+04]
	MOV	AH,0
	INT	10h
	
	POP	BP
	RET
;----------------------------------------
;uncalled?
__067D:
	PUSH	BP
	MOV	BP,SP
	
	MOV	AX,[BP+04]
	MOV	DL,AL
	MOV	AX,[BP+06]
	MOV	DH,AL
	XOR	BH,BH
	MOV	AH,02
	INT	10h
	
	POP	BP
	RET	
;----------------------------------------0692
PUBLIC k_rand
k_rand:
	PUSH	BP
	MOV	BP,SP
	
	CALL	C_414A
	SHL	AX,1
	MOV	D_42A8,AX
	
	MOV	AX,[BP+04]
	INC	AX
	MUL	D_42A8
	MOV	AX,DX
	
	POP	BP
	RET
;----------------------------------------06A9
putFig_flipx:
	PUSH	BP
	MOV	BP,SP
	;-- --
	MOV	rleDataCount,0
	MOV	rleMaskCount,0

	MOV	SI,[BP+04]	;fig?
	SHL	SI,1

	MOV	BX,ks_index[SI]
	MOV	figDataIndex,BX

	MOV	AX,km_index[SI]
	MOV	figMaskIndex,AX

	MOV	AL,ks_data[BX]
	MOV	fig_stride,AL
	CBW	
	SHL	AX,1
	SHL	AX,1
	MOV	fig_width,AX

	MOV	AL,ks_data[BX+1]
	MOV	fig_height,AL
	CBW	
	SUB	[BP+08],AX	;y?

	MOV	AX,[BP+08]	;y?
	MUL	D_42B9
	MOV	v_offset,AX

	ADD	figDataIndex,3
	ADD	figMaskIndex,3

	MOV	AX,[BP+06]	;x?
	SUB	AX,cameraClamp
	MOV	[BP+06],AX	;x?
	MOV	BX,AX
	AND	AL,3
	MOV	D_42B4,AL
	SHL	AL,1
	MOV	D_42B5,AL
	SUB	BX,320
	ADD	BX,fig_width
	JGE	C_071B
	JMP	C_0796	;(+nop)
C_071B:
	CMP	WORD PTR [BP+06],320	;x?
	JL	C_0725
	JMP	C_08A3	;ret
C_0725:
	SHR	BX,1
	SHR	BX,1
	JZ	C_073E
C_072B:
	MOV	AL,fig_height
	CBW	
	MOV	DI,AX
C_0731:
	CALL	C_0BC7	;read RLE infos?
	DEC	DI
	JNZ	C_0731
	;
	DEC	fig_stride
	DEC	BX
	JNZ	C_072B
C_073E:
	;-- --
	CMP	D_42B4,0
	JZ	C_0796

	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,v_offset
	ADD	DI,4Fh
C_0752:
	CALL	C_0BC7	;read RLE infos?
	CMP	AL,0	;==rleMaskByte
	JZ	C_0784
	MOV	CX,8
C_075C:
	SHR	AL,1
	RCL	AH,1
	SHR	DL,1	;==rleDataByte
	RCL	DH,1
	LOOP	C_075C
	XCHG	AL,AH
	MOV	DL,DH
	;
	;
	NOT	AX
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AL,cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AL,DL
	MOV	cga_buffer[DI],AL
C_0784:
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0752
	DEC	fig_stride
	JNZ	C_0796
	JMP	C_08A3	;ret
C_0796:
	MOV	AX,[BP+06]	;x?
	ADD	AX,fig_width
	JGE	C_07A2
	JMP	C_08A3	;ret
C_07A2:
	MOV	D_42B6,0
	CMP	WORD PTR [BP+06],0	;x?
	JGE	C_07B4
	MOV	D_42B6,1
C_07B4:
	MOV	AX,[BP+06]	;x?
	CMP	AX,0
	JGE	C_07BE
	XOR	AX,AX
C_07BE:
	MOV	DX,[BP+06]	;x?
	ADD	DX,fig_width
	CMP	DX,320
	JL	C_07D6
	MOV	DX,320
	MOV	BH,0
	MOV	BL,D_42B4
	SUB	DX,BX
C_07D6:
	SHR	AX,1
	SHR	AX,1
	SHR	DX,1
	SHR	DX,1
	MOV	D_42AA,DX
	SUB	DX,AX
	MOV	BL,DL
	JLE	C_0855
	MOV	AX,v_offset
	ADD	D_42AA,AX
	DEC	D_42AA
C_07F3:
	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,D_42AA
C_07FD:
	CALL	C_0BC7	;read RLE infos?
	CMP	AL,0	;==rleMaskByte
	JZ	C_0844
	MOV	CX,8
C_0807:
	SHR	AL,1
	RCL	AH,1
	SHR	DL,1	;==rleDataByte
	RCL	DH,1
	LOOP	C_0807
	XCHG	AL,AH
	MOV	DL,DH
	CMP	D_42B4,0
	JZ	C_0838
	NOT	AX
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AX,WORD PTR cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AX,DX
	MOV	WORD PTR cga_buffer[DI],AX

	JMP	SHORT C_0844
C_0838:
	XOR	AL,0FFh
	AND	AL,cga_buffer[DI]
	OR	AL,DL
	MOV	cga_buffer[DI],AL
C_0844:
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_07FD
	DEC	D_42AA
	DEC	BL
	JNZ	C_07F3
C_0855:
	CMP	D_42B6,0
	JZ	C_08A3	;ret
	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,v_offset
C_0866:
	CALL	C_0BC7	;read RLE infos?
	CMP	AL,0	;==rleMaskByte
	JZ	C_089A
	MOV	CX,8
C_0870:
	SHR	AL,1
	RCL	AH,1
	SHR	DL,1	;==rleDataByte
	RCL	DH,1
	LOOP	C_0870
	MOV	AL,AH
	MOV	DL,DH
	MOV	AH,0
	NOT	AX
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AH,cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AH,DH
	MOV	cga_buffer[DI],AH
C_089A:
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0866
C_08A3:	;-- --
	POP	BP
	RET	
;----------------------------------------08A5
putFig:
	PUSH	BP
	MOV	BP,SP
	;-- --
	MOV	rleDataCount,0
	MOV	rleMaskCount,0

	MOV	SI,[BP+04]	;fig?
	SHL	SI,1

	MOV	BX,ks_index[SI]
	MOV	figDataIndex,BX

	MOV	AX,km_index[SI]
	MOV	figMaskIndex,AX

	MOV	AL,ks_data[BX]
	MOV	fig_stride,AL
	CBW	
	SHL	AX,1
	SHL	AX,1
	MOV	fig_width,AX

	MOV	AL,ks_data[BX+1]
	MOV	fig_height,AL
	CBW	
	SUB	[BP+08],AX	;y?

	MOV	AX,[BP+08]	;y?
	MUL	D_42B9
	MOV	v_offset,AX

	ADD	figDataIndex,3
	ADD	figMaskIndex,3

	MOV	AX,[BP+06]	;x?
	SUB	AX,cameraClamp
	MOV	[BP+06],AX	;x?
	JL	C_0903
	JMP	C_09C0
C_0903:
	MOV	BX,AX
	ADD	BX,fig_width
	JG	C_090E
	JMP	C_0AD2	;ret
C_090E:
	XOR	BX,BX
	SUB	BX,AX
	MOV	AL,BL
	AND	AL,3
	MOV	AH,4
	SUB	AH,AL
	AND	AH,3
	MOV	D_42B4,AH
	SHL	AH,1
	MOV	D_42B5,AH
	SHR	BX,1
	SHR	BX,1
	JZ	C_0960
	;-- --
	CMP	BYTE PTR [BP+04],200	;fig?
	JB	C_0949

	MOV	AL,fig_height
	MUL	BL
	ADD	figDataIndex,AX
	SUB	fig_stride,BL
	SHL	BX,1
	SHL	BX,1
	ADD	[BP+06],BX	;x?
	
	JMP	SHORT C_0960
C_0949:
	MOV	AL,fig_height
	CBW	
	MOV	DI,AX
C_094F:
	CALL	C_0BC7	;read RLE infos?
	DEC	DI
	JNZ	C_094F
	ADD	WORD PTR [BP+06],4	;x?
	DEC	fig_stride
	DEC	BX
	JNZ	C_0949
C_0960:
	;-- --
	CMP	D_42B4,0
	JZ	C_09B5

	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,v_offset

	CMP	BYTE PTR [BP+04],200	;fig?
	JB	C_097A
	JMP	C_0AD4
C_097A:	;-- fig < 200 --
	CALL	C_0BC7	;read RLE infos?
	CMP	AL,0	;==rleMaskByte
	JZ	C_099F

	MOV	AH,0
	AND	DX,AX
	NOT	AX
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AH,cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AH,DH
	MOV	cga_buffer[DI],AH
C_099F:
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_097A
C_09A8:
	ADD	WORD PTR [BP+06],4	;x?
	DEC	fig_stride
	JNZ	C_09B5
	;-- --
	JMP	C_0AD2	;ret
C_09B5:
	MOV	AL,fig_stride
	CBW	
	SHL	AX,1
	SHL	AX,1
	MOV	fig_width,AX
C_09C0:
	MOV	AX,[BP+06]	;x?
	AND	AL,3
	MOV	D_42B4,AL
	SHL	AL,1
	MOV	D_42B5,AL
	MOV	AX,[BP+06]	;x?
	SUB	AX,320
	JLE	C_09D8
	JMP	C_0AD2	;ret
C_09D8:
	MOV	D_42B6,0
	ADD	AX,fig_width
	JLE	C_09FE
	CMP	D_42B4,0
	JZ	C_09F1
	MOV	D_42B6,1
C_09F1:
	MOV	AL,D_42B4
	CBW	
	ADD	AX,320
	SUB	AX,[BP+06]	;x?
	MOV	fig_width,AX
C_09FE:
	MOV	AX,[BP+06]	;x?
	MOV	DX,AX
	ADD	AX,fig_width
	SHR	AX,1
	SHR	AX,1
	SHR	DX,1
	SHR	DX,1
	SUB	AX,DX
	SUB	AX,D_42B6
	MOV	CH,AL
	JZ	C_0A87
	MOV	DI,v_offset
	MOV	DX,[BP+06]	;x?
	SHR	DX,1
	SHR	DX,1
	ADD	DI,DX
	MOV	D_42C0,DI
	;-- --
	CMP	BYTE PTR [BP+04],200	;fig?
	JB	C_0A33
	JMP	C_0B08
C_0A33:	;-- fig < 200 --
	MOV	AL,fig_height
	MOV	BL,AL
	MOV	DI,D_42C0
C_0A3C:
	CALL	C_0BC7	;read RLE infos?
	CMP	AL,0	;==rleMaskByte
	JZ	C_0A74

	CMP	D_42B4,0
	JZ	C_0A68

	MOV	AH,0
	NOT	AX
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AX,WORD PTR cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AX,DX	;==rleDataByte
	MOV	WORD PTR cga_buffer[DI],AX

	JMP	SHORT C_0A74
C_0A68:
	NOT	AL
	AND	AL,cga_buffer[DI]
	OR	AL,DL	;==rleDataByte
	MOV	cga_buffer[DI],AL
C_0A74:
	ADD	DI,50h
	DEC	BL
	JNZ	C_0A3C
	ADD	WORD PTR [BP+06],4	;x?
	INC	D_42C0
	DEC	CH
	JNZ	C_0A33
C_0A87:
	CMP	D_42B6,0
	JZ	C_0AD2	;ret
	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,v_offset
	ADD	DI,4Fh
	;-- --
	CMP	BYTE PTR [BP+04],200	;fig?
	JB	C_0AA4
	JMP	C_0B87
C_0AA4:	;-- fig < 200 --
	CALL	C_0BC7	;read RLE infos?
	CMP	AL,0	;==rleMaskByte
	JZ	C_0AC9

	MOV	AH,0
	AND	DX,AX
	NOT	AX
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AL,cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AL,DL
	MOV	cga_buffer[DI],AL
C_0AC9:
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0AA4
C_0AD2:	;-- --
	POP	BP
	RET	
C_0AD4:	;-- fig >= 200 --
	;-- --
	MOV	SI,figDataIndex
C_0AD8:
	MOV	DL,ks_data[SI]
	MOV	AX,0FF00h
	INC	SI
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AH,cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AH,DH
	MOV	cga_buffer[DI],AH

	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0AD8
	MOV	figDataIndex,SI
	JMP	C_09A8
C_0B08:	;-- fig >= 200 --
	MOV	BL,D_42B5
	XOR	BH,BH
	MOV	SI,figDataIndex
	CMP	BX,0
	JNZ	C_0B46
C_0B17:
	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,D_42C0
C_0B21:
	MOV	DL,ks_data[SI]
	INC	SI
	MOV	cga_buffer[DI],DL
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0B21
	ADD	WORD PTR [BP+06],4	;x?
	INC	D_42C0
	DEC	CH
	JNZ	C_0B17
	MOV	figDataIndex,SI
	JMP	C_0A87
C_0B46:
	MOV	AL,fig_height
	MOV	D_42B8,AL
	MOV	DI,D_42C0
C_0B50:
	MOV	DL,ks_data[SI]
	INC	SI
	MOV	AX,D_42C2[BX]
	AND	AX,WORD PTR cga_buffer[DI]
	MOV	CL,D_42B5
	XOR	DH,DH
	ROR	DX,CL
	OR	AX,DX
	MOV	WORD PTR cga_buffer[DI],AX
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0B50
	ADD	WORD PTR [BP+06],4	;x?
	INC	D_42C0
	DEC	CH
	JNZ	C_0B46
	MOV	figDataIndex,SI
	
	JMP	C_0A87
C_0B87:	;-- fig >= 200 --
	MOV	SI,figDataIndex
C_0B8B:
	MOV	DL,ks_data[SI]
	INC	SI
	MOV	AX,0FF00h
	MOV	CL,D_42B5
	ROR	AX,CL
	AND	AL,cga_buffer[DI]
	MOV	CL,D_42B5
	MOV	DH,0
	ROR	DX,CL
	OR	AL,DL
	MOV	cga_buffer[DI],AL
	ADD	DI,50h
	DEC	D_42B8
	JNZ	C_0B8B
	JMP	C_0AD2	;ret
;----------------------------------------0BB7
PUBLIC BB_clear
BB_clear:
	PUSH	ES
	;--
	PUSH	DS
	POP	ES
	LEA	DI,[cga_buffer]
	XOR	AX,AX
	MOV	CX,1F40h
	REPZ	STOSW
	;--
	POP	ES
	RET
;----------------------------------------
;read RLE infos?
C_0BC7:
	;-- data --
	CMP	rleDataCount,0
	JZ	C_0BD9
	MOV	DL,rleDataByte
	DEC	rleDataCount
	JMP	C_0BFE	;(+nop)
C_0BD9:
	MOV	SI,figDataIndex
	MOV	DL,ks_data[SI]
	INC	SI
	CMP	DL,7Bh	;rle tag
	JNZ	C_0BFA

	MOV	DL,ks_data[SI+1]
	MOV	rleDataCount,DL

	MOV	DL,ks_data[SI]
	MOV	rleDataByte,DL

	ADD	SI,2
C_0BFA:
	MOV	figDataIndex,SI
C_0BFE:	;-- mask --
	CMP	rleMaskCount,0
	JZ	C_0C0F
	MOV	AL,rleMaskByte
	DEC	rleMaskCount
	JMP	C_0C31	;(+nop)
C_0C0F:
	MOV	SI,figMaskIndex
	MOV	AL,km_data[SI]
	INC	SI
	CMP	AL,7Bh	;rle tag
	JNZ	C_0C2D

	MOV	AL,km_data[SI+1]
	MOV	rleMaskCount,AL

	MOV	AL,km_data[SI]
	MOV	rleMaskByte,AL

	ADD	SI,2
C_0C2D:
	MOV	figMaskIndex,SI
C_0C31:	;-- --
	RET	
;----------------------------------------0C32
PUBLIC render
render:
	MOV	D_B9BE,3
	;-- --
	MOV	AL,D_B9C0[2]	;bg
	CALL	renderBG
	;-- render figs --
C_0C3E:
	MOV	SI,D_B9BE
	;-- ax --
	MOV	AL,D_B9C0[SI].f_00
	CMP	AL,0FFh
	JZ	C_0C7D
	MOV	AH,0
	;-- cx --
	MOV	CL,D_B9C0[SI].f_03
	MOV	CH,0
	PUSH	CX	;y
	;-- dx --
	MOV	DX,WORD PTR D_B9C0[SI].f_01
	TEST	DX,8000h
	JNZ	C_0C6E
	TEST	DX,4000h	;flipx
	JZ	C_0C6E
	XOR	DX,4000h
	PUSH	DX	;x
	PUSH	AX	;fig
	CALL	putFig_flipx
	JMP	SHORT C_0C73
C_0C6E:
	PUSH	DX	;x
	PUSH	AX	;fig
	CALL	putFig
C_0C73:
	ADD	SP,6
	;-- --
	ADD	D_B9BE,4

	JMP	C_0C3E
C_0C7D:
	;-- present backbuffer --
	CMP	D_B9C0[1],0	;wipe
	JNZ	C_0CAB
	;-- --
	CMP	D_00EE,0
	JZ	C_0C9B
	;-- --
	CALL	BB_flip
	;--
	MOV	AL,D_B9C0[0]
	MOV	AH,0
	PUSH	AX
	CALL	sound
	ADD	SP,2
	;--
	RET	
C_0C9B:	;-- --
	CALL	BB_flip_part
	;--
	MOV	AL,D_B9C0[0]
	MOV	AH,0
	PUSH	AX
	CALL	sound
	ADD	SP,2
	;--
	RET	
C_0CAB:	;-- wipe --
	CALL	BB_flip_wipe
	;--
	MOV	AL,D_B9C0[0]
	MOV	AH,0
	PUSH	AX
	CALL	sound
	ADD	SP,2
	;--
	RET	
;----------------------------------------0CBB
renderBG:
	PUSH	ES
	;-- --
	PUSH	DS
	POP	ES
	;-- --
	CMP	D_00EE,1
	JNZ	C_0CE8
	;--
	CMP	D_00EA,2
	JGE	C_0CDB
	;--
	MOV	CX,0C80h
	MOV	AX,5555h
	LEA	DI,[cga_buffer]
	REPZ	STOSW

	JMP	C_0CE8	;(+nop)
C_0CDB:
	CALL	BB_clear
	;-- life bars--
	CMP	D_BB60,0
	JNZ	C_0CE8
	CALL	DrMeters
C_0CE8:	;-- --
	CMP	D_00EA,2
	JGE	C_0D4F
	;-- --
	MOV	AX,cameraClamp
	CMP	AX,D_B9BA
	JZ	C_0D24
	;-- render wall? --
	MOV	CX,0F00h
	MOV	AX,5555h
	LEA	DI,cga_buffer[0A00h]
	REPZ	STOSW

	MOV	CX,550h
	LEA	SI,[D_A606 + 1500]	;ABE2
	LEA	DI,cga_buffer[1900h]
	REPZ	MOVSW

	LEA	DI,cga_buffer[2120h]	;2457
	MOV	AX,0FFFFh
	MOV	CX,28h
	REPZ	STOSW
	XOR	AX,AX
	MOV	CX,78h
	REPZ	STOSW
C_0D24:	;-- --
	CALL	C_0E39	;clear backbuffer:line 114~135 or 108~132
	;-- render floor? --
	LEA	DI,cga_buffer[3020h]	;3357
	MOV	BL,0Fh
	MOV	DX,9999h
	MOV	AX,cameraClamp
	AND	AX,1
	JZ	C_0D3B
	MOV	DX,6666h
C_0D3B:
	MOV	AX,DX
	MOV	CX,28h
	REPZ	STOSW
	NOT	AX
	MOV	CX,28h
	REPZ	STOSW
	DEC	BL
	JNZ	C_0D3B
	;-- --
	POP	ES
	RET	
C_0D4F:	;-- --
	PUSH	AX
	CALL	C_0E39	;clear backbuffer:line 114~135 or 108~132
	POP	AX
	;-- --
	CMP	D_00EA,3
	JGE	C_0DA2
	;-- --
	MOV	AX,cameraClamp
	CMP	AX,D_B9BA
	JZ	C_0D87

	LEA	DI,cga_buffer[0A00h]
	MOV	CX,0F00h
	XOR	AX,AX
	REPZ	STOSW

	LEA	SI,[D_A606 + 1500]	;ABE2
	LEA	DI,cga_buffer[1900h]
	MOV	CX,370h
	REPZ	MOVSW

	LEA	DI,cga_buffer[1C20h] ;1F57
	XOR	AX,AX
	MOV	CX,50h
	REPZ	STOSW
C_0D87: ;-- --
	LEA	DI,cga_buffer[3020h]	;3357
	MOV	BL,0Fh
C_0D8D:
	MOV	AX,0AAAAh
	MOV	CX,28h
	REPZ	STOSW
	XOR	AX,AX
	MOV	CX,28h
	REPZ	STOSW
	DEC	BL
	JNZ	C_0D8D
	;-- --
	POP	ES
	RET	
C_0DA2:	;-- --
	MOV	AX,cameraClamp
	CMP	AX,D_B9BA
	JZ	C_0DB6

	LEA	DI,[cga_buffer]
	MOV	CX,11D0h
	XOR	AX,AX
	REPZ	STOSW








C_0DB6: ;-- --
	LEA	DI,cga_buffer[3020h]	;3357
	MOV	BL,0Fh
C_0DBC:
	MOV	AX,5555h
	MOV	CX,28h
	REPZ	STOSW
	XOR	AX,AX
	MOV	CX,28h
	REPZ	STOSW
	DEC	BL
	JNZ	C_0DBC
	;-- --
	POP	ES
	RET	
;----------------------------------------0DD1
PUBLIC BB_flip
BB_flip:
	MOV	BX,0
	LEA	SI,[cga_buffer]
	PUSH	ES
	MOV	AX,GR_SEG
	MOV	ES,AX
C_0DDE:
	MOV	DI,D_DE85[BX]
	MOV	CX,28h
	REPZ	MOVSW
	ADD	BX,2
	CMP	BX,190h
	JNZ	C_0DDE
	POP	ES
	;-- --
	RET
;----------------------------------------0DF2
BB_flip_part:
	;-- --
	MOV	DL,168
	LEA	SI,cga_buffer[0A00h]
	MOV	BX,64
	;-- --
	MOV	AX,cameraClamp
	CMP	AX,D_B9BA
	JNZ	C_0E1E
	;-- --
	MOV	DL,84
	LEA	SI,cga_buffer[2440h]	;2777
	MOV	BX,232
	;--
	CMP	D_00EA,2
	JL	C_0E1E
	;--
	ADD	DL,6
	SUB	SI,1E0h
	SUB	BX,12
C_0E1E:
	MOV	D_B9BA,AX
	PUSH	ES
	MOV	AX,GR_SEG
	MOV	ES,AX
C_0E27:
	MOV	DI,D_DE85[BX]
	MOV	CX,28h
	REPZ	MOVSW	
	ADD	BX,2
	DEC	DL
	JNZ	C_0E27
	POP	ES
	;-- --
	RET	
;----------------------------------------
;clear backbuffer:line 114~135 or 108~132
C_0E39:
	LEA	DI,cga_buffer[23A0h]	;26D7
	MOV	CX,690h
	CMP	D_00EA,2
	JL	C_0E4F
	SUB	DI,1E0h
	ADD	CX,0F0h
C_0E4F:
	XOR	AX,AX
	PUSH	ES
	PUSH	DS
	POP	ES
	REPZ	STOSW
	POP	ES

	RET	
;----------------------------------------0E58
BB_flip_wipe:
	PUSH	BP
	XOR	BX,BX
	PUSH	ES
	MOV	AX,GR_SEG
	MOV	ES,AX
C_0E61:
	;-- --
	LEA	SI,[cga_buffer]
	XOR	BP,BP
	MOV	CX,100
C_0E6A:
	MOV	DI,DS:D_DE85[BP]
	MOV	AL,[BX+SI]
	AND	AL,0C0h
	AND	BYTE PTR ES:[BX+DI],3Fh
	OR	ES:[BX+DI],AL
	MOV	AL,[BX+SI+50h]
	AND	AL,0C0h
	ADD	BP,2
	MOV	DI,DS:D_DE85[BP]
	AND	BYTE PTR ES:[BX+DI],3Fh
	OR	ES:[BX+DI],AL
	ADD	SI,0A0h
	ADD	BP,2
	LOOP	C_0E6A
	;-- --
	CALL	wipe_delay
	;-- --
	LEA	SI,[cga_buffer]
	XOR	BP,BP
	MOV	CX,100
C_0EA3:
	MOV	DI,DS:D_DE85[BP]
	MOV	AL,[BX+SI]
	AND	AL,30h
	AND	BYTE PTR ES:[BX+DI],0CFh
	OR	ES:[BX+DI],AL
	MOV	AL,[BX+SI+50h]
	AND	AL,30h
	ADD	BP,2
	MOV	DI,DS:D_DE85[BP]
	AND	BYTE PTR ES:[BX+DI],0CFh
	OR	ES:[BX+DI],AL
	ADD	SI,0A0h
	ADD	BP,2
	LOOP	C_0EA3
	;-- --
	CALL	wipe_delay
	;-- --
	LEA	SI,[cga_buffer]
	XOR	BP,BP
	MOV	CX,100
C_0EDC:
	MOV	DI,DS:D_DE85[BP]
	MOV	AL,[BX+SI]
	AND	AL,0Ch
	AND	BYTE PTR ES:[BX+DI],0F3h
	OR	ES:[BX+DI],AL
	MOV	AL,[BX+SI+50h]
	AND	AL,0Ch
	ADD	BP,2
	MOV	DI,DS:D_DE85[BP]
	AND	BYTE PTR ES:[BX+DI],0F3h
	OR	ES:[BX+DI],AL
	ADD	SI,0A0h
	ADD	BP,2
	LOOP	C_0EDC
	;-- --
	CALL	wipe_delay
	;-- --
	LEA	SI,[cga_buffer]
	XOR	BP,BP
	MOV	CX,100
C_0F15:
	MOV	DI,DS:D_DE85[BP]
	MOV	AL,[BX+SI]
	AND	AL,3
	AND	BYTE PTR ES:[BX+DI],0FCh
	OR	ES:[BX+DI],AL
	MOV	AL,[BX+SI+50h]
	AND	AL,3
	ADD	BP,2
	MOV	DI,DS:D_DE85[BP]
	AND	BYTE PTR ES:[BX+DI],0FCh
	OR	ES:[BX+DI],AL
	ADD	SI,0A0h
	ADD	BP,2
	LOOP	C_0F15
	;-- --
	CALL	wipe_delay

	INC	BX
	CMP	BX,50h
	JZ	C_0F4E
	JMP	C_0E61
C_0F4E:
	POP	ES
	POP	BP
	RET	
;----------------------------------------0F51
wipe_delay:
	MOV	CX,1000
	LOOP	$

	RET	
;----------------------------------------
PUBLIC intro_karateka_title
;"KARATEKA"
intro_karateka_title:
	CALL	BB_clear
	;--
	PUSH	ES
	;-- "KARATEKA" --
	PUSH	DS
	POP	ES
	MOV	CX,820h
	LEA	SI,[D_A606]
	LEA	DI,cga_buffer[15E0h]
	REPZ	MOVSW
	;-- --
	CALL	BB_flip
	;-- some delay --
	MOV	AX,12h
	CALL	C_191C	;delay+control?
	;-- "Copyright 1986 Jordan Mechner" --
	MOV	AX,180	;y
	PUSH	AX
	MOV	AX,40	;x
	PUSH	AX
	MOV	AX,5Ch	;fig
	PUSH	AX
	CALL	putFig
	ADD	SP,6
	;-- --
	CALL	BB_flip
	;-- some delay --
	MOV	AX,48h
	CALL	C_191C	;delay+control?
	;-- --
	POP	ES
	RET	
;----------------------------------------
PUBLIC intro_publisher
;"broderbund software presents"
intro_publisher:
	;-- --
	CALL	BB_clear
	;-- --
	MOV	AX,115	;y
	PUSH	AX
	MOV	AX,106	;x
	PUSH	AX
	MOV	AX,5Bh	;fig
	PUSH	AX
	CALL	putFig
	ADD	SP,6
	;-- --
	CALL	BB_flip
	;-- --
	MOV	D_0168,1	;allows CTRL+V?
	;-- --
	MOV	AX,48h
	CALL	C_191C	;delay+control?
	;-- --
	MOV	D_0168,0	;allows CTRL+V?
	;-- --
	RET	
;----------------------------------------
PROG	ENDS
;########################################
END
