AREA    |.text|, CODE, READONLY
    
firstProc PROC
    export firstProc
    mov sp, r0 			;Verschieben des Stackpointers
    pop {r4-r12, pc} 	;Stackwerte -> Register r4-r12 & Linkregister -> Programcounter (Rücksprung in p1)
    ENDP
        
ctxsw PROC
	export ctxsw
		
	mov sp, r0
	push {r4-r12, lr}
	
	mov sp, r1
	pop {r4-r12, pc}
	ENDP		
END
			