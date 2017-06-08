#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <fstream>
#include <cmath>
#include <typeinfo>
#include <numeric>
#include "../include/AudioBase.h"

namespace RSSELI007{
   
    template <typename sample_type, int channel=1> class Audio : public AudioBase{
        private:
            std::vector<sample_type> data;
            int cutoff;
        public:
            Audio() = delete;
            Audio(std::string filename, int samplePsec)
            :   AudioBase(samplePsec, filename),
                cutoff(pow(2.0, 8*sizeof(sample_type))-1){
                std::ifstream file(AudioBase::filename, std::ios::binary);
                if(file.is_open()){
                    while(file){
                        sample_type s = 0;
                        s |= (unsigned char)file.get();
                        for(int i = 0; i < sizeof(sample_type)-1; ++i){
                            s |= file.get() << 8;
                        }
                        data.push_back(s);
                    }    
                    // remove empty byte
                    data.erase(data.begin()+data.size()-1);
                }
            }

            ~Audio(){
                this->AudioBase::sampleRate = 0;
            }
            
            Audio(const AudioBase & audio)
            :   AudioBase(audio.getSampleRate(), audio.getFilename()){
                const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                this->cutoff = a.cutoff;
                for(sample_type s : a.data){
                    this->data.push_back(s);
                }
            }

            
            Audio(const Audio<sample_type, channel> & audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename),
                cutoff(audio.cutoff){
                for(sample_type s : audio.data){
                    this->data.push_back(s);
                }
            }
            
            
            Audio(Audio<sample_type, channel> && audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename),
                data(std::move(audio.data)){
                // NOT WORKING
                audio.sampleRate = 0;
                delete audio;
            }

            // get data
            const std::vector<sample_type> getData() const{
                return data;
            }

            virtual AudioBase & operator=(const AudioBase & audio){
                // NOT WORKING
                //std::cout << "Copy assignment" << std::endl;
                if(this != &audio){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = a.AudioBase::filename;
                    this->data = std::vector<sample_type>(a.data.size());
                    //copy(a.data.begin(), a.data.end(), a.data.size());
                    for(sample_type s : a.data){
                        this->data.push_back(s);
                    }
                    //std::cout << "Completed copy assignment" << std::endl;
                }
                return *this;
            }

            virtual AudioBase & operator=(AudioBase && audio){
                // NOT WORKING
                if(this != &audio){
                    Audio<sample_type, channel> & a = dynamic_cast<Audio<sample_type, channel>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = std::move(a.AudioBase::filename);
                    this->data = a.data;
                    a.sampleRate = 0;
                }
                return *this;
            }

            // get number of samples
            virtual int dataPieces() const{
                return data.size();
            }

            // write to output file
            virtual void write(std::string outputFile) {
                std::ofstream file("output/" + outputFile + ".raw", std::ios::binary);
                if(file.good()){
                    for(sample_type s : data){
                        unsigned char * c= (unsigned char *)&s;
                        for(int i = 0; i < sizeof(sample_type); ++i){
                            file << c[i];
                        }   
                    }               
                }
            }
        
            //OVERLOADS

            // add
            virtual AudioBase & operator+=(const AudioBase & audio){
                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    if(this->dataPieces() == a.dataPieces()){
                        for(int i=0; i<this->dataPieces(); ++i){
                            //std::cout << this->data[i] << " + " << a.data[i] << " = ";
                            //std::cout << this->data[i] + a.data[i] << " or " ;
                            this->data[i] += a.data[i];
                            this->data[i] = this->data[i] > this->cutoff ? this->cutoff : this->data[i];
                            //std::cout << this->data[i] << std::endl;
                        }
                    }
                }
                return *this;
            }
            virtual AudioBase * operator+(const AudioBase & audio){
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                *result += audio;
                return result;
            }

            // volume factor
            virtual AudioBase & operator*=(std::pair<float, float> scaleFactor){
                if(scaleFactor.first >= 0 && scaleFactor.first <= 1){
                    for(int i=0; i<this->dataPieces(); ++i){
                        this->data[i] = (sample_type)(this->data[i]*scaleFactor.first);
                    }
                }
                return *this;
            }
            virtual AudioBase * operator*(std::pair<float, float> scaleFactor){
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                *result *= scaleFactor;
                return result;
            }

            // concatenate
            virtual AudioBase & operator|=(const AudioBase & audio){
                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    for(sample_type s : a.data){
                        this->data.push_back(s);
                    }
                }
                return *this;
            }
            virtual AudioBase * operator|(const AudioBase & audio){
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                *result |= audio;
                return result;
            }

            //cut
            virtual AudioBase & operator^=(const std::pair<int, int> range){
                if(range.first >= 0 && range.second < this->dataPieces()){
                    this->data.erase(this->data.begin()+range.first*channel, this->data.begin()+(range.second+1)*channel);
                }
                return *this;
            }
            virtual AudioBase * operator^(const std::pair<int, int> range){
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                *result ^= range;
                return result;
            }

            // reverse
            virtual AudioBase * operator!(){
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                std::reverse(result->data.begin(), result->data.end());
                return result;
            }
            
            // equality
            virtual bool operator==(const AudioBase & audio){
                if(this != &audio && typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    if(this->AudioBase::sampleRate == a.AudioBase::sampleRate 
                    && this->AudioBase::filename == a.AudioBase::filename){
                        for(int i = 0; i<this->data.size(); ++i){
                            if(this->data[i] != a.data[i]){
                                return false;
                            }
                        }
                    }
                }
                return true;;
            }

            // METHODS

            // add samples over range
            // not working fully
            virtual AudioBase * radd(const AudioBase & audio, const std::pair<int, int> range) {
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    if(range.first >= 0 && range.second < result->dataPieces()){
                        for(int i=range.first; i<range.second; ++i){
                            result->data[i] += a.data[i];
                            result->data[i] = result->data[i] > result->cutoff ? result->cutoff : result->data[i];
                        }
                    }
                }
                return result;
            }

            // root-mean-square of audio
            virtual float rms() {
                std::vector<float> v;
                for(sample_type s : this->data){
                    v.push_back(s);
                }
                for(float & s : v){
                    s *= s;
                }
                float result = std::accumulate(v.begin(), v.end(), 0);
                return sqrt(result/v.size());
            }

            // normalise over range
            virtual AudioBase * norm(std::pair<float, float> scaleFactor) {
                
            }

            // fade in 
            virtual AudioBase * fadeIn() {
                
            }

            // fade out
            virtual AudioBase * fadeOut() {
                
            }
 
   };

    /*
    template <typename sample_type> class Audio<sample_type, 2> : public AudioBase{
        private:
            std::vector<std::pair<sample_type, sample_type> > data;
        public:
            Audio(std::string filename, int samplePsec)
            : AudioBase(samplePsec, filename){
                this->sampleRate = samplePsec;
                std::ifstream file("input/"+filename, std::ios::binary);
                if(file.good()){
                    file.seekg(0, std::ios::end);
                    size_t size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    sample_type * temp;
                    //file.read(temp, size);
                    // need to read in manually
                    for(int i=0; i<size; i+=2){
                        std::pair<sample_type, sample_type> p(temp[i], temp[i+1]);
                        data.push_back(p);
                    }
                }
            }
            Audio(const Audio & audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename){
                this->data = std::vector<sample_type>(audio.data.size());
                copy(audio.data.begin(), audio.data.end(), audio.data.size());
            }
            Audio(Audio && audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename),
                data(audio.data){
                audio.sampleRate = 0;
                audio.data = NULL;
            }

            virtual AudioBase & operator=(const AudioBase & audio){
                std::cout << "Copy assignment" <<  std::endl;
                if(this != &audio){
                    const Audio<sample_type, 2> & a = dynamic_cast<const Audio<sample_type, 2>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = a.AudioBase::filename;
                    this->data = std::vector<std::pair<sample_type, sample_type> >(a.data.size());
                    //copy(a.data.begin(), a.data.end(), a.data.size());
                    for(std::pair<sample_type, sample_type> s : a.data){
                        this->data.push_back(s);
                    }
                }
                return *this;
            }

            virtual AudioBase & operator=(AudioBase && audio){
                if(this != &audio){
                    Audio<sample_type, 2> & a = dynamic_cast<Audio<sample_type, 2>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = std::move(a.AudioBase::filename);
                    this->data = a.data;
                    a.sampleRate = 0;
                }
                return *this;
            }

            virtual int dataPieces(){
                return data.size()*2;
            }
            // write to output file
            virtual void write(std::string outputFile) {

            }

            //OVERLOADS

            // add
            virtual AudioBase & operator+=(const AudioBase & audio){

            }
            virtual AudioBase * operator+(const AudioBase & audio){

            }

            // volume factor
            virtual AudioBase & operator*=(std::pair<float, float> scaleFactor){

            }
            virtual AudioBase * operator*(std::pair<float, float> scaleFactor){

            }

            // concatenate
            virtual AudioBase & operator|=(const AudioBase & audio){

            }
            virtual AudioBase * operator|(const AudioBase & audio){

            }

            //cut
            virtual AudioBase & operator^=(const std::pair<int, int> range){
                
            }
            virtual AudioBase * operator^(const std::pair<int, int> range){
                
            }

            // reverse
            virtual AudioBase * operator!(){

            }

            // equality
            virtual bool operator==(const AudioBase & audio){
                
            }

            // METHODS

            // add samples over range
            virtual AudioBase * radd(const AudioBase * audio, int start, int end) {

            }

            // root-mean-square of audio
            virtual AudioBase * rms() {
                
            }

            // normalise over range
            virtual AudioBase * norm(float start, float end) {
                
            }

            // fade in 
            virtual AudioBase * fadeIn() {
                
            }

            // fade out
            virtual AudioBase * fadeOut() {
                
            }
    };
    */
}
#endif