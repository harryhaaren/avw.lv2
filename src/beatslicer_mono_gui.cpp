#include <gtkmm-2.4/gtkmm.h>


#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "beatslicer_mono_gui.hpp"
#include "beatslicer_mono.hpp"

BeatSlicerMonoGUI::BeatSlicerMonoGUI(const std::string& URI)
{
	EventBox *p_background = manage (new EventBox());
	Gdk::Color* color = new  Gdk::Color();
	color->set_rgb(7710, 8738, 9252);
	p_background->modify_bg(Gtk::STATE_NORMAL, *color);

	VBox *p_mainWidget = manage (new VBox(false));


	slot<void> p_slotSliceSize = compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_slice), mem_fun(*this,  &BeatSlicerMonoGUI::get_sizeSlice));
	m_dialSizeSlice = new LabeledDial("Size of Slice", p_slotSliceSize, p_slice, 0.125, 1, false, 0.125, 3);
	p_mainWidget->pack_start(*m_dialSizeSlice);

	m_checkReverse = manage(new CheckButton("Reverse"));
	slot<void> p_slotReverse= compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_reverse), mem_fun(*m_checkReverse, &CheckButton::get_active));
	m_checkReverse->signal_toggled().connect(p_slotReverse);
	p_mainWidget->pack_start(*m_checkReverse);


	p_mainWidget->set_size_request(80, 160);

	p_background->add(*p_mainWidget);
	add(*p_background);

	Gtk::manage(p_mainWidget);
}

float BeatSlicerMonoGUI::get_sizeSlice() { return m_dialSizeSlice->get_value(); }

void BeatSlicerMonoGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_reverse)
	{
		m_checkReverse->set_active(*static_cast<const float*> (buffer) == 1);
	}
	else if (port == p_slice)
	{
		m_dialSizeSlice->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = BeatSlicerMonoGUI::register_class("http://avwlv2.sourceforge.net/plugins/avw/beatslicer_mono/gui");
