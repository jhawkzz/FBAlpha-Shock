workspace "FBAlpha-Shock"
   systemversion "latest"
   set_configurations()
   set_location()
   set_architecture()
   set_system()
   
project "FBAlpha-Shock"
   language "C++"
   characterset ("MBCS")
   set_location()
   set_targetdir()
   set_defines()
   set_kind()
   
   -- Set the directories containing source files
   local sourceDirs = {
      "../src",
      -- Add more source directories here
   }

   -- Set the include directories
   includedirs {
      "../src/burn",
      "../src/burn/devices",
      "../src/burn/drv/capcom",
      "../src/burn/drv/cave",
      "../src/burn/drv/dataeast",
      "../src/burn/drv/konami",
      "../src/burn/drv/neogeo",
      "../src/burn/drv/psikyo",
      "../src/burn/drv/sega",
      "../src/burn/drv/taito",
      "../src/burn/drv/toaplan",
      "../src/burner",
      "../src/burn/snd",
      "../src/cpu",
      "../src/cpu/i8039",
      "../src/cpu/i8051",
      "../src/cpu/i8x41",
      "../src/cpu/m6805",
      "../src/cpu/tms32010",
      "../src/cpu/upd7725",
      "../src/cpu/upd7810",
      "../src/cpu/v60",
      "../src/cpu/z180",
      "../src/cpu/z80",
      "../src/dep/generated",
      "../src/dep/libs/zlib",
      "../src/intf",
      "../src/intf/audio",
      "../src/intf/cd",
   }

   local sources = {}
   for _, sourceDir in ipairs(sourceDirs) do
      files { sourceDir .. "/**.cpp", sourceDir .. "/**.c", sourceDir .. "/**.h" }
   end

   set_buildoptions()
   set_links()
   set_additional_includedirs()

   set_exclude_files()

   removefiles {
      "../src/cpu/a68k/fba_make68k.c",
      "../src/cpu/a68k/mips/fba_make68k.c"
   }

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
