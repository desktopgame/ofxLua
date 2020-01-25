curl -O https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
nuget install lua -OutputDirectory ../libs/lua/packages -Version 5.3.5.1
del nuget.exe