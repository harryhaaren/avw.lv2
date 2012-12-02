#ifndef VCO2_H
#define VCO2_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"
#include "vco2.peg"

using namespace lvtk;

enum WaveFormType
{
	SINUS, TRIANGLE, SAWTOOTH, RECTANGLE, AWAVE_SAW, AWAVE_SAW2, AWAVE_SAW3
};

class Vco2: public Plugin<Vco2>
{
	private:
		float freq, phi0, pw0;
		float edge;
		float wave_period;
		float phi;
		float wave_period_2, wave_period_3, Pi2Times, PKonst;
		float vcoExpFMGain, vcoLinFMGain, pwGain;
		int waveForm;
		int harmonic, subharmonic, octave;

		double m_rate;

		SynthData *synthdata;

	public:
		Vco2(double rate);
		void run(uint32_t nframes);
};

#endif
