; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{A8D3AD0D-D36E-4970-BE77-76A840EA2831}
AppName=HeeksCAD
AppVerName=HeeksCAD 0.10.1
AppPublisher=Heeks Software
AppPublisherURL=http://code.google.com/p/heekscad/
AppSupportURL=http://code.google.com/p/heekscad/
AppUpdatesURL=http://code.google.com/p/heekscad/
DefaultDirName={pf}\HeeksCAD
DefaultGroupName=HeeksCAD
OutputBaseFilename=HeeksCAD 0.10.1
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\Dan\HeeksCAD\HeeksCAD.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\bitmaps\*.png"; DestDir: "{app}\bitmaps"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\bitmaps\font.glf"; DestDir: "{app}\bitmaps"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\icons\*.png"; DestDir: "{app}\icons"; Flags: ignoreversion
Source: "C:\Users\Dan\OCC dlls for HeeksCAD\*"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\de\*"; DestDir: "{app}\de"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\it\*"; DestDir: "{app}\it"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\wxmsw28u_gl_vc_custom.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\wxmsw28u_core_vc_custom.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\wxmsw28u_aui_vc_custom.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Dan\HeeksCAD\wxbase28u_vc_custom.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "C:\Program Files\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\*"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\HeeksCAD"; Filename: "{app}\HeeksCAD.exe"
Name: "{commondesktop}\HeeksCAD"; Filename: "{app}\HeeksCAD.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\HeeksCAD.exe"; Description: "{cm:LaunchProgram,HeeksCAD}"; Flags: nowait postinstall skipifsilent

