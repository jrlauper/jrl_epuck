; File Notes:
;
; 1. This subroutine maps a number received by the UART,
;    to a DTMF tone and kicks off DCI transmissions.
;
; 2. All tone samples are stored in Program Memory, so we use PSV to access
;    this data.
;
; 3. A tone sample is 100 milliseconds long and is followed by
;    15 milliseconds of silence.
;
; 4. A tone sample is obtained by adding amplitudes of the sinusoid components.
;    The low frequency omponent has a signal strength 8 dB higher than the
;    high frequency component

.include "p30F6014A.inc"
.include "e_common.inc"


.section .ndata,data,near
.align WORD

.global _e_dci_unavailable, _e_stop_flag, e_tone_ptr
.global e_samples_count, e_tones_record, e_actual_page

.section .nbss,bss,near
.align 2
e_tone_ptr:             .space WORD

.section                .ndata
.align  WORD

_e_stop_flag:				.hword 0x0000

_e_dci_unavailable:        .hword 0x0000                ;Flag that indicates the DCI
                                                        ;module is transmitting a tone
                                                        ;Flag is used to prevent one tone
                                                        ;"over-writing" another
e_samples_count:       	.hword 0x0000                   ;Keeps a count of number of DTMF
                                                        ;samples left to transmit
e_actual_page:				.hword 0x0000
 

.global _e_sub_dci_kickoff
.section .text

; sound offset in parameter 0, length in parameter 1
_e_sub_dci_kickoff:
        push    w1                              ; save W1 and W2 on the stack
        push    w2
        bra     initiateDCI

exit_dci_kickoff:
        pop     w2
        pop     w1
        return


initiateDCI:
load_samp_cnt:
        mov     w1, e_samples_count		;set number of samples
        clr     w2
	mov	w0, w1				;move pointer to sound data in w1
		
continue1:
	;FIRST BYTE
        mov	[w1], w0			;move two samples in w0			
	and	#0x00FF, w0			;extract LS-byte (first sample)
	sl	w0, #8, w0			;shift left by one byte in order to amplify the signal
        bclr	w0, #0                          ;LS-bit cleared so that the codec does not
        mov	w0, TXBUF0                      ;assume a secondary frame request is being made
	;SECOND BYTE
	clr	w0
	mov	[w1], w0			;move two samples in w0
	bclr	w0, #0                          ;extract MS-byte (second sample) + LS-bit cleared so that the codec does not
        bclr	w0, #1				;assume a secondary frame request is being made
	bclr	w0, #2
        bclr	w0, #3
	bclr	w0, #4
        bclr	w0, #5
	bclr	w0, #6
        bclr	w0, #7                          ;LS-bit cleared so that the codec does not
        mov	w0, TXBUF1                      ;assume a secondary frame request is being made
	inc2	w1, w1				;increment the pointer to next two samples
	;THIRD BYTE
	clr	w0
        mov	[w1], w0			;move two samples in w0			
	and	#0x00FF, w0			;extract LS-byte (first sample)
	sl	w0, #8, w0			;shift left by one byte in order to amplify the signal			
        bclr	w0, #0                          ;LS-bit cleared so that the codec does not
        mov	w0, TXBUF2                      ;assume a secondary frame request is being made
	;FOURTH BYTE
	clr	w0
	mov	[w1], w0			;move two samples in w0
	bclr	w0, #0                          ;extract MS-byte (second sample) + LS-bit cleared so that the codec does not
        bclr	w0, #1				;assume a secondary frame request is being made
	bclr	w0, #2
        bclr	w0, #3
	bclr	w0, #4
        bclr	w0, #5
	bclr	w0, #6
        bclr	w0, #7                          ;LS-bit cleared so that the codec does not
        mov	w0, TXBUF3                      ;assume a secondary frame request is being made
	inc2	w1, w1				;increment the pointer to next two samples

        dec2    e_samples_count                	;Decrement samples count by 4 since
        dec2    e_samples_count                	;we sent 4 samples out
        mov     w1, e_tone_ptr                  ;Update the High and Low tone pointers

        bclr    IFS2, #DCIIF                    ;Clear the DCI Interrupt flag

        bra     exit_dci_kickoff

.end

