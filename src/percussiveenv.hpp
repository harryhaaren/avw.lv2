#ifndef PERCUSSIVEENV_H
#define PERCUSSIVEENV_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"
#include "percussiveenv.peg"

using namespace lvtk;

#define ENVELOPE_RESPONSE               256

class PercussiveEnv: public Plugin<PercussiveEnv>
{
	private:
		float delay, attack, hold, decay;
		float e_noteOff, de;
		//Port *port_gate, *port_retrigger, *port_inverse_out, *port_gain_out;

		double m_rate;

	public:
		float e;
		float timeScale;bool gate, retrigger;
		int noteOnOfs;
		float *gateData, *retriggerData;

		PercussiveEnv(double rate);
		void run(uint32_t nframes);
};

#endif
