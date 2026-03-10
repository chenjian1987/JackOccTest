@echo off
call env.bat vc143 Debug
msbuild JackOcctQuiz.sln /t:Clean /p:Configuration=Debug /p:Platform=x64
