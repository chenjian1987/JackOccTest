@echo off

rem 1. 设置基础配置和版本
set PLATFORM=x64
set "VCVER=vc144"

if not ["%1"] == [""] set "VCVER=%1"
set Configuration=Debug
if not ["%2"] == [""] set "Configuration=%2"

set JACKC_SOLUTION_DIR=%~dp0
set "JACKC_SOLUTION=%JACKC_SOLUTION_DIR%JackOcctQuiz.sln"
echo Solution Path: %JACKC_SOLUTION%

rem 2. 设置项目和依赖库路径
set JACKC_SOLUTION_SOURCE_DIR=%JACKC_SOLUTION_DIR%sources
set JACKC_BUILD_OUTPUT_DIR=%JACKC_SOLUTION_DIR%output
set JACKC_SOLUTION_INC_DIR=%JACKC_SOLUTION_DIR%Include
set JACKC_SOLUTION_INCI_DIR=%JACKC_SOLUTION_DIR%Inci
set JACKC_SOLUTION_LIB_DIR=%JACKC_SOLUTION_DIR%lib\%PLATFORM%\%Configuration%
set JACKC_SOLUTION_BIN_DIR=%JACKC_SOLUTION_DIR%bin\%PLATFORM%\%Configuration%
set JACKC_SOLUTION_PDB_DIR=%JACKC_SOLUTION_DIR%pdb\%PLATFORM%\%Configuration%
set JACKC_SOLUTION_3DPARTY_DIR=%JACKC_SOLUTION_DIR%thirdParty

set JACKC_GEOMETRY_UT_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\JackUtility\inc
set JACKC_GEOMETRY_UT_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\JackUtility\lib

set JACKC_PUGXML_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\pugixml\include
set JACKC_PUGXML_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\pugixml\lib\%PLATFORM%\%Configuration%

set JACKC_JSONCPP_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\jsoncpp\include
set JACKC_JSONCPP_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\jsoncpp\lib\%PLATFORM%\%Configuration%

set JACKC_QT_DIR=%JACKC_SOLUTION_DIR%thirdParty\Qt6.3
set JACKC_QT_BIN_DIR=%JACKC_SOLUTION_DIR%\bin
set JACKC_QT_INC_DIR=%JACKC_QT_DIR%\include
set JACKC_QT_LIB_DIR=%JACKC_QT_DIR%\lib

set JACKC_OCC_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\opencascade-7.6.0\Include
set JACKC_OCC_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\opencascade-7.6.0\Lib\x64Debug

set JACKC_TCMALLOC_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\tcmalloc\include
set JACKC_TCMALLOC_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\tcmalloc\lib\%PLATFORM%\%Configuration%

set JACKC_VLD_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\vld2.5.1\include
set JACKC_VLD_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\vld2.5.1\%PLATFORM%\%Configuration%

set JACKC_SARIBBON_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\SARibbon\include
set JACKC_SARIBBON_BIN_DIR=%JACKC_SOLUTION_DIR%thirdParty\SARibbon\bin
set JACKC_SARIBBON_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\SARibbon\lib

set JACKC_IFCOPENSHELL_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\ifcopenshell\include
set JACKC_IFCOPENSHELL_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\ifcopenshell\lib\debug

set JACKC_IFCPP_INC_DIR=%JACKC_SOLUTION_DIR%thirdParty\ifcplusplus
set JACKC_IFCPP_LIB_DIR=%JACKC_SOLUTION_DIR%thirdParty\ifcplusplus\libs

set "PATH=%PATH%;%ProgramFiles(x86)%\Microsoft Visual Studio\Installer"

rem 3. 解析 Visual Studio 平台版本并查找路径
set VCFMT=%VCVER%
set VCLIB=%VCVER:~0,4%
set VCPROP=NativeDesktop

if "%VCVER:~-4%" == "-uwp" (
  set VCFMT=%VCVER:~0,-4%
  set VCLIB=%VCLIB%-uwp
  set VCPROP=Universal
)
if "%VCFMT%" == "vclang" (
  set VCLIB=vc110
)

set "VisualStudioExpressName=VCExpress"

if not "%DevEnvDir%" == "" (
  rem If DevEnvDir is already defined, use that value
) else if /I "%VCFMT%" == "vc142" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[16.0,16.99]" -latest -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else if /I "%VCFMT%" == "vc143" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[17.0,17.99]" -latest -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else if /I "%VCFMT%" == "vc144" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[18.0,18.99]" -latest -prerelease -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else (
  echo Error: Unsupported Visual C++ version ^(%VCVER%^).
  pause
  exit /B
)

if "%DevEnvDir%" == "" (
    echo Error: vswhere.exe could not locate Visual Studio 2026 ^(vc144^).
    pause
    exit /B
)

rem 4. 调用环境配置脚本
echo [DEBUG] Ready to call env.bat with %VCVER%...
call "%~dp0env.bat" %VCVER% %Configuration%
echo [DEBUG] Successfully returned from env.bat!

echo [DEBUG] Visual Studio IDE Path: %DevEnvDir%

rem 5. 启动 Visual Studio
if exist "%DevEnvDir%\devenv.exe" (
  start "" "%DevEnvDir%\devenv.exe" "%JACKC_SOLUTION%"
) else if exist "%DevEnvDir%\%VisualStudioExpressName%.exe" (
  start "" "%DevEnvDir%\%VisualStudioExpressName%.exe" "%JACKC_SOLUTION%"
) else (
  echo Error: Could not find MS Visual Studio ^(%VCVER%^) devenv.exe at "%DevEnvDir%"
  pause
)