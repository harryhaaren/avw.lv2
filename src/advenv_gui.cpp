#include <gtkmm-2.4/gtkmm.h>
#include <gdkmm-2.4/gdkmm.h>

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "advenv_gui.hpp"
#include "advenv_gui_scope.hpp"
#include "advenv.hpp"

AdvEnvGUI::AdvEnvGUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/advenv_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("advenv_window", p_window);

	Gtk::Box* p_mainWidget = 0;
	builder->get_widget("boxMain", p_mainWidget);

	p_window->remove();
	add(*p_mainWidget);

	m_scaleTimeScale = 0;
	builder->get_widget("scaleTimeScale", m_scaleTimeScale);
	slot<void> p_slotTimeScale = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_timeScale), mem_fun(*m_scaleTimeScale, &HScale::get_value));
	m_scaleTimeScale->signal_value_changed().connect(p_slotTimeScale);

	m_scaleSustain = 0;
	builder->get_widget("scaleSustain", m_scaleSustain);
	slot<void> p_slotSustain = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_sustain), mem_fun(*m_scaleSustain, &HScale::get_value));
	m_scaleSustain->signal_value_changed().connect(p_slotSustain);

	m_scaleDelay = 0;
	builder->get_widget("scaleDelay", m_scaleDelay);
	slot<void> p_slotDelay = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_delay), mem_fun(*m_scaleDelay, &HScale::get_value));
	m_scaleDelay->signal_value_changed().connect(p_slotDelay);

	m_scaleAttackTime1 = 0;
	builder->get_widget("scaleAttackTime1", m_scaleAttackTime1);
	slot<void> p_slotAttackTime1 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackTime1), mem_fun(*m_scaleAttackTime1, &HScale::get_value));
	m_scaleAttackTime1->signal_value_changed().connect(p_slotAttackTime1);

	m_scaleAttackTime2 = 0;
	builder->get_widget("scaleAttackTime2", m_scaleAttackTime2);
	slot<void> p_slotAttackTime2 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackTime2), mem_fun(*m_scaleAttackTime2, &HScale::get_value));
	m_scaleAttackTime2->signal_value_changed().connect(p_slotAttackTime2);

	m_scaleAttackTime3 = 0;
	builder->get_widget("scaleAttackTime3", m_scaleAttackTime3);
	slot<void> p_slotAttackTime3 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackTime3), mem_fun(*m_scaleAttackTime3, &HScale::get_value));
	m_scaleAttackTime3->signal_value_changed().connect(p_slotAttackTime3);

	m_scaleAttackTime4 = 0;
	builder->get_widget("scaleAttackTime4", m_scaleAttackTime4);
	slot<void> p_slotAttackTime4 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackTime4), mem_fun(*m_scaleAttackTime4, &HScale::get_value));
	m_scaleAttackTime4->signal_value_changed().connect(p_slotAttackTime4);

	m_scaleAttackLevel1 = 0;
	builder->get_widget("scaleAttackLevel1", m_scaleAttackLevel1);
	slot<void> p_slotAttackLevel1 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackLevel1), mem_fun(*m_scaleAttackLevel1, &HScale::get_value));
	m_scaleAttackLevel1->signal_value_changed().connect(p_slotAttackLevel1);

	m_scaleAttackLevel2 = 0;
	builder->get_widget("scaleAttackLevel2", m_scaleAttackLevel2);
	slot<void> p_slotAttackLevel2 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackLevel2), mem_fun(*m_scaleAttackLevel2, &HScale::get_value));
	m_scaleAttackLevel2->signal_value_changed().connect(p_slotAttackLevel2);

	m_scaleAttackLevel3 = 0;
	builder->get_widget("scaleAttackLevel3", m_scaleAttackLevel3);
	slot<void> p_slotAttackLevel3 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_attackLevel3), mem_fun(*m_scaleAttackLevel3, &HScale::get_value));
	m_scaleAttackLevel3->signal_value_changed().connect(p_slotAttackLevel3);

	m_scaleReleaseTime1 = 0;
	builder->get_widget("scaleReleaseTime1", m_scaleReleaseTime1);
	slot<void> p_slotReleaseTime1 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_releaseTime1), mem_fun(*m_scaleReleaseTime1, &HScale::get_value));
	m_scaleReleaseTime1->signal_value_changed().connect(p_slotReleaseTime1);

	m_scaleReleaseTime2 = 0;
	builder->get_widget("scaleReleaseTime2", m_scaleReleaseTime2);
	slot<void> p_slotReleaseTime2 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_releaseTime2), mem_fun(*m_scaleReleaseTime2, &HScale::get_value));
	m_scaleReleaseTime2->signal_value_changed().connect(p_slotReleaseTime2);

	m_scaleReleaseTime3 = 0;
	builder->get_widget("scaleReleaseTime3", m_scaleReleaseTime3);
	slot<void> p_slotReleaseTime3 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_releaseTime3), mem_fun(*m_scaleReleaseTime3, &HScale::get_value));
	m_scaleReleaseTime3->signal_value_changed().connect(p_slotReleaseTime3);

	m_scaleReleaseLevel1 = 0;
	builder->get_widget("scaleReleaseLevel1", m_scaleReleaseLevel1);
	slot<void> p_slotReleaseLevel1 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_releaseLevel1), mem_fun(*m_scaleReleaseLevel1, &HScale::get_value));
	m_scaleReleaseLevel1->signal_value_changed().connect(p_slotReleaseLevel1);

	m_scaleReleaseLevel2 = 0;
	builder->get_widget("scaleReleaseLevel2", m_scaleReleaseLevel2);
	slot<void> p_slotReleaseLevel2 = compose(bind<0> (mem_fun(*this, &AdvEnvGUI::write_control), p_releaseLevel2), mem_fun(*m_scaleReleaseLevel2, &HScale::get_value));
	m_scaleReleaseLevel2->signal_value_changed().connect(p_slotReleaseLevel2);

	Gtk::Alignment* p_graphFrame = 0;
	builder->get_widget("alignmentFrame", p_graphFrame);
	m_envScope = new AdvEnvGUIScope();
	p_graphFrame->add(*m_envScope);

	//m_envScope->m_valueTimeScale = m_scaleTimeScale->get_value();
	m_envScope->m_valueSustain = m_scaleSustain->get_value();
	m_envScope->m_valueDelay = m_scaleDelay->get_value();

	m_envScope->m_valueAttackTime1 = m_scaleAttackTime1->get_value();
	m_envScope->m_valueAttackTime2 = m_scaleAttackTime2->get_value();
	m_envScope->m_valueAttackTime3 = m_scaleAttackTime3->get_value();
	m_envScope->m_valueAttackTime4 = m_scaleAttackTime4->get_value();

	m_envScope->m_valueAttackLevel1 = m_scaleAttackLevel1->get_value();
	m_envScope->m_valueAttackLevel2 = m_scaleAttackLevel2->get_value();
	m_envScope->m_valueAttackLevel3 = m_scaleAttackLevel3->get_value();

	m_envScope->m_valueReleaseTime1 = m_scaleReleaseTime1->get_value();
	m_envScope->m_valueReleaseTime2 = m_scaleReleaseTime2->get_value();
	m_envScope->m_valueReleaseTime3 = m_scaleReleaseTime3->get_value();

	m_envScope->m_valueReleaseLevel1 = m_scaleReleaseLevel1->get_value();
	m_envScope->m_valueReleaseLevel2 = m_scaleReleaseLevel2->get_value();

	m_envScope->Redraw();

	Gtk::manage(p_mainWidget);
}

void AdvEnvGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_sustain)
	{
		m_valueSustain = *static_cast<const float*> (buffer);
		m_scaleSustain->set_value(m_valueSustain);
		m_envScope->m_valueSustain = m_valueSustain;
		m_envScope->Redraw();
	}
	else if (port == p_delay)
	{
		m_valueDelay = *static_cast<const float*> (buffer);
		m_scaleDelay->set_value(m_valueDelay);
		m_envScope->m_valueDelay = m_valueDelay;
		m_envScope->Redraw();
	}
	else if (port == p_attackTime1)
	{
		m_valueAttackTime1 = *static_cast<const float*> (buffer);
		m_scaleAttackTime1->set_value(m_valueAttackTime1);
		m_envScope->m_valueAttackTime1 = m_valueAttackTime1;
		m_envScope->Redraw();
	}
	else if (port == p_attackTime2)
	{
		m_valueAttackTime2 = *static_cast<const float*> (buffer);
		m_scaleAttackTime2->set_value(m_valueAttackTime2);
		m_envScope->m_valueAttackTime2 = m_valueAttackTime2;
		m_envScope->Redraw();
	}
	else if (port == p_attackTime3)
	{
		m_valueAttackTime3 = *static_cast<const float*> (buffer);
		m_scaleAttackTime3->set_value(m_valueAttackTime3);
		m_envScope->m_valueAttackTime3 = m_valueAttackTime3;
		m_envScope->Redraw();
	}
	else if (port == p_attackTime4)
	{
		m_valueAttackTime4 = *static_cast<const float*> (buffer);
		m_scaleAttackTime4->set_value(m_valueAttackTime4);
		m_envScope->m_valueAttackTime4 = m_valueAttackTime4;
		m_envScope->Redraw();
	}
	else if (port == p_attackLevel1)
	{
		m_valueAttackLevel1 = *static_cast<const float*> (buffer);
		m_scaleAttackLevel1->set_value(m_valueAttackLevel1);
		m_envScope->m_valueAttackLevel1 = m_valueAttackLevel1;
		m_envScope->Redraw();
	}
	else if (port == p_attackLevel2)
	{
		m_valueAttackLevel2 = *static_cast<const float*> (buffer);
		m_scaleAttackLevel2->set_value(m_valueAttackLevel2);
		m_envScope->m_valueAttackLevel2 = m_valueAttackLevel2;
		m_envScope->Redraw();
	}
	else if (port == p_attackLevel3)
	{
		m_valueAttackLevel3 = *static_cast<const float*> (buffer);
		m_scaleAttackLevel3->set_value(m_valueAttackLevel3);
		m_envScope->m_valueAttackLevel3 = m_valueAttackLevel3;
		m_envScope->Redraw();
	}
	else if (port == p_releaseTime1)
	{
		m_valueReleaseTime1 = *static_cast<const float*> (buffer);
		m_scaleReleaseTime1->set_value(m_valueReleaseTime1);
		m_envScope->m_valueReleaseTime1 = m_valueReleaseTime1;
		m_envScope->Redraw();
	}
	else if (port == p_releaseTime2)
	{
		m_valueReleaseTime2 = *static_cast<const float*> (buffer);
		m_scaleReleaseTime2->set_value(m_valueReleaseTime2);
		m_envScope->m_valueReleaseTime2 = m_valueReleaseTime2;
		m_envScope->Redraw();
	}
	else if (port == p_releaseTime3)
	{
		m_valueReleaseTime3 = *static_cast<const float*> (buffer);
		m_scaleReleaseTime3->set_value(m_valueReleaseTime3);
		m_envScope->m_valueReleaseTime3 = m_valueReleaseTime3;
		m_envScope->Redraw();
	}
	else if (port == p_releaseLevel1)
	{
		m_valueReleaseLevel1 = *static_cast<const float*> (buffer);
		m_scaleReleaseLevel1->set_value(m_valueReleaseLevel1);
		m_envScope->m_valueReleaseLevel1 = m_valueReleaseLevel1;
		m_envScope->Redraw();
	}
	else if (port == p_releaseLevel2)
	{
		m_valueReleaseLevel2 = *static_cast<const float*> (buffer);
		m_scaleReleaseLevel2->set_value(m_valueReleaseLevel2);
		m_envScope->m_valueReleaseLevel2 = m_valueReleaseLevel2;
		m_envScope->Redraw();
	}
	else if (port == p_timeScale)
	{
		m_scaleTimeScale->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = AdvEnvGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/advenv/gui");
