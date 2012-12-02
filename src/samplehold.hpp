#ifndef SAMPLEHOLD_H
#define SAMPLEHOLD_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "samplehold.peg"

using namespace lvtk;

class SampleHold: public Plugin<SampleHold>
{
	private:
		float triggerLevel, sample;bool gate;
	public:
		SampleHold(double rate);
		void run(uint32_t nframes);
};

#endif
