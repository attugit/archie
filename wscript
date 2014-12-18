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

from waflib import Logs
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

oldtets = [
'test/aggregates_test.cpp',
'test/at_test.cpp',
'test/back_test.cpp',
'test/bitfields_test.cpp',
'test/for_each_test.cpp',
'test/front_test.cpp',
'test/message_test.cpp',
'test/nth_test.cpp',
'test/requires_test.cpp',
'test/tuple_test.cpp',
'test/tuple_view_test.cpp',
'test/typeholder_test.cpp',
'test/type_list_test.cpp',
'test/typeset_test.cpp']

tests = [
'test/compose_test.cpp',
'test/inspect_test.cpp',
'test/is_distinct_test.cpp',
'test/meta_identity.cpp',
'test/mover_test.cpp',
'test/traits_test.cpp']

def build(bld):
  inc = ['.', 'gtest', 'include']
  gtest = ['gtest/src/gtest-all.cc', 'gtest/src/gtest_main.cc']
  bld.stlib(
    target='gtest',
    includes=inc,
    cxxflags=['-std=c++1y'],
    source=gtest)

  if bld.variant in ('debug', 'release'):
    cxxflags = bld.cxxflags
  else:
    cxxflags = buildflags

  #src = bld.path.ant_glob('test/*.cpp')
  for f in oldtets:
    bld(
      target=str(f)[:-len('.cpp')],
      features='cxx cxxprogram test',
      includes=inc,
      use='gtest',
      lib=['pthread'],
      cxxflags=cxxflags,
      source=f)
  for f in tests:
    bld(
      target=str(f)[:-len('.cpp')],
      features='cxx cxxprogram test',
      includes=inc,
      cxxflags=cxxflags,
      source=f)

  bld.add_post_fun(summary)

from waflib.Build import BuildContext
class debug(BuildContext):
  cmd = 'debug'
  variant = 'debug'
  cxxflags = buildflags + ['-g', '-O0']

class release(BuildContext):
  cmd = 'release'
  variant = 'release'
  cxxflags = buildflags + ['-O2']
