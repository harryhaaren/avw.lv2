#ifndef VCAEXP_H
#define VCAEXP_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/vcaexp";

enum p_port_enum {
  p_gain1Data,
  p_gain2Data,
  p_in1Data,
  p_in2Data,
  p_gain1,
  p_gain2,
  p_in1,
  p_in2,
  p_outputLevel,
  p_out,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


class VcaExp: public Plugin<VcaExp>
{
	private:
		float gain1, gain2, in1, in2, out;
		//Port *port_M_gain1, *port_M_in1, *port_M_gain2, *port_M_in2, *port_out;
		SynthData *synthdata;

	public:
		VcaExp(double rate);
		void run(uint32_t nframes);
};

#endif
