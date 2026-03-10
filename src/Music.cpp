#include "Music.h"

// Music()
//
// Class constructor.
Music::Music() {
  Playing = false;
  Repeat  = false;
  initialized_ = false;
}

// ~Music()
//
// Class destructor.
Music::~Music() {
  unload();
}

// load()
//
// Sets up OpenAL and initialized state flags.
void Music::load() {
  if (initialized_)
    return;

  alGenBuffers( 2, Buffers );
  ALenum errorCode = alGetError();
  if (errorCode != AL_NO_ERROR) {
    PRINT_ERROR("Could not generate buffers; AL error code: '%d'.\n", errorCode);
    return;
  }

  alGenSources( 1, &Source );
  errorCode = alGetError();
  if (errorCode != AL_NO_ERROR) {
    PRINT_ERROR("Could not generate sources; AL error code: '%d'.\n", errorCode);
    return;
  }

  alSource3f( Source, AL_POSITION, 0.0f, 0.0f, 0.0f );
  errorCode = alGetError();
  if (errorCode != AL_NO_ERROR) {
    PRINT_ERROR("Could not set source position; AL error code: '%d'.\n", errorCode);
    return;
  }

  initialized_ = true;

  SetMusicListener( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

// unload()
//
// Cleans up OpenAL buffer information.
void Music::unload() {
  if (!initialized_)
    return;

  PRINT_DEBUG("Cleaning up...\n");
  StopSong();

  alDeleteBuffers( 2, Buffers );
  ALenum errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not delete buffers; AL error code: '%d'.\n", errorCode);

  alDeleteSources( 1, &Source );
  errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not delete sources; AL error code: '%d'.\n", errorCode);

  initialized_ = false;
}

// PlaySong()
//
// Plays a specified song file.  Accepts a file path to the song and
// a boolean representing whether or not the song should be looped.
void Music::PlaySong(const char* file_path, const bool &rep) {
  if (!initialized_)
    return;

  PRINT_DEBUG("Playing Song '%s'...\n", file_path);
  // Open binary file and set up the ov_open function for reading.
  Music_File = fopen( file_path, "rb" );

  if (Music_File == NULL)
    return;

  Repeat = rep;

  ov_open( Music_File, &Ogg_File, NULL, 0 );

  // Get format infomation from the info struct.
  Ogg_Info = ov_info( &Ogg_File, -1 );
  if ( Ogg_Info->channels == 1 )
    Format = AL_FORMAT_MONO16;
  else
    Format = AL_FORMAT_STEREO16;

  // Fill sound buffers.
  for ( int i = 0; i < 2; i++ ) {
    Size = 0;
    while ( Size < BUFFER_SIZE ) {
      Bytes = ov_read(
        &Ogg_File,
        Sound_Buffer + Size,
        BUFFER_SIZE - Size,
        0, 2, 1, &Section
      );
      Size += Bytes;
    }
    alBufferData( Buffers[i], Format, Sound_Buffer, Size, Ogg_Info->rate );
    ALenum errorCode = alGetError();
    if (errorCode != AL_NO_ERROR)
      PRINT_ERROR("Could setup buffers; AL error code: '%d'.\n", errorCode);
  }
  Size = 0;

  alSourceQueueBuffers( Source, 2, Buffers );
  ALenum errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could queue buffers; AL error code: '%d'.\n", errorCode);

  alSourcePlay( Source );
  errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not play source; AL error code: '%d'.\n", errorCode);

  Playing = true;
}

// StopSong()
//
// Halts the currently playing song, closes the Ogg file, and unqueues the
// currently allocated song buffers.  Takes no parameters.
void Music::StopSong() {
  if (!initialized_ || !Playing)
    return;

  alSourceStop( Source );  // Stops OpenAL.
  ALenum errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not stop source; AL error code: '%d'.\n", errorCode);

  ov_clear( &Ogg_File );   // Closes the music file.

  // Tears down each buffer in the queue.
  ALuint temp_buff;
  alSourceUnqueueBuffers( Source, 1, &temp_buff );
  errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not unqueue buffers; AL error code: '%d'.\n", errorCode);

  alSourceUnqueueBuffers( Source, 1, &temp_buff );
  errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not unqueue buffers; AL error code: '%d'.\n", errorCode);

  Playing = false;
  Repeat  = false;
}

// PauseSong()
//
// Pauses the currently playing song.  Resumes play if the current song is
// already paused.  Takes no parameters.
void Music::PauseSong() {
  if (!initialized_ || !Playing)
    return;

  alSourcePause( Source );
  ALenum errorCode = alGetError();
  if (errorCode != AL_NO_ERROR)
    PRINT_ERROR("Could not pause source; AL error code: '%d'.\n", errorCode);

  Playing = !Playing;
}

// Update()
//
// Fills up the sound buffers with the next data from the currently playing
// song file.  Takes no parameters.
void Music::Update() {
  if (!initialized_ || !Playing)
    return;

  // Make sure to restart the song if we have a break in the data stream.
  alGetSourcei( Source, AL_SOURCE_STATE, &State );
  if ( State != AL_PLAYING )
    alSourcePlay( Source );

  // Fill any empty buffers and queue them up.
  alGetSourcei( Source, AL_BUFFERS_PROCESSED, &Processed );
  if ( Processed > 0 ) {
    alSourceUnqueueBuffers( Source, 1, &Temp_Buffer );

    while ( Size < BUFFER_SIZE ) {
      Bytes = ov_read(
        &Ogg_File,
        Sound_Buffer + Size,
        BUFFER_SIZE - Size,
        0, 2, 1, &Section
      );

      if ( Bytes == 0 && Repeat )
        ov_raw_seek( &Ogg_File, 1 );
      else if ( Bytes == 0 && !Repeat )
        StopSong();

      Size += Bytes;
    }

    alBufferData( Temp_Buffer, Format, Sound_Buffer, Size, Ogg_Info->rate );
    alSourceQueueBuffers( Source, 1, &Temp_Buffer );
    Size = 0;
  }
}

// SetMusicListener()
//
// Sets the position and velocity of the listener.  Takes six parameters.  The
// first three represent the listener's X, Y, and Z position, and the second
// three represent the relative X, Y, and Z velocity.
void Music::SetMusicListener(
    const ALfloat &PosX, const ALfloat &PosY, const ALfloat &PosZ,
    const ALfloat &VelX, const ALfloat &VelY, const ALfloat &VelZ
) {
  if (!initialized_)
    return;

  ALfloat ListenerPos[3] = { PosX, PosY, PosZ };
  ALfloat ListenerVel[3] = { VelX, VelY, VelZ };
  ALfloat ListenerOri[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

  alListenerfv( AL_POSITION, ListenerPos );
  ALenum errorcode = alGetError();
  if (errorcode != AL_NO_ERROR)
    PRINT_ERROR("Could not set listener position; al error code: '%d'.\n", errorcode);

  alListenerfv( AL_VELOCITY, ListenerVel );
  errorcode = alGetError();
  if (errorcode != AL_NO_ERROR)
    PRINT_ERROR("Could not set listener velocity; al error code: '%d'.\n", errorcode);

  alListenerfv( AL_ORIENTATION, ListenerOri );
  errorcode = alGetError();
  if (errorcode != AL_NO_ERROR)
    PRINT_ERROR("Could not set listener orientation; al error code: '%d'.\n", errorcode);
}
