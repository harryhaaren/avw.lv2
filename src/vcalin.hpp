#ifndef VCALIN_H
#define VCALIN_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"

#include "vcalin.peg"

using namespace lvtk;

class VcaLin: public Plugin<VcaLin>
{
	private:
		float gain1, gain2, in1, in2, out;

	public:
		VcaLin(double rate);
		void run(uint32_t nframes);
};

#endif
