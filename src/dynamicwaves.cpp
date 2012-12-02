#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"

#include "dynamicwaves.hpp"

DynamicWaves::DynamicWaves(double rate) :
	Plugin<DynamicWaves> (p_n_ports)
{
	synthdata = new SynthData(1);
	m_rate = rate;

	int l1, l2;

	wave_period = (float) WAVE_PERIOD;
	tune = 0;
	octave = 3;
	expFMGain = 0;
	linFMGain = 0;
	oscCount = 4;
	allEnvTerminated = true;
	timeScale = 1;
	for (l1 = 0; l1 < 4; l1++)
	{
		gain[l1] = 1;
		osc_tune[l1] = 0;
		harmonic[l1] = 1 + l1;
		subharmonic[l1] = 1;
		osc_octave[l1] = 0;
		waveForm[l1] = DYNAMICWAVE_SINE;
		phi0[l1] = 0;
		attack[0][l1] = 0;
		attack[1][l1] = 0.01;
		attack[2][l1] = 0.5;
		attack[3][l1] = 0.01;
		attack[4][l1] = 1.0;
		attack[5][l1] = 0.1;
		attack[6][l1] = 0.9;
		attack[7][l1] = 0.1;
		sustain[l1] = 0.8;
		release[0][l1] = 0.01;
		release[1][l1] = 0.7;
		release[2][l1] = 0.01;
		release[3][l1] = 0.5;
		release[4][l1] = 0.01;
	}

	gate = false;
	retrigger = false;
	noteActive = false;
	for (l2 = 0; l2 < oscCount; l2++)
	{
		phi[l2] = 0;
		oscActive[l2] = false;
		noteOnOfs[l2] = 0;
		noteOffOfs[l2] = 0;
		e[l2] = 0;
		de[l2] = 0;
	}

	//port_M_freq = new Port("Freq", PORT_IN, 0, this);
	//port_M_exp = new Port("Exp. FM", PORT_IN, 1, this);
	//port_M_lin = new Port("Lin. FM", PORT_IN, 2, this);
	//port_gate = new Port("Gate", PORT_IN, 3, this);
	//port_retrigger = new Port("Retrigger", PORT_IN, 4, this);

}

void DynamicWaves::run(uint32_t nframes)
{
	int l3, l4, status;
	unsigned int l2;
	float dphi, phi1;
	float freq_const[MODULE_DYNAMICWAVES_MAX_OSC], freq_tune[MODULE_DYNAMICWAVES_MAX_OSC];
	float gain_linfm, wave_period_2, current_gain;
	float gain_const[MODULE_DYNAMICWAVES_MAX_OSC], phi_const[MODULE_DYNAMICWAVES_MAX_OSC];
	float t[8][MODULE_DYNAMICWAVES_MAX_OSC], tscale;
	float de_a[4][MODULE_DYNAMICWAVES_MAX_OSC];
	float de_d[3][MODULE_DYNAMICWAVES_MAX_OSC];

	wave_period_2 = wave_period / 2.0;

	freqData = p(p_freq); //port_M_freq->getinputdata();
	expFMData = p(p_expFM); //port_M_exp->getinputdata();
	linFMData = p(p_linFM); //port_M_lin->getinputdata();
	gateData = p(p_gate); //port_gate->getinputdata();
	retriggerData = p(p_retrigger); //port_retrigger->getinputdata();

	octave = floor(*p(p_octave));
	tune = *p(p_tune);

	gain[0] = *p(p_volume1);
	gain[1] = *p(p_volume2);
	gain[2] = *p(p_volume3);
	gain[3] = *p(p_volume4);

	waveForm[0] = floor(*p(p_osc1waveform));
	waveForm[1] = floor(*p(p_osc2waveform));
	waveForm[2] = floor(*p(p_osc3waveform));
	waveForm[3] = floor(*p(p_osc4waveform));

	osc_octave[0] = floor(*p(p_osc1octave));
	osc_octave[1] = floor(*p(p_osc2octave));
	osc_octave[2] = floor(*p(p_osc3octave));
	osc_octave[3] = floor(*p(p_osc4octave));

	osc_tune[0] = *p(p_osc1tune);
	osc_tune[1] = *p(p_osc2tune);
	osc_tune[2] = *p(p_osc3tune);
	osc_tune[3] = *p(p_osc4tune);

	harmonic[0] = floor(*p(p_osc1harmonic));
	harmonic[1] = floor(*p(p_osc2harmonic));
	harmonic[2] = floor(*p(p_osc3harmonic));
	harmonic[3] = floor(*p(p_osc4harmonic));

	subharmonic[0] = floor(*p(p_osc1subharmonic));
	subharmonic[1] = floor(*p(p_osc2subharmonic));
	subharmonic[2] = floor(*p(p_osc3subharmonic));
	subharmonic[3] = floor(*p(p_osc4subharmonic));

	phi0[0] = *p(p_osc1phi0);
	phi0[1] = *p(p_osc2phi0);
	phi0[2] = *p(p_osc3phi0);
	phi0[3] = *p(p_osc4phi0);

	attack[0][0] = *p(p_env1delay);
	attack[0][1] = *p(p_env2delay);
	attack[0][2] = *p(p_env3delay);
	attack[0][3] = *p(p_env4delay);

	attack[1][0] = *p(p_env1attackTime1);
	attack[1][1] = *p(p_env2attackTime1);
	attack[1][2] = *p(p_env3attackTime1);
	attack[1][3] = *p(p_env4attackTime1);

	attack[2][0] = *p(p_env1attackLevel1);
	attack[2][1] = *p(p_env2attackLevel1);
	attack[2][2] = *p(p_env3attackLevel1);
	attack[2][3] = *p(p_env4attackLevel1);

	attack[3][0] = *p(p_env1attackTime2);
	attack[3][1] = *p(p_env2attackTime2);
	attack[3][2] = *p(p_env3attackTime2);
	attack[3][3] = *p(p_env4attackTime2);

	attack[4][0] = *p(p_env1attackLevel2);
	attack[4][1] = *p(p_env2attackLevel2);
	attack[4][2] = *p(p_env3attackLevel2);
	attack[4][3] = *p(p_env4attackLevel2);

	attack[5][0] = *p(p_env1attackTime3);
	attack[5][1] = *p(p_env2attackTime3);
	attack[5][2] = *p(p_env3attackTime3);
	attack[5][3] = *p(p_env4attackTime3);

	attack[6][0] = *p(p_env1attackLevel3);
	attack[6][1] = *p(p_env2attackLevel3);
	attack[6][2] = *p(p_env3attackLevel3);
	attack[6][3] = *p(p_env4attackLevel3);

	attack[6][0] = *p(p_env1attackTime4);
	attack[6][1] = *p(p_env2attackTime4);
	attack[6][2] = *p(p_env3attackTime4);
	attack[6][3] = *p(p_env4attackTime4);

	sustain[0] = *p(p_env1Sustain);
	sustain[1] = *p(p_env2Sustain);
	sustain[2] = *p(p_env3Sustain);
	sustain[3] = *p(p_env4Sustain);

	release[0][0] = *p(p_env1releaseTime1);
	release[0][1] = *p(p_env2releaseTime1);
	release[0][2] = *p(p_env3releaseTime1);
	release[0][3] = *p(p_env4releaseTime1);

	release[1][0] = *p(p_env1releaseLevel1);
	release[1][1] = *p(p_env2releaseLevel1);
	release[1][2] = *p(p_env3releaseLevel1);
	release[1][3] = *p(p_env4releaseLevel1);

	release[2][0] = *p(p_env1releaseTime2);
	release[2][1] = *p(p_env2releaseTime2);
	release[2][2] = *p(p_env3releaseTime2);
	release[2][3] = *p(p_env4releaseTime2);

	release[3][0] = *p(p_env1releaseLevel2);
	release[3][1] = *p(p_env2releaseLevel2);
	release[3][2] = *p(p_env3releaseLevel2);
	release[3][3] = *p(p_env4releaseLevel2);

	release[4][0] = *p(p_env1releaseTime3);
	release[4][1] = *p(p_env2releaseTime3);
	release[4][2] = *p(p_env3releaseTime3);
	release[4][3] = *p(p_env4releaseTime3);

	gain_linfm = 1000.0 * *p(p_linFMGain);
	expFMGain = *p(p_expFMGain);
	tscale = *p(p_timeScale) * m_rate;
	for (l3 = 0; l3 < oscCount; l3++)
	{
		gain_const[l3] = gain[l3] / (float) oscCount;
		freq_tune[l3] = 4.0313842 + octave + tune + osc_octave[l3] + osc_tune[l3];
		freq_const[l3] = wave_period / (float) m_rate * (float) harmonic[l3] / (float) subharmonic[l3];
		phi_const[l3] = phi0[l3] * wave_period / (2.0 * M_PI);
		de_a[0][l3] = (attack[1][l3] > 0) ? attack[2][l3] / (tscale * attack[1][l3]) : 0;
		de_a[1][l3] = (attack[3][l3] > 0) ? (attack[4][l3] - attack[2][l3]) / (tscale * attack[3][l3]) : 0;
		de_a[2][l3] = (attack[5][l3] > 0) ? (attack[6][l3] - attack[4][l3]) / (tscale * attack[5][l3]) : 0;
		de_a[3][l3] = (attack[7][l3] > 0) ? (sustain[l3] - attack[6][l3]) / (tscale * attack[7][l3]) : 0;
		de_d[0][l3] = (release[0][l3] > 0) ? (release[1][l3] - sustain[l3]) / (tscale * release[0][l3]) : 0;
		de_d[1][l3] = (release[2][l3] > 0) ? (release[3][l3] - release[1][l3]) / (tscale * release[2][l3]) : 0;
		de_d[2][l3] = (release[4][l3] > 0) ? -release[3][l3] / (tscale * release[4][l3]) : 0;
		t[0][l3] = tscale * attack[0][l3];
		t[1][l3] = t[0][l3] + tscale * attack[1][l3];
		t[2][l3] = t[1][l3] + tscale * attack[3][l3];
		t[3][l3] = t[2][l3] + tscale * attack[5][l3];
		t[4][l3] = t[3][l3] + tscale * attack[7][l3];
		t[5][l3] = tscale * release[0][l3];
		t[6][l3] = t[5][l3] + tscale * release[2][l3];
		t[7][l3] = t[6][l3] + tscale * release[4][l3];
	}

	for (l2 = 0; l2 < nframes; l2++)
	{
		noteActive = !allEnvTerminated;
		allEnvTerminated = true;
		if (!retrigger && (retriggerData[l2] > 0.5))
		{
			retrigger = true;
		}
		if (retrigger && (retriggerData[l2] < 0.5))
		{
			retrigger = false;
		}

		for (l3 = 0; l3 < oscCount; l3++)
		{
			if (l3 == 0)
			{
				if (!gate && (gateData[l2] > 0.5))
				{
					gate = true;
					noteActive = true;
					for (l4 = 0; l4 < oscCount; l4++)
					{
						oscActive[l4] = true;
						if (e[l4] > 0)
						{
							noteOnOfs[l4] = -DYNAMICWAVES_ENVELOPE_RESPONSE;
							de[l4] = e[l4] / (float) DYNAMICWAVES_ENVELOPE_RESPONSE;
						}
						else
						{
							noteOnOfs[l4] = 0;
						}
					}
				}
				if (gate && (gateData[l2] < 0.5))
				{
					gate = false;
					for (l4 = 0; l4 < oscCount; l4++)
					{
						noteOffOfs[l4] = 0;
						e_noteOff[l4] = e[l4];
						de_release[l4] = (release[0][l4] > 0) ? (release[1][l4] - e_noteOff[l4]) / (tscale * release[0][l4]) : 0;
					}
				}
			}
			if (retrigger)
			{
				if (e[l3] > 0)
				{
					noteOnOfs[l3] = 0;
					if (e[l3] < attack[2][l3])
					{
						noteOnOfs[l3] = (de_a[0][l3] > 0) ? t[0][l3] + e[l3] / de_a[0][l3] : t[0][l3];
					}
					else if (e[l3] < attack[4][l3])
					{
						noteOnOfs[l3] = (de_a[1][l3] > 0) ? t[1][l3] + (e[l3] - attack[2][l3]) / de_a[1][l3] : t[1][l3];
					}
					else if (e[l3] < attack[6][l3])
					{
						noteOnOfs[l3] = (de_a[2][l3] > 0) ? t[2][l3] + (e[l3] - attack[4][l3]) / de_a[2][l3] : t[2][l3];
					}
					else if (e[l3] <= sustain[l3])
					{
						noteOnOfs[l3] = (de_a[3][l3] > 0) ? t[3][l3] + (e[l3] - attack[6][l3]) / de_a[3][l3] : t[3][l3];
					}
				}
				else
				{
					noteOnOfs[l3] = 0;
				}
			}
			if (gate)
			{
				status = 1;
				if (noteOnOfs[l3] < 0)
					status = 0;
				if (noteOnOfs[l3] >= long(t[0][l3]))
					status = 2;
				if (noteOnOfs[l3] >= long(t[1][l3]))
					status = 3;
				if (noteOnOfs[l3] >= long(t[2][l3]))
					status = 4;
				if (noteOnOfs[l3] >= long(t[3][l3]))
					status = 5;
				if (noteOnOfs[l3] >= long(t[4][l3]))
					status = 6;
				switch (status)
				{
					case 0:
						e[l3] -= de[l3];
						break;
					case 1:
						e[l3] = 0;
						break;
					case 2:
						e[l3] += de_a[0][l3];
						break;
					case 3:
						e[l3] += de_a[1][l3];
						break;
					case 4:
						e[l3] += de_a[2][l3];
						break;
					case 5:
						e[l3] += de_a[3][l3];
						break;
					case 6:
						e[l3] = sustain[l3];
						break;
					default:
						e[l3] = 0;
						break;
				}
				if (e[l3] < 0)
					e[l3] = 0;
				noteOnOfs[l3]++;
			}
			else
			{
				if (oscActive[l3] > 0)
				{
					status = 1;
					if (noteOffOfs[l3] < 0)
						status = 0;
					if (noteOffOfs[l3] >= long(t[5][l3]))
						status = 2;
					if (noteOffOfs[l3] >= long(t[6][l3]))
						status = 3;
					if (noteOffOfs[l3] >= long(t[7][l3]))
						status = 4;
					switch (status)
					{
						case 0:
							e[l3] = 0;
							break;
						case 1:
							e[l3] += de_release[l3];
							break;
						case 2:
							e[l3] += de_d[1][l3];
							break;
						case 3:
							e[l3] += de_d[2][l3];
							break;
						case 4:
							e[l3] = 0;
							break;
						default:
							e[l3] = 0;
							break;
					}
					if (e[l3] < 0)
						e[l3] = 0;
				}
				noteOffOfs[l3]++;
				if (noteOffOfs[l3] >= int(t[7][l3]))
				{
					oscActive[l3] = false;
					e[l3] = 0;
				}
			}
			if (oscActive[l3])
			{
				allEnvTerminated = false;
			}
			dphi = freq_const[l3] * (synthdata->exp2_table(freq_tune[l3] + freqData[l2] + expFMGain * expFMData[l2]) + gain_linfm * linFMData[l2]);
			if (dphi > wave_period_2)
			{
				dphi = wave_period_2;
				current_gain = 0;
			}
			else
			{
				current_gain = gain_const[l3] * e[l3];
			}

			phi1 = phi[l3] + phi_const[l3];

			if (phi1 < 0)
				phi1 += wave_period;
			else if (phi1 >= wave_period)
				phi1 -= wave_period;
			switch (waveForm[l3])
			{
				case DYNAMICWAVE_SINE:
					p(p_out)[l2] += current_gain * synthdata->wave_sine[(int) phi1];
					break;
				case DYNAMICWAVE_SAW:
					p(p_out)[l2] += current_gain * synthdata->wave_saw[(int) phi1];
					break;
				case DYNAMICWAVE_TRI:
					p(p_out)[l2] += current_gain * synthdata->wave_tri[(int) phi1];
					break;
				case DYNAMICWAVE_RECT:
					p(p_out)[l2] += current_gain * synthdata->wave_rect[(int) phi1];
					break;
				case DYNAMICWAVE_SAW2:
					p(p_out)[l2] += current_gain * synthdata->wave_saw2[(int) phi1];
					break;
			}
			phi[l3] += dphi;
			while (phi[l3] < 0)
				phi[l3] += wave_period;
			while (phi[l3] >= wave_period)
				phi[l3] -= wave_period;
		}
	}
}

static int _ = DynamicWaves::register_class("http://avwlv2.sourceforge.net/plugins/avw/dynamicwaves");

