#ifndef VCO2_H
#define VCO2_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/vco2";

enum p_port_enum {
  p_freq,
  p_expFM,
  p_linFM,
  p_pwPort,
  p_waveForm,
  p_octave,
  p_tune,
  p_harmonic,
  p_subharmonic,
  p_pw0,
  p_pwGain,
  p_phi0,
  p_expFMGain,
  p_linFMGain,
  p_out,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 6, 0, 0, 0, 0 },
  { 0, 6, 3, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 1, 16, 1, 0, 0, 0 },
  { 1, 16, 1, 0, 0, 0 },
  { 0.1, 0.9, 0.5, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 0, 6.283, 0, 0, 0, 0 },
  { 0, 10, 0, 0, 0, 0 },
  { 0, 10, 0, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


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
		//float *freqData;
		//float *pwData;
		//float *expFMData;
		//float *linFMData;

		Vco2(double rate);
		void run(uint32_t nframes);
};

#endif
