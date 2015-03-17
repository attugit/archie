newoption {
  trigger = "libc++",
  description = "C++ Standard Library (clang only)",
}

newoption {
  trigger = "use-std-tuple",
  description = "Choose std::tuple implementation",
}

newoption {
  trigger = "testcmd",
  description = "Prefix argument to test command",
}

TESTCMD = _OPTIONS["testcmd"] or ""

for _,file in pairs(os.matchfiles("*.cpp"))
do
  if string.find(file, ".cpp$") then
    local name = string.sub(file, 0, -5)
    project (name)
      includedirs { "../include" }
      kind "ConsoleApp"
      files { file }
      local test = TESTCMD .. " ./"..name
      postbuildcommands{ test }
      configuration "use-std-tuple"
        defines { "USE_STD_TUPLE" }
      configuration "not use-std-tuple"
        defines { "USE_ARCHIE_TUPLE" }
      configuration "libc++"
        defines { "USE_LIBCPP" }
  end
end
