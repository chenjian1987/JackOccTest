@echo off
:: 修改为 Visual Studio 的实际安装路径
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 输出 PATH 以检查 msbuild 是否在路径中
echo %PATH%

:: 检查 msbuild 是否可用
where msbuild
if %ERRORLEVEL% neq 0 (
    echo Error: msbuild not found in PATH. Ensure Visual Studio Build Tools are installed and configured.
    exit /b 1
)

:: 设置项目环境变量
call env.bat vc143 Debug

:: 执行构建
msbuild JackOcctQuiz.sln /p:Configuration=Debug /p:Platform=x64
