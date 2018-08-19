	AREA    |.text|, CODE, READONLY
    
firstProc PROC
    export firstProc
	
    mov sp, r0 	
    pop {r4-r12, pc}
	
    ENDP
        
ctxsw PROC
	export ctxsw
		
	mov sp, r0
	push {r4-r12, lr}
	
	mov sp, r1
	pop {r4-r12, pc}
	
	ENDP
	
END
			