#include <gtkmm-2.4/gtkmm.h>

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "vcpanning_gui.hpp"
#include "vcpanning.hpp"

VcPanningGUI::VcPanningGUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/vcpanning_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("vcpanning_window", p_window);

	Gtk::Widget* p_mainWidget = 0;
	builder->get_widget("vbox_main", p_mainWidget);

	p_window->remove();

	add(*p_mainWidget);

	m_comboPanningMode = 0;
	builder->get_widget("comboboxtextVCFType", m_comboPanningMode);
	slot<void> p_slotPanningMode = compose(bind<0> (mem_fun(*this, &VcPanningGUI::write_control), p_panningMode), mem_fun(*m_comboPanningMode, &ComboBoxText::get_active_row_number));
	m_comboPanningMode->signal_changed().connect(p_slotPanningMode);

	m_scalePanOffset = 0;
	builder->get_widget("hscaleInputGain", m_scalePanOffset);
	slot<void> p_slotPanOffset = compose(bind<0> (mem_fun(*this, &VcPanningGUI::write_control), p_panOffset), mem_fun(*m_scalePanOffset, &HScale::get_value));
	m_scalePanOffset->signal_value_changed().connect(p_slotPanOffset);

	m_scalePanGain = 0;
	builder->get_widget("hscaleFrequency", m_scalePanGain);
	slot<void> p_slotPanGain = compose(bind<0> (mem_fun(*this, &VcPanningGUI::write_control), p_panGain), mem_fun(*m_scalePanGain, &HScale::get_value));
	m_scalePanGain->signal_value_changed().connect(p_slotPanGain);
}

void VcPanningGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_panningMode)
	{
		int p_panningModeValue = (int) (*static_cast<const float*> (buffer));
		if (p_panningModeValue >= 0 && p_panningModeValue <= 4)
		{
			m_comboPanningMode->set_active((int) p_panningModeValue);
		}
	}
	else if (port == p_panOffset)
	{
		m_scalePanOffset->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_panGain)
	{
		m_scalePanGain->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = VcPanningGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/vcpanning/gui");
