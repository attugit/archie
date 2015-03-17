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
  configurations { "debug", "release" }
  buildoptions { "-std=c++1y" }
  language "C++"
  flags {
    "ExtraWarnings",
    "FatalWarnings",
    "NoExceptions",
    "NoRTTI",
    "StaticRuntime"}
  configuration "release"
    flags { "OptimizeSpeed" }
  configuration "debug"
    flags { "Symbols" }

  include "test"
