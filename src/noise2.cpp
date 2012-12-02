#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"
#include "noise2.hpp"

Noise2::Noise2(double rate)
: Plugin<Noise2>(p_n_ports)
  {
	int l2;
	long t;

	rate = 5;
	level = 0.5;
	count = 0;
	NoiseType=WHITE;
	randmax = 2.0f / (float)RAND_MAX;

	//configDialog->addComboBox(tr("Noise Type"), NoiseType, noiseNames);
	//configDialog->addSlider(tr("Random Rate"), rate, 0, 10);
	//configDialog->addSlider(tr("Random Level"), level, 0, 1);
	r = 0;
	for (l2 = 0; l2 < 3; ++l2)
	{
		buf[l2] = 0;
	}
	t = time(NULL) % 1000000;
	srand(abs(t - 10000 * (t % 100)));
  }

void Noise2::run(uint32_t nframes)
{
	unsigned int l2;
	float white_noise;

	NoiseType = floor(*p(p_noiseType));
	rate = *p(p_rate);
	level = *p(p_level);

	switch (NoiseType)
	{
	case WHITE:
	{
		for (l2 = 0; l2 < nframes; ++l2)
		{
			white_noise = rand() * randmax - 1.0f;
			p(p_out)[l2] = white_noise;
		}
	}
	break;
	case RAND:
	{
		unsigned int random_rate = (unsigned int)(5000.0 * (double)rate + 100.0);
		for (l2 = 0; l2 < nframes; ++l2)
		{
			count++;
			if (count > random_rate)
			{
				count = 0;
				r = level * rand() * randmax - 1.0f;
			}
			p(p_out)[l2] = r;
		}
	}
	break;
	case PINK:
	{

		for (l2 = 0; l2 < nframes; ++l2)
		{
			white_noise = rand() * randmax - 1.0f;

			buf[0] = 0.99765f * buf[0] + white_noise * 0.099046f;
			buf[1] = 0.963f * buf[1] + white_noise * 0.2965164f;
			buf[2] = 0.57f * buf[2] + white_noise * 1.0526913f;

			p(p_out)[l2] = buf[0] + buf[1] + buf[2] + white_noise * 0.1848f;
		}
	}
	break;
	}
}

static int _ = Noise2::register_class("http://avwlv2.sourceforge.net/plugins/avw/noise2");

