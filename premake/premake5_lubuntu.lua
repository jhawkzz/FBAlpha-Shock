function set_location()
   location "../projects/lubuntu"
end

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
   
   buildoptions = 
   {
      "-Wl,-Bdynamic",
      "-s",
   },

   links = 
   {
      "m",
      "pthread",
      "rt",
      "asound",
   }
}

include ("premake_shared.lua")
