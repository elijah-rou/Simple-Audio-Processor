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

            // Copy/Move Assignment
            virtual AudioBase & operator=(const AudioBase & audio) = 0;
            virtual AudioBase & operator=(AudioBase && audio) = 0;

            // getters
            int getSampleRate() const{
                return this->sampleRate;
            }
            std::string getFilename() const{
                return this->filename;
            }
            virtual int dataPieces() = 0;

            //OVERLOAD

            // add audio objects
            virtual AudioBase & operator+=(const AudioBase & audio) = 0;
            virtual AudioBase * operator+(const AudioBase & audio) = 0;

            // apply volume factor to scale channels
            virtual AudioBase & operator*=(const std::pair<float, float> scaleFactor) = 0;
            virtual AudioBase * operator*(const std::pair<float, float> scaleFactor) = 0;

            // concatenate 2 audio objects
            virtual AudioBase & operator|=(const AudioBase & audio) = 0;
            virtual AudioBase * operator|(const AudioBase & audio) = 0;

            // cut a range of data from audio object
            virtual AudioBase & operator^=(const std::pair<int, int> range) = 0;
            virtual AudioBase * operator^(const std::pair<int, int> range) = 0;

            // reverse
            virtual AudioBase * operator!() = 0;

            // equality
            virtual bool operator==(const AudioBase & audio) = 0;
            virtual bool operator!=(const AudioBase & audio){
                return !(*this==audio);
            }

        
            //METHODS

            // write to output file
            virtual void write(std::string outputFile) = 0;

            // add samples over range
            virtual AudioBase * radd(const AudioBase * audio, int start, int end) = 0;

            // root-mean-square of audio
            virtual AudioBase * rms() = 0;

            // normalise over range
            virtual AudioBase * norm(float start, float end) = 0;

            // fade in 
            virtual AudioBase * fadeIn() = 0;

            // fade out
            virtual AudioBase * fadeOut() = 0;
    }; 
}
#endif