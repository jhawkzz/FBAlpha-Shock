@echo off
premake5 --file=./premake/premake5_windows.lua vs2022
premake5 --file=./premake/premake5_mvsx.lua gmake2
premake5 --file=./premake/premake5_asp.lua gmake2

rem premake5 --file=./premake/premake5_mvsx.lua vs2022
rem premake5 --file=./premake/premake5_asp.lua vs2022
rem premake5 --file=./premake/premake5_lubuntu.lua gmake2
rem premake5 --file=./premake/premake5_lubuntu.lua codelite
rem premake5 --file=./premake/premake5_mvsx.lua codelite
rem premake5 --file=./premake/premake5_asp.lua codelite

copy .\premake\make.bat_ .\projects\asp\make.bat
copy .\premake\make.bat_ .\projects\mvsx\make.bat