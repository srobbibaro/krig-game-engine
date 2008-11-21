#include "sound.h"
#include <AL/alut.h>

Sound::Sound(char *music, char *sfx)
{
    FILE *SFXFile, *SongFile;   //file handles for song and sfx lists

    ///////////// Read in song list ////////////////////////
    SongFile = fopen (music, "rt");
        
        fscanf (SongFile, "%d", &numofSongs);
        songs = new char*[numofSongs];
        for (int j=0; j < numofSongs; j++)
            songs[j] = new char[80];
            
        for (int i=0; i < numofSongs; i++)
            fscanf (SongFile, "%s", songs[i]);
    
    fclose(SongFile);    

    alGenBuffers(2, MusicBuffers);
    alGenSources(1, &MusicSource);
    
    alSource3f (MusicSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
    ////////////////////////////////////////////////////////  
    
    /////////////// Read in sfx and load them into memory //
    char SFXName[80];
    ALbyte *p;
    ALenum format;
    ALsizei size;
    ALvoid *data;
    ALsizei freq;
    ALboolean loop;    
    
    SFXFile = fopen (sfx, "rt");
        fscanf (SFXFile, "%d", &numofSFX);
        
        SFXBuffers = new ALuint[numofSFX];
        SFXSources = new ALuint[numofSFX];
        alGenBuffers(numofSFX, SFXBuffers);
        alGenSources(numofSFX, SFXSources);
        
        
        for (int k=0 ; k < numofSFX ; k++)
        {
            fscanf (SFXFile, "%s", SFXName);
            p = SFXName;
            
            alutLoadWAVFile (p, &format, &data, &size, &freq, &loop);
            alBufferData(SFXBuffers[k], format, data, size, freq);
                    
            SetSFX(k, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AL_FALSE);
            //alutUnloadWAV(format, data, size, freq);
        }
        
    fclose(SFXFile);
    ////////////////////////////////////////////////////////
    
    SetCamera (0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    playing = false;
}

Sound::~Sound()
{
    alDeleteBuffers(numofSFX, SFXBuffers);
    alDeleteSources(numofSFX, SFXSources);
    alDeleteBuffers(2, MusicBuffers);
    alDeleteSources(1, &MusicSource);
}

void Sound::PlaySFX(int index)
{
    alSourcePlay (SFXSources[index]);
}

void Sound::PlaySong(int songindex, bool rep)
{
    currentSong = songindex;
    repeat = rep;
    //open binary file and set up the ov_open function for reading
    musicFile = fopen (songs[songindex], "rb");
    ov_open(musicFile, &oggFile, NULL, 0);

    oggInfo = ov_info(&oggFile, -1);    //fill info struct and set format
    if (oggInfo->channels == 1)
        MusicFormat = AL_FORMAT_MONO16;
    else
        MusicFormat = AL_FORMAT_STEREO16;
        
    // Fill the first music buffer from the file
    size = 0;
    while (size < BUFFER_SIZE)
    {
        bytes = ov_read(&oggFile, soundbuffer + size, BUFFER_SIZE - size, 0, 2, 1, &section);
        size += bytes;
    }
    alBufferData(MusicBuffers[0], MusicFormat, soundbuffer, size, oggInfo->rate);
    size = 0;
    
    // Fill the second music buffer from more of the file data
    while (size < BUFFER_SIZE)
    {
        bytes = ov_read(&oggFile, soundbuffer + size, BUFFER_SIZE - size, 0, 2, 1, &section);
        size += bytes;
    }  
    alBufferData(MusicBuffers[1], MusicFormat, soundbuffer, size, oggInfo->rate);
    size = 0;
    
    alSourceQueueBuffers(MusicSource, 2, MusicBuffers);
    
    alSourcePlay(MusicSource);
    playing = true;
}

void Sound::StopSong()
{
    alSourceStop(MusicSource);  //stop OpenAL
    ov_clear(&oggFile);         //close the ogg file

    ALuint tempbuff;            //tear down the buffers in place
    alSourceUnqueueBuffers(MusicSource, 1, &tempbuff);
    alSourceUnqueueBuffers(MusicSource, 1, &tempbuff);
    playing = false;
}

void Sound::PauseSong()
{
    alSourcePause(MusicSource);
    playing = !playing;       //toggle off playing so that the update function won't run.
}

void Sound::Update()
{
    if (!playing)       //only check and fill buffers if the song is playing
        return;
        
    //if a break in the data stream, the song will start playing again
    alGetSourcei(MusicSource, AL_SOURCE_STATE, &MusicState);
    if (MusicState != AL_PLAYING)
    {
        alSourcePlay(MusicSource);
    }
    
    //Check and fill any empty buffers and queue them up
    alGetSourcei(MusicSource, AL_BUFFERS_PROCESSED, &processed);
    if (processed > 0)
    {
        alSourceUnqueueBuffers(MusicSource, 1, &tempbuffer);
            
        while (size < BUFFER_SIZE)
        {
            bytes = ov_read(&oggFile, soundbuffer + size, BUFFER_SIZE - size, 0, 2, 1, &section);
            if (bytes == 0 && repeat)
                ov_raw_seek(&oggFile, 1);
            else if (bytes == 0 && !repeat)
                StopSong();

            size += bytes;
        }
            
                          
        alBufferData(tempbuffer, MusicFormat, soundbuffer, size, oggInfo->rate);
         
        alSourceQueueBuffers(MusicSource, 1, &tempbuffer);
        size = 0;
        
    }
}

void Sound::SetSFX(int index, ALfloat PosX, ALfloat PosY, ALfloat PosZ, ALfloat VelX, ALfloat VelY, ALfloat VelZ, ALboolean LoopIt)
{
    ALfloat SourcePos[3] = { PosX, PosY, PosZ };
    ALfloat SourceVel[3] = { VelX, VelY, VelZ };
              
    alSourcei (SFXSources[index], AL_BUFFER, SFXBuffers[index]);
    alSourcef (SFXSources[index], AL_PITCH, 1.0f        );
    alSourcef (SFXSources[index], AL_GAIN,  1.0f        );
    alSourcefv(SFXSources[index], AL_POSITION, SourcePos);
    alSourcefv(SFXSources[index], AL_VELOCITY, SourceVel);
    alSourcei (SFXSources[index], AL_LOOPING, LoopIt  );
}

void Sound::SetCamera (ALfloat PosX, ALfloat PosY, ALfloat PosZ, ALfloat VelX, ALfloat VelY, ALfloat VelZ)
{
    ALfloat ListenerPos[3] = { PosX, PosY, PosZ };
    ALfloat ListenerVel[3] = { VelX, VelY, VelZ };
    ALfloat ListenerOri[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
}
