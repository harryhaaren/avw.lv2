#ifndef HZTOVC_H
#define HZTOVC_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/hztovc";

enum p_port_enum
{
	p_octaveOffset, p_input, p_output, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -3, 3, 0, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

class HzToVc: public Plugin<HzToVc>
{
	private:
		int octave;
		double m_rate;
		float m_eighth, m_log2inv;

	public:
		HzToVc(double rate);
		void run(uint32_t nframes);
};

#endif
