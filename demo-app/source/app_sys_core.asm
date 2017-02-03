;-------------------------------------------------------------------------------
; app_sys_core.asm
;
; (c) Texas Instruments 2009-2012, All rights reserved.
;


    .text
    .arm



;-------------------------------------------------------------------------------
; Store Core register set

       .def _CoreRegisterSetSave_
       .asmfunc

_CoreRegisterSetSave_
        str r0, [r0]
        str r1, [r0,#0x4]
        str r2, [r0,#0x8]
        str r3, [r0,#0xC]
        str r4, [r0,#0x10]
        str r5, [r0,#0x14]
        str r6, [r0,#0x18]
        str r7, [r0,#0x1C]
        str r8, [r0,#0x20]
        str r9, [r0,#0x24]
        str r10, [r0,#0x28]
        str r11, [r0,#0x2C]
        str r12, [r0,#0x30]
        str r13, [r0,#0x34]
        str r14, [r0,#0x38]
        bx    lr
        .endasmfunc


;-------------------------------------------------------------------------------
; Restore Core register set

       .def _CoreRegisterSetRestore_
       .asmfunc

_CoreRegisterSetRestore_
        ldr r0, [r0]
        ldr r1, [r0,#0x04]
        ldr r2, [r0,#0x08]
        ldr r3, [r0,#0x0C]
        ldr r4, [r0,#0x10]
        ldr r5, [r0,#0x14]
        ldr r6, [r0,#0x18]
        ldr r7, [r0,#0x1C]
        ldr r8, [r0,#0x20]
        ldr r9, [r0,#0x24]
        ldr r10, [r0,#0x28]
        ldr r11, [r0,#0x2C]
        ldr r12, [r0,#0x30]
        ldr r13, [r0,#0x34]
        ldr r14, [r0,#0x38]

;--     add 20 bytes to return behind the STC call
        add r14, r14, #0x18
		bx    lr
		.endasmfunc



;-------------------------------------------------------------------------------
; Store Bank registers


       .def _CoreBankRegisterSetSave_
       .asmfunc

_CoreBankRegisterSetSave_
; save the link register to be able to return
		mov r2,lr

; undefine mode
		cps	#0x1B
        str r13, [r0]
;        mrs   r1, CPSR
;        str r1, [r0,#0x4]
        add r0, r0,#0x8
; abort mode
		cps	#0x17
        str r13, [r0]
;        mrs   r1, CPSR
;        str r1, [r0,#0x4]
        add r0, r0,#0x8

; system mode
    	cps	#0x1F
        str r13, [r0]
;        mrs   r1, CPSR
;        str r1, [r0,#0x4]
        add r0, r0,#0x8
; irq mode
		cps	#0x12
        str r13, [r0]
;        mrs   r1, CPSR
;        str r1, [r0,#0x4]
        add r0, r0,#0x8
; fiq mode
		cps	#0x11
        str r13, [r0]
;        mrs   r1, CPSR
;        str r1, [r0,#0x4]
        add r0, r0,#0x8

; supervisor mode
		cps	#0x13
        str r13, [r0]
;        mrs   r1, CPSR
;        str r1, [r0,#0x4]
        add r0, r0,#0x8

; user mode stack pointer is identical to system mode SP, so not required
		mov lr, r2
        bx    lr
        .endasmfunc

;-------------------------------------------------------------------------------
; Store Bank registers


       .def _CoreBankRegisterSetRestore_
       .asmfunc

_CoreBankRegisterSetRestore_

		mov r2,lr
; undefine mode
		cps	#0x1B
		ldr r13, [r0]
		ldr r1, [r0,#0x4]
;		cps r1
        add r0, r0,#0x8
; abort mode
		cps	#0x17
		ldr r13, [r0]
		ldr r1, [r0,#0x4]
;		cps r1
        add r0, r0,#0x8

; system mode
		cps	#0x1F
		ldr r13, [r0]
		ldr r1, [r0,#0x4]
;		cps r1
        add r0, r0,#0x8
; irq mode
		cps	#0x12
		ldr r13, [r0]
		ldr r1, [r0,#0x4]
;		cps r1
        add r0, r0,#0x8

; fiq mode
    	cps	#0x11
		ldr r13, [r0]
		ldr r1, [r0,#0x4]
;		cps r1
        add r0, r0,#0x8

; supervisor mode
		cps	#0x13
		ldr r13, [r0]
		ldr r1, [r0,#0x4]
;		cps r1
        add r0, r0,#0x8

;		return
       	mov lr, r2
        bx  lr

        .endasmfunc

;-------------------------------------------------------------------------------
; Get region size register value

    .def     _mpuGetRegionSizeRegister_
    .asmfunc

_mpuGetRegionSizeRegister_

        mrc   p15, #0, r0, c6, c1, #2
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------

