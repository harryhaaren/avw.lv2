#ifndef AD_H
#define AD_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/ad";

enum p_port_enum
{
	p_cvin, p_detuneAmplitude, p_detuneModulation, p_detuneRate, p_driftAmplitude, p_driftModulation, p_driftRate, p_cvout0, p_cvout1, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0, 0.084, 0.005, 0, 0, 0 },
{ 0.01, 1, 0.01, 0, 0, 0 },
{ 0.01, 10, 0.01, 0, 0, 0 },
{ 0, 0.084, 0.005, 0, 0, 0 },
{ 0.01, 1, 0.01, 0, 0, 0 },
{ 0.01, 10, 3, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

class Ad: public Plugin<Ad>
{
	private:
		float detune_a, detune_c;
		float drift_a[MAX_ANALOGUE_DRIVER_OUT], drift_c[MAX_ANALOGUE_DRIVER_OUT];
		int detuneCount, driftCount, voice[2];
		double m_rate;
		SynthData *synthdata;

	public:
		Ad(double rate);
		void run(uint32_t nframes);
};

#endif
