#ifndef BEATSLICER_MONO_H
#define BEATSLICER_MONO_H

#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "beatrepeater_stereo.peg"

using namespace lvtk;
using namespace std;

class BeatRepeaterStereo: public Plugin<BeatRepeaterStereo>
{
	private:
		double m_rate;

		jack_default_audio_sample_t *m_envelope;
		jack_default_audio_sample_t *m_envelopeRec;
		jack_default_audio_sample_t *m_attackEnv;
		jack_default_audio_sample_t *m_releaseEnv;

		jack_default_audio_sample_t* m_sampleL;
		jack_default_audio_sample_t* m_sampleR;
		int m_sampleFullSize;
		bool m_sampleFull;

		bool m_gate;

		int m_sampleRepeatingPosition;
		int m_sampleRecordingPosition;
		int m_attackEnvPosition;
		int m_releaseEnvPosition;

		int m_tempo;
		float m_beatSize;

		int m_attack, m_release;

	public:
		BeatRepeaterStereo(double rate);
		void run(uint32_t nframes);
};

#endif
