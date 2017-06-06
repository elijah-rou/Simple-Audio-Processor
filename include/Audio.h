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

            Audio(const Audio<sample_type, channel> & audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename){
                this->data = std::vector<sample_type>(audio.data.size());
                copy(audio.data.begin(), audio.data.end(), audio.data.size());
            }
            Audio(Audio<sample_type, channel> && audio)
            :   AudioBase(audio.AudioBase::sampleRate, audio.AudioBase::filename),
                data(audio.data){
                audio.sampleRate = 0;
                audio.data = NULL;
            }

            Audio<sample_type, channel> & operator=(const Audio<sample_type, channel> & audio){
                if(this != &audio){
                    this->AudioBase::sampleRate = audio.AudioBase::sampleRate;
                    this->AudioBase::filename = audio.AudioBase::filename;
                    this->data = std::vector<sample_type>(audio.data.size());
                    copy(audio.data.begin(), audio.data.end(), audio.data.size());
                }
                return *this;
            }

            Audio<sample_type, channel> & operator=(Audio<sample_type, channel> & audio){
                if(this != &audio){
                    this->AudioBase::sampleRate = audio.AudioBase::sampleRate;
                    this->AudioBase::filename = std::move(audio.AudioBase::filename);
                    this->data = audio.data;
                    audio.sampleRate = 0;
                    audio.data = NULL;
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

            virtual AudioBase & operator+=(const AudioBase & audio){

            }

            // add two audio clips
            virtual AudioBase & add(const AudioBase * audio) {
                
            }

            // remove samples over range
            virtual AudioBase & cut(int start, int end) {
                
            }

            // add samples over range
            virtual AudioBase & radd(const AudioBase * audio, int start, int end) {

            }

            // concatenate audio clips
            virtual AudioBase & cat(const AudioBase * audio) {
                
            }

            // volume factor over range
            virtual AudioBase & v(int start, int end) {
                
            }

            // reverse audio
            virtual Audio<sample_type, channel> & rev() {
                //Audio<sample_type, channel> * a = new Audio<sample_type, channel>(*this);
                //return *a; 
                //return new Audio<sample_type, channel>(*this);
            }

            // root-mean-square of audio
            virtual AudioBase & rms() {
                
            }

            // normalise over range
            virtual AudioBase & norm(float start, float end) {
                
            }

            // fade in 
            virtual AudioBase & fadeIn() {
                
            }

            // fade out
            virtual AudioBase & fadeOut() {
                
            }
    };

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

            Audio & operator=(const Audio & audio){
                std::cout << "Copy assignment" <<  std::endl;
                if(this != &audio){
                    this->AudioBase::sampleRate = audio.AudioBase::sampleRate;
                    this->AudioBase::filename = audio.AudioBase::filename;
                    this->data = std::vector<sample_type>(audio.data.size());
                    copy(audio.data.begin(), audio.data.end(), audio.data.size());
                }
                return *this;
            }

            Audio & operator=(Audio & audio){
                if(this != &audio){
                    this->AudioBase::sampleRate = audio.AudioBase::sampleRate;
                    this->AudioBase::filename = std::move(audio.AudioBase::filename);
                    this->data = audio.data;
                    audio.sampleRate = 0;
                    audio.data = NULL;
                }
                return *this;
            }

            virtual int dataPieces(){
                return data.size()*2;
            }
            // write to output file
            virtual void write(std::string outputFile) {

            }
            virtual AudioBase & operator+=(const AudioBase & audio){

            }

            // add two audio clips
            virtual AudioBase & add(const AudioBase * audio) {
                
            }

            // remove samples over range
            virtual AudioBase & cut(int start, int end) {
                
            }

            // add samples over range
            virtual AudioBase & radd(const AudioBase * audio, int start, int end) {

            }

            // concatenate audio clips
            virtual AudioBase & cat(const AudioBase * audio) {
                
            }

            // volume factor over range
            virtual AudioBase & v(int start, int end) {
                
            }

            // reverse audio
            virtual AudioBase & rev() {
                 
            }

            // root-mean-square of audio
            virtual AudioBase & rms() {
                
            }

            // normalise over range
            virtual AudioBase & norm(float start, float end) {
                
            }

            // fade in 
            virtual AudioBase & fadeIn() {
                
            }

            // fade out
            virtual AudioBase & fadeOut() {
                
            }
    };
}
#endif