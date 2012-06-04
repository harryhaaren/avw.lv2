#ifndef DELAY_H
#define DELAY_H

#define MAX_DELAY_FRAMES                16384

#include "lv2plugin.hpp"

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/delay";

enum p_port_enum
{
	p_delay, p_input, p_output, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ 0, 10, 0, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

using namespace LV2;

class Delay: public Plugin<Delay>
{
	public:
		Delay(double rate);
		~Delay();
		void run(uint32_t nframes);

	private:
		float m_delay;
		int read_ofs;
		float *buf;
};

#endif
