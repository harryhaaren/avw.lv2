#ifndef BEATSLICER_STEREO_GUI_H
#define BEATSLICER_STEREO_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "labeleddial.hpp"

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

class BeatSlicerStereoGUI: public UI<BeatSlicerStereoGUI, GtkUI<true>>
{
	public:
		BeatSlicerStereoGUI(const std::string& URI);
		void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

	protected:
		Gtk::CheckButton* m_checkReverse;
		LabeledDial* m_dialSizeSlice;

		float get_sizeSlice();
};

#endif
