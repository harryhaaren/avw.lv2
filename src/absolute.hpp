#ifndef ABSOLUTE_H
#define ABSOLUTE_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/absolute";

enum p_port_enum
{
	p_input, p_output, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

class Absolute: public Plugin<Absolute>
{

	public:
		Absolute(double rate);
		void run(uint32_t nframes);
};

#endif
