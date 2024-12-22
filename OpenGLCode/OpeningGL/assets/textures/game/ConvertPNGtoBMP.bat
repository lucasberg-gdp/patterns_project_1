@echo off
setlocal enabledelayedexpansion

set "input_directory=C:\Users\Lucas\OneDrive\Desktop\GameJam\OpeningGL\OpenGLCode\OpeningGL\assets\textures\game\Tiles"

set "output_directory=C:\Users\Lucas\OneDrive\Desktop\GameJam\OpeningGL\OpenGLCode\OpeningGL\assets\textures\game\Tiles"

if not exist "%output_directory%" mkdir "%output_directory%"

rem Loop through all PNG files in the input directory
for %%F in ("%input_directory%\*.png") do (
    rem Get the filename and extension
    set "filename=%%~nF"
    set "extension=%%~xF"

    rem Convert PNG to BMP using ImageMagick
    magick "%%F" "%output_directory%\!filename!.bmp"

    rem Output conversion status
    echo Converted "%%F" to BMP format
)

echo All PNG files converted to BMP format.
pause