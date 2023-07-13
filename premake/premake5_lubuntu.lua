function set_location()
   location "../projects/lubuntu"
end

function set_targetdir()
   targetdir "../projects/lubuntu/bin/%{cfg.buildcfg}"
end

function set_defines()
   defines { "INLINE = static inline", "SH2_INLINE = static inline", "LSB_FIRST", "LUBUNTU" }
   defines { "USE_SPEEDHACKS" }
end

function set_kind()
   kind "ConsoleApp"
end

function set_architecture()
   architecture "x86"
end

function set_system()
   system "linux"
end

function set_links()
   links {
      "m",
      "pthread",
      "rt",
      "asound",
   }
end

function set_buildoptions()
   buildoptions
   {
      "-Wl,-Bdynamic",
      "-s",
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
      "../src/burner/shock/core/platform/lubuntu",
      "../src/burner/shock/core/platform/posix",
   }
end

function set_exclude_files()
   local excludedirs = 
   {
         "../src/burner/shock/core/platform/mvsx_asp",
         "../src/burner/shock/core/platform/windows",
   }

   for _, excludedir in ipairs(excludedirs) do
      removefiles { excludedir .. "/**.cpp", excludedir .. "/**.c", excludedir .. "/**.h" }
   end
   
   removefiles { "../src/burner/winmain.cpp" }
end

include ("premake_shared.lua")
