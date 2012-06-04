#ifndef VCSWITCH_H
#define VCSWITCH_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/vcswitch";

enum p_port_enum {
  p_switchlevel,
  p_cv,
  p_in1,
  p_in2,
  p_out1,
  p_out2,
  p_outmix,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { 0, 10, 0.5, 0, 0, 0 },
  { -1, 1, 0, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


class VCSwitch: public Plugin<VCSwitch>
{
	private:
		float switchlevel;
	public:
		VCSwitch(double rate);
		void run(uint32_t nframes);
};

#endif
