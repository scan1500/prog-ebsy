	AREA    |.text|, CODE, READONLY
    
	; Globale Variablen
	EXTERN SysTick_CTRL
	EXTERN cur_pid
	EXTERN cur_sp
    EXTERN next_pid
    EXTERN next_sp
		
firstProc PROC
    export firstProc

	; Von MSP auf PSP wechseln
	MOV R1, #2
    MSR CONTROL, r1     
	isb
	
	; Einstieg aus R0 in LR kopieren
	MOV	SP, R0
    POP {LR}
	
	; Am aktuellen PSP-Register werden: xPSR, PC, LR, R12, R3, R2, R1, R1 (= 8 Reg.)
	; Diese 8 Reg. werden via Hardware bei Sprung in Handler auf PSP-Stack gespeichert
	; Das heißt ab ctxsw hat PSP den Wert: PSP+(8x4)
	MOV R0, SP
    MSR psp, R0
	
	; SysTick an [..111]
	LDR R1, =SysTick_CTRL
    LDR R2, [R1]
    LDR R1, [R2]
    ORR R1, #0x7
    STR R1, [R2]        	
	
	BX LR
	
    ENDP
        
ctxsw PROC
	export ctxsw
		
	;Aktuellen Wert von PSP nach R1 kopieren	
	;Platz auf Stack schaffen (8 Worte)
	;R4-R11 auf Stack sichern
	MRS R1, psp 		
	SUBS R1, #32 		
	STMIA R1,{R4-R11} 	
	
	; Aktuellen Stackpointer updaten.
	; SP ganz unten am Prozess-Stack, ab hier später 8 Worte rücksichern 
	LDR R0, =cur_sp
    LDR R0,[R0]
    STR R1, [R0]        
		
	; Pointer auf nächsten Stack in R1 laden
    LDR R0, =next_sp
    LDR R0, [R0]
    LDR R1, [R0]        
	
	; cur_pid = next_pid
	LDR R2, =cur_pid
    LDR R3, =next_pid
    LDR R3, [R3]		
    STR R3, [R2]        
	
	; Rücksichern an letzter Prozess-Stack Adresse beginnen (8 Worte)
	; Aktuellen Stackpointer in PSP speichern
	; Implizite Rücksicherung der restlichen 8 Worte ab Rücksprung in Prozess
	; Rückkehr in PendSV
	LDMIA R1!, {R4-R11} 
    MSR psp, R1     
    
    BX LR               
	
	ENDP
		
	END