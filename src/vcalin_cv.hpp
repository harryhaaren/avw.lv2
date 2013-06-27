#ifndef VCALIN_CV_H
#define VCALIN_CV_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "vcalin_cv.peg"

using namespace lvtk;

class VcaLinCV: public Plugin<VcaLinCV>
{
	private:
		float gain1, gain2, in1, in2, out;

	public:
		VcaLinCV(double rate);
		void run(uint32_t nframes);
};

#endif
