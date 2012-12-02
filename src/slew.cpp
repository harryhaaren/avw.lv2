#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "synthdata.hpp"

#include "slew.hpp"

Slew::Slew(double rate):
Plugin<Slew>(p_n_ports)
{
	//QString qs;
	//int l1;

	//setGeometry(MODULE_NEW_X, MODULE_NEW_Y, MODULE_SLEW_WIDTH, MODULE_SLEW_HEIGHT);
	//port_M_in = new Port(tr("In"), PORT_IN, 0, this);
	//cv.out_off = 55;
	//port_out = new Port(tr("Out"), PORT_OUT, 0, this);
	//timeUp = 0.5;
	//timeDown = 0.5;
	//configDialog->addSlider(tr("Time Up"), timeUp, 0, 10);
	//configDialog->addSlider(tr("Time Down"), timeDown, 0, 10);
	//for (l1 = 0; l1 < synthdata->poly; l1++) {
	//	lastData[l1] = 0;
	//}

	lastData = 0;
	m_rate = rate;
}

void Slew::run(uint32_t nframes)
{
	unsigned int l2;
	float ds, slewUp, slewDown;

	inData = p(p_in);

	if (*p(p_timeUp) > 0.0001)
	{
		slewUp = 1.0 / (*p(p_timeUp) * (float)m_rate);
	}
	else
	{
		slewUp = 1.0 / (0.0001 * (float)m_rate);
	}
	if (*p(p_timeDown) > 0.0001)
	{
		slewDown = -1.0 / (*p(p_timeDown) * (float)m_rate);
	}
	else
	{
		slewDown = -1.0 / (0.0001 * (float)m_rate);
	}

	for (l2 = 0; l2 < nframes; l2++)
	{
		ds = inData[l2] - lastData;
		if (ds > 0)
		{
			if (ds > slewUp) ds = slewUp;
		}
		else
		{
			if (ds < slewDown) ds = slewDown;
		}
		p(p_out)[l2] = lastData + ds;
		lastData = p(p_out)[l2];
	}
}

static int _ = Slew::register_class("http://avwlv2.sourceforge.net/plugins/avw/slew");

