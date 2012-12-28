#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "env.hpp"

Env::Env(double rate):
Plugin<Env>(p_n_ports)
{
	delay = 0;
	attack = 0.05;
	hold = 0.02;
	decay = 0.1;
	sustain = 0.7;
	release = 0.05;
	timeScale = 1.0;

	gate = false;
	retrigger = false;
	noteOnOfs = 0;
	e = 0;
	de = 0;
	e_noteOff = 0;

	m_rate = rate;
}

void Env::run(uint32_t nframes)
{
	int status;
	unsigned int l2;
	float tscale, de_attack, de_decay, de_release;
	float a, dl, dc, h;
	int idl, idla, idlah, idlahdc;

	gateData = p(p_gate);
	retriggerData = p(p_retrigger);

	delay = *p(p_delay);
	attack = *p(p_attack);
	hold = *p(p_hold);
	decay = *p(p_decay);
	sustain = *p(p_sustain);
	release = *p(p_release);
	timeScale = *p(p_timeScale);

	tscale = timeScale * (float)m_rate;
	de_attack = (attack > 0) ? 1.0 / (attack * tscale) : 0;
	de_decay = (decay > 0) ? (1.0 - sustain) / (decay * tscale) : 0;
	a = tscale * attack;
	dl = tscale * delay;
	idl = (int)dl;
	h = tscale * hold;
	dc = tscale * decay;
	idla = (int)(dl + a);
	idlah = idla + (int)h;
	if (idlah == idla)
	{
		++idlah;
	}

	idlahdc = idlah + (int)dc;
	for (l2 = 0; l2 < nframes; l2++)
	{
		if (!gate && (gateData[l2] > 0.5))
		{
			gate = true;
			if (e > 0)
			{
				noteOnOfs = -ENVELOPE_RESPONSE;
				de = e / (float)ENVELOPE_RESPONSE;
			}
			else
			{
				noteOnOfs = 0;
			}
		}
		if (gate && (gateData[l2] < 0.5))
		{
			gate = false;
			e_noteOff = e;
		}
		if (!retrigger && (retriggerData[l2] > 0.5))
		{
			retrigger = true;
			if (e > 0)
				noteOnOfs = (de_attack > 0) ? (int)(e / de_attack) : 0;
			else
				noteOnOfs = 0;
		}
		if (retrigger && (retriggerData[l2] < 0.5))
			retrigger = false;

		if (gate)
		{
			status = 1;
			if (noteOnOfs < 0)
				status = 0;
			if (noteOnOfs >= idl)
				status = 2;
			if (noteOnOfs >= idla)
				status = 3;
			if (noteOnOfs >= idlah)
				status = 4;
			if (noteOnOfs >= idlahdc)
				status = 5;
			switch (status)
			{
				case 0: e -= de;
				break;
				case 1: e = 0;
				break;
				case 2: e += de_attack;
				break;
				case 3: e = 1.0;
				break;
				case 4: e -= de_decay;
				break;
				case 5: e = sustain;
				break;
				default: e = 0;
				break;
			}
			if (e < 0)
				e = 0;
			p(p_out)[l2] = e;
			p(p_invOut)[l2] = -e;
			noteOnOfs++;
		}
		else
		{                          // Release
			de_release = (release > 0) ? e_noteOff / (release * tscale) : 0;
			e -= de_release;

			if ((release == 0) || (e < 0))
				e = 0;

			p(p_out)[l2] = e;
			p(p_invOut)[l2] = -e;
		}
	}
}

static int _ = Env::register_class("http://avwlv2.sourceforge.net/plugins/avw/env");

