	AREA    pendsv_s, CODE, READONLY
            
	IMPORT ctxsw

; called after SysTick_Handler
PendSV_Handler PROC
    export PendSV_Handler
		
    BL ctxsw
    LDR LR, =0xFFFFFFFD ; return to THREAD-Mode using PSP
    BX LR               ; branch into the new process (next_pid)
	
    ENDP
			
	END