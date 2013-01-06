#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "percussiveenv.hpp"

PercussiveEnv::PercussiveEnv(double rate): Plugin<PercussiveEnv>(p_n_ports)
{
	gate = false;
	retrigger = false;
	noteOnOfs = 0;
	e = 0;
	de = 0;
	e_noteOff = 0;

	m_rate = rate;
}

void PercussiveEnv::run(uint32_t nframes)
{
	int status;
	unsigned int l2;
	float tscale, de_attack, de_decay;
	float a, dl, dc, h;
	int idl, idla, idlah, idlahdc;

	tscale = *p(p_timeScale) * (float) m_rate;
	de_attack = (*p(p_attack) > 0) ? 1.0 / (*p(p_attack) * tscale) : 0;
	de_decay = (*p(p_decay) > 0) ? 1.0 / (*p(p_decay) * tscale) : 0;
	a = tscale * *p(p_attack);
	dl = tscale * *p(p_delay);
	idl = (int) dl;
	h = tscale * *p(p_hold);
	dc = tscale * *p(p_delay);
	idla = (int) (dl + a);
	idlah = idla + (int) h;
	if (idlah == idla)
	{
		++idlah;
	}

	idlahdc = idlah + (int) dc;
	for (l2 = 0; l2 < nframes; l2++)
	{
		if (!gate && (p(p_gate)[l2] > 0.5))
		{
			gate = true;
			if (e > 0)
			{
				noteOnOfs = -ENVELOPE_RESPONSE;
				de = e / (float) ENVELOPE_RESPONSE;
			}
			else
			{
				noteOnOfs = 0;
			}
		}
		if (gate && (p(p_gate)[l2] < 0.5))
		{
			gate = false;
			e_noteOff = e;
		}
		if (!retrigger && (p(p_retrigger)[l2] > 0.5))
		{
			retrigger = true;
			if (e > 0)
				noteOnOfs = (de_attack > 0) ? (int) (e / de_attack) : 0;
			else
				noteOnOfs = 0;
		}
		if (retrigger && (p(p_retrigger)[l2] < 0.5))
			retrigger = false;

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
			case 0:
				e -= de;
				break;
			case 1:
				e = 0;
				break;
			case 2:
				e += de_attack;
				break;
			case 3:
				e = 1.0;
				break;
			case 4:
				e -= de_decay;
				break;
			default:
				e = 0;
				break;
		}
		if (e < 0)
			e = 0;
		p(p_out)[l2] = e;
		p(p_invOut)[l2] = -e;
		noteOnOfs++;
	}
}

static int _ = PercussiveEnv::register_class("http://avwlv2.sourceforge.net/plugins/avw/percussiveenv");

