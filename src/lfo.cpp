#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "synthdata.hpp"
#include "lfo.hpp"

using namespace lvtk;

Lfo::Lfo(double rate)
: Plugin<Lfo, URID<true>>(p_n_ports)
  {
	long tm;

	freq = 5;
	phi0 = 0;
	wave_period = rate/ (16.0 * freq);
	m_rate = rate;

	trigger = false;
	si = 0;
	old_si = 0;
	sa = -1;
	old_sa = 0;
	t = 0;
	old_t = 0;
	r = 0;
	old_r = 0;
	old_sh = 0;
	sh = 0;
	state = 0;
	dt = 4.0 / wave_period;

	m_bpm = 0;

	waveForm = SINUS;

	tm = time(NULL) % 1000000;
	srand(abs(tm - 10000 * (tm % 100)));

	uris.atom_Blank          = map(LV2_ATOM__Blank);
	uris.atom_Float          = map(LV2_ATOM__Float);
	uris.atom_Path           = map(LV2_ATOM__Path);
	uris.atom_Resource       = map(LV2_ATOM__Resource);
	uris.atom_Sequence       = map(LV2_ATOM__Sequence);
	uris.time_Position       = map(LV2_TIME__Position);
	uris.time_barBeat        = map(LV2_TIME__barBeat);
	uris.time_beatsPerMinute = map(LV2_TIME__beatsPerMinute);
	uris.time_speed          = map(LV2_TIME__speed);
  }

void Lfo::run(uint32_t nframes)
{
	if(*p(p_sync) == 1)
	{
		float p_bpm = 0;
		const LV2_Atom_Sequence* timing = p<LV2_Atom_Sequence> (p_control);
		uint32_t                 last_t = 0;
		for (LV2_Atom_Event* ev = lv2_atom_sequence_begin(&timing->body); !lv2_atom_sequence_is_end(&timing->body, timing->atom.size, ev); ev = lv2_atom_sequence_next(ev))
		{
			if (ev->body.type == uris.atom_Blank)
			{
				const LV2_Atom_Object* obj = (LV2_Atom_Object*)&ev->body;
				if (obj->body.otype == uris.time_Position)
				{
					LV2_Atom *bpm = NULL;
					lv2_atom_object_get(obj, uris.time_beatsPerMinute, &bpm, NULL);

					if (bpm && bpm->type == uris.atom_Float)
					{
						/* Tempo changed, update BPM */
						p_bpm = ((LV2_Atom_Float*)bpm)->body;
						std::cout << "tempo: " << p_bpm << std::endl;
					}
				}
			}

			/* Update time for next iteration and move to next event*/
			last_t = ev->time.frames;
			ev = lv2_atom_sequence_next(ev);
		}

		if(p_bpm>0)
		{
			m_bpm = p_bpm;
		}

		freq = *p(p_temp_mul) * m_bpm / 60;
	}
	else
	{
		freq = *p(p_frequency);
	}

	int l2, k, len, phi0i, l2_out;
	double ldsi, ldsa, ldt, ldr, ldsh, dt0, dsa;


	phi0 = *p(p_phi0);
	waveForm = *p(p_waveForm);

	float *triggerData = p(p_reset);

	wave_period = m_rate / (16.0 * freq);
	dsa = 2.0 / wave_period;
	dt0 = 4.0 / wave_period;
	phi0i = (int)(phi0 / 6.283 * wave_period);

	len = nframes;
	l2 = -1;
	l2_out = 0;
	do
	{
		k = (len > 24) ? 16 : len;
		l2 += k;
		if (!trigger && (triggerData[l2] > 0.5))
		{
			trigger = true;
			t = 0;
			state = 0;
			dt = dt0;
			r = -1;
			si = 0;
		}
		if (trigger && (triggerData[l2] < 0.5))
		{
			trigger = false;
		}

		if (t >= 1.0)
		{
			state = 1;
			dt = -dt0;
		}
		else if (t <= -1.0)
		{
			state = 3;
			dt = dt0;
		}
		else if ((state == 1) && (t < 0))
		{
			state = 2;
			r = 1;
			sh = 2.0 * (double)rand() / (double)RAND_MAX - 1.0;
		}
		else if ((state == 3) && (t > 0))
		{
			state = 0;
			r = -1;
			sh = 2.0 * (double)rand() / (double)RAND_MAX - 1.0;
			sa = -1;
		}

		si = (state < 2) ? t * (2.0 - t) : t * (2.0 + t);
		sa += dsa;
		t += dt;
		len -= k;
		ldsi = (si - old_si) / (double)k;
		ldsa = (sa - old_sa) / (double)k;
		ldsh = (sh - old_sh) / (double)k;
		ldt = (t - old_t) / (double)k;
		ldr = (r - old_r) / (double)k;
		while (k--)
		{
			old_si += ldsi;
			old_sa += ldsa;
			old_sh += ldsh;
			old_t += ldt;
			old_r += ldr;

			switch (waveForm)
			{
				case SINUS:
					p(p_output)[l2_out] = old_si;
					break;
				case TRIANGLE:
					p(p_output)[l2_out] = old_t;
					break;
				case SAWTOOTHUP:
					p(p_output)[l2_out] = old_sa;
					break;
				case SAWTOOTHDOWN:
					p(p_output)[l2_out] = -old_sa;
					break;
				case RECTANGLE:
					p(p_output)[l2_out] = -old_r;
					break;
				case SAMPLEANDHOLD:
					p(p_output)[l2_out] = old_sh;
					break;
			}

			l2_out++;
		}
	} while(len);
}

static int _ = Lfo::register_class("http://avwlv2.sourceforge.net/plugins/avw/lfo");
