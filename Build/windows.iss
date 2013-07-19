[Setup]
; Here are all the things that contain the version number
AppVersion=1.0
OutputBaseFilename=setup_Curmudgeon-1.0
OutputDir=C:\Steampunk\Curmudgeon\release\1.0

AppId={{B010F5C8-D9CC-4F2C-A778-3D619A4BC070}
AppName=Curmudgeon
AppPublisher=Qweex
AppPublisherURL=http://www.qweex.com/
AppSupportURL=http://www.qweex.com/
AppUpdatesURL=http://www.qweex.com/
DefaultDirName={pf}\Curmudgeon
DefaultGroupName=Curmudgeon
LicenseFile=C:\Steampunk\Curmudgeon\LICENSE.QPL
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Steampunk\build-Curmudgeon\release\Curmudgeon.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.4\bin\QtCore4.dll"; DestDir: "{app}"
Source: "C:\Qt\4.8.4\bin\QtGui4.dll"; DestDir: "{app}"
Source: "C:\Qt\4.8.4\bin\QtNetwork4.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\Curmudgeon"; Filename: "{app}\Curmudgeon.exe"
Name: "{commondesktop}\Curmudgeon"; Filename: "{app}\Curmudgeon.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Curmudgeon.exe"; Description: "{cm:LaunchProgram,Curmudgeon}"; Flags: nowait postinstall skipifsilent

