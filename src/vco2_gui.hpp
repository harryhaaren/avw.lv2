#ifndef VCO2_GUI_H
#define VCO2_GUI_H

using namespace sigc;
using namespace Gtk;

class Vco2GUI: public LV2::GUI<Vco2GUI>
{
public:
	Vco2GUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

protected:
	Gtk::ComboBox* m_comboWaveForm;
	Gtk::HScale* m_scaleOctave;
	Gtk::HScale* m_scaleTune;
	Gtk::HScale* m_scaleHarmonic;
	Gtk::HScale* m_scaleSubHarmonic;
	Gtk::HScale* m_scalePW;
	Gtk::HScale* m_scalePWGain;
	Gtk::HScale* m_scalePhi0;
	Gtk::HScale* m_scaleExpFMGain;
	Gtk::HScale* m_scaleLinFMGain;
};

#endif
