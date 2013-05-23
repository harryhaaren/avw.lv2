#ifndef CONTROLTOVC_H
#define CONTROLTOVC_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "controltovc.peg"

using namespace lvtk;

class ControlToVc: public Plugin<ControlToVc>
{
	public:
		ControlToVc(double rate);
		void run(uint32_t nframes);
};

#endif
