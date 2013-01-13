#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/types.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "beatslicer_mono.hpp"

using namespace lvtk;

BeatSlicerMono::BeatSlicerMono(double rate)
: Plugin<BeatSlicerMono>(p_n_ports)
  {
	m_rate = rate;

	// 60 / tempo * rate * sampleSize
	m_sampleFullSize = int(60 * m_rate * 4 / 120 + 0.5);

	m_gate = false;
  }

void BeatSlicerMono::run(uint32_t nframes)
{
	int tempo = int(*p(p_tempo) + 0.5);
	if(tempo < 40)
	{
		tempo = 40;
	}

	int sampleSize = int(*p(p_sampleSize));
	if(sampleSize < 2)
	{
		sampleSize = 2;
	}

	if(sampleSize != m_sampleSize || tempo != m_tempo)
	{
		m_sampleSize = sampleSize;
		m_tempo = tempo;
		m_sampleFullSize = int(60 * m_rate * m_sampleSize / m_tempo + 0.5);
		m_sample.clear();
		m_sampleFull = false;
	}

	for (unsigned int n = 0; n < nframes; n++)
	{
		m_sample.push_back(float(p(p_input)[n]));
		while(m_sample.size() > m_sampleFullSize)
		{
			m_sample.pop_front();
			m_sampleFull = true;
		}

		if (!m_gate && (p(p_gate)[n] > 0.5))
		{
			m_gate = true;
			if(m_sampleFull)
			{
				m_readingSample = m_sample;
			}
			m_readingSampleSize = int(m_sampleFullSize * *p(p_sliceSize) + 0.5);
			giveMeReverse(int(*p(p_reverseMode)));
		}
		else if(m_gate && (p(p_gate)[n] < 0.5))
		{
			m_gate = false;
		}

		if(m_gate && m_sampleFull)
		{
			p(p_output)[n] = m_readingSample[m_readingPosition];
			if(m_reverse)
			{
				m_readingPosition--;
				if(m_readingPosition < m_positionEnd)
				{
					giveMeReverse(int(*p(p_reverseMode)));
				}
			}
			else
			{
				m_readingPosition++;
				if(m_readingPosition > m_positionEnd)
				{
					giveMeReverse(int(*p(p_reverseMode)));
				}
			}
		}
		else
		{
			p(p_output)[n] = p(p_input)[n];
		}
	}
}

void BeatSlicerMono::giveMeReverse(int ReverseMode)
{
	switch(ReverseMode)
	{
		case 0:
			m_reverse = false;
			break;
		case 1:
			m_reverse = ((rand() % 2) == 0);
			break;
		case 2:
			m_reverse = true;
			break;
		default:
			m_reverse = false;
			break;
	}
	if(m_reverse)
	{
		m_positionEnd = (rand() % (int(1 / *p(p_sliceSize)))) * m_readingSampleSize;
		m_readingPosition = m_positionEnd + m_readingSampleSize - 1;
	}
	else
	{
		m_readingPosition = (rand() % (int(1 / *p(p_sliceSize)))) * m_readingSampleSize;
		m_positionEnd = m_readingPosition + m_readingSampleSize - 1;
	}
}

static int _ = BeatSlicerMono::register_class("http://avwlv2.sourceforge.net/plugins/avw/beatslicer_mono");

