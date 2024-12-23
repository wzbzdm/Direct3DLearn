; 程序安装脚本

[Setup]
; 安装程序基本设置
AppName="3DShape"
AppVersion=1.0.0
DefaultDirName="{commonpf}\3DShape"
DefaultGroupName="3DShape"
OutputDir=.\Output
OutputBaseFilename=3DShapeInstaller
PrivilegesRequired=none


[Languages]
Name:"chinese"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Icons]
; 设置快捷方式
Name: "{group}\3DShape"; Filename:"{app}\3DShape.exe" ;WorkingDir:"{app}"

[Files]
; 指定需要安装的文件
Source: "..\x64\Release\DirectXD.exe"; DestDir: "{app}"
Source: "..\*.ini"; DestDir: "{app}"
Source: "..\CSO\*.cso"; DestDir: "{app}\CSO"
Source: "..\DLL\*.dll"; DestDir: "{app}\DLL"
Source: "..\TextureImg\*.png"; DestDir: "{app}\TextureImg"; Flags: ignoreversion
Source: "..\source\*.ttf"; DestDir: "{app}\source"; Flags: ignoreversion recursesubdirs

[Registry]
Root: HKCU; Subkey: "Software\3DShape"; Flags: uninsdeletekey

[Run]
; 设置安装后执行的程序
Filename: "{app}\DirectXD.exe"; Description: "启动应用程序"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; 设置卸载时删除的文件
Type: files; Name: "{app}\TextureImg\*"
Type: files; Name: "{app}\source\*"
Type: files; Name: "{app}\DLL\*"
Type: files; Name: "{app}\CSO\*"
Type: files; Name: "{app}\*"
Type: dirifempty; Name: "{app}\TextureImg"
Type: dirifempty; Name: "{app}\source"
Type: dirifempty; Name: "{app}\DLL"
Type: dirifempty; Name: "{app}\CSO"