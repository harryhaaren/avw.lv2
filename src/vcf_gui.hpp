#ifndef VCF_GUI_H
#define VCF_GUI_H

using namespace sigc;
using namespace Gtk;

class VcfGUI: public LV2::GUI<VcfGUI>
{
public:
	VcfGUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

protected:
	Gtk::ComboBox* m_comboVCFType;
	Gtk::HScale* m_scaleInputGain;
	Gtk::HScale* m_scaleFrequency;
	Gtk::HScale* m_scaleExpFMGain;
	Gtk::HScale* m_scaleLinFMGain;
	Gtk::HScale* m_scaleResonance;
	Gtk::HScale* m_scaleResonanceGain;
};

#endif
