@echo off
call env.bat vc143 Release
msbuild JackOcctQuiz.sln /p:Configuration=Release /p:Platform=x64
