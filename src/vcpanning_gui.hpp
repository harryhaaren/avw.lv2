#ifndef VCPANNING_GUI_H
#define VCPANNING_GUI_H

using namespace sigc;
using namespace Gtk;

class VcPanningGUI: public LV2::GUI<VcPanningGUI>
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
