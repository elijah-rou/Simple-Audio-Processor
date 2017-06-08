Hi there tutor!

So with regards to running the program, you run it as follows:
bin/samp -r sampleRate -b sampleSize -c channels -o outputName -command (parameter 1) (parameter 2) (parameter 3) (parameter 4) audio1 (audio2)

The parameters depend on the command, and must come before the audio files.

Here are examples of every command
    Add (audio files must have same properties):
    bin/samp -r 44100 -b 8 -c 1 -o monoAdd8 -add input/beez18sec_44100_signed_8bit_mono.raw input/frogs18sec_44100_signed_8bit_mono.raw

    Cut (sample x to sample y, must fall within range):
	bin/samp -r 44100 -b 8 -c 1 -o monoCut8 -cut 198450 595350 input/beez18sec_44100_signed_8bit_mono.raw

    Range Add (ranges(in seconds) should be of same length and fall in the range of the clips):
	bin/samp -r 44100 -b 8 -c 1 -o monoRAdd8 -radd 0 8 9 17 input/beez18sec_44100_signed_8bit_mono.raw input/frogs18sec_44100_signed_8bit_mono.raw

    Concatenate:
	bin/samp -r 44100 -b 8 -c 1 -o monoCat8 -cat input/beez18sec_44100_signed_8bit_mono.raw input/frogs18sec_44100_signed_8bit_mono.raw

    Volume scale (M/null for mono, L/R for stereo)
	bin/samp -r 44100 -b 8 -c 1 -o monoV8 -v 0.5 0.5 input/beez18sec_44100_signed_8bit_mono.raw

    Reverse:
	bin/samp -r 44100 -b 8 -c 1 -o monoRev8 -rev input/beez18sec_44100_signed_8bit_mono.raw

    Normalise: set audio channels to a certain RMS (M/null for mono, L/R for stereo)
	bin/samp -r 44100 -b 8 -c 1 -o monoNorm8 -norm 10 10 input/beez18sec_44100_signed_8bit_mono.raw

    Root-Mean-Square: Print RMS value of each channel to console
	bin/samp -r 44100 -b 8 -c 1 -rms input/beez18sec_44100_signed_8bit_mono.raw

The makefile contains a variety of rules, namely:
    make -> compile and link
    make clean -> remove all outputted files, object files and executables
    make run args="arguments" -> run the program with certain args
    make test -> Compile and run all catch tests

Also, the makefile contains rules to compile output of certain types easily, utilising all commands
    make quickMono8
    make quickMono16
    make quickStereo8
    make quickStereo16

NOTE: if runnning manually, please insure that the input parameters are correct

Also, the fade commands are not implemented, I got lazy XD (ie compilation generates warnings)
GL HF