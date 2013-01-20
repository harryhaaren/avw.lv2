#ifndef GRANULATOR_STEREO_H
#define GRANULATOR_STEREO_H

#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "granulator_stereo.peg"

using namespace lvtk;

class GranulatorStereo: public Plugin<GranulatorStereo>
{
	public:
		GranulatorStereo(double rate);
		void run(uint32_t nframes);

	private:
		double m_rate;

		int m_lastGrainSize;

		int m_lastAttack, m_lastRelease;
		float* m_envelope;

		int m_recordingGrainPosition;
		jack_default_audio_sample_t *m_recordingGrainL, *m_recordingGrainR;

		bool m_playingGrainMode;

		int m_grainIndex, m_grainPosition;
		jack_default_audio_sample_t *m_playingGrainL, *m_playingGrainR;

		int m_silencePosition;

		std::vector<jack_default_audio_sample_t*> m_grainsL, m_grainsR;
		std::vector<int> m_sizeGrains;
};

#endif
