#include <string.h>

#include "sound.h"

Sound::Sound()
{
    alGenBuffers(2, MusicBuffers);
    alGenSources(1, &MusicSource);
    alSource3f (MusicSource, AL_POSITION, 0.0f, 0.0f, 0.0f);

    /////////////// Read in sfx and load them into memory //

    // Read in sound file names.
    DIR *SFXDir;
    struct dirent *de;
    string files[256]; //delete this after object construction?

    SFXDir = opendir( "./sounds/" );
    numofSFX = 0;
    while ( de = readdir( SFXDir ) ) {
        if ( strstr( de->d_name, ".wav" ) ) {
            files[numofSFX] = string(de->d_name);
            numofSFX++;
        }
    }
    closedir( SFXDir );

    // Init the buffers and sources.
    SFXBuffers = new ALuint[numofSFX];
    SFXSources = new ALuint[numofSFX];
    alGenBuffers(numofSFX, SFXBuffers);
    alGenSources(numofSFX, SFXSources);

    // Load the sound files into memory.
    ALbyte filename[80];
    ALenum format;
    ALvoid *data;
    ALboolean loop;
    ALsizei size, freq;
    for (int i = 0 ; i < numofSFX ; i++)
    {
        sprintf( filename, "./sounds/%s", files[i].c_str() );

        alutLoadWAVFile( filename, &format, &data, &size, &freq, &loop );
        alBufferData( SFXBuffers[i], format, data, size, freq );

        SetSFX( i, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AL_FALSE );

        sfxHash[ files[i] ] = i; // Create name to index map.

        cout << "Loaded sound '" << filename << "' key '" << files[i] << "'\n";

        //alutUnloadWAV(format, data, size, freq);
    }

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

void Sound::PlaySFX(string sfx)
{
    alSourcePlay (SFXSources[ sfxHash[sfx] ]);
}

void Sound::PlaySong(const char* filePath, bool rep)
{
    repeat = rep;
    //open binary file and set up the ov_open function for reading
    musicFile = fopen (filePath, "rb");
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
        bytes = ov_read(
                    &oggFile,
                    soundbuffer + size,
                    BUFFER_SIZE - size,
                    0, 2, 1, &section
                );
        size += bytes;
    }
    alBufferData(
        MusicBuffers[0],
        MusicFormat,
        soundbuffer,
        size,
        oggInfo->rate
    );
    size = 0;

    // Fill the second music buffer from more of the file data
    while (size < BUFFER_SIZE)
    {
        bytes = ov_read(
                    &oggFile,
                    soundbuffer + size,
                    BUFFER_SIZE - size,
                    0, 2, 1, &section
                );
        size += bytes;
    }
    alBufferData(
        MusicBuffers[1],
        MusicFormat,
        soundbuffer,
        size,
        oggInfo->rate
    );
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
    //toggle off playing so that the update function won't run.
    playing = !playing;
}

void Sound::Update()
{
    if (!playing) //only check and fill buffers if the song is playing
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
            bytes = ov_read(
                        &oggFile,
                        soundbuffer + size,
                        BUFFER_SIZE - size,
                        0, 2, 1, &section
                    );
/*
            if (bytes == 0 && repeat)
                ov_raw_seek(&oggFile, 1);
            else if (bytes == 0 && !repeat)
                StopSong();
*/
            size += bytes;
        }


        alBufferData(
            tempbuffer,
            MusicFormat,
            soundbuffer,
            size,
            oggInfo->rate
        );

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
