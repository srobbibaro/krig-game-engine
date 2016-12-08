#ifndef _SOUNDFX_H_
#define _SOUNDFX_H_

#include <AL/al.h>
#include <AL/alut.h>
#include <map>
#include <dirent.h>
#include "constants.h"

class SoundFX {
  public:
    SoundFX();
    ~SoundFX();

    void PlaySFX(std::string);
    void SetSFX(
      std::string,
      ALfloat, ALfloat, ALfloat,
      ALfloat, ALfloat, ALfloat,
      ALboolean
    );

    void SetSFXListener(
      ALfloat, ALfloat, ALfloat,
      ALfloat, ALfloat, ALfloat
    );

  private:
    ALuint *Sources; // Data for loading in the sound effects
    ALuint *Buffers;

    int Num_of_SFX;
    std::map <std::string, int> File_Hash;
};

#endif
