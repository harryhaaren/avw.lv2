#ifndef MIXER_H
#define MIXER_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "mixer.peg"

using namespace lvtk;

class Mixer : public Plugin<Mixer>
{
public:
	Mixer(double rate);
	void run(uint32_t nframes);
};

#endif
