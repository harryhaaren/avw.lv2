#ifndef JACKLFO_H
#define JACKLFO_H

#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "jacklfo.peg"

using namespace lvtk;

enum WaveFormType
{
	SINUS, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN, RECTANGLE, SAMPLEANDHOLD
};

class JackLfo: public Plugin<JackLfo>
{
	private:
		float freq, phi0;
		double si, old_si;
		double sa, old_sa;
		double t, old_t;
		double r, old_r;
		double sh, old_sh;
		int state;
		double dt, wave_period;
		bool trigger;

		int waveForm;

		double m_rate;
		jack_client_t *client;

	public:
		JackLfo(double rate);
		void run(uint32_t nframes);
};

#endif
