set Configuration=Debug

call "%~dp0env.bat" vc143 %Configuration%

echo %DevEnvDir%

rem Launch Visual Studio - either professional(devenv) or Express, as available

rem Launch Visual Studio - either professional (devenv) or Express, as available
if exist "%DevEnvDir%\devenv.exe"  (
  start "" "%DevEnvDir%\devenv.exe" "%JACKC_SOLUTION%"
) else if exist "%DevEnvDir%\%VisualStudioExpressName%.exe"  (
  start "" "%DevEnvDir%\%VisualStudioExpressName%.exe" "%JACKC_SOLUTION%"
) else (
  echo Error: Could not find MS Visual Studio ^(%VCVER%^)
  echo For VS 2010-2019, check relevant environment variable ^(e.g. VS100COMNTOOLS^)
)