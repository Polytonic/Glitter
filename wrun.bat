@echo off

SET STARTING_DIR=%cd%

cd Build && ^
msbuild Glitter.sln && ^
cd Glitter\Debug && ^
Glitter.exe %*
cd %STARTING_DIR%
