#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "controltovc.hpp"

using namespace lvtk;

ControlToVc::ControlToVc(double rate)
:Plugin<ControlToVc>(p_n_ports)
 {
 }

void ControlToVc::run(uint32_t nframes)
{
	unsigned int l2;
	for (l2 = 0; l2 < nframes; l2++)
	{
		p(p_output)[l2] = p(p_input)[l2] * 2 - 1;
	}
}

static int _ = ControlToVc::register_class("http://avwlv2.sourceforge.net/plugins/avw/controltovc");

