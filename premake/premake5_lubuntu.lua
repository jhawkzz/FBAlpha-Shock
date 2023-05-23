build = 
{
   platforms = { "lubuntu" },
   configs = { "Debug", "Release" },
   excludedirs = 
   {
         "../src/burner/shock/platform/mvsx",
         "../src/burner/shock/platform/posix",
         "../src/burner/shock/core/lubuntu",
         "../src/burner/shock/platform/asp",
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
      "../src/burner/shock/core/platform/lubuntu",
   },
}

include ("premake_shared.lua")
