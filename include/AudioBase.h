#ifndef AUDIOBASE_H
#define AUDIOBASE_H

#include <string>

namespace RSSELI007{
    class AudioBase{
        protected:
            int sampleRate;
            std::string filename;
        public:
            AudioBase() = delete;
            AudioBase(int samplePsec, std::string file) 
            :   sampleRate(samplePsec),
                filename(file){}

            virtual ~AudioBase(){
                sampleRate = 0;
                filename = "";
            }
            // write to output file
            virtual void write(std::string outputFile) = 0;

            // add two audio clips
            virtual AudioBase & add(const AudioBase * audio) = 0;

            // remove samples over range
            virtual AudioBase & cut(int start, int end) = 0;

            // add samples over range
            virtual AudioBase & radd(const AudioBase * audio, int start, int end) = 0;

            // concatenate audio clips
            virtual AudioBase & cat(const AudioBase * audio) = 0;

            // volume factor over range
            virtual AudioBase & v(int start, int end) = 0;

            // reverse audio
            virtual AudioBase & rev() = 0;

            // root-mean-square of audio
            virtual AudioBase & rms() = 0;

            // normalise over range
            virtual AudioBase & norm(float start, float end) = 0;

            // fade in 
            virtual AudioBase & fadeIn() = 0;

            // fade out
            virtual AudioBase & fadeOut() = 0;
    }; 
}
#endif