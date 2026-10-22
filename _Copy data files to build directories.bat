REM #echo off

REM Create directories

md Build
md Build\Debug
md Build\Debug\Data
REM md Build\Debug\Resources
md Build
md Build\Release
md Build\Release\Data
REM md Build\Release\Resources

REM Copy data files

xcopy /E /Y "Data\x_significant_history_events.csv" "Build\Debug\Data"
xcopy /E /Y "Data\x_significant_history_events.csv" "Build\Release\Data"

REM Copy Resource files

REM copy /Y Resources\*.* Build\Debug\Resources
REM copy /Y Resources\*.* Build\Release\Resources

