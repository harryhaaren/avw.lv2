#ifndef MIXER_CV_H
#define MIXER_CV_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "mixer_cv.peg"

using namespace lvtk;

class MixerCV : public Plugin<MixerCV>
{
	public:
		MixerCV(double rate);
		void run(uint32_t nframes);
};

#endif
