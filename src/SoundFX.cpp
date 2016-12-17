#include <cstdio>
#include <cstring>

#include "SoundFX.h"

// SoundFX()
//
// Class constructor.  Sets up OpenAL and loads all sound effect files into
// memory.
SoundFX::SoundFX() {
  Buffers = NULL;
  Sources = NULL;

  DIR *SFXDir = opendir( "./sounds/" );

  if (SFXDir == NULL) {
    PRINT_DEBUG_LVL(2, "'sounds' directory not present; no sounds will be loaded.\n");
    return;
  }

  dirent *de;
  std::string files[256];

  /* TODO: Instead of holding all of the sound effects in memory in wav format,
   * it would be great to switch to compressed audio files.  It would
   * drastically decrease the overall memory footprint.
   */
  if (SFXDir != NULL) {
    Num_of_SFX = 0;
    while ((de = readdir(SFXDir)) != NULL) {
      if ( std::strstr( de->d_name, ".wav" ) ) {
        files[Num_of_SFX] = std::string( de->d_name );
        Num_of_SFX++;
      }
    }
    closedir( SFXDir );
  }

  if (Num_of_SFX < 1) {
    PRINT_DEBUG_LVL(2, "No sound files present in 'sounds' directory; no sounds will be loaded.\n");
    return;
  }

  // Init buffers and sources.
  Buffers = new ALuint[Num_of_SFX];
  Sources = new ALuint[Num_of_SFX];
  alGenBuffers( Num_of_SFX, Buffers );
  alGenSources( Num_of_SFX, Sources );

  // Load sound files.
  char filename[80];

  for ( int i = 0; i < Num_of_SFX; i++ ) {
    sprintf( filename, "./sounds/%s", files[i].c_str() );

    Buffers[i] = alutCreateBufferFromFile(filename);

    File_Hash[ files[i] ] = i;  // Create name to index map.
    SetSFX( files[i], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AL_FALSE );
  }

  SetSFXListener( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

// ~SoundFX()
//
// Cleans up OpenAL buffer information.
SoundFX::~SoundFX() {
  if (Buffers != NULL) {
    alDeleteBuffers( Num_of_SFX, Buffers );
    delete[] Buffers;
  }

  if (Sources != NULL) {
    alDeleteSources( Num_of_SFX, Sources );
    delete[] Sources;
  }

  PRINT_DEBUG("Clean-up complete.\n");
}

// PlaySFX()
//
// Plays a specified sound file.  Accepts a file name (not the complete path).
void SoundFX::PlaySFX( const std::string &sfx ) {
  if (Sources == NULL)
    return;

  alSourcePlay( Sources[ File_Hash[sfx] ] );
}

// SetSFX
//
// Sets the location, velocity, and repeat bit for a given file name.  Takes
// eight parameters: The name of the wav file; X, Y, and Z locational
// coordinates; X, Y, and Z velocity values; and a boolean representing whether
// or not the sound should be looped.
void SoundFX::SetSFX(
    const std::string &sfx,
    const ALfloat &PosX, const ALfloat &PosY, const ALfloat &PosZ,
    const ALfloat &VelX, const ALfloat &VelY, const ALfloat &VelZ,
    const ALboolean &repeat
) {
  if (Buffers == NULL || Sources == NULL)
    return;

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
void SoundFX::SetSFXListener(
    const ALfloat &PosX, const ALfloat &PosY, const ALfloat &PosZ,
    const ALfloat &VelX, const ALfloat &VelY, const ALfloat &VelZ
) {
  ALfloat ListenerPos[3] = { PosX, PosY, PosZ };
  ALfloat ListenerVel[3] = { VelX, VelY, VelZ };
  ALfloat ListenerOri[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

  alListenerfv( AL_POSITION,    ListenerPos );
  alListenerfv( AL_VELOCITY,    ListenerVel );
  alListenerfv( AL_ORIENTATION, ListenerOri );
}
