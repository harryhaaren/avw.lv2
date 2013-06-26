#ifndef VCAEXP_AUDIO_H
#define VCAEXP_AUDIO_H

#include "synthdata.hpp"

#include <lvtk-1/lvtk/plugin.hpp>

#include "vcaexp_audio.peg"

using namespace lvtk;

class VcaExpAudio: public Plugin<VcaExpAudio>
{
	private:
		float gain1, gain2, in1, in2, out;
		SynthData *synthdata;

	public:
		VcaExpAudio(double rate);
		void run(uint32_t nframes);
};

#endif
