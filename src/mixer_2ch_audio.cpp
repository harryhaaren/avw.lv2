#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "mixer_2ch_audio.hpp"

using namespace lvtk;

Mixer2ChAudio::Mixer2ChAudio(double rate)
: Plugin<Mixer2ChAudio>(p_n_ports)
  {
  }

void Mixer2ChAudio::run(uint32_t nframes)
{
	unsigned int l2;
	float mixgain;

	mixgain = *p(p_gain) * *p(p_volume1);
	for (l2 = 0; l2 < nframes; l2++)
	{
		p(p_out)[l2]  = mixgain * p(p_in1)[l2];
	}

	mixgain = *p(p_gain) * *p(p_volume2);
	for (l2 = 0; l2 < nframes; l2++)
	{
		p(p_out)[l2]  += mixgain * p(p_in2)[l2];
	}
}

static int _ = Mixer2ChAudio::register_class("http://avwlv2.sourceforge.net/plugins/avw/mixer_2ch_audio");

