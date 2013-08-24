!include LogicLib.nsh
!include WinVer.nsh
!include x64.nsh

!define PRODUCT_NAME "PerigeeCopy"
!define PRODUCT_VERSION "1.7"
!define PRODUCT_PUBLISHER "Jeremy Stanley"
!define PRODUCT_WEB_SITE "http://jstanley.pingerthinger.com/pscopy.html"
!define PRODUCT_ROOT_KEY "HKCU"
!define PRODUCT_KEY "Software\Perigee Software\PerigeeCopy"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor /SOLID lzma
RequestExecutionLevel Admin

; MUI 1.67 compatible ------
!include "MUI.nsh"
!include "Library.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "license.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\PerigeeCopy.html"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Var reginstalldir
Var myprogramfiles

; Set INSTDIR based on registry
Function .onInit
  ${If} ${AtLeastWin2000}
    ${If} ${RunningX64}
      SetRegView 64
      StrCpy $myprogramfiles $PROGRAMFILES64
    ${Else}
      SetRegView 32
      StrCpy $myprogramfiles $PROGRAMFILES
    ${EndIf} 
  ${Else}
    MessageBox MB_OK "This program requires Windows 2000 or newer."
    Abort
  ${EndIf}

  ReadRegStr $reginstalldir ${PRODUCT_UNINST_ROOT_KEY} ${PRODUCT_UNINST_KEY} "InstallLocation"
  ${If} $reginstalldir == ''
    StrCpy $INSTDIR "$myprogramfiles\PerigeeCopy"
  ${Else}
    StrCpy $INSTDIR $reginstalldir
  ${EndIf}
FunctionEnd

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "pscopy-${PRODUCT_VERSION}.exe"
ShowInstDetails show
ShowUnInstDetails show

Section "!Program Files" SEC01
  SetOutPath "$INSTDIR"
  !define LIBRARY_SHELL_EXTENSION
  !define LIBRARY_IGNORE_VERSION
  ${If} ${RunningX64}
  	File "pscopy\x64\Release\PerigeeCopy.exe"
    !define LIBRARY_X64	
    !InsertMacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "x64\Release\PerigeeCopy.dll" "$INSTDIR\PerigeeCopy.dll" "$TEMP"
    !undef LIBRARY_X64	
  ${Else}
    File "pscopy\Release\PerigeeCopy.exe"  
    !InsertMacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "Release\PerigeeCopy.dll" "$INSTDIR\PerigeeCopy.dll" "$TEMP"
  ${EndIf}
  File "doc\PerigeeCopy.html"
  File "doc\config.png"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateDirectory "$SMPROGRAMS\PerigeeCopy"
  CreateShortCut "$SMPROGRAMS\PerigeeCopy\Documentation.lnk" "$INSTDIR\PerigeeCopy.html"
  CreateShortCut "$SMPROGRAMS\PerigeeCopy\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\PerigeeCopy\Uninstall.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\PerigeeCopy\Configure PerigeeCopy.lnk" "$INSTDIR\PerigeeCopy.exe"
SectionEnd

Section "Source Code" SEC02
  SetOutPath "$INSTDIR\src"
  File "dlldata.c"
  File "installer.nsi"
  File "license.txt"
  File "popen32.cpp"
  File "popen32.h"
  File "pscopy.rc2"
  File "PSCopyShellExt.cpp"
  File "PSCopyShellExt.h"
  File "PSCopyShellExt.rgs"
  File "resource.h"
  File "shellext.cpp"
  File "shellext.def"
  File "shellext.vcproj"
  File "shellext.sln"
  File "shellext.h"
  File "shellext.idl"
  File "shellext.rc"
  File "shellext.tlb"
  File "shellextps.def"
  File "shellextps.mk"
  File "shellext_i.c"
  File "shellext_p.c"
  File "StdAfx.cpp"
  File "StdAfx.h"
  SetOutPath "$INSTDIR\src\doc"
  File "doc\config.png"
  File "doc\PerigeeCopy.html"
  SetOutPath "$INSTDIR\src\pscopy"
  File "pscopy\DialogDeleteProgress.cpp"
  File "pscopy\DialogDeleteProgress.h"
  File "pscopy\DialogFailedFiles.cpp"
  File "pscopy\DialogFailedFiles.h"
  File "pscopy\DialogOptions.cpp"
  File "pscopy\DialogOptions.h"
  File "pscopy\DialogOverwritePrompt.cpp"
  File "pscopy\DialogOverwritePrompt.h"
  File "pscopy\DialogProgress.cpp"
  File "pscopy\DialogProgress.h"
  File "pscopy\DlgConfirmDelete.cpp"
  File "pscopy\DlgConfirmDelete.h"
  File "pscopy\DlgError.cpp"
  File "pscopy\DlgError.h"
  File "pscopy\DlgMoveProgress.cpp"
  File "pscopy\DlgMoveProgress.h"
  File "pscopy\PerigeeCopy.cpp"
  File "pscopy\PerigeeCopy.h"
  File "pscopy\PerigeeCopyOptions.cpp"
  File "pscopy\PerigeeCopyOptions.h"
  File "pscopy\PerigeeDelete.cpp"
  File "pscopy\PerigeeDelete.h"
  File "pscopy\PerigeeJob.cpp"
  File "pscopy\PerigeeJob.h"
  File "pscopy\PerigeeMove.cpp"
  File "pscopy\PerigeeMove.h"
  File "pscopy\pscopy.cpp"
  File "pscopy\pscopy.vcproj"
  File "pscopy\pscopy.h"
  File "pscopy\pscopy.rc"
  File "pscopy\resource.h"
  File "pscopy\SizeHelper.cpp"
  File "pscopy\SizeHelper.h"
  File "pscopy\StdAfx.cpp"
  File "pscopy\StdAfx.h"
  File "pscopy\DlgElevate.cpp"
  File "pscopy\DlgElevate.h"
  File "pscopy\VistaHelper.cpp"
  File "pscopy\VistaHelper.h"
  SetOutPath "$INSTDIR\src\pscopy\res"
  File "pscopy\res\pscopy.ico"
  File "pscopy\res\pscopy.rc2"
  CreateShortCut "$SMPROGRAMS\PerigeeCopy\Source Code.lnk" "$INSTDIR\src\shellext.sln"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "InstallLocation" "$INSTDIR"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Program files and documentation"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Source code for PerigeeCopy (VC9 ATL+MFC project)"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function un.onInit
  ${If} ${RunningX64}
    SetRegView 64
  ${Else}
    SetRegView 32
  ${EndIf}
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  Delete "$INSTDIR\src\dlldata.c"
  Delete "$INSTDIR\src\installer.nsi"
  Delete "$INSTDIR\src\license.txt"
  Delete "$INSTDIR\src\popen32.cpp"
  Delete "$INSTDIR\src\popen32.h"
  Delete "$INSTDIR\src\pscopy.rc2"
  Delete "$INSTDIR\src\PSCopyShellExt.cpp"
  Delete "$INSTDIR\src\PSCopyShellExt.h"
  Delete "$INSTDIR\src\PSCopyShellExt.rgs"
  Delete "$INSTDIR\src\resource.h"
  Delete "$INSTDIR\src\shellext.cpp"
  Delete "$INSTDIR\src\shellext.def"
  Delete "$INSTDIR\src\shellext.vcproj"
  Delete "$INSTDIR\src\shellext.sln"
  Delete "$INSTDIR\src\shellext.h"
  Delete "$INSTDIR\src\shellext.idl"
  Delete "$INSTDIR\src\shellext.rc"
  Delete "$INSTDIR\src\shellext.tlb"
  Delete "$INSTDIR\src\shellextps.def"
  Delete "$INSTDIR\src\shellextps.mk"
  Delete "$INSTDIR\src\shellext_i.c"
  Delete "$INSTDIR\src\shellext_p.c"
  Delete "$INSTDIR\src\StdAfx.cpp"
  Delete "$INSTDIR\src\StdAfx.h"
  Delete "$INSTDIR\src\doc\config.png"
  Delete "$INSTDIR\src\doc\PerigeeCopy.html"
  Delete "$INSTDIR\src\doc\thumbs.db"
  Delete "$INSTDIR\src\pscopy\DialogDeleteProgress.cpp"
  Delete "$INSTDIR\src\pscopy\DialogDeleteProgress.h"
  Delete "$INSTDIR\src\pscopy\DialogFailedFiles.cpp"
  Delete "$INSTDIR\src\pscopy\DialogFailedFiles.h"
  Delete "$INSTDIR\src\pscopy\DialogOptions.cpp"
  Delete "$INSTDIR\src\pscopy\DialogOptions.h"
  Delete "$INSTDIR\src\pscopy\DialogOverwritePrompt.cpp"
  Delete "$INSTDIR\src\pscopy\DialogOverwritePrompt.h"
  Delete "$INSTDIR\src\pscopy\DialogProgress.cpp"
  Delete "$INSTDIR\src\pscopy\DialogProgress.h"
  Delete "$INSTDIR\src\pscopy\DlgConfirmDelete.cpp"
  Delete "$INSTDIR\src\pscopy\DlgConfirmDelete.h"
  Delete "$INSTDIR\src\pscopy\DlgError.cpp"
  Delete "$INSTDIR\src\pscopy\DlgError.h"
  Delete "$INSTDIR\src\pscopy\DlgMoveProgress.cpp"
  Delete "$INSTDIR\src\pscopy\DlgMoveProgress.h"
  Delete "$INSTDIR\src\pscopy\PerigeeCopy.cpp"
  Delete "$INSTDIR\src\pscopy\PerigeeCopy.exe.manifest"
  Delete "$INSTDIR\src\pscopy\PerigeeCopy.h"
  Delete "$INSTDIR\src\pscopy\PerigeeCopyOptions.cpp"
  Delete "$INSTDIR\src\pscopy\PerigeeCopyOptions.h"
  Delete "$INSTDIR\src\pscopy\PerigeeDelete.cpp"
  Delete "$INSTDIR\src\pscopy\PerigeeDelete.h"
  Delete "$INSTDIR\src\pscopy\PerigeeJob.cpp"
  Delete "$INSTDIR\src\pscopy\PerigeeJob.h"
  Delete "$INSTDIR\src\pscopy\PerigeeMove.cpp"
  Delete "$INSTDIR\src\pscopy\PerigeeMove.h"
  Delete "$INSTDIR\src\pscopy\pscopy.cpp"
  Delete "$INSTDIR\src\pscopy\pscopy.vcproj"
  Delete "$INSTDIR\src\pscopy\pscopy.h"
  Delete "$INSTDIR\src\pscopy\pscopy.rc"
  Delete "$INSTDIR\src\pscopy\resource.h"
  Delete "$INSTDIR\src\pscopy\SizeHelper.cpp"
  Delete "$INSTDIR\src\pscopy\SizeHelper.h"
  Delete "$INSTDIR\src\pscopy\StdAfx.cpp"
  Delete "$INSTDIR\src\pscopy\StdAfx.h"
  Delete "$INSTDIR\src\pscopy\DlgElevate.cpp"
  Delete "$INSTDIR\src\pscopy\DlgElevate.h"
  Delete "$INSTDIR\src\pscopy\VistaHelper.cpp"
  Delete "$INSTDIR\src\pscopy\VistaHelper.h"
  Delete "$INSTDIR\src\pscopy\res\pscopy.ico"
  Delete "$INSTDIR\src\pscopy\res\pscopy.rc2"
  Delete "$INSTDIR\src\pscopy\res\thumbs.db"

  Delete "$INSTDIR\config.png"
  Delete "$INSTDIR\PerigeeCopy.html"

  UnRegDll "$INSTDIR\PerigeeCopy.dll"
  Delete /REBOOTOK "$INSTDIR\PerigeeCopy.dll"
  Delete "$INSTDIR\PerigeeCopy.exe"
  Delete "$INSTDIR\thumbs.db"

  Delete "$SMPROGRAMS\PerigeeCopy\Uninstall.lnk"
  Delete "$SMPROGRAMS\PerigeeCopy\Website.lnk"
  Delete "$SMPROGRAMS\PerigeeCopy\Documentation.lnk"
  Delete "$SMPROGRAMS\PerigeeCopy\Source Code.lnk" 
  Delete "$SMPROGRAMS\PerigeeCopy\Configure PerigeeCopy.lnk"
  RMDir /REBOOTOK "$SMPROGRAMS\PerigeeCopy"

  RMDir "$INSTDIR\src\pscopy\res"
  RMDir "$INSTDIR\src\pscopy"
  RMDir "$INSTDIR\src\doc"
  RMDir "$INSTDIR\src"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey ${PRODUCT_ROOT_KEY} "${PRODUCT_KEY}"
  SetAutoClose false
SectionEnd
