#include "SoundFX.h"

// SoundFX()
//
// Class constructor.  Sets up OpenAL and loads all sound effect files into
// memory.

SoundFX::SoundFX() {
    DIR *SFXDir;
    struct dirent *de;
    string files[256];

    SFXDir = opendir( "./sounds/" );
    Num_of_SFX = 0;
    while ( de = readdir( SFXDir ) ) {
        if ( strstr( de->d_name, ".wav" ) ) {
            files[Num_of_SFX] = string( de->d_name );
            Num_of_SFX++;
        }
    }
    closedir( SFXDir );

    // Init buffers and sources.
    Buffers = new ALuint[Num_of_SFX];
    Sources = new ALuint[Num_of_SFX];
    alGenBuffers( Num_of_SFX, Buffers );
    alGenSources( Num_of_SFX, Sources );

    // Load sound files.
    ALbyte filename[80];
    ALenum format;
    ALvoid *data;
    ALboolean loop;
    ALsizei size, freq;

    for ( int i = 0; i < Num_of_SFX; i++ ) {
        sprintf( filename, "./sounds/%s", files[i].c_str() );

        alutLoadWAVFile( filename, &format, &data, &size, &freq, &loop );
        alBufferData( Buffers[i], format, data, size, freq );

        File_Hash[ files[i] ] = i;  // Create name to index map.
        SetSFX( files[i], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AL_FALSE );
    }

    SetSFXListener( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

// ~SoundFX()
//
// Cleans up OpenAL buffer information.

SoundFX::~SoundFX() {
    alDeleteBuffers( Num_of_SFX, Buffers );
    alDeleteSources( Num_of_SFX, Sources );
}

// PlaySFX()
//
// Plays a specified sound file.  Accepts a file name (not the complete path).

void SoundFX::PlaySFX( string sfx ) {
    alSourcePlay( Sources[ File_Hash[sfx] ] );
}

// SetSFX
//
// Sets the location, velocity, and repeat bit for a given file name.  Takes
// eight parameters: The name of the wav file; X, Y, and Z locational
// coordinates; X, Y, and Z velocity values; and a boolean representing whether
// or not the sound should be looped.

void SoundFX::SetSFX( string sfx, 
                      ALfloat PosX, ALfloat PosY, ALfloat PosZ, 
                      ALfloat VelX, ALfloat VelY, ALfloat VelZ, 
                      ALboolean repeat ) {

    ALfloat SourcePos[3] = { PosX, PosY, PosZ };
    ALfloat SourceVel[3] = { VelX, VelY, VelZ };
    int index = File_Hash[sfx];
              
    alSourcei ( Sources[index], AL_BUFFER,   Buffers[index] );
    alSourcef ( Sources[index], AL_PITCH,    1.0f      );
    alSourcef ( Sources[index], AL_GAIN,     1.0f      );
    alSourcefv( Sources[index], AL_POSITION, SourcePos );
    alSourcefv( Sources[index], AL_VELOCITY, SourceVel );
    alSourcei ( Sources[index], AL_LOOPING,  repeat    );
}

// SetSFXListener()
//
// Sets the position and velocity of the listener.  Takes six parameters.  The
// first three represent the listener's X, Y, and Z position, and the second
// three represent the relative X, Y, and Z velocity.

void SoundFX::SetSFXListener( ALfloat PosX, ALfloat PosY, ALfloat PosZ, 
                              ALfloat VelX, ALfloat VelY, ALfloat VelZ ) {

    ALfloat ListenerPos[3] = { PosX, PosY, PosZ };
    ALfloat ListenerVel[3] = { VelX, VelY, VelZ };
    ALfloat ListenerOri[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

    alListenerfv( AL_POSITION,    ListenerPos );
    alListenerfv( AL_VELOCITY,    ListenerVel );
    alListenerfv( AL_ORIENTATION, ListenerOri );
}
