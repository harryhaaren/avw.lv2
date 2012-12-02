#ifndef SLEW_H
#define SLEW_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"
#include "slew.peg"

using namespace lvtk;

class Slew: public Plugin<Slew>
{
	private:
		//Port *port_M_in, *port_out;
		//float timeUp, timeDown;
		float lastData;
		double m_rate;

		//public:
		float *inData;

	public:
		Slew(double rate);
		void run(uint32_t nframes);
};

#endif
