_ACTION = _ACTION or "gmake"

newoption {
  trigger = "targetdir",
  value = "PATH",
  description = "Choose target directory"
}

TARGETDIR = _OPTIONS["targetdir"] or "build"

local archie = solution "archie"
  location ( TARGETDIR )
  targetdir ( TARGETDIR )
  configurations { "debug", "release", "coverage" }
  language "C++"
  flags {
    "C++14",
    "ExtraWarnings",
    "FatalWarnings",
    "NoExceptions",
    "NoRTTI",
    "StaticRuntime"}
  buildoptions {
    '-Wshadow',
    '-Wnon-virtual-dtor',
    '-Wold-style-cast',
    '-Wcast-align',
    '-Wno-parentheses',
    '-march=native'
  }
  configuration "release"
    flags { "OptimizeSpeed" }
  configuration "debug"
    flags { "Symbols" }
  configuration "coverage"
    buildoptions {
      "-ftest-coverage",
      "-fprofile-arcs",
      "-pg"}
    linkoptions { "-pg" }
    links { "gcov" }


  include "test"
