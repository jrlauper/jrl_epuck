%_ source : https://web.archive.org/web/20180803060819/http://gctronic.com/files/e-puck_wav2s_matlabScript.txt

% e-puck sound tranformation script
%
% december 2008, Gilles Caprari, www.GCtronic.com
%

% steps:

% trasform sound.mp3 in sound.wav with input specification: mono, 16bit, 16000Hz 
% with for example mp3-2-wav converter

% read file in Matlab:
[Y,FS,NBITS]=WAVREAD('sound.wav');
% resample to 7200 Hz
Y1=resample(Y,7200,16000);
% shorten to about 32000 samples (~4.5 s; ~60KB as *.wav or 450 KB as *.s)
Y1=Y1(1:32000);
% test the sound
disp('input sound')
sound(Y1,7200)
pause

% optionally check the frequency domain
y = fft(Y1,10000);
Pyy = y.* conj(y) /10000;
f = 7200*(0:5000) /10000;
plot(f,Pyy(1:5001))
disp('frequency domain')
pause

% if you need to filter, example band pass filter from 140Hz to 3200Hz
[b,a] = butter(9,[140 3200]/3600);
freqz(b,a,128,7200)
Y2 = filter(b,a,Y1);
sound (Y2,7200)
disp('filtered sound')
pause

% optionally re-check the frequency domain
y = fft(Y2,10000);
Pyy = y.* conj(y) /10000;
f = 7200*(0:5000)/10000;
plot(f,Pyy(1:5001))
disp('new frequency domain')
pause


% check the the max "volume", should be ~0.8
max(Y2)
disp('max "volume", should be ~0.7')
pause
% optionally adjust volume
% Y2=Y2*0.8;

% write the sound.wav file
WAVWRITE(Y2,7200,16,'sound_out')

% write the sound as text file to be compiled for e-puck
Y3=round(Y2*32768);
fid = fopen('sound.s','wt');
fprintf(fid,'.section .sound_samples,code\n');
fprintf(fid,'.global e_sound_sample\n');
fprintf(fid,'.palign 2\n');
fprintf(fid,'; sound.wav : 16 bits, 7200 Hz, %d samples\n',length(Y3));
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