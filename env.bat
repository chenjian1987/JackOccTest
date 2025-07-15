@echo off

set PLATFORM=x64
set "VCVER=vc143"

if not ["%1"]    == [""]      set "VCVER=%1"
set Configuration=Debug
if not ["%2"]    == [""]      set "Configuration=%2"


set JACKC_SOLUTION_DIR=%~dp0
rem Define path to project file
set "JACKC_SOLUTION=%JACKC_SOLUTION_DIR%JackOcctQuiz.sln"
echo %JACKC_SOLUTION%

set JACKC_SOLUTION_SOURCE_DIR=%JACKC_SOLUTION_DIR%sources
set JACKC_BUILD_OUTPUT_DIR=%JACKC_SOLUTION_DIR%output
set JACKC_SOLUTION_INC_DIR=%JACKC_SOLUTION_DIR%Include
set JACKC_SOLUTION_INCI_DIR=%JACKC_SOLUTION_DIR%Inci
set JACKC_SOLUTION_LIB_DIR=%JACKC_SOLUTION_DIR%lib\%PLATFORM%\%Configuration%
set JACKC_SOLUTION_BIN_DIR=%JACKC_SOLUTION_DIR%bin\%PLATFORM%\%Configuration%
set JACKC_SOLUTION_PDB_DIR=%JACKC_SOLUTION_DIR%pdb\%PLATFORM%\%Configuration%
set JACKC_SOLUTION_3DPARTY_DIR=%JACKC_SOLUTION_DIR%thirdParty


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

rem for vc10-12, interpretation is trivial
set VCFMT=%VCVER%
set VCLIB=%VCVER:~0,4%
set VCPROP=NativeDesktop

rem vc14 and later can have optional suffix "-uwp"
if "%VCVER:~-4%" == "-uwp" (
  set VCFMT=%VCVER:~0,-4%
  set VCLIB=%VCLIB%-uwp
  set VCPROP=Universal
)
if "%VCFMT%" == "vclang" (
  set VCLIB=vc110
)

rem ----- Parsing of Visual Studio platform -----
set "VisualStudioExpressName=VCExpress"

if not "%DevEnvDir%" == "" (
  rem If DevEnvDir is already defined (e.g. in custom.bat), use that value
) else if /I "%VCFMT%" == "vc9" (
  set "DevEnvDir=%VS90COMNTOOLS%..\IDE"
) else if /I "%VCFMT%" == "vc10" (
  set "DevEnvDir=%VS100COMNTOOLS%..\IDE"
) else if /I "%VCFMT%" == "vc11" (
  set "DevEnvDir=%VS110COMNTOOLS%..\IDE"
  rem Visual Studio Express starting from VS 2012 is called "for Windows Desktop"
  rem and has a new name for executable - WDExpress
  set "VisualStudioExpressName=WDExpress"
) else if /I "%VCFMT%" == "vc12" (
  set "DevEnvDir=%VS120COMNTOOLS%..\IDE"
  set "VisualStudioExpressName=WDExpress"
) else if /I "%VCFMT%" == "vc14" (
  set "DevEnvDir=%VS140COMNTOOLS%..\IDE"
) else if /I "%VCFMT%" == "vc141" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[15.0,15.99]" -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else if /I "%VCFMT%" == "vc142" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[16.0,16.99]" -latest -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else if /I "%VCFMT%" == "vc143" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[17.0,17.99]" -latest -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else if /I "%VCFMT%" == "vclang" (
  for /f "usebackq delims=" %%i in (`vswhere.exe -version "[16.0,17.99]" -latest -requires Microsoft.VisualStudio.Workload.%VCPROP% -property installationPath`) do (
    set "DevEnvDir=%%i\Common7\IDE\"
  )
) else if /I "%VCFMT%" == "gcc" (
  rem MinGW
) else (
  echo Error: first argument ^(%VCVER%^) should specify supported version of Visual C++, 
  echo one of: 
  echo vc9   = VS 2008 ^(SP1^)
  echo vc10  = VS 2010 ^(SP3^)
  echo vc11  = VS 2012 ^(SP3^)
  echo vc12  = VS 2013 ^(SP3^)
  echo vc14  = VS 2015
  echo vc141 = VS 2017
  echo vc142 = VS 2019
  echo vc143 = VS 2022
  echo vclang = VS 2019 with ClangCL toolset
  exit /B
)
