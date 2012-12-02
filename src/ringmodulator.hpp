#ifndef RINGMODULATOR_H
#define RINGMODULATOR_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "ringmodulator.peg"

using namespace lvtk;

class RingModulator: public Plugin<RingModulator>
{
	private:
		float gain;
	public:
		RingModulator(double rate);
		void run(uint32_t nframes);
};

#endif
