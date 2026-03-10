@echo off

set "PLATFORM=x64"
set "Configuration=Debug"
set "VCVER=vc143"

:: 解析命令行参数：第一个参数为编译器版本，第二个为配置
if not ["%1"] == [""] set "VCVER=%1"
if not ["%2"] == [""] set "Configuration=%2"

set "JACKC_SOLUTION_DIR=%~dp0"
set "JACKC_SOLUTION=%JACKC_SOLUTION_DIR%JackOcctQuiz.sln"

echo Solution Path: %JACKC_SOLUTION%
echo Configuration: %Configuration%
echo Platform:      %PLATFORM%

:: --- 路径设置 ---
set "JACKC_SOLUTION_SOURCE_DIR=%JACKC_SOLUTION_DIR%sources"
set "JACKC_BUILD_OUTPUT_DIR=%JACKC_SOLUTION_DIR%output"
set "JACKC_SOLUTION_INC_DIR=%JACKC_SOLUTION_DIR%Include"
set "JACKC_SOLUTION_LIB_DIR=%JACKC_SOLUTION_DIR%lib\%PLATFORM%\%Configuration%"
set "JACKC_SOLUTION_BIN_DIR=%JACKC_SOLUTION_DIR%bin\%PLATFORM%\%Configuration%"
set "JACKC_SOLUTION_3DPARTY_DIR=%JACKC_SOLUTION_DIR%thirdParty"

:: --- 第三方库路径 (示例) ---
set "JACKC_OCC_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\opencascade-7.6.0\Include"
set "JACKC_OCC_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\opencascade-7.6.0\Lib\x64Debug"
set "JACKC_QT_DIR=%JACKC_SOLUTION_DIR%thirdParty\Qt6.3"

:: --- 编译器版本解析逻辑 ---
set "PATH=%PATH%;%ProgramFiles(x86)%\Microsoft Visual Studio\Installer"
set "VCFMT=%VCVER%"
set "VCPROP=NativeDesktop"
set "VisualStudioExpressName=VCExpress"

if not "%DevEnvDir%" == "" (
    rem 使用预定义的 DevEnvDir
) else if /I "%VCFMT%" == "vc9" (
    set "DevEnvDir=%VS90COMNTOOLS%..\IDE"
) else if /I "%VCFMT%" == "vc10" (
    set "DevEnvDir=%VS100COMNTOOLS%..\IDE"
) else if /I "%VCFMT%" == "vc11" (
    set "DevEnvDir=%VS110COMNTOOLS%..\IDE"
    set "VisualStudioExpressName=WDExpress"
) else if /I "%VCFMT%" == "vc12" (
    set "DevEnvDir=%VS120COMNTOOLS%..\IDE"
    set "VisualStudioExpressName=WDExpress"
) else if /I "%VCFMT%" == "vc14" (
    set "DevEnvDir=%VS140COMNTOOLS%..\IDE"
) else if /I "%VCFMT%" == "vc141" (
    for /f "usebackq delims=" %%i in (`vswhere.exe -version "[15.0,15.99]" -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do set "DevEnvDir=%%i\Common7\IDE\"
) else if /I "%VCFMT%" == "vc142" (
    for /f "usebackq delims=" %%i in (`vswhere.exe -version "[16.0,16.99]" -latest -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do set "DevEnvDir=%%i\Common7\IDE\"
) else if /I "%VCFMT%" == "vc143" (
    for /f "usebackq delims=" %%i in (`vswhere.exe -version "[17.0,17.99]" -latest -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do set "DevEnvDir=%%i\Common7\IDE\"
) else if /I "%VCFMT%" == "vc144" (
    :: --- 新增 VS 2026 支持 ---
    for /f "usebackq delims=" %%i in (`vswhere.exe -version "[18.0,18.99]" -latest -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do set "DevEnvDir=%%i\Common7\IDE\"
) else (
    echo Error: Unsupported Visual C++ version: %VCVER%
    echo Supported versions: vc9, vc10, vc11, vc12, vc14, vc141, vc142, vc143, vc144 (VS 2026)
    exit /B
)

:: --- 启动 Visual Studio ---
echo Visual Studio IDE Path: %DevEnvDir%

if exist "%DevEnvDir%devenv.exe" (
    start "" "%DevEnvDir%devenv.exe" "%JACKC_SOLUTION%"
) else if exist "%DevEnvDir%%VisualStudioExpressName%.exe" (
    start "" "%DevEnvDir%%VisualStudioExpressName%.exe" "%JACKC_SOLUTION%"
) else (
    echo Error: Could not find MS Visual Studio executable in %DevEnvDir%
)