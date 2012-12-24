#include <cairomm/context.h>
#include <cmath>

#include "labeleddial.hpp"

using namespace std;

LabeledDial::LabeledDial(const sigc::slot<void> toggle_slot, double Value, double Min, double Max, bool Log, double Step, int NbDigit)
{
	Gdk::Color* color = new  Gdk::Color();
	color->set_rgb(7710, 8738, 9252);
	this->modify_bg(Gtk::STATE_NORMAL, *color);

	VBox *p_mainWidget = manage (new VBox(false));

	m_dial = new Dial(toggle_slot, Value, Min, Max, Log, Step, NbDigit);

	p_mainWidget->pack_start(*m_dial);

	std::stringstream out;
	out << Value;

	m_label = new Gtk::Label();
	m_label->set_text(out.str());

	p_mainWidget->pack_start(*m_label);

	this->add(*p_mainWidget);

	set_size_request(60, 60);

}

LabeledDial::~LabeledDial()
{
}

bool LabeledDial::Redraw()
{
	return m_dial->Redraw();
}

void LabeledDial::value_changed()
{
	m_dial->Redraw();
	std::stringstream out;
	out << m_dial->get_value();
	m_label->set_text(out.str());
}

float LabeledDial::get_value()
{
	return m_dial->get_value();
}

void LabeledDial::set_value(float Value)
{
	m_dial->set_value(Value);
	std::stringstream out;
	out << m_dial->get_value();
	m_label->set_text(out.str());
	m_dial->Redraw();
}



