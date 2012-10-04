; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{F4866E24-2529-4516-8E45-474F72B4BCB7}
AppName=Le Terrier d'AbulEdu - Fubuki
AppVersion=1.2.0
AppPublisher=RyXeo SARL
AppPublisherURL=http://www.abuledu.org/leterrier/leterrier-fubuki
AppSupportURL=http://www.abuledu.org/leterrier/leterrier-fubuki
AppUpdatesURL=http://www.abuledu.org/leterrier/leterrier-fubuki
DefaultDirName={pf}\AbulEdu\Fubuki
DefaultGroupName=Le Terrier d'AbulEdu
LicenseFile=..\gpl-2.0.txt
OutputDir=.
OutputBaseFilename=fubuki-1.2.0-setup
SetupIconFile=leterrier-fubuki.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french";  MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\release\leterrier-fubuki.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\data\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/mingw/bin/libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/mingw/bin/mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/mingw/bin/libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtScript4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtGUI4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtSQL4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtSVG4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin/QtWebKit4.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins\imageformats/qgif4.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins\imageformats/qico4.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins\imageformats/qjpeg4.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins\imageformats/qmng4.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins\imageformats/qsvg4.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:/QtSDK/Desktop/Qt/4.8.1/mingw/plugins\imageformats/qtiff4.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Fubuki"; Filename: "{app}\leterrier-fubuki.exe"; WorkingDir: "{app}"
Name: "{commondesktop}\Fubuki"; Filename: "{app}\leterrier-fubuki.exe"; Tasks: desktopicon; WorkingDir: "{app}"

[Run]
Filename: "{app}\leterrier-fubuki.exe"; Description: "{cm:LaunchProgram,Fubuki}"; Flags: nowait postinstall skipifsilent

