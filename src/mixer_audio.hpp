#ifndef MIXER_AUDIO_H
#define MIXER_AUDIO_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "mixer_audio.peg"

using namespace lvtk;

class MixerAudio : public Plugin<MixerAudio>
{
public:
		MixerAudio(double rate);
	void run(uint32_t nframes);
};

#endif
