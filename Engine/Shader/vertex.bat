md "%~dp0"dx11
md "%~dp0"glsl
md "%~dp0"essl
md "%~dp0"medal
md "%~dp0"spirv
shaderc -f %1 -o "%~dp0dx11/%~n1.bin" --type vertex --varyingdef varying.def.sc --platform windows -p vs_5_0 -O 3
shaderc -f %1 -o "%~dp0glsl/%~n1.bin" --type vertex --varyingdef varying.def.sc --platform osx 
shaderc -f %1 -o "%~dp0essl/%~n1.bin" --type vertex --varyingdef varying.def.sc --platform android
shaderc -f %1 -o "%~dp0metal/%~n1.bin" --type vertex --varyingdef varying.def.sc --platform osx -p metal
shaderc -f %1 -o "%~dp0spirv/%~n1.bin" --type vertex --varyingdef varying.def.sc --platform linux -p spirv
@pause


