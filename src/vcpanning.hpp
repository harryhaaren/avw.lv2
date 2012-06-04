#ifndef VCPANNING_H
#define VCPANNING_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/vcpanning";

enum p_port_enum {
  p_in,
  p_panCV,
  p_panOffset,
  p_panGain,
  p_panningMode,
  p_out1,
  p_out2,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -1, 1, 0, 0, 0, 0 },
  { 0, 2, 0, 0, 0, 0 },
  { 0, 4, 0, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


class VCPanning: public Plugin<VCPanning>
{
	private:
		float panPos, pan[2], oldpan[2];
	public:
		VCPanning(double rate);
		void run(uint32_t nframes);

};

#endif
