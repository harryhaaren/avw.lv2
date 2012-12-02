#ifndef TRANCHES_H
#define TRANCHES_H

#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "tranches.peg"

using namespace lvtk;

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
