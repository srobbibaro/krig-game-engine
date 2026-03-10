#include <cstdio>
#include <cstring>

#include "SoundFX.h"

// SoundFX()
//
// Class constructor.
SoundFX::SoundFX() {
  Buffers = NULL;
  Sources = NULL;
  Num_of_SFX = 0;
}

// ~SoundFX()
//
// Class destructor.
SoundFX::~SoundFX() {
  unload();
}

// load()
//
// Sets up OpenAL and loads all sound effect files into
// memory from the sounds directory.
void SoundFX::load() {
  if (Buffers != NULL || Sources != NULL) {
    PRINT_ERROR("Sounds already loaded; no sounds will be loaded.\n");
    return;
  }

  Buffers = NULL;
  Sources = NULL;

  DIR *SFXDir = opendir( "./sounds/" );

  if (SFXDir == NULL) {
    PRINT_DEBUG_LVL(2, "'sounds' directory not present; no sounds will be loaded.\n");
    return;
  }

  dirent *de;
  std::string files[256];

  if (SFXDir != NULL) {
    Num_of_SFX = 0;
    while ((de = readdir(SFXDir)) != NULL) {
      if ( std::strstr( de->d_name, ".ogg" ) ) {
        files[Num_of_SFX] = std::string( de->d_name );
        Num_of_SFX++;
      }
    }
    closedir( SFXDir );
  }

  if (Num_of_SFX < 1) {
    PRINT_DEBUG_LVL(2, "No sound files in the ogg format present in 'sounds' directory; no sounds will be loaded.\n");
    return;
  }

  // Init buffers and sources.
  Buffers = new ALuint[Num_of_SFX];
  Sources = new ALuint[Num_of_SFX];
  alGenBuffers( Num_of_SFX, Buffers );
  alGenSources( Num_of_SFX, Sources );
  char Sound_Buffer[BUFFER_SIZE];

  // Load sound files.
  char filename[80];

  for ( int i = 0; i < Num_of_SFX; i++ ) {
    // load model file into model storage //
    sprintf( filename, "./sounds/%s", files[i].c_str() );
    PRINT_DEBUG("Loading sound file '%s'...\n", filename);

    FILE *Music_File = fopen( filename, "rb" );

    if (Music_File == NULL) {
      PRINT_ERROR("Could not open sound file: '%s'...\n", filename);
      continue;
    }

    OggVorbis_File Ogg_File;
    ov_open( Music_File, &Ogg_File, NULL, 0 );

    // Get format infomation from the info struct.
    ALenum Format;
    vorbis_info *Ogg_Info = ov_info( &Ogg_File, -1 );

    if ( Ogg_Info->channels == 1 )
      Format = AL_FORMAT_MONO16;
    else
      Format = AL_FORMAT_STEREO16;

    // Fill sound buffers.
    int Size = 0;
    int Section = 0;
    while ( Size < BUFFER_SIZE ) {
      int Bytes = ov_read(
        &Ogg_File,
        Sound_Buffer + Size,
        BUFFER_SIZE - Size,
        0, 2, 1, &Section
      );

      if (Bytes == 0)
        break;

      Size += Bytes;
    }
    alBufferData( Buffers[i], Format, Sound_Buffer, Size, Ogg_Info->rate );

    ALenum errorCode = alGetError();
    if (errorCode != AL_NO_ERROR)
      PRINT_ERROR("Could not load sound buffer data; AL error code: '%d'.\n", errorCode);

    File_Hash[ files[i] ] = i;  // Create name to index map.
    SetSFX( files[i], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AL_FALSE );

    ov_clear(&Ogg_File);
  }

  SetSFXListener( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

// unload()
//
// Cleans up OpenAL buffer information.
void SoundFX::unload() {
  PRINT_DEBUG("Cleaning-up sounds...\n");
  if (Buffers != NULL) {
    alDeleteBuffers( Num_of_SFX, Buffers );
    delete[] Buffers;
    Buffers = NULL;
  }

  if (Sources != NULL) {
    alDeleteSources( Num_of_SFX, Sources );
    delete[] Sources;
    Sources = NULL;
  }

  PRINT_DEBUG("Clean-up complete.\n");
}

// PlaySFX()
//
// Plays a specified sound file.  Accepts a file name (not the complete path).
void SoundFX::PlaySFX( const std::string &sfx ) {
  if (Sources == NULL)
    return;

  PRINT_DEBUG_LVL(1, "Playing sound (%s)\n", sfx.c_str());
  alSourcePlay( Sources[ File_Hash[sfx] ] );

  ALenum errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not play sound; AL error code: '%d'.\n", errorCode);
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
