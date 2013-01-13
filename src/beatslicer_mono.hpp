#ifndef BEATSLICER_MONO_H
#define BEATSLICER_MONO_H

#include <deque>
#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "beatslicer_mono.peg"

using namespace lvtk;
using namespace std;

class BeatSlicerMono: public Plugin<BeatSlicerMono>
{
	private:
		double m_rate;

		deque<jack_default_audio_sample_t> m_sample;
		deque<jack_default_audio_sample_t> m_readingSample;
		int m_sampleFullSize;
		bool m_sampleFull;

		int m_readingSampleSize;
		int m_positionEnd;
		int m_readingPosition;

		bool m_reverse;

		bool m_gate;

		int m_tempo;
		float m_sampleSize;

		void giveMeReverse(int ReverseMode);

	public:
		BeatSlicerMono(double rate);
		void run(uint32_t nframes);
};

#endif
