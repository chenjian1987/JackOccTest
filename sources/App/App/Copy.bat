:: ============================
:: 注意: 更新此文件后，请在CubeResUpdateHistory.txt做备注，否则不会触发该脚本。
:: ============================
@echo off
setlocal enabledelayedexpansion

:: 获取脚本所在目录
cd /d "%~dp0"

:: 参数解析
set "ROOTDIR=%~1"
set "CONFIG=%~2"
set "PLATFORM=%~3"
set "OUTDIR=%~4"

:: 验证必要参数
if "!OUTDIR!"=="" (
    echo 错误：缺少输出目录参数
    exit /b 1
)

:: 创建主输出目录
if not exist "!OUTDIR!\" (
    mkdir "!OUTDIR!"
    if errorlevel 1 (
        echo 无法创建目录: !OUTDIR!
        exit /b 1
    )
)

:: ============================
:: Debug版资源部署
:: ============================
echo 正在部署 Debug 版资源...

:: 创建必要的子目录
for %%d in ("tls" "platforms" "Resource") do (
    if not exist "!OUTDIR!\%%~d\" (
        mkdir "!OUTDIR!\%%~d"
        if errorlevel 1 (
            echo 无法创建目录: !OUTDIR!\%%~d
            exit /b 1
        )
    )
)

:: 复制单个文件
call :copyfile "%ROOTDIR%thirdParty\freetype-2.5.5-vc14-64\bin\freetype.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\JackUtility\dll\JackCGeometryUtility.dll" "%OUTDIR%"

:: Qt 核心库
for %%f in (Concurrent Core Gui Help Widgets Network Core5Compat) do (
    call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\Qt6%%fd.dll" "%OUTDIR%"
)

:: TLS 组件
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\tls\qcertonlybackendd.dll" "%OUTDIR%\tls\"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\tls\qopensslbackendd.dll" "%OUTDIR%\tls\"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\bin\tls\qschannelbackendd.dll" "%OUTDIR%\tls\"

:: 平台插件
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\plugins\platforms\qminimald.dll" "%OUTDIR%\platforms\"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\plugins\platforms\qoffscreend.dll" "%OUTDIR%\platforms\"
call :copyfile "%ROOTDIR%thirdParty\Qt6.3\plugins\platforms\qwindowsd.dll" "%OUTDIR%\platforms\"

:: 第三方库
call :copydirectory "%ROOTDIR%thirdParty\opencascade-7.6.0\Bin\x64Debug" "%OUTDIR%"
call :copydirectory "%ROOTDIR%thirdParty\jsoncpp\lib\x64\Debug" "%OUTDIR%"
call :copydirectory "%ROOTDIR%thirdParty\pugixml\lib\x64\Debug" "%OUTDIR%"

:: 内存管理和诊断工具
call :copyfile "%ROOTDIR%thirdParty\tcmalloc\lib\%PLATFORM%\Debug\libtcmalloc_minimal.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\vld2.5.1\%PLATFORM%\Debug\dbghelp.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\vld2.5.1\%PLATFORM%\Debug\vld_x64.dll" "%OUTDIR%"
call :copyfile "%ROOTDIR%thirdParty\vld2.5.1\%PLATFORM%\Debug\Microsoft.DTfW.DHL.manifest" "%OUTDIR%"

:: UI组件
call :copyfile "%ROOTDIR%thirdParty\SARibbon\bin\SARibbonBard.dll" "%OUTDIR%"

:: 多线程库
call :copydirectory "%ROOTDIR%thirdParty\tbb_2017.0.100\bin\x64Debug" "%OUTDIR%"

:: 图像处理
call :copyfile "%ROOTDIR%thirdParty\freeimage-3.17.0-vc14-64\bin\FreeImage.dll" "%OUTDIR%"

:: 多媒体处理
for %%f in (avcodec-57 avdevice-57 avfilter-6 avformat-57 avutil-55 swscale-4) do (
    call :copyfile "%ROOTDIR%thirdParty\ffmpeg-3.3.4-64\bin\%%f.dll" "%OUTDIR%"
)

:: IFC支持
call :copydirectory "%ROOTDIR%thirdParty\ifcplusplus\libs" "%OUTDIR%"

:: 应用程序资源
call :copydirectory "%ROOTDIR%data\style" "%OUTDIR%\Resource"
call :copydirectory "%ROOTDIR%data\imgs" "%OUTDIR%\Resource"

echo 资源部署完成!
goto :eof

:: ==================== 函数定义 ====================
:: 删除文件夹及内容
:delete_folders_and_files
rd /s /q "%~1" 2>nul
if exist "%~1" (
    echo 无法删除目录: %~1
    exit /b 1
)
goto :eof

:: 复制单个文件
:copyfile
if not exist "%~1" (
    echo 错误：源文件不存在 "%~1"
    exit /b 1
)

set "target_dir=%~dp2"
if not exist "!target_dir!" (
    mkdir "!target_dir!"
    if errorlevel 1 (
        echo 无法创建目录: !target_dir!
        exit /b 1
    )
)

xcopy /y /f "%~1" "%~2" >nul
if errorlevel 1 (
    echo 复制失败: "%~1" 到 "%~2"
    exit /b 1
)
goto :eof

:: 复制整个目录
:copydirectory
if not exist "%~1\" (
    echo 错误：源目录不存在 "%~1"
    exit /b 1
)

xcopy /e /y /i "%~1" "%~2\" >nul
if errorlevel 1 (
    echo 目录复制失败: "%~1" 到 "%~2"
    exit /b 1
)
goto :eof