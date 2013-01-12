#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/types.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "beatrepeater_mono.hpp"

using namespace lvtk;

BeatRepeaterMono::BeatRepeaterMono(double rate)
: Plugin<BeatRepeaterMono>(p_n_ports)
  {
	m_rate = rate;

	// 60 / tempo * rate * beatSize
	m_sampleFullSize = int(60 / 120 * m_rate * 1 + 0.5);

	m_gate = false;
	m_tempo = 120;
	m_beatSize = -1;

	m_sampleRepeatingPosition = 0;
	m_sampleRecordingPosition = 0;
	m_sampleFull = false;

	m_attack = -1;
	m_release = -1;

	m_sample = new jack_default_audio_sample_t[m_sampleFullSize];
	m_envelope = new jack_default_audio_sample_t[m_sampleFullSize];
	m_attackEnv = new jack_default_audio_sample_t[m_sampleFullSize];
	m_releaseEnv = new jack_default_audio_sample_t[m_sampleFullSize];
	m_envelopeRec = new jack_default_audio_sample_t[m_sampleFullSize];
	m_attackEnvPosition = 0;
	m_releaseEnvPosition = 0;
  }

void BeatRepeaterMono::run(uint32_t nframes)
{
	/*
	 * gate
	 * tempo
	 * beatSize
	 * attack
	 * release
	 * reverse
	 */

	int tempo = int(*p(p_tempo) + 0.5);
	if(tempo < 40)
	{
		tempo = 40;
	}

	float beatSize = *p(p_beatSize);
	if(beatSize < 0.03125)
	{
		beatSize = 0.03125;
	}

	int attack = int(*p(p_attack));
	if(attack < 3)
	{
		attack = 3;
	}

	int release = int(*p(p_release));
	if(release < 3)
	{
		release  = 3;
	}

	attack = m_rate / 1000 * attack;
	release = m_rate / 1000 * release;

	if(beatSize != m_beatSize || tempo != m_tempo)
	{
		m_sampleFullSize = int(60 * m_rate * beatSize / tempo + 0.5);
	}

	if(release + attack > m_sampleFullSize)
	{
		release = int(m_sampleFullSize / 2);
		attack = release;
	}

	if(tempo != m_tempo || beatSize != m_beatSize || attack != m_attack || release != m_release)
	{
		m_tempo = tempo;
		m_beatSize = beatSize;

		m_attack = attack;
		m_release = release;

		m_envelope = new jack_default_audio_sample_t[m_sampleFullSize];
		m_envelopeRec = new jack_default_audio_sample_t[m_sampleFullSize];
		m_attackEnv = new jack_default_audio_sample_t[m_attack];
		m_releaseEnv = new jack_default_audio_sample_t[m_release];

		float m_fadeInStep = 1.0 / m_attack;
		float m_fadeOutStep = 1.0 / m_release;

		float m_fadeIn = 0.0;
		for (int i = 0 ; i < m_attack ; i++)
		{
			m_envelope[i] = m_fadeIn;
			m_envelopeRec[i] = 1;
			m_attackEnv[i] = m_fadeIn;
			m_fadeIn += m_fadeInStep;
		}

		for (int i = m_attack ; i < m_sampleFullSize - m_release ; i++)
		{
			m_envelope[i] = 1;
			m_envelopeRec[i] = 1;
		}

		float m_fadeOut = 1.0;
		for (int i = m_sampleFullSize - m_release ; i < m_sampleFullSize ; i++)
		{
			m_fadeOut -= m_fadeOutStep;
			m_releaseEnv[i - m_sampleFullSize - m_release] = m_fadeOut;
			m_envelope[i] = m_fadeOut;
			m_envelopeRec[i] = m_fadeOut;
		}
	}

	for (unsigned int n = 0; n < nframes; n++)
	{
		if (!m_gate && (p(p_gate)[n] > 0.5))
		{
			m_gate = true;
			m_sample = new jack_default_audio_sample_t[m_sampleFullSize];
			m_sampleRecordingPosition = 0;
			m_sampleRepeatingPosition = 0;
			m_sampleFull = false;
			m_attackEnvPosition = 0;
			m_releaseEnvPosition = 0;
		}
		else if(m_gate && (p(p_gate)[n] < 0.5))
		{
			m_gate = false;
		}

		if(m_gate)
		{
			if(!m_sampleFull)
			{
				if(m_sampleRecordingPosition >= m_sampleFullSize)
				{
					m_sampleFull = true;
					break;
				}
				m_sample[m_sampleRecordingPosition] = p(p_input)[n];
				m_sampleRecordingPosition++;
			}

			if(m_sampleFull)
			{
				if(*p(p_reverse) < 0.5)
				{
					p(p_output)[n] = m_sample[m_sampleRepeatingPosition] * m_envelope[m_sampleRepeatingPosition];
				}
				else
				{
					p(p_output)[n] = m_sample[m_sampleFullSize - 1 - m_sampleRepeatingPosition] * m_envelope[m_sampleRepeatingPosition];
				}

				m_sampleRepeatingPosition++;
				if(m_sampleRepeatingPosition >= m_sampleFullSize)
				{
					m_sampleRepeatingPosition = 0;
				}
			}
			else
			{
				printf("%i %g\n", m_sampleRepeatingPosition, m_envelopeRec[m_sampleRepeatingPosition] );
				p(p_output)[n] = p(p_input)[n] * m_envelopeRec[m_sampleRepeatingPosition];
				m_sampleRepeatingPosition++;
				if(m_sampleRepeatingPosition >= m_sampleFullSize)
				{
					m_sampleRepeatingPosition = 0;
				}
			}
		}
		else
		{
			if(m_sampleRepeatingPosition == 0)
			{
				p(p_output)[n] = p(p_input)[n];
			}
			else
			{
				int p_attackAmp;
				if(m_attackEnvPosition >= m_attack)
				{
					p_attackAmp = 1;
				}
				else
				{
					p_attackAmp = m_attackEnv[m_attackEnvPosition];
				}
				m_attackEnvPosition++;

				int p_releaseAmp;
				if(m_releaseEnvPosition >= m_release)
				{
					p_releaseAmp = 0;
				}
				else
				{
					p_releaseAmp = m_releaseEnv[m_releaseEnvPosition];
				}
				m_releaseEnvPosition++;

				p(p_output)[n] = p(p_input)[n] * p_attackAmp + m_sample[m_sampleRepeatingPosition] * p_releaseAmp;
			}
		}
	}
}


static int _ = BeatRepeaterMono::register_class("http://avwlv2.sourceforge.net/plugins/avw/beatrepeater_mono");

