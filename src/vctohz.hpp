#ifndef VCTOHZ_H
#define VCTOHZ_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "vctohz.peg"

using namespace lvtk;

class VcToHz: public Plugin<VcToHz>
{
	private:
		int convMode, octave;
		double m_rate;

	public:
		VcToHz(double rate);
		void run(uint32_t nframes);
};

#endif
