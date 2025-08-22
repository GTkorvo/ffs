echo ffs.dll
C:\Windows\System32\where.exe ffs.dll
echo
echo api-ms-win-crt-heap-l1-1-0.dll
C:\Windows\System32\where api-ms-win-crt-heap-l1-1-0.dll
echo
echo VCRUNTIME140.dll
C:\Windows\System32\where VCRUNTIME140.dll
echo
echo KERNEL32.dll
C:\Windows\System32\where KERNEL32.dll
echo
echo dir
echo
dir /s /b /o:gn D:\a\ffs\ffs\
echo %PATH%
set PATH=%PATH%;D:\a\ffs\ffs\atl\build\bin\Release\;D:\a\ffs\ffs\dill\build\bin\Release\;
D:\a\ffs\ffs\build\bin\Release\align_test.exe
