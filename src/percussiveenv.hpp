#ifndef PERCUSSIVEENV_H
#define PERCUSSIVEENV_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "percussiveenv.peg"

using namespace lvtk;

#define ENVELOPE_RESPONSE               256

class PercussiveEnv: public Plugin<PercussiveEnv>
{
	private:
		//float delay, attack, hold, decay;
		float e_noteOff, de;

		double m_rate;

	public:
		float e;
		//float timeScale;
		bool gate, retrigger;
		int noteOnOfs;
		//float *gateData, *retriggerData;

		PercussiveEnv(double rate);
		void run(uint32_t nframes);
};

#endif
