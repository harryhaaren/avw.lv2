#include <gtkmm-2.4/gtkmm.h>

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "lfo_gui.hpp"
#include "lfo.hpp"

LfoGUI::LfoGUI(const std::string& URI)
{
	VBox *p_mainWidget = manage (new VBox(true));

	Label *p_labelWaveForm = manage (new Label("Wave Form"));
	p_mainWidget->pack_start(*p_labelWaveForm);

	m_comboWaveForm = manage (new ComboBoxText());
	m_comboWaveForm->append_text("Sine");
	m_comboWaveForm->append_text("Triangle");
	m_comboWaveForm->append_text("Sawtooth Up");
	m_comboWaveForm->append_text("Sawtooth Down");
	m_comboWaveForm->append_text("Rectangle");
	m_comboWaveForm->append_text("S & H");

	slot<void> p_slotWaveForm = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_waveForm), mem_fun(*m_comboWaveForm, &ComboBoxText::get_active_row_number));
	m_comboWaveForm->signal_changed().connect(p_slotWaveForm);

	p_mainWidget->pack_start(*m_comboWaveForm);

	Label *p_labelFrequency = manage (new Label("Frequency"));
	p_mainWidget->pack_start(*p_labelFrequency);

	Adjustment *p_freqAdj = manage (new Adjustment(5, 0, 100, 0, 5));
	m_scaleFrequency = manage (new HScale(*p_freqAdj));

	slot<void> p_slotFrequency = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_frequency), mem_fun(*m_scaleFrequency, &HScale::get_value));
	m_scaleFrequency->signal_value_changed().connect(p_slotFrequency);

	p_mainWidget->pack_start(*m_scaleFrequency);

	Label *p_labelPhi0 = manage (new Label("Phi0"));
	p_mainWidget->pack_start(*p_labelPhi0);

	Adjustment *p_freqPhi0 = manage (new Adjustment(0, 0, 6.28, 0, 1));
	m_scalePhi0 = manage (new HScale(*p_freqPhi0));

	slot<void> p_slotPhi0 = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_phi0), mem_fun(*m_scalePhi0, &HScale::get_value));
	m_scalePhi0->signal_value_changed().connect(p_slotPhi0);

	p_mainWidget->pack_start(*m_scalePhi0);

	add(*p_mainWidget);
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
