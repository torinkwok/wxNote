///:
/*****************************************************************************
 **                                                                         **
 **                               .======.                                  **
 **                               | INRI |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                      .========'      '========.                         **
 **                      |   _      xxxx      _   |                         **
 **                      |  /_;-.__ / _\  _.-;_\  |                         **
 **                      |     `-._`'`_/'`.-'     |                         **
 **                      '========.`\   /`========'                         **
 **                               | |  / |                                  **
 **                               |/-.(  |                                  **
 **                               |\_._\ |                                  **
 **                               | \ \`;|                                  **
 **                               |  > |/|                                  **
 **                               | / // |                                  **
 **                               | |//  |                                  **
 **                               | \(\  |                                  **
 **                               |  ``  |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                   \\    _  _\\| \//  |//_   _ \// _                     **
 **                  ^ `^`^ ^`` `^ ^` ``^^`  `^^` `^ `^                     **
 **                                                                         **
 **                    Copyright © 1997-2013 by Tong G.                     **
 **                          ALL RIGHTS RESERVED.                           **
 **                                                                         **
 ****************************************************************************/

#pragma once
#include "wxNote_Gui/wxNote_Window/AbstractMainWindow.h"

class _TextEditorWindow;
class _NoteEditorSplitter;

class _MainWindowShadow : public _AbstractMainWindow
    {
    Q_OBJECT

public:
    _MainWindowShadow(_TextEditorWindow* _EditorWindow,
                             QWidget* _Parent = nullptr);

    void _AddShadowEditorWindow(_TextEditorWindow* _ShadowWin);

    _TextEditorWindow* _GetShadowEditorWindow() const
                            { return m_CurrentShadowEditorWindow; }

signals:
    void _ShowOptions_InShadowWindowSignal();

protected slots:
    void _MoveNote2TrashSlot();
    void _ThoroughDeleteCurrentNoteSlot();
    void _RestoreNoteSlot();

    void _OptionsSlot();

    void _UndoSlot();
    void _RedoSlot();
    void _CutSlot();
    void _CopySlot();
    void _PasteSlot();
    void _PastePlainTextSlot();
    void _DeleteSlot();
    void _SelectAllSlot();

protected:
    void _CreateMainWinToolBar();

    _NoteEditorSplitter* m_MainSplitter;
    _TextEditorWindow*   m_CurrentShadowEditorWindow;
    };

 ////////////////////////////////////////////////////////////////////////////

 /***************************************************************************
 **                                                                        **
 **      _________                                      _______            **
 **     |___   ___|                                   / ______ \           **
 **         | |     _______   _______   _______      | /      |_|          **
 **         | |    ||     || ||     || ||     ||     | |    _ __           **
 **         | |    ||     || ||     || ||     ||     | |   |__  \          **
 **         | |    ||     || ||     || ||     ||     | \_ _ __| |  _       **
 **         |_|    ||_____|| ||     || ||_____||      \________/  |_|      **
 **                                           ||                           **
 **                                    ||_____||                           **
 **                                                                        **
 ***************************************************************************/
///:~

