@echo off
setlocal

set "sourceDir=..\Tool\Bin\Resources\Models"
set "destDir=..\Client\Bin\Resources\Models"

echo Source Directory: %sourceDir%
echo Destination Directory: %destDir%

echo Copying .bin files from "%sourceDir%" to "%destDir%"

pause
for /d %%i in ("%sourceDir%\*") do (
    if exist "%%i\*.bin" (
        echo Found .bin files in "%%i", copying to "%destDir%\%%~nxi"
        xcopy "%%i\*.bin" "%destDir%\%%~nxi\" /y
    ) else (
        echo No .bin files found in "%%i"
    )
)

endlocal