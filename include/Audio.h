#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <string>
#include <fstream>

namespace RSSELI007{
   
    class AudioBase{
        protected:
            int bitrate;
            std::string filename;
        public:
            AudioBase() = delete;
            AudioBase(int bitpsec, std::string file) 
            :   bitrate(bitpsec),
                filename(file){}

            virtual ~AudioBase(){
                bitrate = 0;
                filename = "";
            }

    }; 
   
    template <typename sample_type, int channel=1> class Audio : AudioBase{
        private:
            std::vector<sample_type> data;
        public:
            Audio() = delete;

            Audio(std::string filename, int bitpsec)
            :   AudioBase(bitpsec, filename){
                std::fstream file("input/"+filename, std::ios::binary);
                if(file.good()){
                    file.seekg(0, std::ios::end);
                    size_t size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    file.read(data, size);
                }
            }

            ~Audio(){
                this->audio.bitrate = 0;
                this->audio.data = NULL;
            }

            Audio(const Audio & audio){
                this->bitrate = audio.bitrate;
                this->data = std::vector<sample_type>(audio.data.size());
                copy(audio.data.begin(), audio.data.end(), audio.data.size());
            }
            Audio(Audio && audio){
                this->bitrate = audio.bitrate;
                this->data = audio.data;
                audio.bitrate = 0;
                audio.data = NULL;
            }

            Audio & operator=(const Audio & audio){
                if(this != &audio){
                    this->data = std::vector<sample_type>(audio.data.size());
                    copy(audio.data.begin(), audio.data.end(), audio.data.size());
                }
                return *this;
            }

            Audio & operator=(Audio & audio){
                if(this != &audio){
                    this->bitrate = audio.bitrate;
                    this->data = audio.data;
                    audio.bitrate = 0;
                    audio.data = NULL;
                }
                return *this;
            }
    };

    template <typename sample_type> class Audio<sample_type, 2> : AudioBase(){
        private:
            std::vector<std::pair<sample_type, sample_type> > data;
        public:
            Audio(std::string filename, int bitpsec){
                this->bitrate = bitpsec;
                std::ifstream file("input/"+filename, std::ios::binary);
                if(file.good()){
                    file.seekg(0, std::ios::end);
                    size_t size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    sample_type * temp;
                    file.read(temp, size);
                    for(int i=0; i<size; i+=2){
                        std::pair<sample_type, sample_type> p(temp[i], temp[i+1]);
                        data.push_back(p);
                    }
                }
            }
    };
}
#endif