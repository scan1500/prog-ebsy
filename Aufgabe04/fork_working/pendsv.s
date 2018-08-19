        AREA    pendsv_s, CODE, READONLY
            
        IMPORT ctxsw

; called after SysTick_Handler
PendSV_Handler PROC
    export PendSV_Handler
        bl ctxsw
        ldr lr, =0xFFFFFFFD ; return to THREAD-Mode using PSP
        bx lr               ; branch into the new process (next_pid)
        endp
            
        end