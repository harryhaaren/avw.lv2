#ifndef JACKLFO_GUI_H
#define JACKLFO_GUI_H

using namespace sigc;
using namespace Gtk;

class JackLfoGUI: public LV2::GUI<JackLfoGUI>
{
public:
	JackLfoGUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

protected:
	Gtk::ComboBox* m_comboWaveForm;
	Gtk::HScale* m_scaleFrequency;
	Gtk::HScale* m_scalePhi0;
};

#endif
