#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='archie'

top = '.'
out = 'build'
buildflags = ['-std=c++1y', '-Wall', '-Wextra', '-pedantic', '-Werror']

def options(opt):
  opt.load('compiler_cxx')
  opt.load('waf_unit_test')
  opt.add_option('--stdlib',
    action='store',
    default='',
    type='string',
    dest='stdlib')

from waflib.Configure import conf
@conf
def read_stdlib(ctx):
  if ctx.env.COMPILER == 'clang':
    ctx.start_msg('Checking for the stdlib')
    if ctx.options.stdlib:
      ctx.env.STDLIB = ctx.options.stdlib
      ctx.end_msg(ctx.env.STDLIB)
      ctx.env.COMPILE_FLAGS += ['-stdlib=%s'%ctx.env.STDLIB]
    else:
      ctx.end_msg('stdlib is not set')
  else:
    ctx.env.STDLIB = 'libstdc++'
  if ctx.env.STDLIB == 'libstdc++':
    ctx.define('USE_LIBSTDCPP', 1)
    ctx.undefine('USE_LIBCPP')
  if ctx.env.STDLIB == 'libc++':
    ctx.define('USE_LIBCPP', 1)
    ctx.undefine('USE_LIBSTDCPP')

@conf
def check_variable_templates(ctx):
  ctx.check_cxx(fragment='''
    template<typename Tp>
    struct type {
    };
    template<typename Tp>
    constexpr type<Tp> var{};
    int main() { return 0; } ''',
    mandatory   = False,
    cxxflags    = ['-std=c++14'],
    define_name = "HAS_VARIABLE_TEMPLATES",
    execute     = False,
    msg         = "Checking for variable templates")

def configure(conf):
  conf.env.COMPILE_FLAGS = buildflags
  conf.load('compiler_cxx')
  conf.load('waf_unit_test')
  conf.define('VERSION', VERSION)
  conf.read_stdlib()
  conf.check_variable_templates()
  conf.write_config_header('config.h')

from waflib.Tools import waf_unit_test
def build(bld):
  inc = ['.', 'include', out]

  linkflags = getattr(bld, 'linkflags', [])
  lib = getattr(bld, 'lib', [])

  tests = bld.path.ant_glob('test/*.cpp')
  for f in tests:
    bld(
      target=str(f)[:-len('.cpp')],
      features='cxx cxxprogram test',
      includes=inc,
      cxxflags=bld.env.COMPILE_FLAGS,
      linkflags=linkflags,
      lib=lib,
      source=f)

  bld.add_post_fun(waf_unit_test.summary)

from waflib.Build import BuildContext
class debug(BuildContext):
  cmd = 'debug'
  variant = 'debug'
  fun = 'debug'

def debug(bld):
  bld.env.COMPILE_FLAGS += ['-g', '-O0']
  build(bld)

class coverage(BuildContext):
  cmd = 'coverage'
  variant = 'coverage'
  fun = 'coverage'

def coverage(bld):
  bld.env.COMPILE_FLAGS += ['-g', '-O0']
  bld.env.COMPILE_FLAGS += ['-fprofile-arcs', '-ftest-coverage', '-pg']
  bld.lib = ['gcov']
  bld.linkflags = ['-pg']
  build(bld)

class release(BuildContext):
  cmd = 'release'
  variant = 'release'
  fun = 'release'

def release(bld):
  bld.env.COMPILE_FLAGS += ['-O2']
  build(bld)
