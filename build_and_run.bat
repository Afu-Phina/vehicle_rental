@echo off
echo Compiling...
g++ car_rental.cpp -o car_rental.exe
if exist car_rental.exe (
    echo ✅ Compilation successful!
    echo Running...
    car_rental.exe
) else (
    echo ❌ Compilation failed. Make sure g++ is installed and added to PATH.
)
pause
