@echo off
cd Build && ^
msbuild Glitter.sln && ^
cd Glitter\Debug && ^
Glitter.exe
cd \Users\jalex\git\RoboRender
