.include "xc.inc"          ; required "boiler-plate" (BP)

;the next two lines set up the actual chip for operation - required
config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

    .bss        ; put the following labels in RAM
counter:
    .space 2    ; a variable that takes two bytes (we won?t use
                ; it for now, but put here to make this a generic
                ; template to be used later).
stack:
    .space  32  ; this will be our stack area, needed for func calls

.text           ; BP (put the following data in ROM(program memory))

;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
.global _main               ;BP
;your functions go here

wait_21cycles:
                     ; 2 cycles for function call
    repeat #14        ; 1 cycle to load and prep
    nop              ; 14+1 cycles to execute NOP 15 times
    return           ; 3 cycles for the return
    
wait_100us: ;1600 total cycles
			; 2 cycles for function call
    repeat #1593	; 1 cycle to load and prep
    nop			; 1593+1 cycles to execute nop
    return		; 3 cycles for the return

wait_1ms: ;16000 total cycles
			; 2 cycles for function call
    repeat #15993	; 1 cycle to load and prep
    nop			; 15993+1 cycles to execute nop
    return		; 3 cycles for the return
    
high24low32:
				  ; 2 cycles for function call
	call    wait_21cycles     ; 21 cycles
	repeat #6		  ; 1 cycle to load and prep
	nop			  ; 6+1 cycles to execute NOP 7 times
	inc     LATA              ; set pin RA0 high = 1 cycle
				  ; Total = 32 cycles low
	nop                       ; 2 cycles to match BRA delay
	nop                       ;
	call    wait_21cycles     ; 21 cycles
	clr   LATA                ; set pin RA0 low = 1 cycle
				  ; Total = 24 cycles high

write_0: ;total = 1.1875 us = 19 cycles
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
    
write_1: ;total = 1.1875 us = 19 cycles
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
	
_main:
    bclr    CLKDIV,#8                 ;BP
    nop
    ;; --- Begin your main program below here ---
    mov     #0x9fff,w0  
    mov     w0,AD1PCFG            ; Set all pins to digital mode
    mov     #0b1111111111111110,w0  
    mov     w0,TRISA            ; set pin RA0 to output
    mov     #0x0000,w0  
    mov     w0,LATA            ; set pin RA0 high
    
    call write_1
    call write_1
    call write_1
    call write_1
    call write_1
    call write_1
    call write_1
    call write_1
    
    call write_0
    call write_1
    call write_0
    call write_0
    call write_1
    call write_1
    call write_0
    call write_0
    
    call write_0
    call write_1
    call write_0
    call write_0
    call write_1
    call write_1
    call write_0
    call write_0
    
    repeat #1280
    nop
    
    foreverLoop:
	bra foreverLoop


