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
def read_compiler(ctx):
  cxx = ctx.env.CXX[0]
  ctx.start_msg('Checking for compiler family')
  if 'clang' in cxx:
   ctx.env.COMPILER='clang'
   ctx.define('COMPILER_CLANG', 1)
   ctx.undefine('COMPILER_GCC')
  else:
    ctx.undefine('COMPILER_CLANG')
    if 'g++' in cxx or 'gcc' in cxx:
      ctx.env.COMPILER='gcc'
      ctx.define('COMPILER_GCC', 1)
    else:
      ctx.env.COMPILER='unknown'
      ctx.undefine('COMPILER_GCC')
  ctx.define('COMPILER_FAMILY', ctx.env.COMPILER)
  ctx.end_msg(ctx.env.COMPILER)

@conf
def read_stdlib(ctx):
  if ctx.env.COMPILER == 'clang':
    ctx.start_msg('Checking for the stdlib')
    if ctx.options.stdlib:
      ctx.env.STDLIB = ctx.options.stdlib
      ctx.end_msg(ctx.env.STDLIB)
      ctx.env.CXXFLAGS += ['-stdlib=%s'%ctx.env.STDLIB]
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

def configure(conf):
  conf.env.CXXFLAGS = buildflags
  conf.load('compiler_cxx')
  conf.load('waf_unit_test')
  conf.define('VERSION', VERSION)
  conf.read_compiler()
  conf.read_stdlib()
  conf.write_config_header('config.h')

from waflib import Logs, Errors
def summary(bld):
  lst = getattr(bld, 'utest_results', [])
  if lst:
    total = len(lst)
    tfail = len([x for x in lst if x[1]])
    val = 100 * (total - tfail) / (1.0 * total)
    Logs.pprint('CYAN', 'test report %3.0f%% success' % val)
    Logs.pprint('CYAN', 'tests that fail %d/%d' % (tfail, total))
    for (f, code, out, err) in lst:
      color = code != 0 and 'RED' or 'CYAN'
      status = code != 0 and 'FAIL' or 'PASS'
      test_name = f.split('/')[-1]
      Logs.pprint(color, '%s %s'%(status, test_name))
      if code:
        Logs.pprint('CYAN', '    %s' % f)
        Logs.pprint('RED', 'status: %r' % code)
        if out: Logs.pprint('RED', 'out: %r' % out)
        if err: Logs.pprint('RED', 'err: %r' % err)
    if tfail != 0:
      raise Errors.WafError("Unit tests failed!")

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
      cxxflags=bld.env.CXXFLAGS,
      linkflags=linkflags,
      lib=lib,
      source=f)

  bld.add_post_fun(summary)

from waflib.Build import BuildContext
class debug(BuildContext):
  cmd = 'debug'
  variant = 'debug'
  fun = 'debug'

def debug(bld):
  bld.env.CXXFLAGS += ['-g', '-O0']
  build(bld)

class coverage(BuildContext):
  cmd = 'coverage'
  variant = 'coverage'
  fun = 'coverage'

def coverage(bld):
  bld.env.CXXFLAGS += ['-g', '-O0']
  bld.env.CXXFLAGS += ['-fprofile-arcs', '-ftest-coverage', '-pg']
  bld.lib = ['gcov']
  bld.linkflags = ['-pg']
  build(bld)

class release(BuildContext):
  cmd = 'release'
  variant = 'release'
  fun = 'release'

def release(bld):
  bld.env.CXXFLAGS += ['-O2']
  build(bld)
