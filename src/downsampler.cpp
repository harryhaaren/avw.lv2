#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "downsampler.hpp"

using namespace lvtk;

Downsampler::Downsampler(double rate)
:Plugin<Downsampler>(p_n_ports)
 {
	ratio = 1;
	p_loop = 0;
	p_lastInput = 0;
 }

void Downsampler::run(uint32_t nframes)
{
	ratio = *p(p_ratio);
	unsigned int l2;
	for (l2 = 0; l2 < nframes; l2++)
	{
		p_loop++;
		if(p_loop >= ratio)
		{
			p_loop = 0;
			p_lastInput = p(p_input)[l2];
		}
		p(p_output)[l2] = p_lastInput;
	}
}

static int _ = Downsampler::register_class("http://avwlv2.sourceforge.net/plugins/avw/downsampler");

