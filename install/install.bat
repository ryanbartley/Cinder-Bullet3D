@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

cd ..

echo removing directorys
rmdir .\lib\msw /q /s
echo adding new directorys
mkdir .\lib\msw\Debug
mkdir .\lib\msw\Release

cd bullet3\build3

call :build_prog Debug _vs2010_x64_debug
call :build_prog Release _vs2010_x64_release 

cd ../..

exit /B

:build_prog
premake4 --no-test --no-demos --targetdir="../../lib/msw/%~n1" vs2010
cd vs2010
echo going through
devenv 0_Bullet3Solution.sln /upgrade
for %%i in (*.vcxproj) do call :check_and_build "%%i" %~n1 
cd ..
rmdir /s /q vs2010
cd ../Extras
rmdir /s /q lib
cd ../../lib/msw/%~n1
for %%i in (*.lib) do call :change_filename "%%i" %~n2 
cd ../../../bullet3/build3

:check_and_build
echo "Building: %1 in %2"
set project_file=%~nx1
set target_name=%~n1
rem echo %target_name%
if x%project_file:App=%==x%project_file% (
	rem echo Building %project_file%
	msbuild %project_file% /p:PlatformToolset=v140 /p:AssemblyName=%target_name% /p:Configuration=%2 /p:Platform=x64 /t:Build
)

:change_filename
set old_filename=%~n1
set remove_portion=%~nx2
set replacement=
call set removed_filename=%%old_filename:%remove_portion%=%replacement%%%
echo about to remove %remove_portion% from %old_filename% which is %removed_filename%
rename %~nx1 %removed_filename%.lib