#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "granulator_stereo.hpp"

using namespace lvtk;

GranulatorStereo::GranulatorStereo(double rate)
: Plugin<GranulatorStereo>(p_n_ports)
  {
	m_rate = rate;

	m_recordingGrainPosition = 0;
	m_grainPosition = 0;
	m_grainIndex = 0;

	m_lastAttack = -1;
	m_lastRelease = -1;
	m_lastGrainSize = -1;

	m_recordingGrainL = new jack_default_audio_sample_t[10];
	m_recordingGrainR = new jack_default_audio_sample_t[10];
	m_envelope = new jack_default_audio_sample_t[10];
	m_playingGrainL = new jack_default_audio_sample_t[10];
	m_playingGrainR = new jack_default_audio_sample_t[10];
	m_grainsL.insert(m_grainsL.begin(), m_recordingGrainL);
	m_grainsR.insert(m_grainsR.begin(), m_recordingGrainR);
	m_sizeGrains.insert(m_sizeGrains.begin(), 10);

	m_playingGrainMode = true;
	m_silencePosition = 0;
  }

void GranulatorStereo::run(uint32_t nframes)
{
	/*
	 * input
	 * grainSize
	 * attack
	 * release
	 * grainDensity
	 * grainSpread
	 * outputGain
	 * output
	 */

	int attack = *p(p_attack);
	if(attack < 3)
	{
		attack = 3;
	}

	int release = *p(p_release);
	if(release < 3)
	{
		release  = 3;
	}

	int grainSize = *p(p_grainSize);
	if(grainSize < 6 || attack + release > grainSize)
	{
		grainSize = attack + release;
	}
	grainSize = m_rate / 1000 * grainSize;
	attack = m_rate / 1000 * attack;
	release = m_rate / 1000 * release;

	int grainSpread = *p(p_grainSpread);
	if(grainSpread < 2)
	{
		grainSpread = 2;
	}

	int grainDensity = *p(p_grainDensity);
	if(grainDensity < 1)
	{
		grainDensity = 1;
	}
	grainDensity = m_rate / 1000 * grainDensity;

	if(m_lastAttack != attack || m_lastRelease != release || m_lastGrainSize != grainSize)
	{
		m_lastAttack = attack;
		m_lastRelease = release;
		m_lastGrainSize = grainSize;

		m_recordingGrainL = new jack_default_audio_sample_t[m_lastGrainSize];
		m_recordingGrainR = new jack_default_audio_sample_t[m_lastGrainSize];
		m_envelope = new jack_default_audio_sample_t[m_lastGrainSize];
		m_recordingGrainPosition = 0;

		float m_fadeInStep = 1.0 / m_lastAttack;
		float m_fadeOutStep = 1.0 / m_lastRelease;

		float m_fadeIn = 0.0;
		for (int i = 0 ; i < m_lastAttack ; i++)
		{
			m_envelope[i] = m_fadeIn;
			m_fadeIn += m_fadeInStep;
		}

		for (int i = m_lastAttack ; i < m_lastGrainSize - m_lastRelease ; i++)
		{
			m_envelope[i] = 1;
		}

		float m_fadeOut = 1.0;
		for (int i = m_lastGrainSize - m_lastRelease ; i < m_lastGrainSize ; i++)
		{
			m_fadeOut -= m_fadeOutStep;
			m_envelope[i] = m_fadeOut;
		}
	}

	for (unsigned int n = 0; n < nframes; n++)
	{
		if(m_recordingGrainPosition < m_lastGrainSize)
		{
			m_recordingGrainL[m_recordingGrainPosition] = p(p_inputL)[n] * m_envelope[m_recordingGrainPosition] * *p(p_inputGain) ;
			m_recordingGrainR[m_recordingGrainPosition] = p(p_inputR)[n] * m_envelope[m_recordingGrainPosition] * *p(p_inputGain) ;
		}
		else
		{
			m_grainsL.insert(m_grainsL.begin(), m_recordingGrainL);
			m_grainsR.insert(m_grainsR.begin(), m_recordingGrainR);
			m_sizeGrains.insert(m_sizeGrains.begin(), m_lastGrainSize);

			m_recordingGrainPosition = 0;
			if(m_lastGrainSize > 0)
			{
				m_recordingGrainL = new jack_default_audio_sample_t[m_lastGrainSize];
				m_recordingGrainR = new jack_default_audio_sample_t[m_lastGrainSize];
			}
		}
		m_recordingGrainPosition++;
	}

	while(m_grainsL.size() > grainSpread)
	{
		m_grainsL.pop_back();
		m_grainsR.pop_back();
		m_sizeGrains.pop_back();
	}

	//printf("Grain Avl: %lu Index: %i\n", m_grains.size(), m_grainIndex);

	for (unsigned int n = 0; n < nframes; n++)
	{
		if(m_playingGrainMode)
		{
			if(m_grainPosition >= m_sizeGrains[m_grainIndex])
			{
				m_grainPosition = 0;
				m_grainIndex = rand() % m_grainsL.size();
				m_playingGrainL = m_grainsL[m_grainIndex];
				m_playingGrainR = m_grainsR[m_grainIndex];
				m_playingGrainMode = false;
			}
			p(p_outputL)[n] = m_playingGrainL[m_grainPosition] * *p(p_outputGain);
			p(p_outputR)[n] = m_playingGrainR[m_grainPosition] * *p(p_outputGain);
			m_grainPosition++;
		}
		else
		{
			if(m_silencePosition >= grainDensity)
			{
				m_silencePosition = 0;
				m_playingGrainMode = true;
			}
			p(p_outputL)[n] = 0;
			p(p_outputR)[n] = 0;
			m_silencePosition++;
		}
	}
}

static int _ = GranulatorStereo::register_class("http://avwlv2.sourceforge.net/plugins/avw/granulator_stereo");

