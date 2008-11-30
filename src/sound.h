#ifndef _SOUND_H_
#define _SOUND_H_

#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>
#include "constants.h"

class Sound
{
    private:
        //variables for song streaming...
        ALuint MusicSource;
        ALenum MusicFormat;
        ALint MusicState;
        ALuint MusicBuffers[2];
        ALuint tempbuffer;
        
        bool playing;   //Flags
        bool repeat;

        FILE *musicFile;        //file decoding variables
        vorbis_info *oggInfo;
        OggVorbis_File oggFile;

        int processed, size, section, bytes; //variables for loop streaming
        char soundbuffer[BUFFER_SIZE];
        
        ALuint *SFXSources;     //Data for loading in the sound effects
        ALuint *SFXBuffers;
        int numofSFX;
          
    public:
        Sound(char *);
        ~Sound();
        
        void PlaySong(const char*, bool); //functions for sound playing
        void Update();
        void StopSong();
        void PauseSong();
        
        void PlaySFX(int);          //functions for sound effects playing
        void SetSFX(int, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALboolean);
        
        void SetCamera(ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat);
};


#endif
