#ifndef LFO_H
#define LFO_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/lfo";

enum p_port_enum
{
	p_reset, p_frequency, p_phi0, p_sine, p_triangle, p_sawUp, p_sawDown, p_rectangle, p_sampleAndHold, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ -1, 1, 0, 0, 0, 0 },
{ 0, 100, 5, 0, 0, 0 },
{ 0, 6.28, 0, 0, 0, 0 },
{ -1, 1, 0, 0, 0, 0 },
{ -1, 1, 0, 0, 0, 0 },
{ -1, 1, 0, 0, 0, 0 },
{ -1, 1, 0, 0, 0, 0 },
{ -1, 1, 0, 0, 0, 0 },
{ -1, 1, 0, 0, 0, 0 }, };

class Lfo: public Plugin<Lfo>
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
		//Port *port_M_trigger, *port_sine, *port_tri, *port_sawup, *port_sawdown, *port_rect, *port_sh;

		double m_rate;

	public:
		Lfo(double rate);
		void run(uint32_t nframes);
};

#endif
