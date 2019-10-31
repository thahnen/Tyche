;   -- QCTyche-CVUI.iss --
;   Script for creating an installer for the QCTyche software!
;   The software uses OpenCV aswell as the picoflexx cameras' royale library.
;   The program runs on 64-Bit Windows 7+

;   TODO: work on this script, it does not fully work and there are no icons etc.


#define NAME            "QCTyche"
#define VERSION         "1.0.0"
#define PUBLISHER       "Tobias Hahnen, iPattern at University of Applied Sciences Niederrhein"
#define URL             "https://github.com/thahnen/Tyche"
#define OUTPUT_DIR      "C:\Users\Tobias Hahnen\GitHub\Tyche\install\Installer"
#define OUTPUT_SUFFIX   "_setup_x64"
#define MIN_NT_VERSION  "6.0"


[Setup]
AppId={{3379666D-69CC-4DB0-B1FD-01C082B05B80}
AppName={#NAME}
AppVersion={#VERSION}
AppVerName={#NAME} {#VERSION}
AppPublisher={#PUBLISHER}
AppPublisherURL={#URL}
AppSupportURL={#URL}
AppUpdatesURL={#URL}
DefaultDirName={pf}\{#NAME}
DefaultGroupName={#NAME}
AllowNoIcons=yes
OutputDir={#OUTPUT_DIR}
OutputBaseFilename={#NAME}{#VERSION}{#OUTPUT_SUFFIX}
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
Compression=lzma2
SolidCompression=yes
MinVersion={#MIN_NT_VERSION}
WizardStyle=modern
WizardImageStretch=False
WizardImageBackColor=clWhite
;WizardImageFile=QCTyche_setup.bmp
;WizardSmallImageFile=QCTyche_setup_icon.bmp


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"


[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1


[Files]
Source: "C:\Users\Tobias Hahnen\GitHub\Tyche\QCTyche-VS\x64\Release\QCTyche-VS.exe"; DestDir: "{app}"; DestName: "QCTyche.exe"; Flags: ignoreversion


; TODO: not every menue item seems to work using Windows 10 (one even vanished when clicking it!)
[Icons]
Name: "{group}\QCTyche"; Filename: "{app}\QCTyche-VS.exe"
Name: "{group}\{cm:ProgramOnTheWeb,QCTyche}"; Filename: "https://github.com/thahnen/Tyche"
Name: "{group}\{cm:UninstallProgram,QCTyche}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\QCTyche"; Filename: "{app}\QCTyche.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\QCTyche"; Filename: "{app}\QCTyche-VS.exe"; Tasks: quicklaunchicon


[Run]
Filename: "{app}\QCTyche.exe"; Description: {cm:LaunchProgram,QCTyche}; Flags: nowait postinstall skipifsilent