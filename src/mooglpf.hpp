#ifndef MOOGLPF_H
#define MOOGLPF_H

#include "lv2plugin.hpp"

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/mooglpf";

enum p_port_enum {
  p_inputGain,
  p_frequency,
  p_expfmgain,
  p_resonance,
  p_resonanceGain,
  p_outputGain,
  p_portinput,
  p_portfrequency,
  p_portexpfm,
  p_portresonance,
  p_output,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -60, 10, 0, 0, 0, 0 },
  { -6, 6, 0, 0, 0, 0 },
  { 0, 10, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { -15, 15, 0, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


using namespace LV2;

class MoogLPF: public Plugin<MoogLPF>
{
	private:
	  float _c1, _c2, _c3, _c4, _c5, _w, _r;
	  double m_rate;

	  float exp2ap (float x);
	public:
		MoogLPF(double rate);
		void run(uint32_t nframes);
};

#endif
