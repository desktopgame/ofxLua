curl -O https://dist.nuget.org/win-x86-commandline/latest/nuget.exe
nuget install lua -OutputDirectory ../libs/lua/packages
del nuget.exe