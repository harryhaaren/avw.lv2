#ifndef AMP_H
#define AMP_H

#include "lv2plugin.hpp"

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/amp";

enum p_port_enum
{
	p_gain, p_input, p_output, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -10, 10, 1, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

using namespace LV2;

class Amp: public Plugin<Amp>
{
	private:
		float gain;
	public:
		Amp(double rate);
		void run(uint32_t nframes);
};

#endif
