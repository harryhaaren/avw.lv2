#ifndef LFO_H
#define LFO_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "lfo.peg"

using namespace lvtk;

enum WaveFormType
{
	SINUS, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN, RECTANGLE, SAMPLEANDHOLD
};

class Lfo: public Plugin<Lfo, URID<true>>
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

		float m_bpm;

		double m_rate;

	public:
		Lfo(double rate);
		void run(uint32_t nframes);

	struct LfoURIs {
			LV2_URID atom_Blank;
			LV2_URID atom_Float;
			LV2_URID atom_Path;
			LV2_URID atom_Resource;
			LV2_URID atom_Sequence;
			LV2_URID time_Position;
			LV2_URID time_barBeat;
			LV2_URID time_beatsPerMinute;
			LV2_URID time_speed;
	} uris;
};

#endif
