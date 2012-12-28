#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "amp.hpp"

using namespace lvtk;

Amp::Amp(double rate)
:Plugin<Amp>(p_n_ports)
 {
	gain=1;
 }

void Amp::run(uint32_t nframes)
{
	gain = *p(p_gain);
	unsigned int l2;
	for (l2 = 0; l2 < nframes; l2++)
	{
		p(p_output)[l2] = gain * p(p_input)[l2];
	}
}

static int _ = Amp::register_class("http://avwlv2.sourceforge.net/plugins/avw/amp");

