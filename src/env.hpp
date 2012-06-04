#ifndef ENV_H
#define ENV_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/env";

enum p_port_enum
{
	p_gate, p_retrigger, p_attack, p_decay, p_sustain, p_release, p_delay, p_hold, p_timeScale, p_out, p_invOut, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0, 1, 0.05, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.7, 0, 0, 0 },
{ 0, 1, 0.05, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 1, 0.02, 0, 0, 0 },
{ 1, 10, 0.1, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

#define ENVELOPE_RESPONSE               256

class Env: public Plugin<Env>
{
	private:
		float delay, attack, hold, decay, sustain, release;
		float e_noteOff, de;
		//Port *port_gate, *port_retrigger, *port_inverse_out, *port_gain_out;

		double m_rate;

	public:
		float e;
		float timeScale;
		bool gate, retrigger;
		int noteOnOfs;
		float *gateData, *retriggerData;

		Env(double rate);
		void run(uint32_t nframes);
};

#endif
