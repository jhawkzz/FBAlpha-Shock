makesettings [[
BINPATH  := ../../../fba-toolchains/windows-host/win-linaro-arm-linux-gnueabi/bin
CC       := ${BINPATH}/arm-linux-gnueabi-gcc
CXX      := ${BINPATH}/arm-linux-gnueabi-g++
STRIP    := ${BINPATH}/arm-linux-gnueabi-strip -s
]]

function set_location()
   location "../projects/asp"
end

function set_targetdir()
   targetdir "../projects/asp/bin/%{cfg.buildcfg}"
end

function set_defines()
   defines { "INLINE = static inline", "SH2_INLINE = static inline", "LSB_FIRST", "ASP" }
   defines { "USE_SPEEDHACKS" }
end

function set_kind()
   kind "ConsoleApp"
end

function set_architecture()
   architecture "ARM"
end

function set_system()
   system "linux"
end

function set_configurations()
   configurations { "Debug", "Release" }
end

function set_links()
   linkoptions { "-static" }

   links {
      "m",
      "pthread",
      "rt",
   }
end

function set_buildoptions()
   buildoptions
   {
      "-static",
      "-s",
      "-fsigned-char",
      "-fsigned-char",
      "-Wno-write-strings",
   }

   filter {"files:**.c"}
      buildoptions {"-std=gnu99"}

   filter {"configurations:Debug"}
      buildoptions { "-O0", "-g" }
   filter {"configurations:Release"}
      buildoptions { "-O3" }
   filter {}
end

function set_additional_includedirs()
   includedirs
   {
      "../src/burner/shock/core/platform/asp",
      "../src/burner/shock/core/platform/posix",
   }
end

function set_exclude_files()
   local excludedirs = 
   {
         "../src/burner/shock/core/platform/lubuntu",
         "../src/burner/shock/core/platform/windows",
         "../src/burner/shock/core/platform/mvsx",
         "../src/burn/drv/midway",
   }

   for _, excludedir in ipairs(excludedirs) do
      removefiles { excludedir .. "/**.cpp", excludedir .. "/**.c", excludedir .. "/**.h" }
   end
   
   removefiles { "../src/burner/winmain.cpp" }
end

include ("premake_shared.lua")
