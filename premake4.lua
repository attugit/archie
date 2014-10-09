local archie = solution "archie"
  location "build"
  configurations { "debug", "release" }
  buildoptions { "-Wall", "-Werror", "-pedantic", "-std=c++1y" }
  language "C++"

local gtest = project "gtest"
  kind "StaticLib"
  files { "gtest/src/*.cc" }
  includedirs { "." }
  includedirs { "gtest/" }
  configuration "debug"
    defines { "DEBUG" }
    flags { "Symbols" }
    targetdir "build/debug/lib"
  configuration "release"
    defines { "NDEBUG" }
    flags { "Optimize" }
    targetdir "build/release/lib"

local ut = project "unittest"
  includedirs { "include/" }
  kind "ConsoleApp"
  links { "gtest", "pthread" }
  files { "test/*.cpp" }
--[[
  buildoptions { "-fprofile-arcs", "-ftest-coverage", "-pg" }
  links { "gcov" }
  linkoptions { "-pg" }
--]]
  configuration "debug"
    defines { "DEBUG" }
    flags { "Symbols" }
    targetdir "build/debug/bin"
    postbuildcommands { archie.location .. "/debug/bin/" .. ut.name }
  configuration "release"
    defines { "NDEBUG" }
    flags { "Optimize" }
    targetdir "build/release/bin"
    postbuildcommands { archie.location .. "/release/bin/" .. ut.name }
