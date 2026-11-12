@echo off
REM see http://www.imagemagick.org/Usage/windows/#convert_issue
REM %~d1
REM CD "%~f1"
MD .\resized
REM FOR %%a in (*.jpg) DO "C:\Program Files\ImageMagick-6.7.5-Q16\convert" %%a -resize 1920x1920 -crop 1920x1080+0+420 .\HDSDI\%%a
@echo on
FOR %%a in (*.tif) DO "convert" "%%a" -resize 2570x2088 -background black -gravity center -extent 2570x2088 ".\resized\%%a.jpg"
@echo off
PAUSE
