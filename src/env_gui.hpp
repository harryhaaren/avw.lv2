#ifndef ENV_GUI_H
#define ENV_GUI_H

using namespace sigc;
using namespace Gtk;

#include "env_gui_scope.hpp"

class EnvGUI: public LV2::GUI<EnvGUI>
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
