#ifndef LFO_GUI_H
#define LFO_GUI_H

using namespace sigc;
using namespace Gtk;

class LfoGUI: public LV2::GUI<LfoGUI>
{
public:
	LfoGUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

protected:
	Gtk::ComboBox* m_comboWaveForm;
	Gtk::HScale* m_scaleFrequency;
	Gtk::HScale* m_scalePhi0;
};

#endif
