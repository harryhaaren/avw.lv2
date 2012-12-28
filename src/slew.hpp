#ifndef SLEW_H
#define SLEW_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "slew.peg"

using namespace lvtk;

class Slew: public Plugin<Slew>
{
	private:
		float lastData;
		double m_rate;
		float *inData;

	public:
		Slew(double rate);
		void run(uint32_t nframes);
};

#endif
