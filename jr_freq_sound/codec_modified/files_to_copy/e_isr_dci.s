
;
; File Notes:
;
; 1. The DCI interrupt service routine transfers 4 new tone samples to TXBUF0-4.
;    It also keeps a count of the number of samples transferred thus far.
;
; 2.  A tone sample is 100 milliseconds long and is followed by
;     15 milliseconds of silence.
;
; 3.  A tone sample is obtained by adding amplitudes of the sinusoid components.
;



.include "p30F6014A.inc"
.include "e_common.inc"


.global __DCIInterrupt
.section .text
__DCIInterrupt:
        bclr    IFS2, #DCIIF                    ;Clear the interrupt flag
        push.d  w0                              ;Save w0-w3 off to stack
        push.d  w2
        mov     RXBUF0, w0                      ;Dummy read of RXBUF0-RXBUF3
        mov     RXBUF1, w0
        mov     RXBUF2, w0
        mov     RXBUF3, w0
	cp0	_e_stop_flag
	bra     nz, spcl_exit_DCI_ISR
        cp0     e_samples_count                ;Is the number of DTMF samples
        bra     z, spcl_exit_DCI_ISR            ;samples have been SENT? If so, then exit

sendsamples:                                    ;If not then send next DTMF sample
        mov     e_tone_ptr, w1                  ;Retrieve pointer to lower tone
                                                ;component into w1
	;FIRST BYTE
        mov	[w1], w0			;move two samples in w0			
	and	#0x00FF, w0			;extract LS-byte (first sample)
	sl	w0, #8, w0			;shift left by one byte in order to amplify the signal
	bclr    w0, #0                          ;Clear the LS bit of the resultant
                                                ;This ensures that the dsPIC does
                                                ;not accidentally request a secondary
                                                ;frame transmission from the codec.
                                                ;Note that the Si3000 codec only provides
                                                ;an 84 dB dynamic range
        mov     w0, TXBUF0                      ;Write the result to TXBUF0
	;SECOND BYTE
	clr	w0
	mov	[w1], w0			;move two samples in w0
	bclr	w0, #0                          ;extract MS-byte (second sample) + LS-bit cleared so that the codec does not				;
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
	bclr    w0, #0
        mov     w0, TXBUF2
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
        bclr	w0, #7
        mov	w0, TXBUF3                      
	inc2	w1, w1				;increment the pointer to next two samples

        dec2    e_samples_count                	;Decrement the number of DTMF samples left
        dec2    e_samples_count                	;to transmit by 4

        mov     w1, e_tone_ptr                  ;Update the Low and High tone pointers

exit_DCI_ISR:
        pop.d   w2                              ;Restore w0, w1, w2 and w3
        pop.d   w0
        retfie                                  ;Return from Interrupt


spcl_exit_DCI_ISR:
        clr     _e_dci_unavailable             	;If silence has also been transmitted
	setm	_e_stop_flag
        pop.d   w2                              ;then clear the DCIUnavailable flag.
        pop.d   w0                              ;Restore w0-w3
        retfie                                  ;Return from Interrupt

.end                                            ;EOF






