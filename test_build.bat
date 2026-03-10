@echo off
echo === 开始构建测试 ===
echo.

echo 1. 检查当前目录
cd /d "%~dp0"
echo 当前目录: %CD%

echo.
echo 2. 设置基本环境变量
set JACKC_SOLUTION_DIR=%~dp0
set JACKC_SOLUTION_INC_DIR=%JACKC_SOLUTION_DIR%Include
echo JACKC_SOLUTION_DIR: %JACKC_SOLUTION_DIR%
echo JACKC_SOLUTION_INC_DIR: %JACKC_SOLUTION_INC_DIR%

echo.
echo 3. 检查重要文件是否存在
if exist "%JACKC_SOLUTION_INC_DIR%\App\IAppFactory.h" (
    echo ✓ IAppFactory.h 文件存在
) else (
    echo ✗ IAppFactory.h 文件不存在
    exit /b 1
)

echo.
echo 4. 检查MSBuild是否可用
where msbuild >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo ✓ MSBuild 可用
    for /f "tokens=*" %%i in ('where msbuild') do echo   位置: %%i
) else (
    echo ✗ MSBuild 不可用
    exit /b 1
)

echo.
echo 5. 测试编译单个文件（可选）
echo   如需测试编译，请运行: msbuild JackOcctQuiz.sln /p:Configuration=Debug /p:Platform=x64

echo.
echo === 构建测试完成 ===
pause