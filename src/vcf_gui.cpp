#include <gtkmm-2.4/gtkmm.h>

#include "lv2plugin.hpp"
#include "lv2gui.hpp"
#include "vcf_gui.hpp"
#include "vcf.hpp"

VcfGUI::VcfGUI(const std::string& URI)
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	builder->add_from_file(g_strdup_printf("%s/vcf_gui.xml", bundle_path()));

	Gtk::Window* p_window = 0;
	builder->get_widget("vcf_window", p_window);

	Gtk::Widget* p_mainWidget = 0;
	builder->get_widget("vbox_main", p_mainWidget);

	p_window->remove();

	add(*p_mainWidget);

	m_comboVCFType = 0;
	builder->get_widget("comboboxtextVCFType", m_comboVCFType);
	slot<void> p_slotVCFType = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_vcfType), mem_fun(*m_comboVCFType, &ComboBoxText::get_active_row_number));
	m_comboVCFType->signal_changed().connect(p_slotVCFType);

	m_scaleInputGain = 0;
	builder->get_widget("hscaleInputGain", m_scaleInputGain);
	slot<void> p_slotInputGain = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_inputGain), mem_fun(*m_scaleInputGain, &HScale::get_value));
	m_scaleInputGain->signal_value_changed().connect(p_slotInputGain);

	m_scaleFrequency = 0;
	builder->get_widget("hscaleFrequency", m_scaleFrequency);
	slot<void> p_slotFrequency = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_freq), mem_fun(*m_scaleFrequency, &HScale::get_value));
	m_scaleFrequency->signal_value_changed().connect(p_slotFrequency);

	m_scaleResonance = 0;
	builder->get_widget("hscaleResonance", m_scaleResonance);
	slot<void> p_slotResonance = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_resonance), mem_fun(*m_scaleResonance, &HScale::get_value));
	m_scaleResonance->signal_value_changed().connect(p_slotResonance);

	m_scaleResonanceGain = 0;
	builder->get_widget("hscaleResonanceGain", m_scaleResonanceGain);
	slot<void> p_slotResonanceGain = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_resonanceGain), mem_fun(*m_scaleResonanceGain, &HScale::get_value));
	m_scaleResonanceGain->signal_value_changed().connect(p_slotResonanceGain);

	m_scaleExpFMGain = 0;
	builder->get_widget("hscaleExpFMGain", m_scaleExpFMGain);
	slot<void> p_slotExpFMGain = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_expFMGain), mem_fun(*m_scaleExpFMGain, &HScale::get_value));
	m_scaleExpFMGain->signal_value_changed().connect(p_slotExpFMGain);

	m_scaleLinFMGain = 0;
	builder->get_widget("hscaleLinFMGain", m_scaleLinFMGain);
	slot<void> p_slotLinFMGain = compose(bind<0> (mem_fun(*this, &VcfGUI::write_control), p_linFMGain), mem_fun(*m_scaleLinFMGain, &HScale::get_value));
	m_scaleLinFMGain->signal_value_changed().connect(p_slotLinFMGain);
}

void VcfGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_vcfType)
	{
		int p_vcfTypeValue = (int) (*static_cast<const float*> (buffer));
		if (p_vcfTypeValue >= 0 && p_vcfTypeValue <= 7)
		{
			m_comboVCFType->set_active((int) p_vcfTypeValue);
		}
	}
	else if (port == p_inputGain)
	{
		m_scaleInputGain->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_freq)
	{
		m_scaleFrequency->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_resonance)
	{
		m_scaleResonance->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_resonanceGain)
	{
		m_scaleResonanceGain->set_value(*static_cast<const float*> (buffer));
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

static int _ = VcfGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/vcf/gui");
