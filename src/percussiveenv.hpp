#ifndef PERCUSSIVEENV_H
#define PERCUSSIVEENV_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/percussiveenv";

enum p_port_enum {
  p_gate,
  p_retrigger,
  p_attack,
  p_decay,
  p_delay,
  p_hold,
  p_timeScale,
  p_out,
  p_invOut,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 1, 0.05, 0, 0, 0 },
  { 0, 1, 0.1, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 0, 1, 0.02, 0, 0, 0 },
  { 1, 10, 0.1, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};

#define ENVELOPE_RESPONSE               256

class PercussiveEnv: public Plugin<PercussiveEnv>
{
	private:
		float delay, attack, hold, decay;
		float e_noteOff, de;
		//Port *port_gate, *port_retrigger, *port_inverse_out, *port_gain_out;

		double m_rate;

	public:
		float e;
		float timeScale;bool gate, retrigger;
		int noteOnOfs;
		float *gateData, *retriggerData;

		PercussiveEnv(double rate);
		void run(uint32_t nframes);
};

#endif
