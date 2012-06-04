#ifndef DYNAMICWAVES_H
#define DYNAMICWAVES_H

#define MODULE_DYNAMICWAVES_MAX_OSC                4
#define DYNAMICWAVES_EXP_TABLE_LEN             32768
#define DYNAMICWAVES_ENVELOPE_RESPONSE           256

#include "synthdata.hpp"
#include "lv2plugin.hpp"

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/dynamicwaves";

enum p_port_enum
{
	p_freq,
	p_expFM,
	p_linFM,
	p_gate,
	p_retrigger,
	p_octave,
	p_tune,
	p_expFMGain,
	p_linFMGain,
	p_timeScale,
	p_volume1,
	p_volume2,
	p_volume3,
	p_volume4,
	p_osc1waveform,
	p_osc1octave,
	p_osc1tune,
	p_osc1harmonic,
	p_osc1subharmonic,
	p_osc1phi0,
	p_osc2waveform,
	p_osc2octave,
	p_osc2tune,
	p_osc2harmonic,
	p_osc2subharmonic,
	p_osc2phi0,
	p_osc3waveform,
	p_osc3octave,
	p_osc3tune,
	p_osc3harmonic,
	p_osc3subharmonic,
	p_osc3phi0,
	p_osc4waveform,
	p_osc4octave,
	p_osc4tune,
	p_osc4harmonic,
	p_osc4subharmonic,
	p_osc4phi0,
	p_env1delay,
	p_env1attackTime1,
	p_env1attackLevel1,
	p_env1attackTime2,
	p_env1attackLevel2,
	p_env1attackTime3,
	p_env1attackLevel3,
	p_env1attackTime4,
	p_env1Sustain,
	p_env1releaseTime1,
	p_env1releaseLevel1,
	p_env1releaseTime2,
	p_env1releaseLevel2,
	p_env1releaseTime3,
	p_env2delay,
	p_env2attackTime1,
	p_env2attackLevel1,
	p_env2attackTime2,
	p_env2attackLevel2,
	p_env2attackTime3,
	p_env2attackLevel3,
	p_env2attackTime4,
	p_env2Sustain,
	p_env2releaseTime1,
	p_env2releaseLevel1,
	p_env2releaseTime2,
	p_env2releaseLevel2,
	p_env2releaseTime3,
	p_env3delay,
	p_env3attackTime1,
	p_env3attackLevel1,
	p_env3attackTime2,
	p_env3attackLevel2,
	p_env3attackTime3,
	p_env3attackLevel3,
	p_env3attackTime4,
	p_env3Sustain,
	p_env3releaseTime1,
	p_env3releaseLevel1,
	p_env3releaseTime2,
	p_env3releaseLevel2,
	p_env3releaseTime3,
	p_env4delay,
	p_env4attackTime1,
	p_env4attackLevel1,
	p_env4attackTime2,
	p_env4attackLevel2,
	p_env4attackTime3,
	p_env4attackLevel3,
	p_env4attackTime4,
	p_env4Sustain,
	p_env4releaseTime1,
	p_env4releaseLevel1,
	p_env4releaseTime2,
	p_env4releaseLevel2,
	p_env4releaseTime3,
	p_out,
	p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0, 6, 3, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 10, 0, 0, 0, 0 },
{ 0, 10, 0, 0, 0, 0 },
{ 0.1, 10, 1, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 4, 0, 0, 0, 0 },
{ 0, 3, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 1, 16, 1, 0, 0, 0 },
{ 1, 16, 1, 0, 0, 0 },
{ 0, 6.28, 0, 0, 0, 0 },
{ 0, 4, 0, 0, 0, 0 },
{ 0, 3, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 1, 16, 2, 0, 0, 0 },
{ 1, 16, 1, 0, 0, 0 },
{ 0, 6.28, 0, 0, 0, 0 },
{ 0, 4, 0, 0, 0, 0 },
{ 0, 3, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 1, 16, 3, 0, 0, 0 },
{ 1, 16, 1, 0, 0, 0 },
{ 0, 6.28, 0, 0, 0, 0 },
{ 0, 4, 0, 0, 0, 0 },
{ 0, 3, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 1, 16, 4, 0, 0, 0 },
{ 1, 16, 1, 0, 0, 0 },
{ 0, 6.28, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.9, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.8, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.7, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.9, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.8, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.7, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.9, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.8, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.7, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 1, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.9, 0, 0, 0 },
{ 0, 1, 0.1, 0, 0, 0 },
{ 0, 1, 0.8, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.7, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ 0, 1, 0.5, 0, 0, 0 },
{ 0, 1, 0.01, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

using namespace LV2;

enum dynamicWaveFormType
{
	DYNAMICWAVE_SINE, DYNAMICWAVE_SAW, DYNAMICWAVE_TRI, DYNAMICWAVE_RECT, DYNAMICWAVE_SAW2
};

class DynamicWaves: public Plugin<DynamicWaves>
{
	private:
		float attack[8][MODULE_DYNAMICWAVES_MAX_OSC], sustain[MODULE_DYNAMICWAVES_MAX_OSC];
		float release[5][MODULE_DYNAMICWAVES_MAX_OSC];
		float timeScale;
		float e_noteOff[MODULE_DYNAMICWAVES_MAX_OSC];
		float de[MODULE_DYNAMICWAVES_MAX_OSC];
		float de_release[MODULE_DYNAMICWAVES_MAX_OSC];
		float tune, osc_tune[MODULE_DYNAMICWAVES_MAX_OSC], gain[MODULE_DYNAMICWAVES_MAX_OSC];
		int octave, osc_octave[MODULE_DYNAMICWAVES_MAX_OSC];
		int harmonic[MODULE_DYNAMICWAVES_MAX_OSC], subharmonic[MODULE_DYNAMICWAVES_MAX_OSC];
		int waveForm[MODULE_DYNAMICWAVES_MAX_OSC];
		float expFMGain, linFMGain;
		float phi0[MODULE_DYNAMICWAVES_MAX_OSC], phi[MODULE_DYNAMICWAVES_MAX_OSC];
		float wave_period;
		bool allEnvTerminated;

		double m_rate;
		SynthData *synthdata;

		//Port *port_M_freq, *port_M_exp, *port_M_lin, *port_gate, *port_retrigger;
		//Port *port_out;

	public:
		int oscCount;
		float e[MODULE_DYNAMICWAVES_MAX_OSC];
		bool noteActive, gate, retrigger;
		bool oscActive[MODULE_DYNAMICWAVES_MAX_OSC];
		long noteOnOfs[MODULE_DYNAMICWAVES_MAX_OSC];
		long noteOffOfs[MODULE_DYNAMICWAVES_MAX_OSC];
		float *gateData;
		float *retriggerData;
		float *freqData;
		float *expFMData; // Frequency modulation exp characteristic
		float *linFMData; // Frequency modulation lin characteristic

		DynamicWaves(double rate);
		void run(uint32_t nframes);
};

#endif
