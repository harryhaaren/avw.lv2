#ifndef DYNAMICWAVES_H
#define DYNAMICWAVES_H

#define MODULE_DYNAMICWAVES_MAX_OSC                4
#define DYNAMICWAVES_EXP_TABLE_LEN             32768
#define DYNAMICWAVES_ENVELOPE_RESPONSE           256

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"

#include "dynamicwaves.peg"

using namespace lvtk;

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
