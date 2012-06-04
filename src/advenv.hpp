#ifndef ADVENV_H
#define ADVENV_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/advenv";

enum p_port_enum
{
	p_gate, p_retrigger, p_timeScale, p_sustain, p_delay, p_attackTime1, p_attackTime2, p_attackTime3, p_attackTime4, p_attackLevel1, p_attackLevel2, p_attackLevel3, p_releaseTime1, p_releaseTime2, p_releaseTime3, p_releaseLevel1, p_releaseLevel2, p_out, p_invOut, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0.1, 10, 1, 0, 0, 0 },
{ 0, 1, 0.7, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 1, 0.05, 0, 0, 0 },
{ 0, 1, 0.05, 0, 0, 0 },
{ 0, 1, 0.05, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 0.9, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.2, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

#define ADVENVELOPE_RESPONSE               256

class AdvEnv: public Plugin<AdvEnv>
{
	private:
		float attack[8], sustain, release[5];
		//float e_noteOff[MAXPOLY], de[MAXPOLY], de_release[MAXPOLY];
		float e_noteOff, de, de_release;
		//Port *port_gate, *port_retrigger, *port_inverse_out, *port_gain_out;

		double m_rate;

	public:
		//float e[MAXPOLY];
		float e;
		float timeScale;
		//bool noteActive[MAXPOLY], gate[MAXPOLY], retrigger[MAXPOLY];
		bool noteActive, gate, retrigger;
		//int noteOnOfs[MAXPOLY];
		int noteOnOfs;
		//int noteOffOfs[MAXPOLY];
		int noteOffOfs;
		float *gateData, *retriggerData;

		AdvEnv(double rate);
		void run(uint32_t nframes);
};

#endif
