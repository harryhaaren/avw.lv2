#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/types.h>
#include <jack/transport.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "beatslicer_stereo.hpp"

using namespace lvtk;

BeatSlicerStereo::BeatSlicerStereo(double rate)
: Plugin<BeatSlicerStereo, URID<true>>(p_n_ports)
  {
	m_rate = rate;
	beatsPerMinute = 120;
	sliceCounter = 0;
	sliceSize = 0;
	maxSliceSize = 2048;
	slicingAsked = false;
	sliceRecorded = false;
	slicing = false;
	fadingTab = NULL;
	previousTrigger = false;
	setSliceSize(1);

	uris.atom_Blank          = map(LV2_ATOM__Blank);
	uris.atom_Float          = map(LV2_ATOM__Float);
	uris.atom_Path           = map(LV2_ATOM__Path);
	uris.atom_Resource       = map(LV2_ATOM__Resource);
	uris.atom_Sequence       = map(LV2_ATOM__Sequence);
	uris.time_Position       = map(LV2_TIME__Position);
	uris.time_barBeat        = map(LV2_TIME__barBeat);
	uris.time_beatsPerMinute = map(LV2_TIME__beatsPerMinute);
	uris.time_speed          = map(LV2_TIME__speed);
  }

void BeatSlicerStereo::run(uint32_t nframes)
{
	//GET THE PORTS

	//jackProcess* proc = (jackProcess*) arg;
	//int nbInputs = proc->inputPorts[0].size();
	//int nbOutputs = proc->outputPorts[0].size();

	//we get the input port(s)
	//jack_default_audio_sample_t *in[2];
	//in[0] = (jack_default_audio_sample_t *) jack_port_get_buffer(proc->inputPorts[0], nframes);
	//in[1] = (jack_default_audio_sample_t *) jack_port_get_buffer(proc->inputPorts[1], nframes);

	//and the output port(s)
	//jack_default_audio_sample_t *out[2];
	//out[0] = (jack_default_audio_sample_t *) jack_port_get_buffer(proc->outputPorts[0], nframes);
	//out[1] = (jack_default_audio_sample_t *) jack_port_get_buffer(proc->outputPorts[1], nframes);

	//put the outputs to 0
	for (unsigned int n = 0; n < nframes; n++) //for each sample
	{
		//out[0][n] = 0;
		//out[1][n] = 0;
		p(p_outputL)[n] = 0;
		p(p_outputR)[n] = 0;
	}

	triggerData = p(p_trigger);
	if (triggerData[0] > 0.5 != previousTrigger)
	{
		previousTrigger = !previousTrigger;
		if (triggerData[0] > 0.5)
		{
			slicingAsked = true;
		}
		else
		{
			stopSlicingAsked = true;
		}
	}
	reverse = *p(p_reverse) == 1;

	setSliceSize(*p(p_slice));

	//GET THE BEAT

	float p_bpm = 0;
	const LV2_Atom_Sequence* timing = p<LV2_Atom_Sequence> (p_control);
	uint32_t                 last_t = 0;
	for (LV2_Atom_Event* ev = lv2_atom_sequence_begin(&timing->body); !lv2_atom_sequence_is_end(&timing->body, timing->atom.size, ev); ev = lv2_atom_sequence_next(ev))
	{
		if (ev->body.type == uris.atom_Blank)
		{
			const LV2_Atom_Object* obj = (LV2_Atom_Object*)&ev->body;
			if (obj->body.otype == uris.time_Position)
			{
				LV2_Atom *bpm = NULL;
				lv2_atom_object_get(obj, uris.time_beatsPerMinute, &bpm, NULL);

				if (bpm && bpm->type == uris.atom_Float)
				{
					/* Tempo changed, update BPM */
					p_bpm = ((LV2_Atom_Float*)bpm)->body;
					std::cout << "tempo: " << p_bpm << std::endl;
				}
			}
		}

		/* Update time for next iteration and move to next event*/
		last_t = ev->time.frames;
		ev = lv2_atom_sequence_next(ev);
	}

	if(p_bpm>0 && p_bpm!=beatsPerMinute)
	{
		beatsPerMinute = p_bpm;
		setSliceSize(1);
	}

	//TEST THE REPEAT

	//we first test if we should stop
	if (stopSlicingAsked)
	{
		clearSlice();
	}

	//we then test if repeat has been pressed and if a new beat started : to wait for a new beat before starting repeating
	//if(proc->slicingAsked && currentBeat!=proc->previousBeat)
	if (slicingAsked)
	{
		slicing = true;
		slicingAsked = false;
		sliceRecorded = false;
	}

	//PROCESS

	//if we are in slicing mode
	if (slicing)
	{
		for (unsigned int n = 0; n < nframes; n++) //for each sample
		{
			//if recording the slice is not over
			if (!(sliceRecorded))
			{
				//we add all the mix of all theinputs to the slice
				jack_default_audio_sample_t in1 = 0;
				jack_default_audio_sample_t in2 = 0;

				in1 += p(p_inputL)[n];
				in2 += p(p_inputR)[n];

				addSliceSample(in1, in2);

				//and we play the sample in the current output
				p(p_outputL)[n] = in1;
				p(p_outputR)[n] = in2;
			}
			else //if it is over
			{
				//we get the sample from the slice
				jack_default_audio_sample_t s1 = getNextSliceSample(1);
				jack_default_audio_sample_t s2 = getNextSliceSample(2);

				//and we play the sample in the current output
				p(p_outputL)[n] = s1;
				p(p_outputR)[n] = s2;
			}
		}
	}
	else //normal mode
	{
		//we fill all the outputs

		for (unsigned int n = 0; n < nframes; n++) //for each sample
		{
			p(p_outputL)[n] = p(p_inputL)[n];
			p(p_outputR)[n] = p(p_inputR)[n];
		}
	}
}

void BeatSlicerStereo::setSliceSize(double nbBeats)
{
	int fadeTime = 100;

	//we compute the new sliceSize
	int nsize = (int) floor(nbBeats / (beatsPerMinute / 60.0) * (double) m_rate);

	//we change the sliceSize if possible
	if (slicing)
	{
		if (sliceRecorded)
		{
			sliceSize = (nsize <= maxSliceSize) ? nsize : maxSliceSize;

			//and we refill the tab
			for (int i = 0; i < fadeTime; ++i)
			{
				fadingTab[i] = fadingTab[sliceSize - 1 - i] = double(i) / double(fadeTime);
			}
			for (int i = fadeTime; i <= sliceSize - fadeTime; ++i)
			{
				fadingTab[i] = 1.0;
			}
		}
	}
	else
	{
		sliceSize = nsize;

		//we rebuild the fading tab
		if (fadingTab)
			delete (fadingTab);
		fadingTab = new jack_default_audio_sample_t[sliceSize];

		//and we fill it
		for (int i = 0; i < fadeTime; ++i)
		{
			fadingTab[i] = fadingTab[sliceSize - 1 - i] = double(i) / double(fadeTime);
		}
		for (int i = fadeTime; i <= sliceSize - fadeTime; ++i)
		{
			fadingTab[i] = 1.0;
		}
	}

	//we put the sliceCounter at the right place
	sliceCounter = sliceCounter % sliceSize;
}

void BeatSlicerStereo::clearSlice()
{
	slicingAsked = false;
	stopSlicingAsked = false;
	slicing = false;
	sliceSamplesL.clear();
	sliceSamplesR.clear();
}

void BeatSlicerStereo::addSliceSample(jack_default_audio_sample_t in1, jack_default_audio_sample_t in2)
{
	//we add the sample
	sliceSamplesL.push_back(in1);
	sliceSamplesR.push_back(in2);
	//test with the size of the repeat
	//if size reached, slice is recorded
	if (sliceSamplesL.size() >= (unsigned int) sliceSize)
	{
		maxSliceSize = sliceSize;
		sliceRecorded = true;
		sliceCounter = 0;
	}
}

jack_default_audio_sample_t BeatSlicerStereo::getNextSliceSample(int channel)
{
	//return the sample faded in/out to avoid clicks
	jack_default_audio_sample_t res;
	if (channel == 1)
		res = sliceSamplesL[sliceCounter] * fadingTab[sliceCounter];
	else
	{
		res = sliceSamplesR[sliceCounter] * fadingTab[sliceCounter];
		//counter ++
		if (reverse)
			sliceCounter = (sliceCounter <= 0) ? (sliceSize - 1) : sliceCounter - 1;
		else
			sliceCounter = (sliceCounter >= (sliceSize - 1)) ? 0 : sliceCounter + 1;
	}
	return res;
}

static int _ = BeatSlicerStereo::register_class("http://avwlv2.sourceforge.net/plugins/avw/beatslicer_stereo");

