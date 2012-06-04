/****************************************************************************

 lv2gui.hpp - Wrapper library to make it easier to write LV2 GUIs in C++

 Copyright (C) 2006-2008 Lars Luthman <lars.luthman@gmail.com>
 Modified by Dave Robillard, 2008 (URI map mixin)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA  02110-1301  USA

 ****************************************************************************/

#ifndef LV2GUI_HPP
#define LV2GUI_HPP

#include <cstdlib>
#include <cstring>
#include <map>

#include <gtkmm/box.h>
#include <gtkmm/main.h>
#include <gtkmm/widget.h>

#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <lv2/lv2plug.in/ns/ext/uri-map/uri-map.h>
#include "lv2plugin.hpp"

namespace LV2
{
	typedef std::vector<LV2UI_Descriptor*> GUIDescList;

	GUIDescList& get_lv2g2g_descriptors();

	template<class Derived>
	class GUI: public Gtk::HBox
	{
		public:
			inline GUI()
			{
				m_ctrl = s_ctrl;
				m_wfunc = s_wfunc;
				m_bundle_path = s_bundle_path;
				s_ctrl = 0;
				s_wfunc = 0;
				s_bundle_path = 0;
			}

			inline void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, void const* buffer)
			{
			}

			static int register_class(char const* uri)
			{
				LV2UI_Descriptor* desc = new LV2UI_Descriptor;
				std::memset(desc, 0, sizeof(LV2UI_Descriptor));
				desc->URI = strdup(uri);
				desc->instantiate = &Derived::create_ui_instance;
				desc->cleanup = &Derived::delete_ui_instance;
				desc->port_event = &Derived::_port_event;
				get_lv2g2g_descriptors().push_back(desc);
				return get_lv2g2g_descriptors().size() - 1;
			}

			bool check_ok() const
			{
				return true;
			}

			inline void* controller()
			{
				return m_ctrl;
			}

		protected:
			inline void write(uint32_t port, uint32_t buffer_size, uint32_t format, void const* buffer)
			{
				(*m_wfunc)(m_ctrl, port, buffer_size, format, buffer);
			}

			inline void write_control(uint32_t port, float value)
			{
				write(port, sizeof(float), 0, &value);
			}

			inline char const* bundle_path() const
			{
				return m_bundle_path;
			}

		private:
			static LV2UI_Handle create_ui_instance(struct _LV2UI_Descriptor const* descriptor, char const* plugin_uri, char const* bundle_path, LV2UI_Write_Function write_func, LV2UI_Controller ctrl, LV2UI_Widget* widget, Feature const* const * features)
			{
				s_ctrl = ctrl;
				s_wfunc = write_func;
				s_bundle_path = bundle_path;

				Gtk::Main::init_gtkmm_internals();

				Derived* t = new Derived(plugin_uri);
				*widget = static_cast<Gtk::Widget*>(t)->gobj();

				if (t->check_ok())
				{
					return reinterpret_cast<LV2UI_Handle>(t);
				}
				delete t;
				return 0;
			}

			static void delete_ui_instance(LV2UI_Handle instance)
			{
				delete static_cast<Derived*>(instance);
			}

			static void _port_event(LV2UI_Handle instance, uint32_t port, uint32_t buffer_size, uint32_t format, void const* buffer)
			{
				static_cast<Derived*>(instance)->port_event(port, buffer_size, format, buffer);
			}

			void* m_ctrl;
			LV2UI_Write_Function m_wfunc;
			char const* m_bundle_path;

			static void* s_ctrl;
			static LV2UI_Write_Function s_wfunc;
			static char const* s_bundle_path;

	};

	template<class Derived>
	void* GUI<Derived>::s_ctrl = 0;

	template<class Derived>
	LV2UI_Write_Function GUI<Derived>::s_wfunc = 0;

	template<class Derived>
	char const* GUI<Derived>::s_bundle_path = 0;
}

#endif
