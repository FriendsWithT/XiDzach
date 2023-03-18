@echo off

set ENT_DIR="EntryPoint"
set MECH_DIR="Mechanism"
set BIN_DIR="Bin"
set ICLD_DIR="Include"
set APP_DIR="App"
set LIB_DIR="Lib"
set CCFLAGS="-g"
echo -----------Building in progress-----------

::building phase
echo Building GameFlow
g++ -c %MECH_DIR%/GameFlow.cxx -I %ICLD_DIR% -o %BIN_DIR%/GameFlow.o %CCFLAGS%

echo Building main.cxx
g++ -c %ENT_DIR%/main.cxx -I %ICLD_DIR% -o %BIN_DIR%/main.o %CCFLAGS%

::linking phase
echo Linking bin files
g++ %BIN_DIR%/GameFlow.o %BIN_DIR%/main.o -o %APP_DIR%/game.exe -L %LIB_DIR% -lgdi32 -lgmsvc

echo -----------Building finished-----------