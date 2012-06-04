#ifndef INV_H
#define INV_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/inv";

enum p_port_enum {
  p_input,
  p_output,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


class Inv
: public Plugin<Inv>
{

public:
	Inv(double rate);
	void run(uint32_t nframes);
};

#endif
