#ifndef LABELEDDIAL_H
#define LABELEDDIAL_H

#include <gtkmm-2.4/gtkmm.h>

#include <gtkmm/drawingarea.h>
#include <gtkmm/adjustment.h>

#include <iostream>

#include "g_dial.hpp"

using namespace Gtk;

class LabeledDial: public Gtk::EventBox
{
	public:
		LabeledDial(const sigc::slot<void> toggle_slot, double Value, double Min, double Max, bool Log, double Step);
		virtual ~LabeledDial();
		bool Redraw();

		void value_changed();
		float get_value();
		void set_value(float value);

	private:
		Dial *m_dial;
		Gtk::Label *m_label;
};

#endif

