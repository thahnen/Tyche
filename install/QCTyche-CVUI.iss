;   -- QCTyche-CVUI.iss --
;   Script for creating an installer for the QCTyche software!
;   The software uses OpenCV aswell as the picoflexx cameras' royale library.
;   The program runs on 64-Bit Windows 7+

;   TODO: work on this script, it does not fully work and there are no icons etc.

[Setup]
AppId={{3379666D-69CC-4DB0-B1FD-01C082B05B80}
AppName=QCTyche
AppVersion=1.0
AppVerName=QCTyche version 1.0
AppPublisher=Tobias Hahnen, iPattern at University of Applied Sciences Niederrhein
AppPublisherURL=https://github.com/thahnen/Tyche
AppSupportURL=https://github.com/thahnen/Tyche
AppUpdatesURL=https://github.com/thahnen/Tyche
WizardStyle=modern
DefaultDirName={pf}\QCTyche
DefaultGroupName=QCTyche
AllowNoIcons=yes
OutputDir=C:\Users\Tobias Hahnen\GitHub\Tyche\install\Installer
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
Compression=lzma2
SolidCompression=yes

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\Tobias Hahnen\GitHub\Tyche\QCTyche-VS\x64\Release\QCTyche-VS.exe"; DestDir: "{app}"; DestName: "QCTyche.exe"; Flags: ignoreversion

[Icons]
Name: "{group}\QCTyche"; Filename: "{app}\QCTyche-VS.exe"
Name: "{group}\{cm:ProgramOnTheWeb,QCTyche}"; Filename: "https://github.com/thahnen/Tyche"
Name: "{group}\{cm:UninstallProgram,QCTyche}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\QCTyche"; Filename: "{app}\QCTyche.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\QCTyche"; Filename: "{app}\QCTyche-VS.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\QCTyche.exe"; Description: {cm:LaunchProgram,QCTyche}; Flags: nowait postinstall skipifsilent