#ifndef DIAL_H
#define DIAL_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/adjustment.h>

#include <iostream>

enum Colour {
	COLOUR_ORANGE_1 = 0,
	COLOUR_ORANGE_2,
	COLOUR_ORANGE_3,
	COLOUR_GREEN_1,
	COLOUR_GREEN_2,
	COLOUR_GREEN_3,
	COLOUR_BLUE_1,
	COLOUR_BLUE_2,
	COLOUR_BLUE_3,
	COLOUR_PURPLE_1,
	COLOUR_PURPLE_2,
	COLOUR_PURPLE_3,
	COLOUR_GREY_1,
	COLOUR_GREY_2,
	COLOUR_GREY_3,
	COLOUR_GREY_4,

	COLOUR_BACKGROUND,
	COLOUR_RECORD_RED,
	COLOUR_TRANSPARENT,
};


class Dial: public Gtk::DrawingArea
{
	public:
		Dial(const sigc::slot<void> toggle_slot, double Value, double Min, double Max, double Step);
		virtual ~Dial();
		bool Redraw();

		void value_changed();
		float get_toggle_value();
		void set_toggle_value(float value);

	protected:
		virtual bool on_expose_event(GdkEventExpose* event);
		virtual bool onMouseMove(GdkEventMotion* event);
		virtual bool on_button_press_event(GdkEventButton* event);
		virtual bool on_button_release_event(GdkEventButton* event);

		float mouseDelta;
		bool mouseDown;

	private:
		void setColour( Cairo::RefPtr<Cairo::Context> cr, Colour c);
		void setColour( Cairo::RefPtr<Cairo::Context> cr, Colour c, float alpha);

		Gtk::Adjustment *a_tog;

};

#endif
