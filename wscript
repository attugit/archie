#! /usr/bin/env python
# encoding: utf-8

VERSION = '1.0.0'
APPNAME = 'archie'

top = '.'
out = 'build'
flags = [
    '-std=c++1z',
    '-fconcepts',
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


def init(ctx):
    ctx.load('build_logs')

import os


def options(opt):
    opt.load('compiler_cxx')
    opt.load('waf_unit_test')


def configure(conf):
    conf.load('waf_unit_test')
    conf.load('compiler_cxx')

from waflib.Tools import waf_unit_test


def build(bld):
    if not bld.variant:
        bld.fatal('try "waf --help"')
    if bld.variant == 'debug':
        bld.env.CXXFLAGS += ['-g', '-O0']
        bld.env.DEFINES += ['DEBUG']
    if bld.variant == 'release':
        bld.env.CXXFLAGS += ['-O3', '-march=native', '-fPIC', '-fno-rtti']
        bld.env.DEFINES += ['NDEBUG']
    if bld.variant == 'coverage':
        bld.env.CXXFLAGS += ['-g', '-O0',
                             '-fprofile-arcs', '-ftest-coverage', '-pg']
        bld.env.LINKFLAGS += ['-pg']
        bld.env.DEFINES += ['NDEBUG']
        bld.env.LIB += ['gcov']
    bld.env.CXXFLAGS += flags
    bld.define('APPNAME', APPNAME)
    bld.define('VERSION', VERSION)
    bld.env.INCLUDES += ['include', 'test', 'gtest', 'gtest/include']
    bld(source=bld.path.ant_glob(['gtest/src/gtest_main.cc',
                                  'gtest/src/gtest-all.cc']),
        target='GTEST',
        features='cxx',
        install_path=None,
        cxxflags=bld.env.CXXFLAGS + ['-Wno-sign-conversion'],
        )
    bld.env.LIB += ['pthread']
    for suite in os.listdir('./test'):
        if 'ignore' not in suite and os.path.isdir(
                os.path.join('./test', suite)):
            bld(
                source=bld.path.ant_glob(['test/%s/**/*.cpp' % suite]),
                target='ut_' + str(suite),
                features='cxx cxxprogram test',
                use=['GTEST'],
                install_path=None,
            )
    bld.add_post_fun(waf_unit_test.summary)
    bld.add_post_fun(waf_unit_test.set_exit_code)
    inc = bld.path.find_dir('include')
    for f in bld.path.ant_glob(['include/archie/**/*.hpp']):
        bld.install_files(
            '${PREFIX}/include/' +
            f.path_from(inc).replace(
                f.name,
                ''),
            f)

from waflib.Build import BuildContext
from waflib.Build import CleanContext
from waflib.Build import InstallContext
from waflib.Build import UninstallContext

for ctx in (BuildContext, CleanContext, InstallContext, UninstallContext):
    name = ctx.__name__.replace('Context', '').lower()

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
        variant = 'debug'
