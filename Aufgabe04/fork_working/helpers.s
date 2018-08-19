    area helpers_s, code, readonly
        
    export interrupts_enable
    export interrupts_disable
	export get_r0
        
interrupts_enable proc
    cpsie i
    bx lr
    endp
        
interrupts_disable proc
    cpsid i
    bx lr
    endp
        
get_r0 proc
	bx lr
	endp
		
	end