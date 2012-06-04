#ifndef TRANCHES_H
#define TRANCHES_H

#include <jack/jack.h>
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/tranches";

enum p_port_enum {
  p_trigger,
  p_inputL,
  p_inputR,
  p_outputL,
  p_outputR,
  p_slice,
  p_reverse,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
};

class Tranches: public Plugin<Tranches>
{
	private:
		double m_rate;

		jack_client_t *client;bool slicingAsked;bool stopSlicingAsked;bool sliceRecorded;bool slicing;bool reverse;
		int sliceCounter;
		int sliceSize;
		int maxSliceSize;
		int previousBeat;
		double beatsPerMinute;
		jack_default_audio_sample_t *fadingTab;
		std::vector<jack_default_audio_sample_t> sliceSamplesL;
		std::vector<jack_default_audio_sample_t> sliceSamplesR;
		float *triggerData;bool previousTrigger;

		void setSliceSize(double nbBeats);
		void clearSlice();
		void addSliceSample(jack_default_audio_sample_t in1, jack_default_audio_sample_t in2);
		jack_default_audio_sample_t getNextSliceSample(int channel);

	public:
		Tranches(double rate);
		void run(uint32_t nframes);
};

#endif
