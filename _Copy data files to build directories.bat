REM Create directories

md Build
md Build\Debug
REM md Build\Debug\Resources
md Build
md Build\Release
REM md Build\Release\Resources

REM Copy data files

copy Data\*.* Build\Debug
copy Data\*.* Build\Release

REM Copy Resource files

REM copy /Y Resources\*.* Build\Debug\Resources
REM copy /Y Resources\*.* Build\Release\Resources

