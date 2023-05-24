function set_location()
   location "../projects/mvsx"
end

build = 
{
   platforms = { "mvsx" },
   configs = { "Debug", "Release" },
   excludedirs = 
   {
         "../src/burner/shock/core/platform/lubuntu",
         "../src/burner/shock/core/platform/windows",
         "../src/burner/shock/core/platform/asp",
   },

   excludefiles = 
   {
      "../src/burner/winmain.cpp",
   },

   includedirs = 
   {
      "../src/burner/shock/core/platform/mvsx",
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
   
   --FBA_ZLIB_DIRS := $(FBA_LIB_DIR)/zlib

--LIBS      := -Wl,-Bdynamic -s -lm -lpthread -lrt -lasound
--FBA_LIB_DIR                     := $(MAIN_FBA_DIR)/dep/libs
--FBA_SRC_DIRS += $(FBA_ZLIB_DIRS)

}

include ("premake_shared.lua")
