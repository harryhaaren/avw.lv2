#ifndef GRANULATOR_MONO_H
#define GRANULATOR_MONO_H

#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "granulator_mono.peg"

using namespace lvtk;

class GranulatorMono: public Plugin<GranulatorMono>
{
	public:
		GranulatorMono(double rate);
		void run(uint32_t nframes);

	private:
		double m_rate;

		int m_lastGrainSize;

		int m_lastAttack, m_lastRelease;
		jack_default_audio_sample_t *m_envelope;

		int m_recordingGrainPosition;
		jack_default_audio_sample_t *m_recordingGrain;

		bool m_playingGrainMode;

		int m_grainIndex, m_grainPosition;
		jack_default_audio_sample_t *m_playingGrain;

		int m_silencePosition;

		std::vector<jack_default_audio_sample_t*> m_grains;
		std::vector<int> m_sizeGrains;
};

#endif
