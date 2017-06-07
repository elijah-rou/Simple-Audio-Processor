#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <fstream>
#include "../include/AudioBase.h"

namespace RSSELI007{
   
    template <typename sample_type, int channel=1> class Audio : public AudioBase{
        private:
            std::vector<sample_type> data;
        public:
            Audio() = delete;
            Audio(std::string filename, int samplePsec)
            :   AudioBase(samplePsec, filename){
                std::ifstream file(AudioBase::filename, std::ios::binary);
                std::cout << "Constructing audio " << AudioBase::filename << std::endl;
                if(file.is_open()){
                    file.seekg(0, std::ios::end);
                    size_t size = file.tellg();
                    std::cout << size << " bits" << std::endl;
                    file.seekg(0, std::ios::beg);
                    sample_type * temp = new sample_type[size];
                    file.read(reinterpret_cast<char *>(temp), size);
                    for(int i=0; i<size; ++i){
                        data.push_back(temp[i]);
                    }
                    delete [] temp;
                }
                std::cout << "Completed construction" << std::endl;
            }

            ~Audio(){
                this->AudioBase::sampleRate = 0;
            }
            
            Audio(const AudioBase & audio)
            :   AudioBase(audio.getSampleRate(), audio.getFilename()){
                std::cout << "Constructing copy " << AudioBase::filename << std::endl;
                const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                //this->data = std::vector<sample_type>(a.data.size());
                //copy(a.data.begin(), a.data.end(), a.data.size())
                for(sample_type s : a.data){
                    this->data.push_back(s);
                }
                std::cout << "Completed copy" << std::endl;
            }

            /*
            Audio(const Audio<sample_type, channel> & audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename){
                this->data = std::vector<sample_type>(audio.data.size());
                //copy(audio.data.begin(), audio.data.end(), audio.data.size())
                for(sample_type s : audio.data){
                    this->data.push_back(s);
                }
            }
            */

            Audio(Audio<sample_type, channel> && audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename),
                data(std::move(audio.data)){
                audio.sampleRate = 0;
                delete audio;
            }

            virtual AudioBase & operator=(const AudioBase & audio){
                std::cout << "Copy assignment" << std::endl;
                if(this != &audio){
                    const Audio<sample_type, channel> & a = dynamic_cast<const Audio<sample_type, channel>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = a.AudioBase::filename;
                    this->data = std::vector<sample_type>(a.data.size());
                    //copy(a.data.begin(), a.data.end(), a.data.size());
                    for(sample_type s : a.data){
                        this->data.push_back(s);
                    }
                    std::cout << "Completed copy assignment" << std::endl;
                }
                return *this;
            }

            virtual AudioBase & operator=(AudioBase && audio){
                if(this != &audio){
                    Audio<sample_type, channel> & a = dynamic_cast<Audio<sample_type, channel>&>(audio);
                    this->AudioBase::sampleRate = a.AudioBase::sampleRate;
                    this->AudioBase::filename = std::move(a.AudioBase::filename);
                    this->data = a.data;
                    a.sampleRate = 0;
                }
                return *this;
            }

            virtual int dataPieces(){
                return data.size();
            }

            // write to output file
            virtual void write(std::string outputFile) {
                std::cout << "Writing file to output/" << std::endl << std::endl;
                std::ofstream file("output/" + outputFile + ".raw", std::ios::binary);
                if(file.good()){
                    for(sample_type s : data){
                        //std::cout << s << std::endl;
                        file << s;
                    }
                }
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
                if(this != &audio){
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