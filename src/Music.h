#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <AL/al.h>
#include <vorbis/vorbisfile.h>

#include "constants.h"

#define BUFFER_SIZE   65536

class Music {
  public:
    Music();
    ~Music();

    void SetMusicListener(
      ALfloat, ALfloat, ALfloat,
      ALfloat, ALfloat, ALfloat
    );

    void PlaySong(const char*, bool);
    void Update();
    void StopSong();
    void PauseSong();

  private:
    ALuint Source;
    ALenum Format;
    ALint  State;
    ALuint Buffers[2];
    ALuint Temp_Buffer;

    // Flags for setting states.
    bool Playing;
    bool Repeat;

    // File decoding variables.
    FILE *Music_File;
    vorbis_info *Ogg_Info;
    OggVorbis_File Ogg_File;

    // variables for loop streaming.
    int Processed, Size, Section, Bytes;
    char Sound_Buffer[BUFFER_SIZE];
};

#endif
