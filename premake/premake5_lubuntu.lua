build = 
{
   platforms = { "lubuntu" },
   configs = { "Debug", "Release" },
   excludedirs = 
   {
         "../src/burner/shock/core/platform/mvsx",
         "../src/burner/shock/core/platform/windows",
         "../src/burner/shock/core/platform/asp",
   },

   excludefiles = 
   {
      "../src/burner/winmain.cpp",
   },

   includedirs = 
   {
      "../src/burner/shock/core/platform/lubuntu",
      "../src/burner/shock/core/platform/posix",
   },
}

include ("premake_shared.lua")
