#ifndef BEATSLICER_MONO_H
#define BEATSLICER_MONO_H

#include <jack/jack.h>
#include <lvtk-1/lvtk/plugin.hpp>

#include "beatslicer_mono.peg"

using namespace lvtk;

class BeatSlicerMono: public Plugin<BeatSlicerMono, URID<true>>
{
	private:
		double m_rate;

		//jack_client_t *client;
		bool slicingAsked;bool stopSlicingAsked;bool sliceRecorded;bool slicing;bool reverse;
		int sliceCounter;
		int sliceSize;
		int maxSliceSize;
		double beatsPerMinute;
		jack_default_audio_sample_t *fadingTab;
		std::vector<jack_default_audio_sample_t> sliceSamples;
		float *triggerData;bool previousTrigger;

		void setSliceSize(double nbBeats);
		void clearSlice();
		void addSliceSample(jack_default_audio_sample_t in);
		jack_default_audio_sample_t getNextSliceSample();

	public:
		BeatSlicerMono(double rate);
		void run(uint32_t nframes);

	struct TranchesURIs
	{
		LV2_URID atom_Blank;
		LV2_URID atom_Float;
		LV2_URID atom_Path;
		LV2_URID atom_Resource;
		LV2_URID atom_Sequence;
		LV2_URID time_Position;
		LV2_URID time_barBeat;
		LV2_URID time_beatsPerMinute;
		LV2_URID time_speed;
	} uris;
};

#endif
