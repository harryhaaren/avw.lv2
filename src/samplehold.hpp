#ifndef SAMPLEHOLD_H
#define SAMPLEHOLD_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/samplehold";

enum p_port_enum {
  p_triggerlevel,
  p_input,
  p_trigger,
  p_output,
  p_gate,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { 0, 10, 0.5, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};

class SampleHold: public Plugin<SampleHold>
{
	private:
		float triggerLevel, sample;bool gate;
	public:
		SampleHold(double rate);
		void run(uint32_t nframes);
};

#endif
