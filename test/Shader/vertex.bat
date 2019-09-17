shaderc -f %1 -o dx11/%~n1.bin --type vertex --varyingdef varying.def.sc --platform windows -p vs_5_0 -O 3

shaderc -f %1 -o glsl/%~n1.bin --type vertex --varyingdef varying.def.sc --platform osx 

shaderc -f %1 -o essl/%~n1.bin --type vertex --varyingdef varying.def.sc --platform android

shaderc -f %1 -o metal/%~n1.bin --type vertex --varyingdef varying.def.sc --platform osx -p metal

shaderc -f %1 -o spirv/%~n1.bin --type vertex --varyingdef varying.def.sc --platform linux -p spirv
@pause


