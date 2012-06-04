#ifndef SLEW_H
#define SLEW_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/slew";

enum p_port_enum {
  p_in,
  p_timeUp,
  p_timeDown,
  p_out,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 10, 0.5, 0, 0, 0 },
  { 0, 10, 0.5, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


class Slew: public Plugin<Slew>
{
	private:
		//Port *port_M_in, *port_out;
		//float timeUp, timeDown;
		float lastData;
		double m_rate;

		//public:
		float *inData;

	public:
		Slew(double rate);
		void run(uint32_t nframes);
};

#endif
