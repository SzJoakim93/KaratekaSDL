;========================================
;	KARATEKA
;	Copyright 1986 Jordan Mechner
;	IBM version by The Connelley Group
;	reverse-coded by ergonomy_joe 2022
;========================================
EXTRN cga_buffer:BYTE

EXTRN D_B6FA:WORD

EXTRN D_C262:WORD

EXTRN D_DE70:WORD
EXTRN D_DE85:WORD

EXTRN main:NEAR
EXTRN BB_clear:NEAR	;0BB7
EXTRN BB_flip:NEAR	;0DD1
EXTRN C_1705:NEAR
EXTRN C_1906:NEAR
EXTRN C_191C:NEAR
EXTRN ClearBottom:NEAR	;31EC
EXTRN DoInput:NEAR	;41BE
EXTRN GetKey:NEAR	;4299
EXTRN WaitNoKey:NEAR	;42D0
EXTRN WaitKey:NEAR
EXTRN C_46CC:NEAR

GR_SEG	EQU 0B800h

DGROUP	GROUP DATA
;########################################
DATA	SEGMENT PUBLIC BYTE 'DATA'
;----------------------------------------
PUBLIC D_D54C
D_D54C	db 0	;sound "conf"
D_D54D	dw OFFSET PROG:C_3C7C	;00
	dw OFFSET PROG:C_3C8F	;01["hit"]
	dw OFFSET PROG:C_3CA3	;02["hit"]
	dw OFFSET PROG:C_3CB7	;03["hit"]
	dw OFFSET PROG:C_3CC8	;04["hit"]
	dw OFFSET PROG:C_3CD9	;05["hit"]
	dw OFFSET PROG:C_3CED	;06["hit"]
	dw OFFSET PROG:C_3CFE	;07["yaaa"]
	dw OFFSET PROG:C_3D2A	;08[melody]
	dw OFFSET PROG:C_3D30	;09[melody]
	dw OFFSET PROG:C_3D36	;0a[melody]
	dw OFFSET PROG:C_3D3C	;0b[melody]
	dw OFFSET PROG:C_3D42	;0c[melody]
	dw OFFSET PROG:C_3D48	;0d[melody]
	dw OFFSET PROG:C_3D4E	;0e[melody]
	dw OFFSET PROG:C_3D54	;0f[melody]
	dw OFFSET PROG:C_3D5A	;10[melody]
	dw OFFSET PROG:C_3D60	;11[melody]
	dw OFFSET PROG:C_3D66	;12[melody]
	dw OFFSET PROG:C_3D6C	;13[melody]
	dw OFFSET PROG:C_3D72	;14[melody]
	dw OFFSET PROG:C_3D09	;15["yaaa"]
	dw OFFSET PROG:C_3D14	;16["yaaa"]
	dw OFFSET PROG:C_3D1F	;17["yaaa"]
	dw OFFSET PROG:C_3D78	;18[melody]
	dw OFFSET PROG:C_3D7E	;19[melody]
D_D581	db 3,0,0,0,0,0,0FFh
D_D588	db 1
__D589	db 0,0,0,0
D_D58D	dw 1
__D58F	db 2
D_D590	db 3
D_D591	db 4
D_D592	db 5 
D_D593	db 6
D_D594	db 7
D_D595	db 8 
D_D596	db 9
D_D597	db 0Ah
D_D598	db 0Bh
D_D599	db 0Ch
__D59A	db 0Dh
__D59B	db 0Eh
;-- --
D_D59C	db 1Eh
D_D59D	db 0
PUBLIC D_D59E
D_D59E	dw 4,0Ch,9,7,4
;-- --
PUBLIC D_D5A8
D_D5A8	dw OFFSET DGROUP:D_D5C6
	dw OFFSET DGROUP:D_D60E
	dw OFFSET DGROUP:D_D642
	dw OFFSET DGROUP:D_D667
	dw OFFSET DGROUP:D_D677
	dw OFFSET DGROUP:D_D690
	dw OFFSET DGROUP:D_D6C4
	dw OFFSET DGROUP:D_D6E3
	dw OFFSET DGROUP:D_D70B
	dw OFFSET DGROUP:D_D724
	dw OFFSET DGROUP:D_D75E
	dw OFFSET DGROUP:D_D7A1
	dw OFFSET DGROUP:D_D7EA
	dw OFFSET DGROUP:D_D8B5
	dw OFFSET DGROUP:D_D6A7
D_D5C6	db 004h,0C0h
	db 0FEh
	db 008h,001h,0A8h
	db 008h,0FFh,000h
	db 002h,001h,0D3h
	db 004h,001h,0A8h
	db 008h,0FFh,000h
	db 018h,001h,07Eh
	db 008h,0FFh,000h
	db 0FEh
	db 00Ah,001h,0BCh
	db 00Ah,001h,07Eh
	db 00Ch,001h,070h
	db 010h,001h,04Fh
	db 004h,0FFh,000h
	db 0FEh
	db 004h,002h,04Fh
	db 004h,001h,0BCh
	db 004h,001h,0A8h
	db 008h,001h,070h
	db 008h,0FFh,000h
	db 002h,001h,085h
	db 002h,0FFh,000h
	db 004h,001h,070h
	db 008h,0FFh,000h
	db 03Bh,001h,052h
	db 0FFh
D_D60E	db 006h,0C0h
	db 0FEh
	db 014h,0FFh,0FFh
	db 011h,002h,04Fh
	db 003h,0FFh,000h
	db 007h,002h,04Fh
	db 004h,003h,0A4h
	db 004h,002h,0FBh
	db 026h,001h,0FBh
	db 005h,003h,080h
	db 005h,002h,0A9h
	db 014h,001h,0FBh
	db 003h,0FFh,000h
	db 00Bh,001h,0FBh
	db 007h,002h,0FBh
	db 007h,002h,04Fh
	db 037h,001h,0FBh
	db 014h,0FFh,0FFh
	db 0FFh
D_D642	db 003h,0C0h
	db 0FEh
	db 004h,001h,0A8h
	db 008h,0FFh,000h
	db 003h,001h,0C7h
	db 001h,0FFh,000h
	db 004h,001h,0A8h
	db 006h,0FFh,000h
	db 008h,001h,07Eh
	db 00Ah,0FFh,000h
	db 004h,001h,062h
	db 003h,0FFh,000h
	db 028h,001h,062h
	db 0FFh
D_D667	db 005h,0C0h
	db 0FEh
	db 004h,002h,063h
	db 004h,002h,04Fh
	db 004h,002h,03Ch
	db 030h,002h,04Fh
	db 0FFh
D_D677	db 003h,0C0h
	db 0FEh
	db 00Ch,002h,04Fh
	db 006h,004h,017h
	db 004h,003h,080h
	db 004h,002h,0FBh
	db 004h,002h,090h
	db 004h,002h,03Ch
	db 040h,001h,0FBh
	db 0FFh
D_D690	db 007h,0C0h
	db 0FDh,002h
	db 004h,001h,07Eh
	db 018h,001h,069h
	db 002h,0FFh,0FFh
	db 004h,001h,0A8h
	db 018h,001h,070h
	db 002h,0FFh,0FFh
	db 0FFh
D_D6A7	db 007h,0C0h
	db 0FDh,002h
	db 004h,001h,07Eh
	db 018h,001h,069h
	db 002h,0FFh,0FFh
	db 004h,001h,0A8h
	db 018h,001h,070h
	db 002h,0FFh,0FFh
	db 004h,001h,0D3h
	db 030h,001h,07Eh
	db 0FFh
D_D6C4	db 007h,0C0h
	db 0FDh,002h
	db 010h,0FFh,0FFh
	db 004h,001h,0D3h
	db 00Ch,001h,07Eh
	db 004h,0FFh,0FFh
	db 0FDh,041h
	db 004h,001h,0BCh
	db 003h,001h,07Eh
	db 004h,001h,070h
	db 028h,001h,04Fh
	db 0FFh
D_D6E3	db 005h,0C0h
	db 0FDh,004h
	db 005h,001h,0BCh
	db 004h,001h,0A8h
	db 004h,001h,085h
	db 00Bh,001h,06Fh
	db 00Dh,0FFh,0FFh
	db 004h,001h,085h
	db 002h,0FFh,0FFh
	db 008h,001h,06Fh
	db 00Bh,0FFh,0FFh
	db 0FDh,002h
	db 004h,001h,0BCh
	db 023h,001h,051h
	db 0FFh
D_D70B	db 004h,0C0h
	db 0FEh
	db 00Ch,002h,04Fh
	db 006h,004h,017h
	db 004h,003h,080h
	db 004h,002h,0FBh
	db 004h,002h,090h
	db 004h,002h,03Ch
	db 040h,001h,0FBh
	db 0FFh
D_D724	db 004h,0C0h
	db 0FEh
	db 006h,001h,0FBh
	db 004h,001h,0A8h
	db 004h,001h,07Eh
	db 028h,001h,063h
	db 006h,004h,04Fh
	db 004h,001h,0BCh
	db 004h,001h,0A8h
	db 004h,001h,085h
	db 01Eh,001h,070h
	db 006h,002h,02Ah
	db 004h,001h,0C7h
	db 004h,001h,096h
	db 00Ah,001h,07Eh
	db 006h,002h,090h
	db 004h,001h,0BCh
	db 004h,001h,07Eh
	db 004h,001h,070h
	db 046h,001h,049h
	db 0FFh
D_D75E	db 004h,0C0h
	db 0FEh
	db 006h,002h,04Fh
	db 004h,001h,0E0h
	db 004h,001h,0A8h
	db 004h,001h,07Eh
	db 004h,001h,070h
	db 028h,001h,052h
	db 006h,002h,04Fh
	db 004h,001h,0BCh
	db 004h,001h,0A8h
	db 004h,001h,096h
	db 004h,001h,085h
	db 004h,001h,070h
	db 028h,001h,052h
	db 006h,001h,0FBh
	db 004h,001h,0A8h
	db 004h,001h,07Eh
	db 005h,001h,070h
	db 005h,001h,063h
	db 006h,001h,052h
	db 006h,001h,048h
	db 050h,001h,03Ch
	db 0FFh
D_D7A1	db 004h,0C0h
	db 0FEh
	db 006h,001h,0A8h
	db 00Ah,0FFh,000h
	db 003h,001h,0C7h
	db 001h,0FFh,000h
	db 004h,001h,0A8h
	db 008h,0FFh,000h
	db 00Ch,001h,07Eh
	db 008h,0FFh,0FFh
	db 004h,001h,063h
	db 001h,0FFh,0FFh
	db 004h,001h,063h
	db 001h,0FFh,0FFh
	db 004h,001h,063h
	db 004h,0FFh,0FFh
	db 004h,001h,07Eh
	db 004h,0FFh,0FFh
	db 004h,001h,063h
	db 008h,0FFh,0FFh
	db 00Ch,001h,052h
	db 00Ch,0FFh,0FFh
	db 004h,001h,03Ch
	db 004h,0FFh,0FFh
	db 028h,001h,03Ch
	db 0FFh
D_D7EA	db 028h,0C0h
	db 0FDh,004h
	db 001h,001h,07Eh
	db 001h,001h,069h
	db 001h,001h,0A8h
	db 001h,001h,070h
	db 001h,001h,0D3h
	db 001h,001h,07Eh
	db 001h,001h,0A8h
	db 001h,001h,070h
	db 001h,001h,0BCh
	db 001h,001h,07Eh
	db 001h,001h,070h
	db 001h,001h,04Fh
	db 001h,001h,0A8h
	db 001h,001h,085h
	db 001h,001h,070h
	db 001h,001h,05Eh
	db 0FEh
	db 001h,001h,0C7h
	db 001h,001h,063h
	db 001h,001h,085h
	db 001h,001h,070h
	db 001h,001h,096h
	db 001h,001h,07Eh
	db 001h,001h,0BCh
	db 001h,001h,049h
	db 001h,001h,0A8h
	db 001h,001h,07Eh
	db 001h,001h,070h
	db 001h,001h,053h
	db 001h,001h,0A8h
	db 001h,001h,085h
	db 001h,001h,053h
	db 001h,001h,05Eh
	db 0FDh,048h
	db 001h,001h,063h
	db 001h,001h,07Eh
	db 001h,001h,05Eh
	db 001h,001h,070h
	db 001h,001h,053h
	db 001h,001h,063h
	db 001h,001h,07Eh
	db 001h,001h,0A8h
	db 0FDh,006h
	db 001h,001h,0C7h
	db 001h,001h,09Eh
	db 001h,001h,07Eh
	db 001h,001h,063h
	db 001h,001h,070h
	db 001h,001h,085h
	db 001h,001h,09Eh
	db 001h,001h,0E0h
	db 0FEh
	db 001h,001h,0FBh
	db 001h,001h,0C7h
	db 001h,001h,096h
	db 001h,001h,07Eh
	db 001h,001h,0BCh
	db 001h,001h,096h
	db 001h,001h,070h
	db 001h,001h,05Eh
	db 001h,001h,0A8h
	db 001h,001h,07Eh
	db 001h,001h,05Eh
	db 001h,001h,063h
	db 001h,001h,0A8h
	db 001h,001h,085h
	db 001h,001h,069h
	db 001h,001h,070h
	db 0FFh
D_D8B5	db 00Ah,0C0h
	db 0FEh
	db 001h,002h,0FBh
	db 001h,002h,04Fh
	db 004h,001h,0FBh
	db 004h,001h,0C8h
	db 004h,001h,0BCh
	db 004h,001h,0B1h
	db 004h,001h,0A8h
	db 004h,001h,09Eh
	db 004h,001h,084h
	db 002h,001h,094h
	db 002h,001h,0A8h
	db 004h,001h,09Eh
	db 002h,001h,094h
	db 002h,002h,0FBh
	db 01Eh,001h,09Eh
	db 0FFh
;-- samples for "yaaa"? --
D_D8E6	db 010h,09Bh,0B3h,076h,04Ch,0D9h,093h,066h,06Eh,0C9h,0BBh,027h,0ECh,0DFh,063h,07Dh
	db 09Bh,066h,06Eh,0D9h,09Bh,066h,06Dh,099h,0F7h,067h,099h,09Eh,0E6h,079h,099h,0EEh
	db 047h,0A9h,01Eh,0E4h,07Bh,0B1h,0CEh,0CDh,03Bh,034h,04Ch,0F5h,033h,0DCh,0CFh,033h
	db 03Ch,0CCh,0F1h,03Bh,0C6h,0E7h,01Bh,09Eh,06Eh,079h,039h,0E6h,067h,091h,09Fh,046h
	db 07Dh,018h,0ECh,063h,0B1h,08Fh,0C7h,07Fh,01Dh,07Eh,072h,0B8h,0CEh,0E3h,015h,0C6h
	db 06Fh,038h,09Ch,071h,07Ah,0E7h,0F1h,08Dh,09Fh,01Fh,03Ah,037h,074h,06Ah,0E8h,0F5h
	db 0C1h,0E3h,0A3h,097h,00Fh,00Eh,01Fh,01Ch,03Eh,038h,07Ch,070h,0F8h,0E1h,0E1h,0C3h
	db 0C3h,087h,087h,08Fh,007h,01Fh,00Fh,03Eh,01Eh,03Ch,01Ch,07Ch,0F8h,078h,0B8h,0F1h
	db 0F1h,0E1h,0E1h,0E1h,0E3h,0C7h,0E3h,08Fh,083h,09Eh,03Ch,03Eh,038h,07Ch,038h,07Ch
	db 038h,078h,039h,0FCh,0D9h,0C2h,0E1h,0E3h,0CEh,038h,0CFh,00Fh,01Fh,087h,087h,003h
	db 099h,0E8h,01Fh,01Fh,01Bh,0F1h,0DCh,03Ch,01Ch,067h,038h,0F8h,07Ch,0E3h,033h,09Ch
	db 00Fh,007h,0C7h,09Eh,00Fh,007h,01Eh,01Eh,03Fh,01Ch,070h,079h,0CFh,031h,0DCh,0F3h
	db 09Ch,0C6h,07Eh,077h,01Dh,08Eh,0E3h,091h,0E6h,0F1h,0C7h,031h,0F0h,0E3h,01Ch,08Eh
	db 03Eh,03Eh,03Ch,07Eh,038h,0F1h,0DCh,0F1h,0F1h,0C7h,0C3h,0E3h,0E1h,0C7h,0F1h,0F0h
	db 0F1h,0F8h,0F8h,078h,071h,0C7h,0F3h,083h,0B9h,0CFh,01Eh,01Ch,07Fh,03Ch,03Ch,07Ch
	db 03Ch,070h,0F0h,0E3h,0CCh,0E3h,0C3h,0E3h,0DFh,09Bh,087h,083h,08Ch,073h,0F1h,0F1h
;----------------------------------------
;
;----------------------------------------
;-- font related informations
D_D9E6	db 0Ah,0Ah,0Ah,0Ah,0Ah,0Ch,0Bh,0Ah,0Ah,0Ch,0Ah,0Ah,0Ah,0Ah,0Ah,0Ch,0Ch,0Ah,09h
	db 0Ah,0Ah,0Ah,0Ah,0Ch,0Ch,0Ah,0Ah,0Ch,0Ah,0Ah
D_DA04	db 02h,02h,01h,02h,02h,02h,02h,02h,01h,01h,02h,01h,02h,02h,02h,02h,02h,02h,02h
	db 02h,02h,02h,02h,01h,02h,02h,01h,01h,01h,01h
D_DA22	db 09h,0Bh,09h,0Bh,0Ah,08h,0Bh,0Bh,06h,07h,0Bh,07h,10h,0Ch,0Bh,0Dh,0Dh,0Dh,0Ah
	db 0Bh,0Bh,0Bh,11h,0Bh,0Bh,0Bh,07h,07h,07h,08h
__DA40	db 8
D_DA41	db 0
D_DA42	db 0
D_DA43	db 0
D_DA44	db 0
;-- --
D_DA45	db 25h
D_DA46	db "high atop a craggy cliff|",0
	db "guarded by an army of",0
	db "fierce warriors| stands the",0
	db "fortress of the evil",0
	db "warlord akuma{ deep in the",0
	db "darkest dungeon of the",0
	db "castle| akuma gloats over",0
	db "his lovely captive| the",0
	db "princess mariko{",0
	db " ",0
	db "you are one trained in the",0
	db "way of karate} a karateka{",0
	db "alone and unarmed| you must",0
	db "defeat akuma and rescue the",0
	db "beautiful mariko{",0
	db " ",0
	db "put fear and self~concern",0
	db "behind you{  focus your will",0
	db "on your objective|",0
	db "accepting death as a",0
	db "possibility{  this is the way",0
	db "of the karateka{",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
	db " ",0
D_DC4E	dw 0
;-- --
D_DC50	db 0Bh
D_DC51	db "and so this adventure ends{",0
	db "the evil akuma lies",0
	db "vanquished| the beautiful",0
	db "mariko safe in your arms{",0
	db "for the first time in",0
	db "memory| the village basks in",0
	db "the sunlight of peace{ but",0
	db "beware} for the true",0
	db "karateka there is always a",0
	db "next time{ { { {",0
	db " ",0
;-- --
D_DD46	db "the end",0
D_DD4E	db "a game by",0
D_DD58	db "jordan mechner",0
D_DD67	db "ibm version by",0
D_DD76	db "the connelley group",0
PUBLIC D_DD8A
D_DD8A	db "make sure your karateka",0
D_DDA2	db "disk is in drive a{",0
PUBLIC D_DDB6
D_DDB6	db "press any key to continue",0
PUBLIC D_DDD0
D_DDD0	db "center joystick and press a key{",0
D_DDF1	db "press j to re~adjust joystick",0
D_DE0F	db "when necessary during game{",0
PUBLIC D_DE2B
D_DE2B	db " no joystick{  press any key{",0
PUBLIC D_DE49
D_DE49	db " press q to quit| d for demo",0
;----------------------------------------
DATA	ENDS
;########################################
PROG	SEGMENT PUBLIC BYTE 'PROG'
	ASSUME CS:PROG,DS:DGROUP
;----------------------------------------3C41
PUBLIC sound
sound:
	PUSH	BP
	MOV	BP,SP
	;--
	MOV	BX,[BP+04]
	CMP	BL,0
	JZ	C_3C7A
	CMP	D_D54C,0
	JZ	C_3C6D	;play all
	CMP	D_D54C,1
	JZ	C_3C5C	;SFX only
	;--
	POP	BP
	RET
C_3C5C:	;-- 1=SFX only  --
	CMP	BL,9
	JB	C_3C6D
	CMP	BL,16h
	JB	C_3C7A
	CMP	BL,19h
	JB	C_3C6D
	;--
	POP	BP
	RET
C_3C6D:	;-- --
	IN	AL,61h
	AND	AL,0FCh
	OUT	61h,AL

	DEC	BX
	SHL	BX,1
	JMP	D_D54D[BX]
C_3C7A:	;--
	POP	BP
	RET
;----------------------------------------
;sound_00
C_3C7C:
	MOV	CX,2
C_3C7F:
	PUSH	CX
	CALL	C_3E68
	CALL	C_3E4C
	POP	CX
	INC	CX
	CMP	CX,28h
	JL	C_3C7F

	POP	BP
	RET
;----------------------------------------
;sound_01
C_3C8F:
	MOV	CX,2
C_3C92:
	PUSH	CX
	CALL	C_3E68
	CALL	C_3E4C
	POP	CX
	INC	CX
	INC	CX
	CMP	CX,3Ah
	JL	C_3C92

	POP	BP
	RET
;----------------------------------------
;sound_02
C_3CA3:
	MOV	CX,2
C_3CA6:
	PUSH	CX
	CALL	C_3E68
	CALL	C_3E4C
	POP	CX
	INC	CX
	INC	CX
	CMP	CX,24h
	JLE	C_3CA6

	POP	BP
	RET
;----------------------------------------
;sound_03
C_3CB7:
	MOV	BL,0Ah
C_3CB9:
	MOV	CX,5
	CALL	C_3E4C
	CALL	C_3E68
	DEC	BL
	JNZ	C_3CB9

	POP	BP                                 
	RET
;----------------------------------------
;sound_04
C_3CC8:
	MOV	BL,10h
C_3CCA:
	MOV	CX,0Ah
	CALL	C_3E4C
	CALL	C_3E68
	DEC	BL
	JNZ	C_3CCA

	POP	BP                                 
	RET
;----------------------------------------
;sound_05
C_3CD9:
	MOV	CX,10h
C_3CDC:
	PUSH	CX
	CALL	C_3E68
	CALL	C_3E4C
	POP	CX
	SHR	CX,1
	CMP	CX,1
	JNZ	C_3CDC

	POP	BP
	RET
;----------------------------------------
;sound_06
C_3CED:
	MOV	BL,3
C_3CEF:
	MOV	CX,10h
	CALL	C_3E4C
	CALL	C_3E68
	DEC	BL
	JNZ	C_3CEF

	POP	BP
	RET
;----------------------------------------
;sound_07
C_3CFE:
	MOV	AX,D_D59E[2]
	MOV	D_D588,AL
	CALL	C_3E71	;"yaaa"
	POP	BP
	RET
;----------------------------------------
;sound_15
C_3D09:
	MOV	AX,D_D59E[4]
	MOV	D_D588,AL
	CALL	C_3E71	;"yaaa"
	POP	BP
	RET
;----------------------------------------
;sound_16
C_3D14:
	MOV	AX,D_D59E[6]
	MOV	D_D588,AL
	CALL	C_3E71	;"yaaa"
	POP	BP
	RET
;----------------------------------------
;sound_17
C_3D1F:
	MOV	AX,D_D59E[8]
	MOV	D_D588,AL
	CALL	C_3E71	;"yaaa"
	POP	BP
	RET
;----------------------------------------
;sound_08
C_3D2A:
	MOV	BX,OFFSET DGROUP:D_D5C6
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_09
C_3D30:
	MOV	BX,OFFSET DGROUP:D_D60E
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_0a
C_3D36:
	MOV	BX,OFFSET DGROUP:D_D642
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_0b
C_3D3C:
	MOV	BX,OFFSET DGROUP:D_D667
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_0c
C_3D42:
	MOV	BX,OFFSET DGROUP:D_D677
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_0d
C_3D48:
	MOV	BX,OFFSET DGROUP:D_D690
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_0e
C_3D4E:
	MOV	BX,OFFSET DGROUP:D_D6C4
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_0f
C_3D54:
	MOV	BX,OFFSET DGROUP:D_D6E3
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_10
C_3D5A:
	MOV	BX,OFFSET DGROUP:D_D70B
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_11
C_3D60:
	MOV	BX,OFFSET DGROUP:D_D724
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_12
C_3D66:
	MOV	BX,OFFSET DGROUP:D_D75E
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_13
C_3D6C:
	MOV	BX,OFFSET DGROUP:D_D7A1
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_14
C_3D72:
	MOV	BX,OFFSET DGROUP:D_D7EA
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_18
C_3D78:
	MOV	BX,OFFSET DGROUP:D_D8B5
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;sound_19
C_3D7E:
	MOV	BX,OFFSET DGROUP:D_D6A7
	JMP	C_3D84	;(+NOP)melody sequencer?
;----
;melody sequencer?
C_3D84:
	MOV	AL,[BX]
	MOV	D_D596,AL
	INC	BX
	MOV	AL,[BX]
	MOV	D_D597,AL
	INC	BX
	MOV	D_D58D,BX
C_3D94:	;-- next byte
	MOV	BX,D_D58D
	MOV	AL,[BX]
	CMP	AL,0FFh	;end mark
	JNZ	C_3DA0
	;-- 0xff --
	POP	BP
	RET
C_3DA0:	;--
	CMP	AL,0FEh
	JNZ	C_3DB0
	;-- 0xfe --
	MOV	D_D598,0
	INC	BX
	MOV	D_D58D,BX
	JMP	C_3D94	;next byte
C_3DB0:	;--
	CMP	AL,0FDh
	JNZ	C_3DC1
	;-- 0xfd xx --
	INC	BX
	MOV	AL,[BX]
	MOV	D_D598,AL
	INC	BX
	MOV	D_D58D,BX
	JMP	C_3D94	;next byte
C_3DC1:	;-- xx xx xx --
	MOV	D_D599,AL
	INC	BX
	MOV	AL,[BX]
	MOV	D_D591,AL
	MOV	D_D592,AL
	MOV	D_D590,AL
	INC	BX
	MOV	AL,[BX]
	MOV	D_D593,AL
	MOV	AL,D_D593
	MOV	CL,AL
	MOV	DL,AL
C_3DDD:
	MOV	AL,D_D596
	MOV	D_D594,AL
C_3DE3:
	MOV	D_D595,0FFh
C_3DE8:
	DEC	CL
	JNZ	C_3E12
	DEC	D_D591
	JNZ	C_3E12
	CMP	D_D590,80h
	JL	C_3DFC
	CALL	C_3E68
C_3DFC:
	MOV	CL,D_D593
	MOV	AL,D_D590
	MOV	D_D591,AL
	CMP	D_D598,0
	JZ	C_3E12
	MOV	AL,D_D598
	MOV	DL,AL
C_3E12:
	DEC	DL
	JNZ	C_3E32
	DEC	D_D592
	JNZ	C_3E12
	CMP	D_D590,80h
	JL	C_3E26
	CALL	C_3E68
C_3E26:
	MOV	DL,D_D593
	DEC	DL
	MOV	AL,D_D590
	MOV	D_D592,AL
C_3E32:
	DEC	D_D595
	JNZ	C_3DE8
	DEC	D_D594
	JNZ	C_3DE3
	DEC	D_D599
	JNZ	C_3DDD
	ADD	D_D58D,3
	JMP	C_3D94	;next byte
;----------------------------------------
C_3E4C:
	PUSH	DX
	PUSH	BX
	;--
	MOV	BX,D_D59E[0]
	MOV	DX,CX
C_3E54:
	MOV	CX,DX
C_3E56:
	PUSH	CX
C_3E57:
	SBB	CX,1
	JNZ	C_3E57
	POP	CX
	SBB	CX,1
	JNZ	C_3E56
	DEC	BX
	JNZ	C_3E54
	;--
	POP	BX
	POP	DX
	RET
;----------------------------------------
C_3E68:
	PUSH	AX
	;--
	IN	AL,61h
	XOR	AL,2
	OUT	61h,AL
	;--
	POP	AX
	RET
;----------------------------------------
;"yaaa"
C_3E71:
	MOV	CX,100h
	MOV	BX,OFFSET DGROUP:D_D8E6
C_3E77:
	MOV	DL,8
	MOV	AL,[BX]
C_3E7B:
	MOV	AH,D_D588
	MOV	D_D59C,AH
C_3E83:
	DEC	D_D59C
	JNZ	C_3E83
	XOR	AL,D_D59D
	JNS	C_3EA3
	XOR	AL,D_D59D
	MOV	D_D59D,AL
	CALL	C_3E68
C_3E99:
	SHL	AL,1
	DEC	DL
	JNZ	C_3E7B
	INC	BX
	LOOP	C_3E77

	RET
C_3EA3:
	XOR	AL,D_D59D
	JMP	C_3E99
;----------------------------------------
;
;----------------------------------------3EA9
PUBLIC k_puts
k_puts:
	MOV	D_DA41,0
C_3EAE:
	MOV	BL,[SI]
	CMP	BL,0
	JNZ	C_3EB8
	JMP	C_3F55
C_3EB8:
	SUB	BL,61h
	JGE	C_3EC3
	ADD	DI,2
	INC	SI
	JMP	C_3EAE
C_3EC3:
	INC	SI
	MOV	BH,0
	PUSH	SI
	MOV	SI,BX
	SHL	BX,1
	MOV	AX,D_B6FA[BX]
	LEA	BX,[D_B6FA]
	ADD	BX,AX
	MOV	CH,D_DA04[SI]
	MOV	D_DA42,CH
C_3EDD:
	PUSH	DI
	MOV	CL,D_D9E6[SI]
	MOV	D_DA44,CL
C_3EE6:
	MOV	AH,[BX]
	MOV	CH,8
C_3EEA:
	SAR	AX,1
	RCL	AX,1
	RCR	DX,1
	RCL	AX,1
	RCR	DX,1
	DEC	CH
	JNZ	C_3EEA
	XOR	AX,AX
	CALL	C_3F56
	XCHG	DH,DL
	XCHG	AH,AL
	OR	[DI],DX
	OR	[DI+02],AX
	ADD	DI,50h
	INC	BX
	DEC	D_DA44
	JNZ	C_3EE6
	POP	DI
	ADD	DI,2
	DEC	D_DA42
	JNZ	C_3EDD
	MOV	AH,D_DA41
	ADD	AH,D_DA22[SI]
	MOV	AL,D_DA04[SI]
	SHL	AL,1
	SHL	AL,1
	SHL	AL,1
	SUB	AL,AH
	CMP	AL,0FCh
	JG	C_3F36
	INC	DI
	JMP	C_3F3D	;+(NOP)
C_3F36:
	SUB	AL,4
	JL	C_3F3D
	DEC	DI
	JMP	C_3F36
C_3F3D:
	MOV	AH,D_DA22[SI]
	ADD	D_DA41,AH
	AND	D_DA41,3
	JZ	C_3F51
	CMP	AL,0FCh
	JL	C_3F51
	DEC	DI
C_3F51:
	POP	SI
	JMP	C_3EAE
C_3F55:
	RET
;----------------------------------------
C_3F56:
	CMP	D_DA41,0
	JZ	C_3F6B
	MOV	CL,D_DA41
	SHL	CL,1
C_3F63:
	SHR	DX,1
	RCR	AX,1
	DEC	CL
	JNZ	C_3F63
C_3F6B:
	RET
;----------------------------------------
PUBLIC intro_story_scroll
;story scroll
intro_story_scroll:
	CALL	BB_clear
	CALL	BB_flip

	MOV	AL,D_DA45
	MOV	D_DA43,AL

	LEA	AX,[D_DA46]
	MOV	D_DC4E,AX

	CALL	C_409D	;scrolling

	RET
;----------------------------------------
PUBLIC C_3F83
C_3F83:
	CALL	BB_clear
	CALL	BB_flip
	
	LEA	DI,cga_buffer[1D7Fh]
	LEA	SI,[D_DD46]	;"the end"
	CALL	k_puts
	
	CALL	BB_flip

	MOV	AX,48h
	CALL	C_191C	;delay+control?

	RET
;----------------------------------------
PUBLIC C_3F9E
;victory scroll?
C_3F9E:
	CALL	WaitNoKey
	CALL	BB_clear
	CALL	BB_flip
	;-- --
	PUSH	WORD PTR D_DC50
	LEA	SI,[D_DC51]
	LEA	DI,cga_buffer[640h]
C_3FB3:
	PUSH	DI
	CALL	k_puts
	POP	DI
	ADD	DI,3C0h
	INC	SI
	DEC	D_DC50
	JNZ	C_3FB3
	POP	WORD PTR D_DC50
	;-- --
	CALL	BB_flip
C_3FCA:	;-- --
	MOV	AX,15h
	PUSH	AX
	CALL	sound
	POP	AX
	;-- --
	CALL	DoInput
	
	CMP	D_DE70,0
	JNZ	C_3FE3
	CMP	AL,0
	JZ	C_3FCA

	CALL	GetKey
C_3FE3:
	RET
;----------------------------------------
PUBLIC intro_developer
intro_developer:
	CALL	BB_clear
	
	LEA	DI,cga_buffer[1A5Bh]
	LEA	SI,[D_DD4E]	;"a game by"
	CALL	k_puts
	
	LEA	DI,cga_buffer[1F54h]
	LEA	SI,[D_DD58]	;"jordan mechner"
	CALL	k_puts
	
	CALL	BB_flip

	MOV	AX,48h
	CALL	C_191C	;delay+control?

	RET
;----------------------------------------
PUBLIC intro_ibm_port
intro_ibm_port:
	CALL	BB_clear
	
	LEA	DI,cga_buffer[1A56h]
	LEA	SI,[D_DD67]	;"ibm version by"
	CALL	k_puts
	
	LEA	DI,cga_buffer[1F50h]
	LEA	SI,[D_DD76]	;"the connelley group"
	CALL	k_puts
	
	CALL	BB_flip

	MOV	AX,48h
	CALL	C_191C	;delay+control?

	RET
;----------------------------------------
;int24h handler
PUBLIC C_402A
C_402A:
	CALL	BB_clear
	
	LEA	DI,cga_buffer[17CAh]
	LEA	SI,[D_DD8A]	;"make sure your karateka"
	CALL	k_puts
	
	LEA	DI,cga_buffer[1CD2h]
	LEA	SI,[D_DDA2]	;"disk is in drive a{"
	CALL	k_puts
	
	LEA	DI,cga_buffer[21C8h]
	LEA	SI,[D_DDB6]	;"press any key to continue"
	CALL	k_puts
	
	CALL	BB_flip
	CALL	GetKey

	RET
;----------------------------------------
PUBLIC C_4055
C_4055:
	CALL	BB_clear
	
	LEA	DI,cga_buffer[17C2h]
	LEA	SI,[D_DDD0]	;"center joystick and press a key{"
	CALL	k_puts
	
	LEA	DI,cga_buffer[21C4h]
	LEA	SI,[D_DDF1]	;"press j to re~adjust joystick"
	CALL	k_puts
	
	LEA	DI,cga_buffer[26C5h]
	LEA	SI,[D_DE0F]	;"when necessary during game{"
	CALL	k_puts
	
	CALL	BB_flip
	CALL	WaitKey
	CALL	C_46CC

	RET
;----------------------------------------4083
PUBLIC Question
Question:
	PUSH	BP
	MOV	BP,SP
	;--
	CALL	ClearBottom

	LEA	DI,cga_buffer[3AC2h]
	MOV	SI,[BP+04]
	CALL	k_puts

	CALL	BB_flip
	CALL	WaitKey
	XOR	AH,AH
	;--
	POP	BP
	RET
;----------------------------------------
;scrolling
C_409D:
	PUSH	ES
	MOV	AX,GR_SEG
	MOV	ES,AX
C_40A3:
	;-- --
	MOV	SI,D_DC4E
	LEA	DI,[cga_buffer]
	XOR	AX,AX
	MOV	CX,4B0h
	PUSH	ES
	PUSH	DS
	POP	ES
	REPZ	STOSW
	POP	ES
	LEA	DI,[cga_buffer]
	CALL	k_puts
	INC	SI
	MOV	D_DC4E,SI
	;-- --
	XOR	DX,DX
C_40C4:
	XOR	BX,BX
C_40C6:
	MOV	DI,D_DE85[BX]
	MOV	SI,D_DE85[BX+2]
	MOV	CX,28h
	PUSH	DS
	PUSH	ES
	POP	DS
	REPZ	MOVSW
	POP	DS
	ADD	BX,2
	CMP	BX,18Eh
	JB	C_40C6

	MOV	DI,[D_DE85 + 18Eh]	;E013
	LEA	SI,[cga_buffer]
	ADD	SI,DX
	MOV	CX,28h
	REPZ	MOVSW
	ADD	DX,50h
	CMP	DX,460h
	JGE	C_410E
	PUSH	DX
	CALL	DoInput
	CMP	AX,0
	JZ	C_4105
	POP	DX
	POP	ES
	POP	BX
	RET
C_4105:
	MOV	AX,1
	CALL	C_1906	;delay?
	POP	DX
	JMP	C_40C4
C_410E:
	DEC	D_DA43
	JNZ	C_40A3
	;-- --
	POP	ES
	RET
;----------------------------------------4116
PUBLIC CodeChecksum
CodeChecksum:
	;-- checksum #1 --
	LEA	SI,BYTE PTR [main]	;C_0255
	MOV	BX,2C0h
C_411D:
	ADD	AL,CS:[BX+SI]
	DEC	BX
	JGE	C_411D
	CMP	AL,70h	;#1
	JNZ	C_413B
	;-- checksum #2 --
	LEA	SI,BYTE PTR [C_1705]
	MOV	BX,80h
	XOR	AL,AL
C_4130:
	ADD	AL,CS:[BX+SI]
	DEC	BX
	JGE	C_4130
	CMP	AL,1Fh	;#2
	JNZ	C_413B
	;-- ok --
	RET
C_413B:	;-- error:destroy some memory --
	MOV	BX,14h
C_413E:
	MOV	D_C262[BX],0
	SUB	BX,2
	JGE	C_413E
	;--
	RET
;----------------------------------------
PROG	ENDS
;########################################
END
