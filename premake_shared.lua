   systemversion "latest"
   location "projects"
   filter { "platforms:asp" }
      architecture "x86"
   filter { "platforms:lubuntu" }
      architecture "x86"
   filter { "platforms:mvsx" }
      architecture "x86"
   filter "platforms:Win64"
      architecture "x64"

project "FBAlpha-Shock"
   language "C++"
   characterset ("MBCS")
   location "projects"
   filter { "platforms:asp" }
      kind "ConsoleApp"
      targetdir "projects/asp/bin/%{cfg.buildcfg}"
      defines { "INLINE = static inline", "SH2_INLINE = static inline", "LSB_FIRST", "ASP" }
   filter { "platforms:lubuntu" }
      kind "ConsoleApp"
      targetdir "projects/lubuntu/bin/%{cfg.buildcfg}"
      defines { "INLINE = static inline", "SH2_INLINE = static inline", "LSB_FIRST", "LUBUNTU" }
   filter { "platforms:mvsx" }
      kind "ConsoleApp"
      targetdir "projects/mvsx/bin/%{cfg.buildcfg}"
      defines { "INLINE = static inline", "SH2_INLINE = static inline", "LSB_FIRST", "MVSX" }
   filter "platforms:Win64"
      kind "WindowedApp"
      targetdir "projects/win64/bin/%{cfg.buildcfg}"
      defines { "INLINE static inline", "SH2_INLINE static inline", "LSB_FIRST", "BUILD_WIN32" }

   -- Set the directories containing source files
   local sourceDirs = {
      "src",
      -- Add more source directories here
   }

   -- Set the include directories
   includedirs {
      "src/burn",
      "src/burn/devices",
      "src/burn/drv/sega",
      "src/burn/drv/dataeast",
      "src/burn/drv/konami",
      "src/burn/snd",
      "src/burn/drv/taito",
      "src/burner",
      "src/burner/shock/core/platform/windows",
      "src/cpu",
      "src/cpu/i8039",
      "src/cpu/i8051",
      "src/cpu/i8x41",
      "src/cpu/m6805",
      "src/cpu/tms32010",
      "src/cpu/upd7725",
      "src/cpu/upd7810",
      "src/cpu/v60",
      "src/cpu/z180",
      "src/cpu/z80",
      "src/dep/generated",
      "src/dep/libs/zlib",
      "src/intf",
      "src/intf/audio",
      "src/intf/cd",
   }

   -- Set the directories to exclude
   local excludeDirs = {
      "src/burner/shock/platform/mvsx",
      "src/burner/shock/platform/posix",
      "src/burner/shock/core//lubuntu",
      "src/burner/shock/platform/asp",
      "src/burner/shock/core/platform/mvsx",
      "src/burner/shock/core/platform/posix",
      "src/burner/shock/core/platform/lubuntu",
      "src/burner/shock/core/platform/asp",
   }

   -- Collect source files recursively from the specified directories
   local sources = {}
   for _, sourceDir in ipairs(sourceDirs) do
      files { sourceDir .. "/**.cpp", sourceDir .. "/**.c", sourceDir .. "/**.h" }
   end

   -- Remove the excluded directories from the list of source files
   for _, excludeDir in ipairs(excludeDirs) do
      removefiles { excludeDir .. "/**.cpp", excludeDir .. "/**.c", excludeDir .. "/**.h" }
   end

   removefiles { "src/burner/main.cpp" }
   removefiles { "src/cpu/a68k/fba_make68k.c" }
   removefiles { "src/cpu/a68k/mips/fba_make68k.c" }

   -- Create the folder structure in Visual Studio
   filter "files:**.h or **.hpp"
      local function getGroupPath(filePath)
         local groupPath = path.getdirectory(filePath)
         groupPath = groupPath:gsub("/", "\\")
         return groupPath
      end

      local function getGroupName(groupPath)
         local groupName = groupPath:gsub("\\", "_")
         return groupName
      end

      local function getSourceDir(filePath)
         for _, sourceDir in ipairs(sourceDirs) do
            if filePath:find(sourceDir) == 1 then
               return sourceDir
            end
         end
         return nil
      end

      filter {}

      local allFiles = {}
      for _, sourceDir in ipairs(sourceDirs) do
         local dirFiles = os.matchfiles(sourceDir .. "/**.cpp") -- Collect all cpp files
         table.insert(allFiles, dirFiles)
         dirFiles = os.matchfiles(sourceDir .. "/**.h") -- Collect all header files
         table.insert(allFiles, dirFiles)
      end

      local fileFilters = {}
      for _, dirFiles in ipairs(allFiles) do
         for _, dirFile in ipairs(dirFiles) do
            local filePath = path.getrelative(os.getcwd(), dirFile)
            local groupPath = getGroupPath(filePath)
            local groupName = getGroupName(groupPath)
            local sourceDir = getSourceDir(filePath)

            if sourceDir then
               fileFilters[groupName] = fileFilters[groupName] or {}
               table.insert(fileFilters[groupName], filePath)
            end
         end
      end
   
      for groupName, files in pairs(fileFilters) do
         filter("files:" .. table.concat(files, ","))
         local sourceDir = getSourceDir(files[1])
         local groupPath = getGroupPath(files[1])
         group(groupPath)
         filter {}
      end
