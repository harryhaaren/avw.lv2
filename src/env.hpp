#ifndef ENV_H
#define ENV_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "env.peg"

using namespace lvtk;

#define ENVELOPE_RESPONSE               256

class Env: public Plugin<Env>
{
	private:
		float delay, attack, hold, decay, sustain, release;
		float e_noteOff, de;

		double m_rate;

	public:
		float e;
		float timeScale;
		bool gate, retrigger;
		int noteOnOfs;
		float *gateData, *retriggerData;

		Env(double rate);
		void run(uint32_t nframes);
};

#endif
