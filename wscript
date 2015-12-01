#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='archie'

top = '.'
out = 'build'
flags = [
  '-std=c++14',
  '-Wall',
  '-Wextra',
  '-Wshadow',
  #'-Wconversion',
  '-Wnon-virtual-dtor',
  '-Wold-style-cast',
  '-Wcast-align',
  '-Wunused',
  '-Woverloaded-virtual',
  '-pedantic',
  '-pedantic-errors',
  '-Werror'
]

def options(opt):
  opt.load('compiler_cxx')
  opt.load('waf_unit_test')

def configure(conf):
  conf.load('waf_unit_test')
  conf.define('APPNAME', APPNAME)
  conf.define('VERSION', VERSION)

  conf.setenv('debug')
  conf.load('compiler_cxx')
  conf.env.CXXFLAGS += flags
  conf.env.CXXFLAGS += ['-g', '-O0']
  conf.env.DEFINES += ['DEBUG']

  conf.setenv('release')
  conf.load('compiler_cxx')
  conf.env.CXXFLAGS += flags
  conf.env.CXXFLAGS += ['-O3', '-march=native', '-fPIC', '-fno-rtti']
  conf.env.DEFINES += ['NDEBUG']
  if conf.check_cxx(fragment='int main() {}\n',
          cxxflags='-flto',
          linkflags='-flto',
          mandatory=False,
          msg='Checking for link time optimization'):
    conf.env.CXXFLAGS += ['-flto']
    conf.env.LINKFLAGS += ['-flto']

from waflib.Tools import waf_unit_test
def build(bld):
  if not bld.variant:
    bld.fatal('try "waf --help"')
  bld.env.DEFINES += ['USE_ARCHIE_TUPLE']
  bld.env.INCLUDES += ['include', 'src']
  bld(
    source       = bld.path.ant_glob(['src/**/*.cpp']),
    target       = APPNAME,
    features     = 'cxx cxxstlib',
    vnum         = VERSION,
    install_path = '${PREFIX}/lib',
  )
  bld.env.INCLUDES += ['test']
  bld.env.DEFINES += ['TEST_BUILD']
  for t in bld.path.ant_glob(['test/**/*.cpp']):
    bld(
     source       = t,
     target       = 'ut_' + t.name,
     features     = 'cxx cxxprogram test',
     use          = APPNAME,
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
    cmd = name
    variant = 'release'
