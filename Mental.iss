[Setup]
AppName=Mental
AppVersion=1.0
DefaultDirName={pf}\Mental
DefaultGroupName=Mental
OutputDir=.
OutputBaseFilename=MentalSetup
Compression=lzma
SolidCompression=yes

[Files]
Source: "out\\build\\x64-Debug\\bin\\Mental.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\glew32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\lua54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\msvcp140d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\vcruntime140d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\vcruntime140_1d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\ucrtbased.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "out\\build\\x64-Debug\\bin\\imgui.ini"; DestDir: "{app}"; Flags: ignoreversion

Source: "out\\build\\x64-Debug\\bin\\Fonts\\*"; DestDir: "{app}\\Fonts"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "out\\build\\x64-Debug\\bin\\Scripts\\*"; DestDir: "{app}\\Scripts"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "out\\build\\x64-Debug\\bin\\Shaders\\*"; DestDir: "{app}\\Shaders"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "out\\build\\x64-Debug\\bin\\Textures\\*"; DestDir: "{app}\\Textures"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\\Mental"; Filename: "{app}\\Mental.exe"
Name: "{userdesktop}\\Mental"; Filename: "{app}\\Mental.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Создать ярлык на рабочем столе"; GroupDescription: "Дополнительные задачи:"