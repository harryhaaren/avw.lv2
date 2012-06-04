#!/usr/bin/env python
import os
import shutil
from waflib import Logs
from waflib.extras import autowaf as autowaf

# Variables for 'waf dist'
APPNAME = 'avw.lv2'
VERSION = '0.0.8'

# Mandatory variables
top = '.'
out = 'build'


def options(opt):
	autowaf.set_options(opt)
	opt.load('compiler_c')
	opt.load('compiler_cxx')
    
    
def configure(conf):
    autowaf.configure(conf)
    conf.line_just = 51
    autowaf.display_header('AVW Configuration')
    
    conf.load('compiler_c')
    conf.load('compiler_cxx')
 
    autowaf.check_pkg(conf, 'gtkmm-2.4',  uselib_store='GTKMM',atleast_version='2.24.0')
    autowaf.check_pkg(conf, 'gtk+-2.0', uselib_store='GTK2', atleast_version='2.24.0')
    autowaf.check_pkg(conf, 'cairo', uselib_store='CAIRO', atleast_version='1.0.0')
    autowaf.check_pkg(conf, 'lv2', uselib_store='LV2',)
    autowaf.check_pkg(conf, 'jack', uselib_store='JACK', atleast_version='0.120.0')	
    
    conf.env.append_value('CFLAGS', '-std=c99')

    # Set env['pluginlib_PATTERN']
    pat = conf.env['cxxshlib_PATTERN']
    if pat[0:3] == 'lib':
        pat = pat[3:]
    conf.env['pluginlib_PATTERN'] = pat
    conf.env['pluginlib_EXT'] = pat[pat.rfind('.'):]

    autowaf.display_msg(conf, "LV2 bundle directory", conf.env['LV2DIR'])
    print('')


def build_plugin(bld, lang, bundle, name, source, cxxflags=[], libs=[]):
    penv = bld.env.derive()
    penv['cshlib_PATTERN']   = bld.env['pluginlib_PATTERN']
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = '%s %sshlib' % (lang,lang))
    obj.env          = penv
    obj.source       = source + ['src/synthdata.cpp', 'src/lv2plugin.cpp']
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


def build_plugin_gui(bld, lang, bundle, name, source, cxxflags=[], libs=[], add_source=[]):
    penv = bld.env.derive()
    penv['cshlib_PATTERN']   = bld.env['pluginlib_PATTERN']
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = '%s %sshlib' % (lang,lang))
    obj.env          = penv
    obj.source       = source + add_source + ['src/lv2gui.cpp']
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

	for i in bld.path.ant_glob('gui/*.xml'):
		bld(rule   = do_copy,
            source = i,
            target = bld.path.get_bld().make_node('avw.lv2/%s' % i),
            install_path = '${LV2DIR}/avw.lv2')

 	plugins = '''
	vco2
	ad
	advenv
	amp
	env
	hztovc
	lfo
	mixer
	noise2
	slew
	vcaexp
	vcalin
	vcf
	vctohz
	cvs
	delay
	dynamicwaves
	absolute
	inv
	jacklfo
	tranches
	ringmodulator
	percussiveenv
	samplehold
	vcpanning
	vcswitch
	mooglpf
	'''.split()

	for i in plugins:
		build_plugin(bld, 'cxx', 'avw.lv2', i, ['src/%s.cpp' % i],
                  ['-DPLUGIN_CLASS=%s' % i,
				  '-fPIC', 
                  '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
                  '-DPLUGIN_URI_SUFFIX="%s"' % i,
                  '-DPLUGIN_HEADER="src/%s.hpp src/lv2plugin.hpp"' % i],
				  ['LV2', 'JACK'])
			  
	plugins_gui = '''
	vco2_gui
	ad_gui
	vcf_gui
	vcpanning_gui
    '''.split()

	for i in plugins_gui:
		build_plugin_gui(bld, 'cxx', 'avw.lv2', i, ['src/%s.cpp' % i],
                  ['-DPLUGIN_CLASS=%s' % i,
				  '-fPIC', 
                  '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
                  '-DPLUGIN_URI_SUFFIX="%s"' % i,
                  '-DPLUGIN_HEADER="src/%s.hpp"' % i],
				  ['LV2', 'GTKMM', 'GTK2', 'CAIRO'], [])
				  
	build_plugin_gui(bld, 'cxx', 'avw.lv2', 'env_gui', ['src/env_gui.cpp'],
                  ['-DPLUGIN_CLASS=env_gui',
				  '-fPIC', 
                  '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
                  '-DPLUGIN_URI_SUFFIX="env_gui"',
                  '-DPLUGIN_HEADER="src/env_gui.hpp"'],
				  ['LV2', 'GTKMM', 'GTK2', 'CAIRO'], ['src/env_gui_scope.cpp'])
				  
	build_plugin_gui(bld, 'cxx', 'avw.lv2', 'advenv_gui', ['src/advenv_gui.cpp'],
                  ['-DPLUGIN_CLASS=advenv_gui',
				  '-fPIC', 
                  '-DURI_PREFIX=\"http://lv2plug.in/plugins/avw/\"',
                  '-DPLUGIN_URI_SUFFIX="advenv_gui"',
                  '-DPLUGIN_HEADER="src/advenv_gui.hpp"'],
				  ['LV2', 'GTKMM', 'GTK2', 'CAIRO'], ['src/advenv_gui_scope.cpp'])