build = 
{
   platforms = { "Win64" },
   configs = { "Debug", "Release" },
   excludedirs = 
   {
         "../src/burner/shock/core/platform/mvsx",
         "../src/burner/shock/core/platform/posix",
         "../src/burner/shock/core/platform/lubuntu",
         "../src/burner/shock/core/platform/asp",
   },

   excludefiles = 
   {
      "../src/burner/main.cpp",
   },

   includedirs = 
   {
      "../src/burner/shock/core/platform/windows",
   },

   buildoptions = 
   {
   },

   links = 
   {
   }
}

include ("premake_shared.lua")