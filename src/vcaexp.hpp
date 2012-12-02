#ifndef VCAEXP_H
#define VCAEXP_H

#include "synthdata.hpp"

#include <lvtk-1/lvtk/plugin.hpp>

#include "vcaexp.peg"

using namespace lvtk;

class VcaExp: public Plugin<VcaExp>
{
	private:
		float gain1, gain2, in1, in2, out;
		SynthData *synthdata;

	public:
		VcaExp(double rate);
		void run(uint32_t nframes);
};

#endif
