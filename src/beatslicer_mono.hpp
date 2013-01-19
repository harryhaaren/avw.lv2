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

		float *m_envelope;
		int m_attack, m_release;

		float *m_fadeIn;
		float *m_fadeOut;
		int m_fadePosition;

		deque<jack_default_audio_sample_t> m_sample;
		deque<jack_default_audio_sample_t> m_readingSample;
		int m_sampleFullSize;
		bool m_sampleFull;

		int m_readingSampleSize;
		int m_positionStart;
		int m_readingPosition;
		int m_calculatedReadingPosition;

		bool m_reverse;

		bool m_gate;
		bool m_slicing;

		int m_tempo;
		int m_sampleSize;
		float m_sliceSize;

		void giveMeReverse(int ReverseMode);

	public:
		BeatSlicerMono(double rate);
		void run(uint32_t nframes);
};

#endif
