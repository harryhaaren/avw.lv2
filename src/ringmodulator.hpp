#ifndef RINGMODULATOR_H
#define RINGMODULATOR_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/ringmodulator";

enum p_port_enum {
  p_gain,
  p_in1,
  p_in2,
  p_output,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { 0, 5, 0.5, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};

class RingModulator: public Plugin<RingModulator>
{
	private:
		float gain;
	public:
		RingModulator(double rate);
		void run(uint32_t nframes);
};

#endif
