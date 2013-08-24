; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDialogOptions
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "shellext.h"
LastPage=0

ClassCount=9
Class1=CDialogDeleteProgress
Class2=CDialogFailedFiles
Class3=CDialogOptions
Class4=CDialogOverwritePrompt
Class5=CDialogProgress
Class6=DlgConfirmDelete
Class7=DlgError
Class8=DlgMoveProgress
Class9=CShellextApp

ResourceCount=8
Resource1=IDD_DIALOG_PROGRESS
Resource2=IDD_OVERWRITE_PROMPT
Resource3=IDD_DIALOG_DELETE_PROGRESS
Resource4=IDD_ERROR
Resource5=IDD_CONFIRM_DELETE
Resource6=IDD_OPTIONS_DIALOG
Resource7=IDD_FAILED_FILES
Resource8=IDD_DIALOG_MOVE_PROGRESS

[CLS:CDialogDeleteProgress]
Type=0
BaseClass=CDialog
HeaderFile=DialogDeleteProgress.h
ImplementationFile=DialogDeleteProgress.cpp

[CLS:CDialogFailedFiles]
Type=0
BaseClass=CDialog
HeaderFile=DialogFailedFiles.h
ImplementationFile=DialogFailedFiles.cpp

[CLS:CDialogOptions]
Type=0
BaseClass=CDialog
HeaderFile=DialogOptions.h
ImplementationFile=DialogOptions.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK_SKIP_ERROR_FILES

[CLS:CDialogOverwritePrompt]
Type=0
BaseClass=CDialog
HeaderFile=DialogOverwritePrompt.h
ImplementationFile=DialogOverwritePrompt.cpp

[CLS:CDialogProgress]
Type=0
BaseClass=CDialog
HeaderFile=DialogProgress.h
ImplementationFile=DialogProgress.cpp

[CLS:DlgConfirmDelete]
Type=0
BaseClass=CDialog
HeaderFile=DlgConfirmDelete.h
ImplementationFile=DlgConfirmDelete.cpp

[CLS:DlgError]
Type=0
BaseClass=CDialog
HeaderFile=DlgError.h
ImplementationFile=DlgError.cpp

[CLS:DlgMoveProgress]
Type=0
BaseClass=CDialog
HeaderFile=DlgMoveProgress.h
ImplementationFile=DlgMoveProgress.cpp

[CLS:CShellextApp]
Type=0
BaseClass=CWinApp
HeaderFile=shellext.cpp
ImplementationFile=shellext.cpp

[DLG:IDD_DIALOG_DELETE_PROGRESS]
Type=1
Class=CDialogDeleteProgress
ControlCount=12
Control1=IDCANCEL,button,1342242816
Control2=IDC_LBL_DELETING,static,1342308864
Control3=IDC_PROGRESS,msctls_progress32,1350565889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_LBL_FILE_DEL,static,1342308866
Control9=IDC_LBL_FILE_TOTAL,static,1342308866
Control10=IDC_LBL_FOLDER_DEL,static,1342308866
Control11=IDC_LBL_FOLDER_TOTAL,static,1342308866
Control12=IDC_STATIC,button,1342177287

[DLG:IDD_FAILED_FILES]
Type=1
Class=CDialogFailedFiles
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDOK,button,1073807361
Control3=IDC_EDIT_ERRORS,edit,1352730628

[DLG:IDD_OPTIONS_DIALOG]
Type=1
Class=CDialogOptions
ControlCount=20
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FRAME_OVERWRITE,button,1342177287
Control3=IDC_RADIO_OVERWRITE_ALWAYS,button,1342373897
Control4=IDC_RADIO_OVERWRITE_NEVER,button,1342242825
Control5=IDC_RADIO_OVERWRITE_NEWER,button,1342242825
Control6=IDC_RADIO_OVERWRITE_PROMP,button,1342242825
Control7=IDC_FRAME_ARCHIVE,button,1342177287
Control8=IDC_CHECK_ARCHIVE_ONLY,button,1342373891
Control9=IDC_CHECK_CLEAR_ARCHIVE_BIT,button,1342242819
Control10=IDC_FRAME_OPTIONS,button,1342177287
Control11=IDC_CHECK_OPT_OVERWRITE_READ_ONLY,button,1342373891
Control12=IDC_CHECK_CLEAR_READ_ONLY_BIT,button,1342242819
Control13=IDC_PROMPT_DEBUG,button,1342373891
Control14=IDC_PROMPT_CONFIRM_DELETE,button,1342373891
Control15=IDC_PROMPT_RECYCLE,button,1342373891
Control16=IDC_CHECK_SKIP_ERROR_FILES,button,1342242819
Control17=IDC_CHECK_USE_PERIGEECOPY_BY_DEFAULT,button,1342252035
Control18=IDC_CHECK_SKIP_INVENTORY,button,1342242819
Control19=IDOK,button,1342242817
Control20=IDCANCEL,button,1342242816

[DLG:IDD_OVERWRITE_PROMPT]
Type=1
Class=CDialogOverwritePrompt
ControlCount=13
Control1=IDC_BUTTON_YES,button,1342242816
Control2=IDC_BUTTON_NO,button,1342242816
Control3=IDC_NEWER,button,1342242816
Control4=IDC_CHECK_APPLY_TO_ALL,button,1342242819
Control5=IDC_TITLE_LINE,static,1342308364
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_SIZE1,static,1342308352
Control9=IDC_DATE1,static,1342308352
Control10=IDC_SIZE2,static,1342308352
Control11=IDC_DATE2,static,1342308352
Control12=IDC_ICON1,static,1342177283
Control13=IDC_ICON2,static,1342177283

[DLG:IDD_DIALOG_PROGRESS]
Type=1
Class=CDialogProgress
ControlCount=22
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_LBL_FILENUM,static,1342308354
Control12=IDC_LBL_SOURCE,static,1342308352
Control13=IDC_LBL_DEST,static,1342308352
Control14=IDC_LBL_FILECNT,static,1342308354
Control15=IDC_LBL_KBCOPIED_FILE,static,1342308354
Control16=IDC_LBL_KBCOPIED_JOB,static,1342308354
Control17=IDC_LBL_KBTOTAL_FILE,static,1342308354
Control18=IDC_LBL_KBTOTAL_JOB,static,1342308354
Control19=IDC_LBL_KB_PER_SEC_FILE,static,1342308354
Control20=IDC_LBL_KB_PER_SEC_JOB,static,1342308354
Control21=IDC_PROGRESS_FILE,msctls_progress32,1350565889
Control22=IDC_PROGRESS_JOB,msctls_progress32,1484783617

[DLG:IDD_CONFIRM_DELETE]
Type=1
Class=DlgConfirmDelete
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PROMPT,static,1342308352
Control4=IDC_FILE_LIST,SysListView32,1350647829

[DLG:IDD_ERROR]
Type=1
Class=DlgError
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_ICON1,static,1342177283
Control3=IDC_PROMPT,static,1342308352
Control4=IDC_TRYAGAIN,button,1342242816
Control5=IDC_SKIPFILE,button,1342242817

[DLG:IDD_DIALOG_MOVE_PROGRESS]
Type=1
Class=DlgMoveProgress
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LBL_SOURCE,static,1342308352
Control5=IDC_LBL_DEST,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_PROGRESS,msctls_progress32,1350565888

