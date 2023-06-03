function set_location()
   location "../projects/windows"
end

function set_targetdir()
   targetdir "../projects/windows/bin/%{cfg.buildcfg}"
end

function set_defines()
   defines { "INLINE static inline", "SH2_INLINE static inline", "LSB_FIRST", "BUILD_WIN32", "_CRT_SECURE_NO_WARNINGS" }
   defines { "USE_SPEEDHACKS" }
end

function set_kind()
   kind "WindowedApp"
end

function set_architecture()
   architecture "x64"
end

function set_system()
   system "windows"
end

function set_configurations()
   configurations { "Debug", "Release" }
end

function set_links()
   links {}
end

function set_buildoptions()
   buildoptions {}
end

function set_additional_includedirs()
   includedirs { "../src/burner/shock/core/platform/windows" }
end

function set_exclude_files()
   local excludedirs = 
   {
      "../src/burner/shock/core/platform/mvsx_asp",
      "../src/burner/shock/core/platform/posix",
      "../src/burner/shock/core/platform/lubuntu",
   }

   for _, excludedir in ipairs(excludedirs) do
      removefiles { excludedir .. "/**.cpp", excludedir .. "/**.c", excludedir .. "/**.h" }
   end
   
   removefiles { "../src/burner/main.cpp" }
end

include ("premake_shared.lua")