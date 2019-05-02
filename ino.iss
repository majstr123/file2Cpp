; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "file2Cpp"
#define MyAppVersion "1.1"
#define MyAppExeName "file2Cpp.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{601A9B25-0499-4227-81C7-14F1AA1ACE58}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir=C:\Users\Jure\Documents\02_Qt\file2Cpp\New folder
OutputBaseFilename=file2cpp
SetupIconFile=C:\Users\Jure\Documents\02_Qt\file2Cpp\icon.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\Jure\Documents\02_Qt\build-file2Cpp-Desktop_Qt_5_10_1_MinGW_32bit-Release\release\file2Cpp.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Jure\Documents\02_Qt\build-file2Cpp-Desktop_Qt_5_10_1_MinGW_32bit-Release\release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

