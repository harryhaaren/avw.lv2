#ifndef CVS_H
#define CVS_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/cvs";

enum p_port_enum
{
	p_cv1, p_cvFine1, p_cvOut1, p_cv2, p_cvFine2, p_cvOut2, p_cv3, p_cvFine3, p_cvOut3, p_cv4, p_cvFine4, p_cvOut4, p_n_ports
};

static const peg_data_t p_ports[] =
{
{ 0, 5, 0, 0, 0, 0 },
{ -0.5, 0.5, 0, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0, 5, 0, 0, 0, 0 },
{ -0.5, 0.5, 0, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0, 5, 0, 0, 0, 0 },
{ -0.5, 0.5, 0, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
{ 0, 5, 0, 0, 0, 0 },
{ -0.5, 0.5, 0, 0, 0, 0 },
{ -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 }, };

class Cvs: public Plugin<Cvs>
{
	public:
		Cvs(double rate);
		void run(uint32_t nframes);
};

#endif
