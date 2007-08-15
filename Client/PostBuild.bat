
echo copy mspdb80.dll to editbin dir
copy "C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\mspdb80.dll" "C:\Program Files\Microsoft Visual Studio 8\VC\bin"

echo Add 3GB Flag to EXE Header
start "Adding 3GB Flag to EXE Header" /D"C:\Program Files\Microsoft Visual Studio 8\VC\bin\" /WAIT editbin.exe "P:\Fresco\Client\Release\Client.exe" /LARGEADDRESSAWARE

echo delete mspdb80.dll from editbin dir
del  "C:\Program Files\Microsoft Visual Studio 8\VC\bin\mspdb80.dll"

