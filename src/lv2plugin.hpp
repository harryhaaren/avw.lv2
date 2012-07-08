/****************************************************************************

 lv2plugin.hpp - support file for writing LV2 plugins in C++

 Copyright (C) 2006-2007 Lars Luthman <lars.luthman@gmail.com>
 Modified by Dave Robillard, 2008

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
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 01222-1307  USA

 ****************************************************************************/

#ifndef LV2PLUGIN_HPP
#define LV2PLUGIN_HPP

#include <cstdarg>

#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/uri-map/uri-map.h>

typedef struct
{
		float min;
		float max;
		float default_value;
		char toggled;
		char integer;
		char logarithmic;
} peg_data_t;

namespace LV2
{
	typedef LV2_Feature Feature;

	class DescList: public std::vector<LV2_Descriptor>
	{
		public:
			~DescList();
	};

	DescList& get_lv2_descriptors();

	template<class Derived>
	class Plugin
	{
		public:
			Plugin(uint32_t ports) :
					m_ports(ports, 0), m_ok(true)
			{
				m_bundle_path = s_bundle_path;
				s_bundle_path = 0;
			}

			void connect_port(uint32_t port, void* data_location)
			{
				m_ports[port] = (float*) data_location;
			}

			void activate()
			{
			}

			void run(uint32_t sample_count)
			{
			}

			void deactivate()
			{
			}

			static unsigned register_class(const std::string& uri)
			{
				LV2_Descriptor desc;
				std::memset(&desc, 0, sizeof(LV2_Descriptor));
				char* c_uri = new char[uri.size() + 1];
				std::memcpy(c_uri, uri.c_str(), uri.size() + 1);
				desc.URI = c_uri;
				desc.instantiate = &Derived::_create_plugin_instance;
				desc.connect_port = &Derived::_connect_port;
				desc.activate = &Derived::_activate;
				desc.run = &Derived::_run;
				desc.deactivate = &Derived::_deactivate;
				desc.cleanup = &Derived::_delete_plugin_instance;
				get_lv2_descriptors().push_back(desc);
				return get_lv2_descriptors().size() - 1;
			}

			bool check_ok()
			{
				return m_ok;
			}

		protected:
			float*& p(uint32_t port)
			{
				return reinterpret_cast<float*&>(m_ports[port]);
			}

			const char* bundle_path() const
			{
				return m_bundle_path;
			}

			void set_ok(bool ok)
			{
				m_ok = ok;
			}

			std::vector<float*> m_ports;

		private:
			char const* m_bundle_path;
			static char const* s_bundle_path;
			bool m_ok;

			static void _connect_port(LV2_Handle instance, uint32_t port, void* data_location)
			{
				reinterpret_cast<Derived*>(instance)->connect_port(port, data_location);
			}

			static void _activate(LV2_Handle instance)
			{
				reinterpret_cast<Derived*>(instance)->activate();
			}

			static void _run(LV2_Handle instance, uint32_t sample_count)
			{
				reinterpret_cast<Derived*>(instance)->run(sample_count);
			}

			static void _deactivate(LV2_Handle instance)
			{
				reinterpret_cast<Derived*>(instance)->deactivate();
			}

			static LV2_Handle _create_plugin_instance(const LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const Feature* const * features)
			{
				s_bundle_path = bundle_path;

				Derived* t = new Derived(sample_rate);
				if (t->check_ok())
				{
					return reinterpret_cast<LV2_Handle>(t);
				}
				delete t;
				return 0;
			}

			static void _delete_plugin_instance(LV2_Handle instance)
			{
				delete reinterpret_cast<Derived*>(instance);
			}
	};

	template<class Derived>
	char const* Plugin<Derived>::s_bundle_path = 0;
}

#endif
