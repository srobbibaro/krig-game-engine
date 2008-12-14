#ifndef _SOUND_H_
#define _SOUND_H_

#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>
#include "constants.h"
#include <map>
#include <dirent.h>

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

        // Maps sound effect file names to array indexes.
        map <string, int> sfxHash;
          
    public:
        Sound();
        ~Sound();
        
        void PlaySong(const char*, bool); //functions for sound playing
        void Update();
        void StopSong();
        void PauseSong();
        
        void PlaySFX(string);          //functions for sound effects playing
        void SetSFX(int, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALboolean);
        
        void SetCamera(ALfloat, ALfloat, ALfloat, ALfloat, ALfloat, ALfloat);
};


#endif
