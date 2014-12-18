#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='archie'

top = '.'
out = 'build'

def options(opt):
  opt.load('compiler_cxx')

def configure(conf):
  conf.load('compiler_cxx')

def build(bld):
  inc = ['.', 'gtest', 'include']
  gtest = ['gtest/src/gtest-all.cc', 'gtest/src/gtest_main.cc']
  bld.stlib(
    target='gtest',
    includes=inc,
    cxxflags=['-std=c++1y'],
    source=gtest)

  src = bld.path.ant_glob('test/*.cpp')
  bld(
    target='unittest',
    features='cxx cxxprogram',
    includes=inc,
    use='gtest',
    lib=['pthread'],
    cxxflags=['-std=c++1y'],
    source=src)

