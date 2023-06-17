@echo off
premake5 --file=./premake/premake5_windows.lua vs2022
premake5 --file=./premake/premake5_mvsx_asp.lua gmake2

rem premake5 --file=./premake/premake5_mvsx_asp.lua vs2022
rem premake5 --file=./premake/premake5_lubuntu.lua gmake2
rem premake5 --file=./premake/premake5_lubuntu.lua codelite
rem premake5 --file=./premake/premake5_mvsx_asp.lua codelite

copy .\premake\make.bat_ .\projects\mvsx_asp\make.bat
copy .\premake\linker-fix.txt .\projects\mvsx_asp\linker-fix.txt