#ifndef AMP_H
#define AMP_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "downsampler.peg"

using namespace lvtk;

class Downsampler: public Plugin<Downsampler>
{
	private:
		float ratio;
		float p_lastInput;
		float p_loop;
	public:
		Downsampler(double rate);
		void run(uint32_t nframes);
};

#endif
