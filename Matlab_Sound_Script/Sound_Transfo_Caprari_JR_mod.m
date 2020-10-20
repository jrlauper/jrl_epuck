%_ ####################################################
% e-puck sound tranformation script
% december 2008, Gilles Caprari, www.GCtronic.com
%_ ####################################################

%_ source : https://web.archive.org/web/20180803060819/http://gctronic.com/files/e-puck_wav2s_matlabScript.txt

%_ Spring/Summer 2019: Jean-Roch LAUPER, unifr.ch
%_ slight modifications/adaptations 
%_  1/ to make it works with Matlab R2018b 
%_  2/ to make it still easier to use 
%_ original code still in comments / my comment %_


%_ Preliminary Step: Conversion MP3 -> WAV
%_ ****************************************
%_ Input needed for this script: .wav, mono, 16bit, 16000Hz

% transform sound.mp3 in sound.wav with input specification: mono, 16bit, 16000Hz 
% with for example mp3-2-wav converter

%_ the open source AUDACITY is a good choice for the transformation too 
%_ with a GUI and a lot of doc :  
%_ https://www.audacityteam.org/ (works on Win/Mac/Linux)
%_
%_ AUDACITY: Steps to do the transformation correctly
%_    1/ once the mp3 is open: track > resample : 16000
%_    2/ left-bottom corner drop-down menu: Project Rate > 16000 
%_    3/ track-window drop-down menu > Format > 16-bit PCM
%_    4/ file > export > WAV(Microsoft) Signed 16-bit PCM
%_ Source: http://www.dynamicsoflanguage.edu.au/research/data-archives/guides/resampling-audio-using-audacity/
%_
%_ AUDACITY enables too to generate directly a tone from a specific frequency and specific shape
%_ menu  "generate" -> tone


%_ Little tip to make your wav
%_ ***************************
%_ First, start just with one sound and try this script
%_ if you get an error, decrease the sample_number till the point
%_ you've got no error anymore -> this number will be the number
%_ of lines in the .s file corresponding to the length of your sounds
%_ e.g. for     200ms -> sample_number = 1440
%_              175ms -> sample_number = 1260

%_ ####################################################

wave_input_file_name = 'Beeps_and_A(880)_mod.wav'
wave_output_file_name = 'sound_out.wav'
s_output_file_name = 'Beeps_and_A(880)_mod.s'

sample_number = 2*1440 + 14*1224
% 200ms wav = 1440
% 175ms wav = 1260
% 170ms wav = 1224
% sample_number = 1440 * #(sounds of 200ms)
%_ with this matlab: max sample_number = 29520

%_ Empirical result: max sample_number for e-puck1 =~ 20'240
%_ Beyond, the sound doesn't play, you just hear a strange noise
%_ E.g. happens if sample_number = 20'520

%_ the sample_number must be a multiple of 4
%_ its max depends on the rest of the programm
%_
%_ e.g. with GCTronic Demo : max is roughly 22000 
%_ while the original file was 19044
%_ to know which part of the original wav is converted 
%_ => look at sound_out.wav length
%_
%  32000 samples (~4.5 s; ~60KB as *.wav or 450 KB as *.s)
%_ 22000 samples (~3.5 s; ~43KB as *.wav or 323 KB as *.s)
%_ 20000 samples (~2.8 s; ~40KB as *.wav or 295 KB as *.s)


%_ ####################################################

% read file in Matlab:
%_ [Y,FS,NBITS]=WAVREAD('sound.wav');
[Y, FS] = audioread(wave_input_file_name);
%_ [Y,FS,NBITS]=WAVREAD('sound.wav');
% resample to 7200 Hz
Y1=resample(Y,7200,16000);

Y1=Y1(1:sample_number);
% test the sound
disp('input sound')
sound(Y1,7200)
% pause

% optionally check the frequency domain
y = fft(Y1,10000);
Pyy = y.* conj(y) /10000;
f = 7200*(0:5000) /10000;
plot(f,Pyy(1:5001))
disp('frequency domain')
% pause

% if you need to filter, example band pass filter from 140Hz to 3200Hz
%_ [b,a] = butter(9,[140 3200]/3600);
[b,a] = butter(9,[140 3584]/3600);
freqz(b,a,128,7200)
Y2 = filter(b,a,Y1);
sound (Y2,7200)
disp('filtered sound')
% pause

% optionally re-check the frequency domain
y = fft(Y2,10000);
Pyy = y.* conj(y) /10000;
f = 7200*(0:5000)/10000;
plot(f,Pyy(1:5001))
disp('new frequency domain')
% pause


% check the the max "volume", should be ~0.8
max(Y2)
disp('max "volume", should be ~0.7')
% pause
% optionally adjust volume
Y2=Y2*0.8;

% _add JRL
disp('after volume changed')
max(Y2)

% write the sound.wav file
%_ WAVWRITE(Y2,7200,16,'sound_out')
audiowrite(wave_output_file_name, Y2,7200)

% write the sound as text file to be compiled for e-puck
Y3=round(Y2*32768);
fid = fopen(s_output_file_name,'wt');
fprintf(fid,'.section .sound_samples,code\n');
fprintf(fid,'.global e_sound_sample\n');
fprintf(fid,'.palign 2\n');
fprintf(fid,'; %s : 16 bits, 7200 Hz, %d samples\n', wave_input_file_name, length(Y3));
fprintf(fid,'e_sound_sample:\n');
for n=1:length(Y3)
  if (Y3(n)>=0)
    fprintf(fid,'.hword 0x%04X\n', Y3(n));
  else 
    fprintf(fid,'.hword 0x%04X\n', 65536+Y3(n));
  end
end
fclose(fid);
disp('files written on the disc')
% pause