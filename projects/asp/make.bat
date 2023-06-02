@echo off

if "%~1"=="clean" goto clean

if not exist "bin" (mkdir bin)
if not exist "obj" (mkdir obj)

if "%~1"=="debug"	goto debug
goto release

:release
	make.exe config=release
	goto eof

:debug
	make.exe config=debug
	goto eof

:clean
	if exist "bin" (rmdir /S /Q bin)
	if exist "obj" (rmdir /S /Q obj)
	goto eof
	
:eof
