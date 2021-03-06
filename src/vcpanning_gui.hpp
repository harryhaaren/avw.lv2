#ifndef VCPANNING_GUI_H
#define VCPANNING_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "labeleddial.hpp"

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

class VcPanningGUI: public UI<VcPanningGUI, GtkUI<true>>
{
	public:
		VcPanningGUI(const std::string& URI);
		void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

	protected:
		Gtk::ComboBoxText* m_comboPanningMode;

		LabeledDial* m_scalePanOffset;
		LabeledDial* m_scalePanGain;

		float get_panOffset();
		float get_panGain();
};

#endif
