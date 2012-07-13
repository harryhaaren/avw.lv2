#include <gtkmm-2.4/gtkmm.h>

#include "lv2plugin.hpp"
#include "lv2gui.hpp"
#include "lfo_gui.hpp"
#include "lfo.hpp"

LfoGUI::LfoGUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/lfo_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("lfo_window", p_window);

	Gtk::Widget* p_mainWidget = 0;
	builder->get_widget("notebookMain", p_mainWidget);

	p_window->remove();

	add(*p_mainWidget);

	m_comboWaveForm = 0;
	builder->get_widget("comboboxtextWaveForm", m_comboWaveForm);
	slot<void> p_slotWaveForm = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_waveForm), mem_fun(*m_comboWaveForm, &ComboBoxText::get_active_row_number));
	m_comboWaveForm->signal_changed().connect(p_slotWaveForm);

	m_scaleFrequency = 0;
	builder->get_widget("scaleFrequency", m_scaleFrequency);
	slot<void> p_slotFrequency = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_frequency), mem_fun(*m_scaleFrequency, &HScale::get_value));
	m_scaleFrequency->signal_value_changed().connect(p_slotFrequency);

	m_scalePhi0 = 0;
	builder->get_widget("scalePhi0", m_scalePhi0);
	slot<void> p_slotPhi0 = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_phi0), mem_fun(*m_scalePhi0, &HScale::get_value));
	m_scalePhi0->signal_value_changed().connect(p_slotPhi0);
}

void LfoGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_waveForm)
	{
		int p_waveFormValue = (int) (*static_cast<const float*> (buffer));
		if (p_waveFormValue >= 0 && p_waveFormValue <= 5)
		{
			m_comboWaveForm->set_active((int) p_waveFormValue);
		}
	}
	else if (port == p_frequency)
	{
		m_scaleFrequency->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_phi0)
	{
		m_scalePhi0->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = LfoGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/lfo/gui");
