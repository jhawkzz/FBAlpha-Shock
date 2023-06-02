@echo off

if "%~1"=="clean" goto clean

if not exist "bin" (mkdir bin)
if not exist "obj" (mkdir obj)

if "%~1"=="debug"	goto debug
goto release

:release
	make.exe config=release

:debug
	make.exe config=debug

:clean
	if exist "bin" (rmdir /S /Q bin)
	if exist "obj" (rmdir /S /Q obj)
