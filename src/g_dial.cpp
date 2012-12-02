#include <cairomm/context.h>

#include "g_dial.hpp"

using namespace std;

Dial::Dial(const sigc::slot<void> toggle_slot, double Value, double Min, double Max, double Step)
{
	a_tog = new Gtk::Adjustment(Value, Min, Max, Step, Step);
	this->mouseDelta = 0;
	this->mouseDown = false;

	add_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK| Gdk::POINTER_MOTION_MASK);
	signal_button_press_event().connect(sigc::mem_fun(*this, &Dial::on_button_press_event) );
	signal_button_release_event().connect(sigc::mem_fun(*this, &Dial::on_button_release_event) );
	signal_motion_notify_event().connect( sigc::mem_fun( *this, &Dial::onMouseMove ) );

	set_size_request(48, 48);

	a_tog->signal_value_changed().connect(mem_fun(*this, &Dial::value_changed));
	a_tog->signal_value_changed().connect(toggle_slot);
}

Dial::~Dial()
{
}

void Dial::value_changed()
{
	Redraw();
}

float Dial::get_toggle_value()
{
	return a_tog->get_value();
}

void Dial::set_toggle_value(float Value)
{
	a_tog->set_value(Value);
	Redraw();
}

bool Dial::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	if (window)
	{
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

		cr->set_source_rgb(0.118,0.133, 0.141);
		cr->paint();

		cr->set_source_rgb(0.0, 0.8, 0.0);

		cr->rectangle((double)event->area.x, (double)event->area.y, (double)event->area.width, (double)event->area.height);
		cr->clip();

		int xc = event->area.width/2;
		int yc = event->area.height/2;

		float radius = 14;

		cr->set_line_cap( Cairo::LINE_CAP_ROUND );
		cr->set_line_join( Cairo::LINE_JOIN_ROUND);
		cr->set_line_width(2.8);

		// Arc Angle Value
		cr->set_line_width(2.4);
		cr->move_to(xc,yc);
		cr->set_source_rgba(0, 0, 0, 0);
		cr->stroke();

		setColour(cr, COLOUR_GREY_4 );

		cr->arc(xc, yc, radius, 2.46, 0.75);
		cr->move_to(xc, yc);
		cr->stroke();

		cr->set_line_width(2.8);
		float angle= 2.46 + (4.54 * ((a_tog->get_value()-a_tog->get_lower()) / (a_tog->get_upper()-a_tog->get_lower())));

		setColour(cr, COLOUR_ORANGE_1 );

		cr->set_line_width(1.7);
		cr->arc(xc,yc, 13, 2.46, angle );
		cr->line_to(xc,yc);
		cr->stroke();
		cr->arc(xc,yc, 17, 2.46, angle );
		cr->line_to(xc,yc);
		cr->stroke();


	}
	return true;
}

bool Dial::Redraw()
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
	return true;
}

bool Dial::onMouseMove(GdkEventMotion* event)
{
	if (mouseDown)
	{
		if(a_tog->get_value()<a_tog->get_upper() && mouseDelta>event->y)
		{
			set_toggle_value(a_tog->get_value()+a_tog->get_step_increment());
			std::cout << "up " << a_tog->get_value() << std::endl;

		}
		else if(a_tog->get_value()>a_tog->get_lower() && mouseDelta<event->y)
		{
			set_toggle_value(a_tog->get_value()-a_tog->get_step_increment());
			std::cout << "down " << a_tog->get_value() << std::endl;
		}
		mouseDelta = event->y;
		Redraw();

		return true;
	}
	else
	{
		return false;
	}
}

bool Dial::on_button_press_event(GdkEventButton* event)
{
	if( event->type == GDK_BUTTON_PRESS  ) // && event->button == 3
	{
		mouseDown = true; // for pointer motion "drag" operations
		mouseDelta = event->x;

		return true;
	}
	else
	{
		return false;
	}
}

bool Dial::on_button_release_event(GdkEventButton* event)
{
	if( event->type == GDK_BUTTON_RELEASE  ) // && event->button == 3
	{
		mouseDown = false;

		return true; //It's been handled.
	}
	else
	{
		return false;
	}
}


void Dial::setColour( Cairo::RefPtr<Cairo::Context> cr, Colour c, float alpha)
{
	switch( c )
	{
		case COLOUR_ORANGE_1:
			cr->set_source_rgba( 255 / 255.f, 104 / 255.f ,   0 / 255.f , alpha ); break;
		case COLOUR_ORANGE_2:
			cr->set_source_rgba( 178 / 255.f,  71 / 255.f ,   0 / 255.f , alpha ); break;
		case COLOUR_ORANGE_3:
			cr->set_source_rgba(  89 / 255.f,  35 / 255.f ,   0 / 255.f , alpha ); break;
		case COLOUR_GREEN_1:
			cr->set_source_rgba(  25 / 255.f, 255 / 255.f ,   0 / 255.f , alpha ); break;
		case COLOUR_GREEN_2:
			cr->set_source_rgba(  17 / 255.f, 179 / 255.f ,   0 / 255.f , alpha ); break;
		case COLOUR_GREEN_3:
			cr->set_source_rgba(   8 / 255.f,  89 / 255.f ,   0 / 255.f , alpha ); break;
		case COLOUR_BLUE_1:
			cr->set_source_rgba(   0 / 255.f, 153 / 255.f , 255 / 255.f , alpha ); break;
		case COLOUR_BLUE_2:
			cr->set_source_rgba(  20 / 255.f,  73 / 255.f , 109 / 255.f , alpha ); break;
		case COLOUR_BLUE_3:
			cr->set_source_rgba(   0 / 255.f,  53 / 255.f ,  89 / 255.f , alpha ); break;
		case COLOUR_PURPLE_1:
			cr->set_source_rgba( 230 / 255.f,   0 / 255.f , 255 / 255.f , alpha ); break;
		case COLOUR_PURPLE_2:
			cr->set_source_rgba( 161 / 255.f,   0 / 255.f , 179 / 255.f , alpha ); break;
		case COLOUR_PURPLE_3:
			cr->set_source_rgba(  80 / 255.f,   0 / 255.f ,  89 / 255.f , alpha ); break;
		case COLOUR_GREY_1:
			cr->set_source_rgba( 130 / 255.f, 130 / 255.f , 130 / 255.f , alpha ); break;
		case COLOUR_GREY_2:
			cr->set_source_rgba(  98 / 255.f,  98 / 255.f ,  98 / 255.f , alpha ); break;
		case COLOUR_GREY_3:
			cr->set_source_rgba(  66 / 255.f,  66 / 255.f ,  66 / 255.f , alpha ); break;
		case COLOUR_GREY_4:
			cr->set_source_rgba(  28 / 255.f,  28 / 255.f ,  28 / 255.f , alpha ); break;
		case COLOUR_RECORD_RED:
			cr->set_source_rgba(  226 / 255.f, 0/255.f , 0/255.f, alpha ); break;
		case COLOUR_TRANSPARENT:
			cr->set_source_rgba(  0, 0, 0, 0.f ); break;
		case COLOUR_BACKGROUND: default:
			cr->set_source_rgba(  40 / 255.f,  40 / 255.f ,  40 / 255.f , alpha ); break;
	}
}

void Dial::setColour( Cairo::RefPtr<Cairo::Context> cr, Colour c)
{
	setColour(cr, c, 1.f);
}


