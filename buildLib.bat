@echo off

set THRD_DIR="Thread"
set SVC_DIR="Services"
set MECH_DIR="Mechanism"
set BIN_DIR="Bin"
set ICLD_DIR="Include"
set LIB_DIR="Lib"
set CCFLAGS="-g"
echo -----------Building in progress-----------

::building phase
echo Building WinThread
g++ -c %THRD_DIR%/WinThread.cxx -I %ICLD_DIR% -o %BIN_DIR%/WinThread.o %CCFLAGS%

echo Building InputService
g++ -c %SVC_DIR%/InputService.cxx -I %ICLD_DIR% -o %BIN_DIR%/InputService.o %CCFLAGS%

echo Building DisplayService
g++ -c %SVC_DIR%/DisplayService.cxx -I %ICLD_DIR% -o %BIN_DIR%/DisplayService.o %CCFLAGS%
g++ -c %SVC_DIR%/DisplayComponents/GraphicObject.cxx -I %ICLD_DIR% -o %BIN_DIR%/GraphicObject.o %CCFLAGS%
g++ -c %SVC_DIR%/DisplayComponents/Bitmap.cxx -I %ICLD_DIR% -o %BIN_DIR%/Bitmap.o %CCFLAGS%
g++ -c %SVC_DIR%/DisplayComponents/Textbox.cxx -I %ICLD_DIR% -o %BIN_DIR%/Textbox.o %CCFLAGS%

echo Building TweenService
g++ -c %SVC_DIR%/TweenService.cxx -I %ICLD_DIR% -o %BIN_DIR%/TweenService.o %CCFLAGS%

echo Buiding Event
g++ -c %SVC_DIR%/Event.cxx -I %ICLD_DIR% -o %BIN_DIR%/Event.o %CCFLAGS%

::linking phase
echo Linking bin files
g++ %BIN_DIR%/WinThread.o %BIN_DIR%/InputService.o %BIN_DIR%/DisplayService.o %BIN_DIR%/GraphicObject.o %BIN_DIR%/Bitmap.o %BIN_DIR%/Textbox.o %BIN_DIR%/TweenService.o %BIN_DIR%/Event.o -shared -o %LIB_DIR%/gmsvc.dll -L %LIB_DIR% -lgdi32