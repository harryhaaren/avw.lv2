#ifndef ENV_GUI_H
#define ENV_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

#include "env_gui_scope.hpp"

class EnvGUI: public UI<EnvGUI, GtkUI<true>>
{
public:
	EnvGUI(const std::string& URI);
	void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

private:
	float m_attackValue, m_decayValue, m_sustainValue, m_releaseValue, m_delayValue, m_holdValue;
	EnvGUIScope* m_envScope;

protected:
	Gtk::HScale* m_scaleAttack;
	Gtk::HScale* m_scaleDecay;
	Gtk::HScale* m_scaleSustain;
	Gtk::HScale* m_scaleRelease;
	Gtk::HScale* m_scaleDelay;
	Gtk::HScale* m_scaleHold;
	Gtk::HScale* m_scaleTimeScale;
};

#endif
