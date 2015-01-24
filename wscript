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

def configure(conf):
  conf.load('compiler_cxx')
  conf.load('waf_unit_test')
  conf.define('VERSION', VERSION)
  conf.define('COMPILER_GCC', 'g++' in conf.env['CXX'] and 1 or 0)
  conf.define('COMPILER_CLANG', 'clang++' in conf.env['CXX'] and 1 or 0)
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

  cxxflags = getattr(bld, 'cxxflags', buildflags)
  linkflags = getattr(bld, 'linkflags', [])
  lib = getattr(bld, 'lib', [])
  if 'clang++' in bld.env.CXX: cxxflags.append('-stdlib=libc++')

  tests = bld.path.ant_glob('test/*.cpp')
  for f in tests:
    bld(
      target=str(f)[:-len('.cpp')],
      features='cxx cxxprogram test',
      includes=inc,
      cxxflags=cxxflags,
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
  bld.cxxflags = buildflags + ['-g', '-O0']
  build(bld)

class coverage(BuildContext):
  cmd = 'coverage'
  variant = 'coverage'
  fun = 'coverage'

def coverage(bld):
  bld.cxxflags = buildflags + ['-g', '-O0']
  bld.cxxflags += ['-fprofile-arcs', '-ftest-coverage', '-pg']
  bld.lib = ['gcov']
  bld.linkflags = ['-pg']
  build(bld)

class release(BuildContext):
  cmd = 'release'
  variant = 'release'
  fun = 'release'

def release(bld):
  bld.cxxflags = buildflags + ['-O2']
  build(bld)
