#include <gtkmm-2.4/gtkmm.h>

#include "lv2plugin.hpp"
#include "lv2gui.hpp"
#include "vco2_gui.hpp"
#include "vco2.hpp"

Vco2GUI::Vco2GUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/vco2_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("vco2_window", p_window);

	Gtk::Widget* p_mainWidget = 0;
	builder->get_widget("notebookMain", p_mainWidget);

	p_window->remove();

	add(*p_mainWidget);

	m_comboWaveForm = 0;
	builder->get_widget("comboboxtextWaveForm", m_comboWaveForm);
	slot<void> p_slotWaveForm = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_waveForm), mem_fun(*m_comboWaveForm, &ComboBoxText::get_active_row_number));
	m_comboWaveForm->signal_changed().connect(p_slotWaveForm);

	m_scaleOctave = 0;
	builder->get_widget("scaleOctave", m_scaleOctave);
	slot<void> p_slotOctave = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_octave), mem_fun(*m_scaleOctave, &HScale::get_value));
	m_scaleOctave->signal_value_changed().connect(p_slotOctave);

	m_scaleTune = 0;
	builder->get_widget("scaleTune", m_scaleTune);
	slot<void> p_slotTune = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_tune), mem_fun(*m_scaleTune, &HScale::get_value));
	m_scaleTune->signal_value_changed().connect(p_slotTune);

	m_scaleHarmonic = 0;
	builder->get_widget("scaleHarmonic", m_scaleHarmonic);
	slot<void> p_slotHarmonic = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_harmonic), mem_fun(*m_scaleHarmonic, &HScale::get_value));
	m_scaleHarmonic->signal_value_changed().connect(p_slotHarmonic);

	m_scaleSubHarmonic = 0;
	builder->get_widget("scaleSubharmonic", m_scaleSubHarmonic);
	slot<void> p_slotSubHarmonic = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_subharmonic), mem_fun(*m_scaleSubHarmonic, &HScale::get_value));
	m_scaleSubHarmonic->signal_value_changed().connect(p_slotSubHarmonic);

	m_scalePW = 0;
	builder->get_widget("scalePW", m_scalePW);
	slot<void> p_slotPW = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_pw0), mem_fun(*m_scalePW, &HScale::get_value));
	m_scalePW->signal_value_changed().connect(p_slotPW);

	m_scalePWGain = 0;
	builder->get_widget("scalePWGain", m_scalePWGain);
	slot<void> p_slotPWGain = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_pwGain), mem_fun(*m_scalePWGain, &HScale::get_value));
	m_scalePWGain->signal_value_changed().connect(p_slotPWGain);

	m_scalePhi0 = 0;
	builder->get_widget("scalePhi0", m_scalePhi0);
	slot<void> p_slotPhi0 = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_phi0), mem_fun(*m_scalePhi0, &HScale::get_value));
	m_scalePhi0->signal_value_changed().connect(p_slotPhi0);

	m_scaleExpFMGain = 0;
	builder->get_widget("scaleExpFMGain", m_scaleExpFMGain);
	slot<void> p_slotExpFMGain = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_expFMGain), mem_fun(*m_scaleExpFMGain, &HScale::get_value));
	m_scaleExpFMGain->signal_value_changed().connect(p_slotExpFMGain);

	m_scaleLinFMGain = 0;
	builder->get_widget("scaleLinFMGain", m_scaleLinFMGain);
	slot<void> p_slotLinFMGain = compose(bind<0> (mem_fun(*this, &Vco2GUI::write_control), p_linFMGain), mem_fun(*m_scaleLinFMGain, &HScale::get_value));
	m_scaleLinFMGain->signal_value_changed().connect(p_slotLinFMGain);
}

void Vco2GUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_waveForm)
	{
		int p_waveFormValue = (int) (*static_cast<const float*> (buffer));
		if (p_waveFormValue >= 0 && p_waveFormValue <= 6)
		{
			m_comboWaveForm->set_active((int) p_waveFormValue);
		}
	}
	else if (port == p_octave)
	{
		m_scaleOctave->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_tune)
	{
		m_scaleTune->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_harmonic)
	{
		m_scaleHarmonic->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_subharmonic)
	{
		m_scaleSubHarmonic->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_pw0)
	{
		m_scalePW->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_pwGain)
	{
		m_scalePWGain->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_phi0)
	{
		m_scalePhi0->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_expFMGain)
	{
		m_scaleExpFMGain->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_linFMGain)
	{
		m_scaleLinFMGain->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = Vco2GUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/vco2/gui");
