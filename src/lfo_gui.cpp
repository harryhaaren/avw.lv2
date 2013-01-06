#include <gtkmm-2.4/gtkmm.h>


#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "lfo_gui.hpp"
#include "lfo.hpp"

LfoGUI::LfoGUI(const std::string& URI)
{
	EventBox *p_background = manage (new EventBox());
	Gdk::Color* color = new  Gdk::Color();
	color->set_rgb(7710, 8738, 9252);
	p_background->modify_bg(Gtk::STATE_NORMAL, *color);

	VBox *p_mainWidget = manage (new VBox(false, 5));

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

	m_checkSync = manage(new CheckButton("Sync with Tempo"));
	slot<void> p_slotSyncWithTemp = compose(bind<0>(mem_fun(*this, &LfoGUI::write_control), p_sync), mem_fun(*m_checkSync, &CheckButton::get_active));
	m_checkSync->signal_toggled().connect(p_slotSyncWithTemp);

	p_mainWidget->pack_start(*m_checkSync);

	slot<void> p_slotFrequency = compose(bind<0>(mem_fun(*this, &LfoGUI::write_control), p_frequency), mem_fun(*this,  &LfoGUI::get_freq));
	m_dialFreq = new LabeledDial("Frequency", p_slotFrequency, p_frequency, 0.001, 100, true, 0.001, 3);
	p_mainWidget->pack_start(*m_dialFreq);

	slot<void> p_slotDivider = compose(bind<0>(mem_fun(*this, &LfoGUI::write_control), p_temp_mul), mem_fun(*this, &LfoGUI::get_div));
	m_dialTempoDiv = new LabeledDial("Tempo Division", p_slotDivider, p_temp_mul, 0, 8, false, 0.25, 2);
	p_mainWidget->pack_start(*m_dialTempoDiv);

	slot<void> p_slotPhi0 = compose(bind<0> (mem_fun(*this, &LfoGUI::write_control), p_phi0), mem_fun(*this, &LfoGUI::get_phi0));
	m_dialPhi0 = new LabeledDial("Phi0", p_slotPhi0, p_phi0, 0, 6.28, false, 0.01, 2);
	p_mainWidget->pack_start(*m_dialPhi0);

	p_mainWidget->set_size_request(160, 320);

	p_background->add(*p_mainWidget);
	add(*p_background);

	Gtk::manage(p_mainWidget);
}

float LfoGUI::get_freq() 	{ return m_dialFreq->get_value(); }
float LfoGUI::get_div() 	{ return m_dialTempoDiv->get_value(); }
float LfoGUI::get_phi0() 	{ return m_dialPhi0->get_value(); }

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
	else if (port == p_sync)
	{
		m_checkSync->set_active(*static_cast<const float*> (buffer) == 1);
	}
	else if (port == p_temp_mul)
	{
		m_dialTempoDiv->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_frequency)
	{
		m_dialFreq->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_phi0)
	{
		m_dialPhi0->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = LfoGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/lfo/gui");
