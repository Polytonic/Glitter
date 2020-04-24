@echo off

SET STARTING_DIR=%cd%

cd Build && ^
msbuild Glitter.sln /p:Configuration=Release && ^
cd Glitter\Release && ^
Glitter.exe %*
cd %STARTING_DIR%
