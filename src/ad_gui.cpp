#include <gtkmm-2.4/gtkmm.h>

#include "lv2plugin.hpp"
#include "lv2gui.hpp"
#include "ad_gui.hpp"
#include "ad.hpp"

AdGUI::AdGUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/ad_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("ad_window", p_window);

	Gtk::Widget* p_mainWidget = 0;
	builder->get_widget("boxParameters", p_mainWidget);

	p_window->remove();

	add(*p_mainWidget);

	m_scaleDetuneAmplitude = 0;
	builder->get_widget("scaleDetuneAmplitude", m_scaleDetuneAmplitude);
	slot<void> p_slotDetuneAmplitude = compose(bind<0> (mem_fun(*this, &AdGUI::write_control), p_detuneAmplitude), mem_fun(*m_scaleDetuneAmplitude, &HScale::get_value));
	m_scaleDetuneAmplitude->signal_value_changed().connect(p_slotDetuneAmplitude);

	m_scaleDetuneModulation = 0;
	builder->get_widget("scaleDetuneModulation", m_scaleDetuneModulation);
	slot<void> p_slotDetuneModulation = compose(bind<0> (mem_fun(*this, &AdGUI::write_control), p_detuneModulation), mem_fun(*m_scaleDetuneModulation, &HScale::get_value));
	m_scaleDetuneModulation->signal_value_changed().connect(p_slotDetuneModulation);

	m_scaleDetuneRate = 0;
	builder->get_widget("scaleDetuneRate", m_scaleDetuneRate);
	slot<void> p_slotDetuneRate = compose(bind<0> (mem_fun(*this, &AdGUI::write_control), p_detuneRate), mem_fun(*m_scaleDetuneRate, &HScale::get_value));
	m_scaleDetuneRate->signal_value_changed().connect(p_slotDetuneRate);

	m_scaleDriftAmplitude = 0;
	builder->get_widget("scaleDriftAmplitude", m_scaleDriftAmplitude);
	slot<void> p_slotDriftAmplitude = compose(bind<0> (mem_fun(*this, &AdGUI::write_control), p_driftAmplitude), mem_fun(*m_scaleDriftAmplitude, &HScale::get_value));
	m_scaleDriftAmplitude->signal_value_changed().connect(p_slotDriftAmplitude);

	m_scaleDriftModulation = 0;
	builder->get_widget("scaleDriftModulation", m_scaleDriftModulation);
	slot<void> p_slotDriftModulation = compose(bind<0> (mem_fun(*this, &AdGUI::write_control), p_driftModulation), mem_fun(*m_scaleDriftModulation, &HScale::get_value));
	m_scaleDriftModulation->signal_value_changed().connect(p_slotDriftModulation);

	m_scaleDriftRate = 0;
	builder->get_widget("scaleDriftRate", m_scaleDriftRate);
	slot<void> p_slotDriftRate = compose(bind<0> (mem_fun(*this, &AdGUI::write_control), p_driftRate), mem_fun(*m_scaleDriftRate, &HScale::get_value));
	m_scaleDriftRate->signal_value_changed().connect(p_slotDriftRate);
}

void AdGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_detuneAmplitude)
	{
		m_scaleDetuneAmplitude->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_detuneModulation)
	{
		m_scaleDetuneModulation->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_detuneRate)
	{
		m_scaleDetuneRate->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_driftAmplitude)
	{
		m_scaleDriftAmplitude->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_driftModulation)
	{
		m_scaleDriftModulation->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_driftRate)
	{
		m_scaleDriftRate->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = AdGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/ad/gui");
