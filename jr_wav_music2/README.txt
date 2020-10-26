The sound lib is an adaptation of the Microchip sample program.
Now it allows you to play a single WAV.
You can choose to play the wole file, or only a part of it.

the wav is stored in const_peur_sound.s
it is a list of sample value. A simple matlab script can be used to create such a file from a wav input.

functions :
- void init_sound(void);
  prepare the sound module. 


- void play_sound(int sound_offset, int sound_length);
  play the sound in const_peur_sound.s from the (file_start + offset) sample to the (file_start + offset + sound_length) sample.
  WARNING :	sound_length integer must be a multiple of 4
		4 samples are sent at each interrupt, so if length is not a multiple of 4, the stop condition is never true. the program may crash and reboot the DsPic.

Two global variables are exported :
StopFlag
and
DCIUnavailable

StopFlag is automatically set to 0 when a sound is played. But if you want to stop the sound during a play, you can set this variable to 1. Next time the interrupt tries to send new sound data, it will stop.
DCIUnavailable is a flag that indicate that the sound module is playing something. When sound is over, the flag return to 0.


----------------------------------------
JRL Addition

Start of e_const_sound.s file
----------------------------------------

.section .sound_samples,code
;.section .text
.global e_sound_sample
.palign 2
e_sound_sample:
;ahhhh.wav : 16 bits, 1 channels, 19044 samples
;sound plage:
;position,lengh
;0,2112 "haa"
;2116,1760 "spaah"
;3878,3412 "ouah"
;7294,3732 "yaouh"
;11028,8016 "wouaaaaaaaah"
.hword 0x0006
.hword 0x0026
.hword 0x001F
.hword 0x0012
.hword 0x0027
.hword 0x0018
...
.hword 0x004D
.hword 0xFFAD
.hword 0x004A
.hword 0xFFC6
.hword 0x0026
.hword 0xFFEB
.hword 0x0009
.hword 0xFFFE
.hword 0xFFFE
.hword 0x0002

(19058 line)
