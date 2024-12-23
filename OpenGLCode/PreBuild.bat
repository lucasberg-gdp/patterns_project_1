@echo off
echo "Starting PreBuild.bat" > "C:\Users\Lucas\OneDrive\Desktop\test\log.txt"
echo "SOLUTION_DIR: %1" >> "C:\Users\Lucas\OneDrive\Desktop\test\log.txt"
echo "CONFIGURATION: %2" >> "C:\Users\Lucas\OneDrive\Desktop\test\log.txt"

set SOLUTION_DIR=%1
set CONFIGURATION=%2
set OUT_DIR=%3

echo "Copying .dll files to: %OUT_DIR%" >> "C:\Users\Lucas\OneDrive\Desktop\test\log.txt"

xcopy /y /s "%SOLUTION_DIR%dev\fmod\lib\*.dll" "%OUT_DIR%"
xcopy /y /s "%SOLUTION_DIR%dev\assimp\lib\*.dll" "%OUT_DIR%"