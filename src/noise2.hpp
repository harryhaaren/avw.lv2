#ifndef NOISE2_H
#define NOISE2_H

#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/noise2";

enum p_port_enum {
  p_noiseType,
  p_rate,
  p_level,
  p_out,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { 0, 2, 0, 0, 0, 0 },
  { 0, 10, 5, 0, 0, 0 },
  { 0, 1.1, 0.5, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};

enum Noises
{
	WHITE, RAND, PINK
};

class Noise2: public Plugin<Noise2>
{
	private:
		int NoiseType;
		unsigned int count;
		float rate, level;
		float buf[3], r;
		//Port *port_white, *port_pink, *port_random;
		float randmax;

	public:
		Noise2(double rate);
		void run(uint32_t nframes);
};

#endif
