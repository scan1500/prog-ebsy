	AREA svchandler_s, CODE, READONLY
	PRESERVE8
	export SVC_Handler
	import SVCHandler_main
	import update_cur_sp

SVC_Handler proc
	push {lr}
	tst lr, #4
	mrseq r0, msp
	mrsne r0, psp
	blne  update_cur_sp
	pop {lr}
	b SVCHandler_main
	endp
	
	end
