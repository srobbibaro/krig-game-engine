#ifndef _SOUNDFX_H_
#define _SOUNDFX_H_

#include <AL/al.h>
#include <AL/alut.h>
#include <string>
#include <map>
#include <dirent.h>
#include "constants.h"

class SoundFX {
  public:
    SoundFX();
    ~SoundFX();

    void PlaySFX(const std::string&);
    void SetSFX(
      const std::string&,
      const ALfloat&, const ALfloat&, const ALfloat&,
      const ALfloat&, const ALfloat&, const ALfloat&,
      const ALboolean&
    );

    void SetSFXListener(
      const ALfloat&, const ALfloat&, const ALfloat&,
      const ALfloat&, const ALfloat&, const ALfloat&
    );

  private:
    ALuint *Sources; // Data for loading in the sound effects
    ALuint *Buffers;

    int Num_of_SFX;
    std::map <std::string, int> File_Hash;
};

#endif
