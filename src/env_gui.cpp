#include <gtkmm-2.4/gtkmm.h>
#include <gdkmm-2.4/gdkmm.h>

#include "lv2plugin.hpp"
#include "lv2gui.hpp"
#include "env_gui.hpp"
#include "env_gui_scope.hpp"
#include "env.hpp"

EnvGUI::EnvGUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/env_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("env_window", p_window);

	Gtk::Box* p_mainWidget = 0;
	builder->get_widget("boxMain", p_mainWidget);

	p_window->remove();
	add(*p_mainWidget);

	m_scaleAttack = 0;
	builder->get_widget("scaleAttack", m_scaleAttack);
	slot<void> p_slotAttack = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_attack), mem_fun(*m_scaleAttack, &HScale::get_value));
	m_scaleAttack->signal_value_changed().connect(p_slotAttack);

	m_scaleDecay = 0;
	builder->get_widget("scaleDecay", m_scaleDecay);
	slot<void> p_slotDecay = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_decay), mem_fun(*m_scaleDecay, &HScale::get_value));
	m_scaleDecay->signal_value_changed().connect(p_slotDecay);

	m_scaleSustain = 0;
	builder->get_widget("scaleSustain", m_scaleSustain);
	slot<void> p_slotSustain = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_sustain), mem_fun(*m_scaleSustain, &HScale::get_value));
	m_scaleSustain->signal_value_changed().connect(p_slotSustain);

	m_scaleRelease = 0;
	builder->get_widget("scaleRelease", m_scaleRelease);
	slot<void> p_slotRelease = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_release), mem_fun(*m_scaleRelease, &HScale::get_value));
	m_scaleRelease->signal_value_changed().connect(p_slotRelease);

	m_scaleDelay = 0;
	builder->get_widget("scaleDelay", m_scaleDelay);
	slot<void> p_slotDelay = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_delay), mem_fun(*m_scaleDelay, &HScale::get_value));
	m_scaleDelay->signal_value_changed().connect(p_slotDelay);

	m_scaleHold = 0;
	builder->get_widget("scaleHold", m_scaleHold);
	slot<void> p_slotHold = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_hold), mem_fun(*m_scaleHold, &HScale::get_value));
	m_scaleHold->signal_value_changed().connect(p_slotHold);

	m_scaleTimeScale = 0;
	builder->get_widget("scaleTimeScale", m_scaleTimeScale);
	slot<void> p_slotTimeScale = compose(bind<0> (mem_fun(*this, &EnvGUI::write_control), p_timeScale), mem_fun(*m_scaleTimeScale, &HScale::get_value));
	m_scaleTimeScale->signal_value_changed().connect(p_slotTimeScale);

	Gtk::Alignment* p_graphFrame = 0;
	builder->get_widget("alignmentFrame", p_graphFrame);
	m_envScope = new EnvGUIScope();
	p_graphFrame->add(*m_envScope);

	m_envScope->m_attackValue = m_scaleAttack->get_value();
	m_envScope->m_decayValue = m_scaleDecay->get_value();
	m_envScope->m_sustainValue = m_scaleSustain->get_value();
	m_envScope->m_releaseValue = m_scaleRelease->get_value();
	m_envScope->m_delayValue = m_scaleDelay->get_value();
	m_envScope->m_holdValue = m_scaleHold->get_value();

	m_envScope->Redraw();

	Gtk::manage(p_mainWidget);
}

void EnvGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_attack)
	{
		m_attackValue = *static_cast<const float*> (buffer);
		m_scaleAttack->set_value(m_attackValue);
		m_envScope->m_attackValue = m_attackValue;
		m_envScope->Redraw();
	}
	else if (port == p_decay)
	{
		m_decayValue = *static_cast<const float*> (buffer);
		m_scaleDecay->set_value(m_decayValue);
		m_envScope->m_decayValue = m_decayValue;
		m_envScope->Redraw();
	}
	else if (port == p_sustain)
	{
		m_sustainValue = *static_cast<const float*> (buffer);
		m_scaleSustain->set_value(m_sustainValue);
		m_envScope->m_sustainValue = m_sustainValue;
		m_envScope->Redraw();
	}
	else if (port == p_release)
	{
		m_releaseValue = *static_cast<const float*> (buffer);
		m_scaleRelease->set_value(m_releaseValue);
		m_envScope->m_releaseValue = m_releaseValue;
		m_envScope->Redraw();
	}
	else if (port == p_delay)
	{
		m_delayValue = *static_cast<const float*> (buffer);
		m_scaleDelay->set_value(m_delayValue);
		m_envScope->m_delayValue = m_delayValue;
		m_envScope->Redraw();
	}
	else if (port == p_hold)
	{
		m_holdValue = *static_cast<const float*> (buffer);
		m_scaleHold->set_value(m_holdValue);
		m_envScope->m_holdValue = m_holdValue;
		m_envScope->Redraw();
	}
	else if (port == p_timeScale)
	{
		m_scaleTimeScale->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = EnvGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/env/gui");
