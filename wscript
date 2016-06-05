#! /usr/bin/env python
# encoding: utf-8

VERSION='1.0.0'
APPNAME='archie'

top = '.'
out = 'build'
flags = [
  '-std=c++1z',
  '-Wall',
  '-Wextra',
  '-Wshadow',
  '-Wconversion',
  '-Wnon-virtual-dtor',
  '-Wold-style-cast',
  '-Wcast-align',
  '-Wunused',
  '-Woverloaded-virtual',
  '-pedantic',
  '-pedantic-errors',
  '-Werror'
]

modes = [
  'debug',
  'release',
  'coverage'
]

def options(opt):
  opt.load('compiler_cxx')
  opt.load('waf_unit_test')
  opt.add_option( '-m', '--mode',
          action  = 'store',
          default = modes[0],
          metavar = 'MODE',
          dest    = 'mode',
          choices = modes,
          help    = 'Default build type, supported: ' + ', '.join(modes)
          )

def configure(conf):
  conf.setenv('base')
  conf.load('waf_unit_test')
  conf.define('APPNAME', APPNAME)
  conf.define('VERSION', VERSION)
  conf.load('compiler_cxx')
  conf.env.CXXFLAGS += flags

  for variant in modes:
    conf.setenv('base')
    newenv = conf.env.derive()
    newenv.detach()
    conf.setenv(variant, newenv)

  conf.setenv('debug')
  conf.env.CXXFLAGS += ['-g', '-O0']
  conf.env.DEFINES += ['DEBUG']

  conf.setenv('coverage')
  conf.env.CXXFLAGS += ['-g', '-O0', '-fprofile-arcs', '-ftest-coverage', '-pg']
  conf.env.DEFINES += ['NDEBUG']
  conf.env.LIB += ['gcov']
  conf.env.LINKFLAGS += ['-pg']

  conf.setenv('release')
  conf.env.CXXFLAGS += ['-O3', '-march=native', '-fPIC', '-fno-rtti']
  conf.env.DEFINES += ['NDEBUG']

  conf.env.MODE = conf.options.mode
  conf.msg(msg='Default build mode',
          result=conf.env.MODE)

  conf.setenv(conf.env.MODE)
  mode = conf.env.derive()
  mode.detach()
  conf.setenv('default', mode)

import os
from waflib.Tools import waf_unit_test
def build(bld):
  if not bld.variant:
    bld.fatal('try "waf --help"')
  bld.env.DEFINES += ['USE_ARCHIE_TUPLE']
  bld.env.INCLUDES += ['include', 'test']
  bld(
    source       = bld.path.ant_glob(['test/test_main.cpp']),
    target       = 'TEST_MAIN',
    features     = 'cxx',
    install_path = None,
  )
  for suite in os.listdir('./test'):
    if 'ignore' not in suite and os.path.isdir(os.path.join('./test', suite)):
      bld(
        source       = bld.path.ant_glob(['test/%s/**/*.cpp'%suite]),
        target       = 'ut_' + str(suite),
        features     = 'cxx cxxprogram test',
        use          = ['TEST_MAIN'],
        install_path = None,
      )
  bld.add_post_fun(waf_unit_test.summary)
  bld.add_post_fun(waf_unit_test.set_exit_code)
  inc = bld.path.find_dir('include')
  for f in bld.path.ant_glob(['include/archie/**/*.hpp']):
    bld.install_files('${PREFIX}/include/' + f.path_from(inc).replace(f.name, ''), f)

from waflib.Build import BuildContext
from waflib.Build import CleanContext
from waflib.Build import InstallContext
from waflib.Build import UninstallContext

for ctx in (BuildContext, CleanContext, InstallContext, UninstallContext):
  name = ctx.__name__.replace('Context','').lower()
  class debug(ctx):
    cmd = name + '_debug'
    variant = 'debug'
  class release(ctx):
    cmd = name + '_release'
    variant = 'release'
  class coverage(ctx):
    cmd = name + '_coverage'
    variant = 'coverage'
  class default(ctx):
    cmd = name
    variant = 'default'
