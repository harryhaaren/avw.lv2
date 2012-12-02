#ifndef VCPANNING_GUI_H
#define VCPANNING_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

class VcPanningGUI: public UI<VcPanningGUI, GtkUI<true>>
{
public:
	VcPanningGUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

protected:
	Gtk::ComboBox* m_comboPanningMode;
	Gtk::HScale* m_scalePanOffset;
	Gtk::HScale* m_scalePanGain;
};

#endif
