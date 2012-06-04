#ifndef AD_GUI_H
#define AD_GUI_H

using namespace sigc;
using namespace Gtk;

class AdGUI: public LV2::GUI<AdGUI>
{
public:
	AdGUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

protected:
	Gtk::HScale* m_scaleDetuneAmplitude;
	Gtk::HScale* m_scaleDetuneModulation;
	Gtk::HScale* m_scaleDetuneRate;
	Gtk::HScale* m_scaleDriftAmplitude;
	Gtk::HScale* m_scaleDriftModulation;
	Gtk::HScale* m_scaleDriftRate;
};

#endif
