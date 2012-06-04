#ifndef MIXER_H
#define MIXER_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/mixer";

enum p_port_enum {
  p_in1,
  p_in2,
  p_in3,
  p_in4,
  p_gain,
  p_volume1,
  p_volume2,
  p_volume3,
  p_volume4,
  p_out,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { 0, 2, 1, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


//#define MAX_MIX_IN                       16

class Mixer : public Plugin<Mixer>
{
	// QList<Port*> in_port_list;
	// Port *port_out;
	//float gain;
	//float mixer_gain[MAX_MIX_IN];

public:
	//int in_channels;
	Mixer(double rate);
	void run(uint32_t nframes);
};

#endif
