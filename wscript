#!/usr/bin/env python
import os
import subprocess
import shutil
from waflib import Logs
from waflib.extras import autowaf as autowaf

# Variables for 'waf dist'
APPNAME = 'avw.lv2'
VERSION = '0.1.4'

# Mandatory variables
top = '.'
out = 'build'


def options(opt):
    autowaf.set_options(opt)
    opt.load('compiler_cxx')
    
    
def configure(conf):
    autowaf.configure(conf)
    autowaf.display_header('AVW Configuration')
    
    conf.load('compiler_cxx')
    
    autowaf.check_pkg(conf, 'gtkmm-2.4',  uselib_store='GTKMM',atleast_version='2.24.0')
    autowaf.check_pkg(conf, 'gtk+-2.0', uselib_store='GTK2', atleast_version='2.24.0')
    autowaf.check_pkg(conf, 'cairo', uselib_store='CAIRO', atleast_version='1.0.0')
    autowaf.check_pkg(conf, 'lv2', uselib_store='LV2', atleast_version='1.2.0')
    autowaf.check_pkg(conf, 'lvtk-plugin-1', uselib_store='LVTK_PLUGIN', atleast_version='1.0.3')
    autowaf.check_pkg(conf, 'lvtk-ui-1', uselib_store='LVTK_UI', atleast_version='1.0.3')
    autowaf.check_pkg(conf, 'lvtk-gtkui-1', uselib_store='LVTK_GTKGUI', atleast_version='1.0.3')
    autowaf.check_pkg(conf, 'jack', uselib_store='JACK', atleast_version='0.120.0')	

    check = 'Extended Initializer Lists'
    conf.check_cxx(
        msg         = check,
        define_name = 'WAF_CXX_FEATURE_' + check.replace(' ', '_').upper(),
        fragment    = '#include <vector>\n'+
			'struct foo {\n'+
    			'int i;\n'+
    			'int j;\n'+
    			'int k;\n'+
			'};\n'+
			'int main() {\n'+
    			'std::vector<foo> v(1);\n'+
    			'v[0] = {0, 0, 0};\n'+
    			'return 0;\n'+
			'}',
        mandatory   = False)
    
    # Set env['pluginlib_PATTERN']
    pat = conf.env['cxxshlib_PATTERN']
    if pat[0:3] == 'lib':
        pat = pat[3:]
    conf.env['pluginlib_PATTERN'] = pat

    autowaf.display_msg(conf, "LV2 bundle directory", conf.env['LV2DIR'])
    print('')


def build_plugin(bld, bundle, name, source, cxxflags=[], libs=[], add_source=[]):
    penv = bld.env.derive()
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = 'cxx cxxshlib')
    obj.env          = penv
    obj.source       = source + add_source
    obj.includes     = ['.', './src']
    obj.name         = name
    obj.target       = os.path.join(bundle, name)
    if cxxflags != []:
        obj.cxxflags = cxxflags
    if libs != []:
	obj.uselib = libs
    obj.install_path = '${LV2DIR}/' + bundle

    # Install data file
    data_file = '%s.ttl' % name
    bld.install_files('${LV2DIR}/' + bundle, os.path.join(bundle, data_file))


def build_plugin_gui(bld, bundle, name, source, cxxflags=[], libs=[], add_source=[]):
    penv = bld.env.derive()
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = 'cxx cxxshlib')
    obj.env          = penv
    obj.source       = source + add_source
    obj.includes     = ['.', './src']
    obj.name         = name
    obj.target       = os.path.join(bundle, name)
    if cxxflags != []:
        obj.cxxflags = cxxflags
    if libs != []:
	obj.uselib = libs
    obj.install_path = '${LV2DIR}/' + bundle


def build(bld):
	def do_copy(task):
		src = task.inputs[0].abspath()
		tgt = task.outputs[0].abspath()
		return shutil.copy(src, tgt)
    
	for i in bld.path.ant_glob('avw.lv2/*.ttl'):
		bld(rule   = do_copy,
            source = i,
            target = bld.path.get_bld().make_node('avw.lv2/%s' % i),
            install_path = '${LV2DIR}/avw.lv2')

 	plugins = '''
	advenv
	amp
	env
	hztovc
	lfo
	mixer_4ch_audio
	mixer_4ch_cv
	mixer_2ch_audio
	mixer_2ch_cv
	noise2_audio
	noise2_cv
	slew
	vcalin
	vctohz
	cvs
	delay
	absolute
	inv
	ringmodulator_audio
	ringmodulator_cv
	percussiveenv
	samplehold
	vcpanning
	vcswitch_audio
	vcswitch_cv
	mooglpf
	downsampler_mono
	downsampler_stereo
	controltovc
	'''.split()

	for i in plugins:
		build_plugin(bld, 'avw.lv2', i, ['src/%s.cpp' % i],
			['-DPLUGIN_CLASS=%s' % i,
			'-std=c++11',
            '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
            '-DPLUGIN_URI_SUFFIX="%s"' % i,
            '-DPLUGIN_HEADER="src/%s.hpp"' % i],
		  	['LV2', 'LVTK_PLUGIN', 'JACK'],
		  	[])
		  	
	plugins = '''
	granulator_mono
	granulator_stereo
	beatrepeater_mono
	beatrepeater_stereo
	beatslicer_mono
	beatslicer_stereo
	'''.split()

	for i in plugins:
		build_plugin(bld, 'avw.lv2', i, ['src/%s.cpp' % i],
			['-DPLUGIN_CLASS=%s' % i,
			'-std=c++11',
            '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
            '-DPLUGIN_URI_SUFFIX="%s"' % i,
            '-DPLUGIN_HEADER="src/%s.hpp"' % i],
		  	['LV2', 'LVTK_PLUGIN', 'JACK'],
		  	['src/envgen.cpp'])

	plugins = '''
	vco2
	ad
	vcf
	vcaexp
	'''.split()
	
	for i in plugins:
		build_plugin(bld, 'avw.lv2', i, ['src/%s.cpp' % i],
			['-DPLUGIN_CLASS=%s' % i,
			'-std=c++11',
            '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
            '-DPLUGIN_URI_SUFFIX="%s"' % i,
            '-DPLUGIN_HEADER="src/%s.hpp"' % i],
		  	['LV2', 'LVTK_PLUGIN', 'JACK'],
		  	['src/synthdata.cpp'])
		  
	plugins_gui = '''
	vco2_gui
	ad_gui
	vcf_gui
	vcpanning_gui
	lfo_gui
	slew_gui
	amp_gui
	cvs_gui
	delay_gui
	hztovc_gui
	mixer_2ch_audio_gui
	mixer_2ch_cv_gui
	mixer_4ch_audio_gui
	mixer_4ch_cv_gui
	mooglpf_gui
	noise2_audio_gui
	noise2_cv_gui
	ringmodulator_audio_gui
	ringmodulator_cv_gui
	vcaexp_gui
	vcalin_gui
	vcswitch_audio_gui
	vcswitch_cv_gui
	vctohz_gui
	downsampler_mono_gui
	downsampler_stereo_gui
	granulator_mono_gui
	granulator_stereo_gui
	beatrepeater_mono_gui
	beatrepeater_stereo_gui
	beatslicer_mono_gui
	beatslicer_stereo_gui
	'''.split()

	for i in plugins_gui:
		build_plugin_gui(bld, 'avw.lv2', i, ['src/%s.cpp' % i],
			['-DPLUGIN_CLASS=%s' % i,
		  	'-std=c++11',
            '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
            '-DPLUGIN_URI_SUFFIX="%s"' % i,
            '-DPLUGIN_HEADER="src/%s.hpp"' % i],
		  	['LV2', 'LVTK_PLUGIN', 'LVTK_GTKGUI', 'GTKMM', 'GTK2', 'CAIRO'], 
			['src/dial.cpp', 'src/labeleddial.cpp'])

	build_plugin_gui(bld, 'avw.lv2', 'env_gui', ['src/env_gui.cpp'],
		['-DPLUGIN_CLASS=env_gui',
		'-std=c++11',
        '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
        '-DPLUGIN_URI_SUFFIX="env_gui"',
        '-DPLUGIN_HEADER="src/env_gui.hpp"'],
		['LV2', 'LVTK_PLUGIN', 'LVTK_GTKGUI', 'GTKMM', 'GTK2', 'CAIRO'], 
		['src/dial.cpp', 'src/labeleddial.cpp', 'src/env_gui_scope.cpp'])
		
	build_plugin_gui(bld, 'avw.lv2', 'percussiveenv_gui', ['src/percussiveenv_gui.cpp'],
		['-DPLUGIN_CLASS=percussiveenv_gui',
		'-std=c++11',
        '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
        '-DPLUGIN_URI_SUFFIX="percussiveenv_gui"',
        '-DPLUGIN_HEADER="src/percussiveenv_gui.hpp"'],
		['LV2', 'LVTK_PLUGIN', 'LVTK_GTKGUI', 'GTKMM', 'GTK2', 'CAIRO'], 
		['src/dial.cpp', 'src/labeleddial.cpp', 'src/percussiveenv_gui_scope.cpp'])
				  
	build_plugin_gui(bld, 'avw.lv2', 'advenv_gui', ['src/advenv_gui.cpp'],
		['-DPLUGIN_CLASS=advenv_gui',
		'-std=c++11',
		'-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
		'-DPLUGIN_URI_SUFFIX="advenv_gui"',
		'-DPLUGIN_HEADER="src/advenv_gui.hpp"'],
		['LV2', 'LVTK_PLUGIN', 'LVTK_GTKGUI', 'GTKMM', 'GTK2', 'CAIRO'], 
		['src/dial.cpp', 'src/labeleddial.cpp', 'src/advenv_gui_scope.cpp'])
		
