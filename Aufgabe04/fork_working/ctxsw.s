	AREA    ctxsw_s, CODE, READONLY
    ; acces variables, defined in our C-code
    EXTERN cur_pid
    EXTERN next_pid
    EXTERN cur_sp
    EXTERN next_sp
    EXTERN SysTick_CTRL
    
; set up first process
; This pops the entry-address for the first process from it's stack.
; Further, the updated SP is stored into PSP, because we're using PSP for our 'user-procs' later on.
; At start, Cortex-M3 uses the MSP.
; At last we turn on the SysTick-Timer and then branch to entry-point of first process.
firstProc PROC
    export firstProc    ; make routine callable from C-code
    mov r1, sp          ; save current msp
    mov sp, r0          ; change sp to first process' sp
    pop {lr}            ; get entry-point from stack
    mov r0, sp
    msr psp, r0         ; write current sp to psp, as we're using this one later
    msr msp, r1         ; restore msp
    mov r1, #2
    msr control, r1     ; switch to psp
    isb                 ; Docs say we need to do this, whenever we're switching stacks. 
    ldr r0, =SysTick_CTRL
    ldr r1, [r0]
    ldr r0, [r1]
    orr r0, #0x7
    str r0, [r1]        ; turn on SysTick
    bx lr               ; branch ot entry-point of first process
    ENDP
        
ctxsw PROC
	export ctxsw
        mrs r3, psp         ; copy value from psp to r3. This is the current proc's stack-pointer
        subs r3, #0x20      ; 32 Byte space on stack for 8 registers
		stmia r3, {r4-r11} ; store registers to stack. lowest register number goes to lowest stack-address. lr = r14, so lr is pushed last.
        ldr r0, =cur_sp
        ldr r0,[r0]
        str r3, [r0]        ; update the proc's stack pointer to the new location
        ldr r0, =next_sp
        ldr r0,[r0]
        ldr r3, [r0]        ; Grab stack pointer from next process to access it's stack
        ldr r1, =cur_pid
        ldr r2, =next_pid
        ldr r2, [r2]
        str r2, [r1]        ; cur_pid = next_pid
		ldmia r3!, {r4-r11} ; restore next proc's context (see the comment for stmia before)
        msr psp, r3         ; move r3 to psp, so the next process has the correct stack pointer
        bx lr               ; return to PendSV_Handler
		ENDP
			
		END
			