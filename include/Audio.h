#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <fstream>
#include <cmath>
#include <typeinfo>
#include <numeric>
#include "../include/AudioBase.h"

namespace RSSELI007{

    template <typename sample_type, int channel=1> struct Normalise{
        float rmsScale;
        Normalise(float newRMS, float oldRMS){
            rmsScale = newRMS/oldRMS;
        }
        sample_type operator()(sample_type s){
            s *= rmsScale;
            return s;
        }
    };
    
   
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
                this->AudioBase::filename = "";
                this->cutoff = 0;
                this->data.resize(0);
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
            :   AudioBase(audio.AudioBase::sampleRate, std::move(audio.AudioBase::filename)),
                data(std::move(audio.data)),
                cutoff(audio.cutoff){
                audio.sampleRate = 0;
                audio.cutoff = 0;
            }

            // get data
            const std::vector<sample_type> getData() const{
                return data;
            }

            const int getCutoff() const{
                return cutoff;
            }

            virtual AudioBase & operator=(const AudioBase & audio){
                if(this != &audio){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = a.AudioBase::filename;
                    this->cutoff =a.cutoff;
                    for(sample_type s : a.data){
                        this->data.push_back(s);
                    }
                }
                return *this;
            }

            virtual AudioBase & operator=(AudioBase && audio){
                // NOT WORKING
                if(this != &audio){
                    Audio<sample_type, channel> & a = dynamic_cast<Audio<sample_type, channel>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = std::move(a.AudioBase::filename);
                    this->data = std::move(a.data);
                    this->cutoff =a.cutoff;
                    a.AudioBase::sampleRate = 0;
                    a.cutoff = 0;
                }
                return *this;
            }

            // get number of samples
            virtual int dataPieces() const{
                return data.size();
            }

            // write to output file
            virtual void write(std::string outputFile) {
                std::ofstream file("output/" + outputFile + "_" + std::to_string(this->AudioBase::sampleRate)
                                  + "_" + std::to_string(sizeof(sample_type)*8) + "bit_mono.raw", std::ios::binary);
                if(file.good()){
                    for(sample_type s : data){
                        unsigned char * c = (unsigned char *)&s;
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
                            this->data[i] += a.data[i];
                            this->data[i] = this->data[i] > this->cutoff ? this->cutoff : this->data[i];
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
                if(this == &audio)
                    return true;

                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    if(this->AudioBase::sampleRate == a.AudioBase::sampleRate 
                    && this->AudioBase::filename == a.AudioBase::filename){
                        for(int i = 0; i<this->data.size(); ++i){
                            if(this->data[i] != a.data[i]){
                                return false;
                            }
                        }
                    }
                    return true;
                }
                return false;
                
            }
            virtual bool operator!=(const AudioBase & audio){
                return !(*this==audio);
            }

            // METHODS

            // add samples over range
            virtual AudioBase * radd(const AudioBase & audio, const std::pair<float, float> range1, const std::pair<float, float> range2) {
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    if(range1.first*this->AudioBase::sampleRate >= 0 
                    && range1.second*this->AudioBase::sampleRate+1 < result->dataPieces()){
                        /*
                        for(int i=range1.first*this->AudioBase::sampleRate; i<(int)(range1.second*this->AudioBase::sampleRate)+1; ++i){
                            result->data[i] += a.data[i];
                            result->data[i] = result->data[i] > result->cutoff ? result->cutoff : result->data[i];
                        }
                        */
                        auto it = a.data.begin()+range1.first*this->AudioBase::sampleRate;
                        for_each(result->data.begin()+range1.first*this->AudioBase::sampleRate, 
                                 result->data.begin()+range1.second*this->AudioBase::sampleRate+1,
                                    [&](sample_type & s){
                                        s += *it;
                                        s = s > result->cutoff ? result->cutoff : s;
                                        ++it;
                                    }
                                );
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
                for_each(v.begin(), v.end(),
                    [&](float & f){
                        f *=f; 
                    }
                );
                float result = std::accumulate(v.begin(), v.end(), 0);
                return sqrt(result/v.size());
            }

            // normalise over range
            virtual AudioBase * norm(std::pair<float, float> requiredRMS) {
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                Normalise<sample_type, channel> n(requiredRMS.first, result->rms());
                std::transform(result->data.begin(), result->data.end(), result->data.begin(), n);
                return result;
            }

            // fade in 
            virtual AudioBase * fadeIn() {
                
            }

            // fade out
            virtual AudioBase * fadeOut() {
                
            }
 
   };

   // STEREO
   
    template <typename sample_type> struct Normalise<sample_type, 2>{
        std::pair<float, float> rmsScale;
        Normalise(std::pair<float, float> newRMS, std::pair<float, float> oldRMS){
            rmsScale =  std::pair<float, float>(newRMS.first/oldRMS.first,newRMS.second/oldRMS.second);
        }
        sample_type operator()(std::pair<sample_type, sample_type> s){
            s.first *= rmsScale.first;
            s.second *= rmsScale.second;
            return s;
        }
    };
    
   
    template <typename sample_type> class Audio<sample_type, 2> : public AudioBase{
        private:
            std::vector<std::pair<sample_type, sample_type> > data;
            int cutoff;
        public:
            Audio() = delete;
            Audio(std::string filename, int samplePsec)
            :   AudioBase(samplePsec, filename),
                cutoff(pow(2.0, 8*sizeof(sample_type))-1){
                std::ifstream file(AudioBase::filename, std::ios::binary);
                if(file.is_open()){
                    while(file){
                        std::pair<sample_type, sample_type> d;
                        sample_type s1 = 0;
                        // channel 1
                        s1 |= (unsigned char)file.get();
                        for(int i = 0; i < sizeof(sample_type)-1; ++i){
                            s1 |= file.get() << 8;
                        }
                        // channel 2
                        sample_type s2 = 0;
                        s2 |= (unsigned char)file.get();
                        for(int i = 0; i < sizeof(sample_type)-1; ++i){
                            s2 |= file.get() << 8;
                        }
                        d = std::pair<sample_type, sample_type>(s1, s2);
                        data.push_back(d);
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
                const Audio<sample_type, 2> & a = dynamic_cast<const Audio<sample_type, 2>&>(audio);
                this->cutoff = a.cutoff;
                for(std::pair<sample_type, sample_type> s : a.data){
                    this->data.push_back(s);
                }
            }

            
            Audio(const Audio<sample_type, 2> & audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename),
                cutoff(audio.cutoff){
                for(std::pair<sample_type, sample_type> s : audio.data){
                    this->data.push_back(s);
                }
            }
            
            
            Audio(Audio<sample_type, 2> && audio)
            :   AudioBase(audio.AudioBase::sampleRate, std::move(audio.AudioBase::filename)),
                data(std::move(audio.data)),
                cutoff(audio.cutoff){
                audio.sampleRate = 0;
                audio.cutoff = 0;
            }

            // get data
            const std::vector<std::pair<sample_type, sample_type> > getData() const{
                return data;
            }

            const int getCutoff() const{
                return cutoff;
            }

            virtual AudioBase & operator=(const AudioBase & audio){
                if(this != &audio){
                    const Audio<sample_type, 2> & a = dynamic_cast<const Audio<sample_type, 2>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = a.AudioBase::filename;
                    //this->data = std::vector<std::pair<sample_type, sample_type> >(a.data.size());
                    this->cutoff =a.cutoff;
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
                    this->data = std::move(a.data);
                    this->cutoff =a.cutoff;
                    a.AudioBase::sampleRate = 0;
                    a.cutoff = 0;
                }
                return *this;
            }

            // get number of samples
            virtual int dataPieces() const{
                return data.size();
            }

            // write to output file
            virtual void write(std::string outputFile) {
                std::ofstream file("output/" + outputFile + "_" + std::to_string(this->AudioBase::sampleRate)
                                  + "_" + std::to_string(sizeof(sample_type)*8) + "bit_stereo.raw", std::ios::binary);
                if(file.good()){
                    for(std::pair<sample_type, sample_type> s : data){
                        // channel 1
                        unsigned char * c = (unsigned char *)&s.first;
                        for(int i = 0; i < sizeof(sample_type); ++i){
                            file << c[i];
                        }  
                        // channel 2
                        c = (unsigned char *)&s.second;
                        for(int i = 0; i < sizeof(sample_type); ++i){
                            file << c[i];
                        }
                    }               
                }
            }
        
            //OVERLOADS

            // add
            virtual AudioBase & operator+=(const AudioBase & audio){  
                if(typeid(audio) == typeid(Audio<sample_type, 2>)){
                    const Audio<sample_type, 2> & a = dynamic_cast<const Audio<sample_type, 2>&>(audio);
                    if(this->dataPieces() == a.dataPieces()){
                        for(int i=0; i<this->dataPieces(); ++i){
                            this->data[i] = std::make_pair(this->data[i].first+a.data[i].first, this->data[i].second+a.data[i].second);
                            this->data[i].first = this->data[i].first > this->cutoff ? this->cutoff : this->data[i].first;
                            this->data[i].second = this->data[i].second > this->cutoff ? this->cutoff : this->data[i].second;
                        }
                    }
                }
                return *this;
            }
            virtual AudioBase * operator+(const AudioBase & audio){
                Audio<sample_type, 2> * result = new Audio<sample_type, 2>(*this);
                *result += audio;
                return result;
            }

            // volume factor
            virtual AudioBase & operator*=(std::pair<float, float> scaleFactor){
                /*
                if(scaleFactor.first >= 0 && scaleFactor.first <= 1){
                    for(int i=0; i<this->dataPieces(); ++i){
                        this->data[i] = (sample_type)(this->data[i]*scaleFactor.first);
                    }
                }
                return *this;
                */
            }
            virtual AudioBase * operator*(std::pair<float, float> scaleFactor){
                Audio<sample_type, 2> * result = new Audio<sample_type, 2>(*this);
                *result *= scaleFactor;
                return result;
            }

            // concatenate
            virtual AudioBase & operator|=(const AudioBase & audio){
                /*
                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    for(sample_type s : a.data){
                        this->data.push_back(s);
                    }
                }
                return *this;
                */
            }
            virtual AudioBase * operator|(const AudioBase & audio){
                Audio<sample_type, 2> * result = new Audio<sample_type, 2>(*this);
                *result |= audio;
                return result;
            }

            //cut
            virtual AudioBase & operator^=(const std::pair<int, int> range){
                /*
                if(range.first >= 0 && range.second < this->dataPieces()){
                    this->data.erase(this->data.begin()+range.first*channel, this->data.begin()+(range.second+1)*channel);
                }
                return *this;
                */
            }
            virtual AudioBase * operator^(const std::pair<int, int> range){
                Audio<sample_type, 2> * result = new Audio<sample_type, 2>(*this);
                *result ^= range;
                return result;
            }

            // reverse
            virtual AudioBase * operator!(){
                Audio<sample_type, 2> * result = new Audio<sample_type, 2>(*this);
                std::reverse(result->data.begin(), result->data.end());
                return result;
            }
            
            // equality
            virtual bool operator==(const AudioBase & audio){
                if(this == &audio)
                    return true;

                if(typeid(audio) == typeid(Audio<sample_type, 2>)){
                    const Audio<sample_type, 2> & a = dynamic_cast<const Audio<sample_type, 2>&>(audio);
                    if(this->AudioBase::sampleRate == a.AudioBase::sampleRate 
                    && this->AudioBase::filename == a.AudioBase::filename){
                        for(int i = 0; i<this->data.size(); ++i){
                            if(this->data[i].first != a.data[i].first && this->data[i].second != a.data[i].second){
                                return false;
                            }
                        }
                    }
                    return true;
                }
                return false;
            }
            virtual bool operator!=(const AudioBase & audio){
                return !(*this==audio);
            }

            // METHODS

            // add samples over range
            virtual AudioBase * radd(const AudioBase & audio, const std::pair<float, float> range1, const std::pair<float, float> range2) {
                /*
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                if(typeid(audio) == typeid(Audio<sample_type, channel>)){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    if(range1.first*this->AudioBase::sampleRate >= 0 
                    && range1.second*this->AudioBase::sampleRate+1 < result->dataPieces()){
                        for(int i=range1.first*this->AudioBase::sampleRate; i<(int)(range1.second*this->AudioBase::sampleRate)+1; ++i){
                            result->data[i] += a.data[i];
                            result->data[i] = result->data[i] > result->cutoff ? result->cutoff : result->data[i];
                        }
                    }
                }
                return result;
                */
            }

            // root-mean-square of audio
            virtual float rms() {
                /*
                std::vector<float> v;
                for(sample_type s : this->data){
                    v.push_back(s);
                }
                for(float & s : v){
                    s *= s;
                }
                float result = std::accumulate(v.begin(), v.end(), 0);
                return sqrt(result/v.size());
                */
            }

            // normalise over range
            virtual AudioBase * norm(std::pair<float, float> requiredRMS) {
                /*
                Audio<sample_type, channel> * result = new Audio<sample_type, channel>(*this);
                Normalise<sample_type, channel> n(requiredRMS.first, result->rms());
                std::transform(result->data.begin(), result->data.end(), result->data.begin(), n);
                return result;
                */
            }

            // fade in 
            virtual AudioBase * fadeIn() {
                
            }

            // fade out
            virtual AudioBase * fadeOut() {
                
            }
 
   };
   
}
#endif