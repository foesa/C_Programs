	AREA	AsmTemplate, CODE, READONLY
	IMPORT	main

; sample program makes the 4 LEDs P1.16, P1.17, P1.18, P1.19 go on and off in sequence
; (c) Mike Brady, 2011 -- 2019.

	EXPORT	start
start

IO1DIR	EQU	0xE0028018
IO1SET	EQU	0xE0028014
IO1CLR	EQU	0xE002801C
IO1PIN	EQU	0xE0028010



getPress
	STMFD sp, {R1-R5, LR}
	; R0 = original values
	; R1 = mask
	; R2 = isLongPress
	; R3 = timerDelay
	; R4 = button addr
	; R5 = button values
	; R6 = num1
	; R7 = num2
	; R8 = first
	; R9 = operand
	LDR R0, =IO1PIN		; load button addr
	LDR R0, [R0]		; load originalValue
	LDR R1, 0xF00000	; load mask
	AND R0, R1, R0		; originalValue &= mask
	
	CMP R1, R0			; if(button values == mask)
	BNE pressedGetPress	; {
	LDR R0, =0			;		originalValue = 0
	B notNegGetPress	; }
pressedGetPress			; else{
	LDR R2, =1			; 		isLongPress = 1
	LDR R3, =4000000	; 		timerDelay
	LDR R4, =IO1PIN		;		load button addr

waitTimeGetPress
	SUBS R3, R3, #1		;		while(timerdelay)
	BEQ doneWaitGetPress;		{
	LDR R5, [R4]		; 			load button values()
	AND R5, R5, R1		;			button values &= mask
	CMP R5, R0			;			if (original values != button values){
	BEQ waitTimeGetPress;				isLongPress = false
	LDR R2, =0			;				break
doneWaitGetPress		;			}
						;		}
	LSR R0, #20			; 		originalValue >> 20
	AND R0, R0, R1		; 		originalValue &= mask
	CMP R0, #1			; 		if(originalValue == 1)
	BNE chkTwoGetPress	; 		{
	LDR R0, =20			; 			originalValue = 20
	B mayNegGetPress	; 		}
chkTwoGetPress			;
	CMP R0, #2			; 		else if(originalValue == 2)
	BNE chkFourGetPress	; 		{
	LDR R0, =21			; 			originalValue = 21
	B mayNegGetPress	; 		}
chkFourGetPress			;		
	CMP R0, #4			; 		else if(originalValue == 4)
	BNE chk8GetPress	; 		{
	LDR R0, =22			; 			originalValue = 22
	B mayNegGetPress	; 		}  
chk8GetPress			;		else {
	LDR R0, =23			; 			originalValue = 23
mayNegGetPress			; 		}
	CMP R2, #0			; 		if(isLongPress)
	BEQ notNegGetPress	; 		{
	NEG R0, R0			; 			originalValue = 2'sComplementOf(originalValue)
notNegGetPress			; 		}
	LSR R0, #20			; 		originalValue >> 20
	AND R0, R0, R1		; 		originalValue &= mask
	CMP R0, #1			; 		if(originalValue == 1)
	BNE chkTwoGetPressN	; 		{
	LDR R0, =20			; 			originalValue = 20
	B mayNegGetPress	; 		}
chkTwoGetPressN			;
	CMP R0, #2			; 		else if(originalValue == 2)
	BNE chkFourGetPressN; 		{
	LDR R0, =21			; 			originalValue = 21
	B mayNegGetPress	; 		}
chkFourGetPressN		;		
	CMP R0, #4			; 		else if(originalValue == 4)
	BNE chk8GetPressN	; 		{
	LDR R0, =22			; 			originalValue = 22
	B mayNegGetPress	; 		}  
chk8GetPressN			;		else {
	LDR R0, =23			; 			originalValue = 23
increment
	CMP R0, #20
	BNE decrement
	ADD R6,R6,#1
	B finish
decrement
	CMP R0,#21
	BNE adder
	RSB R6,R6,#1
	B finish
adder
	CMP R0,#22
	BNE subtract
	CMP R8, #0
	BNE adder2
	MOV R7,R6
	MOV R6,#0
	//clear display code goes here
	MOV R9,#1
	B finish
adder2
	CMP R9,#1
	BNE insub
	
insub
finish
	LDMFD sp!, {R1-R5, PC}	

	END