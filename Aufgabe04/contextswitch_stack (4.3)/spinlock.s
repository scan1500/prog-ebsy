	area spinlock_s,code,readonly

LOCKED		equ 1
UNLOCKED	equ 0

	export	acquire
	export	release

acquire proc
	ldr			r2, =LOCKED
spinlock
	ldrex		r1, [r0]
	cmp			r1, #LOCKED
	strexne		r3, r2, [r0]
	cmpne		r3, #1
	beq			spinlock
	dmb
	bx lr
	endp

release proc
	ldr		r2, =UNLOCKED
	str		r2, [r0]
	dmb
	bx lr
	endp

	end
