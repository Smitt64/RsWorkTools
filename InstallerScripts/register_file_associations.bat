@echo off
setlocal EnableDelayedExpansion

rem ============================================================
rem File associations registration:
rem   *.lbr            -> WorkRes.exe
rem   *.md, *.markdown -> MarkdownViewer.exe
rem
rem Registers for the current user (HKCU\Software\Classes),
rem admin rights are NOT required.
rem
rem Usage:
rem   register_file_associations.bat            - exe next to the bat
rem   register_file_associations.bat "D:\bin"   - exe in the given folder
rem   register_file_associations.bat /u         - remove registration
rem ============================================================

set "BASE=%~dp0"
if not "%~1"=="" if not "%~1"=="/u" if not "%~1"=="-u" set "BASE=%~1"
if "%BASE:~-1%"=="\" set "BASE=%BASE:~0,-1%"

if /i "%~1"=="/u" goto :unregister
if /i "%~1"=="-u" goto :unregister

set "FOUND=0"

rem ---------- WorkRes (*.lbr) ----------
if exist "%BASE%\WorkRes.exe" (
    set "FOUND=1"
    echo Registering *.lbr -^> "%BASE%\WorkRes.exe"

    reg add "HKCU\Software\Classes\.lbr" /ve /d "RsWorkTools.LbrFile" /f >nul
    reg add "HKCU\Software\Classes\.lbr" /v "Content Type" /d "application/x-lbr" /f >nul
    reg add "HKCU\Software\Classes\RsWorkTools.LbrFile" /ve /d "LBR Resource Library" /f >nul
    if exist "%BASE%\lbr-file-icon.ico" (
        reg add "HKCU\Software\Classes\RsWorkTools.LbrFile\DefaultIcon" /ve /d "\"%BASE%\lbr-file-icon.ico\"" /f >nul
    )
    rem IMPORTANT: exe path quoted (in case of spaces), %%1 -> %1
    reg add "HKCU\Software\Classes\RsWorkTools.LbrFile\shell\open\command" /ve /d "\"%BASE%\WorkRes.exe\" \"%%1\"" /f >nul
) else (
    echo Skipping *.lbr: "%BASE%\WorkRes.exe" not found
)

rem ---------- MarkdownViewer (*.md, *.markdown) ----------
if exist "%BASE%\MarkdownViewer.exe" (
    set "FOUND=1"
    echo Registering *.md, *.markdown -^> "%BASE%\MarkdownViewer.exe"

    for %%E in (md markdown) do (
        reg add "HKCU\Software\Classes\.%%E" /ve /d "RsWorkTools.MarkdownViewer" /f >nul
        reg add "HKCU\Software\Classes\.%%E" /v "Content Type" /d "text/markdown" /f >nul
    )
    reg add "HKCU\Software\Classes\RsWorkTools.MarkdownViewer" /ve /d "Markdown Document" /f >nul
    if exist "%BASE%\markdown-file-icon.ico" (
        reg add "HKCU\Software\Classes\RsWorkTools.MarkdownViewer\DefaultIcon" /ve /d "\"%BASE%\markdown-file-icon.ico\"" /f >nul
    )
    reg add "HKCU\Software\Classes\RsWorkTools.MarkdownViewer\shell\open\command" /ve /d "\"%BASE%\MarkdownViewer.exe\" \"%%1\"" /f >nul
) else (
    echo Skipping *.md: "%BASE%\MarkdownViewer.exe" not found
)

if "!FOUND!"=="0" (
    echo.
    echo ERROR: no applications found in "%BASE%".
    echo Put the bat next to the exe files or pass the folder as an argument.
    exit /b 1
)

goto :refresh

:unregister
echo Removing file associations...
reg delete "HKCU\Software\Classes\.lbr" /f >nul 2>&1
reg delete "HKCU\Software\Classes\RsWorkTools.LbrFile" /f >nul 2>&1
reg delete "HKCU\Software\Classes\.md" /f >nul 2>&1
reg delete "HKCU\Software\Classes\.markdown" /f >nul 2>&1
reg delete "HKCU\Software\Classes\RsWorkTools.MarkdownViewer" /f >nul 2>&1

:refresh
rem Refresh Explorer association/icon cache
ie4uinit.exe -show >nul 2>&1
echo Done.
endlocal
