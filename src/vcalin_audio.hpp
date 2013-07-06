#ifndef VCALIN_AUDIO_H
#define VCALIN_AUDIO_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "vcalin_audio.peg"

using namespace lvtk;

class VcaLinAudio: public Plugin<VcaLinAudio>
{
	public:
		VcaLinAudio(double rate);
		void run(uint32_t nframes);
};

#endif
