#ifndef ADVENV_GUI_H
#define ADVENV_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

#include "advenv_gui_scope.hpp"

class AdvEnvGUI: public UI<AdvEnvGUI, GtkUI<true>>
{
	public:
		AdvEnvGUI(const std::string& URI);
		void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

	private:
		float m_valueTimeScale;
		float m_valueSustain;
		float m_valueDelay;

		float m_valueAttackTime1;
		float m_valueAttackTime2;
		float m_valueAttackTime3;
		float m_valueAttackTime4;

		float m_valueAttackLevel1;
		float m_valueAttackLevel2;
		float m_valueAttackLevel3;

		float m_valueReleaseTime1;
		float m_valueReleaseTime2;
		float m_valueReleaseTime3;

		float m_valueReleaseLevel1;
		float m_valueReleaseLevel2;

		AdvEnvGUIScope* m_envScope;

	protected:
		Gtk::HScale* m_scaleTimeScale;
		Gtk::HScale* m_scaleSustain;
		Gtk::HScale* m_scaleDelay;

		Gtk::HScale* m_scaleAttackTime1;
		Gtk::HScale* m_scaleAttackTime2;
		Gtk::HScale* m_scaleAttackTime3;
		Gtk::HScale* m_scaleAttackTime4;

		Gtk::HScale* m_scaleAttackLevel1;
		Gtk::HScale* m_scaleAttackLevel2;
		Gtk::HScale* m_scaleAttackLevel3;

		Gtk::HScale* m_scaleReleaseTime1;
		Gtk::HScale* m_scaleReleaseTime2;
		Gtk::HScale* m_scaleReleaseTime3;

		Gtk::HScale* m_scaleReleaseLevel1;
		Gtk::HScale* m_scaleReleaseLevel2;
};

#endif
