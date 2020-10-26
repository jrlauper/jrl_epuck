extract from runfftlistener_mod.h

 *************** FFT MULTIPLE Tones/Freq recognition attempts **************
 * Summer 2020, unifr.ch, Jean-Roch LAUPER
 * 
 * Preliminary note: 
 *      Although the code has been cleaned up, it is still a bit drafty. 
 *      This reflects its "attempts" and "tests" aspect. 
 *      However it should be clear enough to allow others to continue work in the same direction.
 * 
 * The basic idea behind this file/project
 *      "Now that I'm able to make Robots emits tones
 *      My goal : see how the others robots can recognize them"
 * 
 * For the correspondence tones recognized/leds : 
 * see in this folder the picture: 
 *      Sensors and Leds - faded - with tones (JR).png
 * 
 * 
 *  
 * Videos:  
 * (1) E-Puck1 - Tones Recognition (FFT) - piano and whistling
 *      https://youtu.be/516BiB3IN2Q
 * 
 * (2) E-Puck1 - Robots "Communication" through sounds - Unsuccessful Attemps with A880
 *      https://youtu.be/bFydJq7alVA
 *       0:03 run_fft_listener() - only the very frequency meant to be recognized
 *       1:40 run_fft_listener_COMM() with init_COMM_freq_A880() 
 *             group of possible frequencies for each tone (instead of a unique frequency)
 *       3:10 run_fft_listener_comm_STAT() with init_comm_freq_A880_STAT()
 *              group of possible frequencies 
 *              with occurrence assessment to find the right tone (20 samples used in the video)
 * 
 * (3) E-Puck1 - Robots "Communication" through sounds - Successful with HF7
 *      https://youtu.be/w7GG8Ibt3go
 *      run_fft_listener_comm_stat()
 *      with init_comm_freq_HF7_stat()
 * 
***************************************************************************** */
