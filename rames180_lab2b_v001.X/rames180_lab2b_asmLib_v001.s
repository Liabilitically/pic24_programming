.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _write_0, _write_1, _wait_tr_100us, _wait_tr_1ms
    
_write_0: ;total = 1.1875 us = 19 cycles
			; 2 cycles for function call
    inc LATA		; 1 cycle to set
			; 13*0.0625 = 0.8125
    
    repeat #3		; 1 cycle to load
    nop			; 3+1 cycles to execute nop 4 times
    clr LATA		; 1 cycle to clear
			; 6*0.0625 = 0.375 us high
			
    repeat #5		; 1 cycle to load
    nop			; 5+1 cycles to execute nop 6 times
    return		; 3 cycles for return
    
_write_1: ;total = 1.1875 us = 19 cycles
			; 2 cycles for function call
    inc LATA		; 1 cycle to set
			; 8*0.0625 = 0.5 us low
    
    repeat #8		; 1 cycle to load
    nop			; 8+1 cycles to execute nop 9 times
    clr LATA		; 1 cycle to clear
			; 11*0.0625 = 0.6875 us high
			
    nop			; 1 cycle
    nop			; 1 cycle
    return		; 3 cycles for return
    
_wait_tr_100us: ;1600 total cycles
			; 2 cycles for function call
    repeat #1593	; 1 cycle to load and prep
    nop			; 1593+1 cycles to execute nop
    return		; 3 cycles for the return

_wait_tr_1ms: ;16000 total cycles
			; 2 cycles for function call
    repeat #15993	; 1 cycle to load and prep
    nop			; 15993+1 cycles to execute nop
    return		; 3 cycles for the return