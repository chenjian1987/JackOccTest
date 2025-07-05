:: ============================
:: 注意: 更新此文件后，请在CubeResUpdateHistory.txt做备注，否则不会触发该脚本。
:: ============================
 
::echo %cd%
::echo %~dp0
cd %~dp0

rem @echo off

set ROOTDIR=%~1
set CONFIG=%~2
set PLATFORM=%~3
set OUTDIR=%~4
echo "ROOTDIR" "%ROOTDIR%"
echo "CD" "%CD%"
echo "%OUTDIR%"


:: ============================
:: Debug版
:: ============================
:: x64Debugfiles
call :copyfile "%ROOTDIR%thirdParty\freetype-2.5.5-vc14-64\bin\freetype.dll" "%OUTDIR%"

call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Concurrentd.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Cored.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Guid.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Helpd.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Widgetsd.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Networkd.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6Core5Compatd.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\tls\qcertonlybackendd.dll" "%OUTDIR%\tls\
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\tls\qopensslbackendd.dll" "%OUTDIR%\tls\
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\tls\qschannelbackendd.dll" "%OUTDIR%\tls\
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\plugins\platforms\qminimald.dll" "%OUTDIR%\platforms\
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\plugins\platforms\qoffscreend.dll" "%OUTDIR%\platforms\
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\plugins\platforms\qwindowsd.dll" "%OUTDIR%\platforms\

call :copydirectory "%ROOTDIR%thirdParty\opencascade-7.6.0\Bin\x64Debug" "%OUTDIR%"
:: call :copydirectory "%ROOTDIR%thirdParty\occt-7.9.1\dll\debug" "%OUTDIR%"
:: call :copydirectory "%ROOTDIR%thirdParty\opencascade-7.6.0\7.7dlls" "%OUTDIR%"
call :copydirectory "%ROOTDIR%thirdParty\jsoncpp\lib\x64\Debug" "%OUTDIR%"
call :copydirectory "%ROOTDIR%thirdParty\pugixml\lib\x64\Debug" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\tcmalloc\lib\%PLATFORM%\Debug\libtcmalloc_minimal.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\vld2.5.1\%PLATFORM%\Debug\dbghelp.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\vld2.5.1\%PLATFORM%\Debug\vld_x64.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\vld2.5.1\%PLATFORM%\Debug\Microsoft.DTfW.DHL.manifest" "%OUTDIR%"

call :copyfile "%ROOTDIR%thirdParty\SARibbon\bin\SARibbonBard.dll" "%OUTDIR%"

call :copydirectory "%ROOTDIR%thirdParty\tbb_2017.0.100\bin\x64Debug" "%OUTDIR%"

call :copyfile "%ROOTDIR%thirdParty\freeimage-3.17.0-vc14-64\bin\FreeImage.dll" "%OUTDIR%"

call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\avcodec-57.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\avdevice-57.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\avfilter-6.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\avformat-57.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\avutil-55.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\swscale-4.dll" "%OUTDIR%"

call :copydirectory "%ROOTDIR%thirdParty\ifcplusplus\libs" "%OUTDIR%"

md "%OUTDIR%\Resource"

:: ===resources============================================
call :copydirectory "%ROOTDIR%data\style" "%OUTDIR%\Resource"
call :copydirectory "%ROOTDIR%data\imgs"  "%OUTDIR%\Resource"





goto done

:: ========================================================
:done
:: ========================================================

goto:eof

:: Delete all files and folders/subfoldrs.
:: Usage: Require top folder name as the only argument 
:delete_folders_and_files
del /q /s %~1
IF %ERRORLEVEL% GTR 0 (
    echo 不能删除 %~1 里的文件
    exit %ERRORLEVEL%)
rd /q /s %~1
IF %ERRORLEVEL% GTR 0 (
    echo 不能删除 %~1 或里面的文件夹
    exit %ERRORLEVEL%)
goto:eof

:copyfile 
xcopy /y /f %~1 %~2
IF %ERRORLEVEL% GTR 0 (
    echo Fail to copy "%~1" to "%~2"
    exit %ERRORLEVEL%)
goto:eof

:copy_and_modify_name
copy /y %~1 %~2
IF %ERRORLEVEL% GTR 0 (
    echo Fail to copy "%~1" to "%~2"
    exit %ERRORLEVEL%
)
goto:eof

:copydirectory
xcopy /e /y %~1 %~2
IF %ERRORLEVEL% GTR 0 (
    echo Fail to copy "%~1" to "%~2"
    exit %ERRORLEVEL%)
goto:eof
