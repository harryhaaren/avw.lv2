#ifndef VCAEXP_CV_H
#define VCAEXP_CV_H

#include "synthdata.hpp"

#include <lvtk-1/lvtk/plugin.hpp>

#include "vcaexp_cv.peg"

using namespace lvtk;

class VcaExpCV: public Plugin<VcaExpCV>
{
	private:
		float gain1, gain2, in1, in2, out;
		SynthData *synthdata;

	public:
		VcaExpCV(double rate);
		void run(uint32_t nframes);
};

#endif
