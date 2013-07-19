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

#include "wxNote_Gui/wxNote_Window/MainWindowShadow.h"
#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Gui/wxNote_Window/MainWindowNormal.h"
#include "wxNote_Gui/wxNote_Edit/NoteEditor.h"
#include "wxNote_Gui/wxNote_Other/NoteEditorSplitter.h"
#include "wxNote_Gui/wxNote_Button/wxNote_ToolButton.h"

#include "wxNote_Global.h"

#include <QApplication>
#include <QToolButton>
#include <QTextBrowser>
#include <QToolBar>
#include <QClipboard>

//.._MainWindowShadow类实现

    /* 构造函数实现 */
    _MainWindowShadow::_MainWindowShadow(_TextEditorWindow *_EditorWindow,
                                         QWidget *_Parent)
        : _AbstractMainWindow(_Parent)
        {
        _CreateMainWinToolBar();

        m_MainSplitter = new _NoteEditorSplitter(Qt::Vertical);
        m_CurrentShadowEditorWindow = _EditorWindow;
        m_MainSplitter->addWidget(m_CurrentShadowEditorWindow);

        setCentralWidget(m_MainSplitter);
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
        }

    /* _AddShadowEditorWindow()函数实现 */
    void _MainWindowShadow
        ::_AddShadowEditorWindow(_TextEditorWindow *_ShadowWin)
        {
        m_MainSplitter->addWidget(_ShadowWin);
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateMainWinToolBar()函数重写 */
    void _MainWindowShadow::_CreateMainWinToolBar()
        {
        m_MainWinToolBar->addWidget(m_EliminateToolButton);
        m_MainWinToolBar->addWidget(m_RestoreNoteToolButton);
        }

    /* _MoveNote2Trash_inNoteBookList()槽重写 */
    void _MainWindowShadow::_MoveNote2TrashSlot()
        {
        // TODO
        }

    /* _ThoroughDeleteCurrentNoteSlot()槽重写 */
    void _MainWindowShadow::_ThoroughDeleteCurrentNoteSlot()
        {
        // TODO
        }

    /* _RestoreNoteSlot()槽重写 */
    void _MainWindowShadow::_RestoreNoteSlot()
        {
        // TODO
        }

    /* _OptionsSlot()槽重写 */
    void _MainWindowShadow::_OptionsSlot()
        {
        emit _ShowOptions_InShadowWindowSignal();
        }

    /* _UndoSlot()槽重写 */
    void _MainWindowShadow::_UndoSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->undo();
            }
        }

    /* _RedoSlot()槽重写 */
    void _MainWindowShadow::_RedoSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->redo();
            }
        }

    /* _CutSlot()槽重写 */
    void _MainWindowShadow::_CutSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->cut();
            }
        }

    /* _CopySlot()槽重写 */
    void _MainWindowShadow::_CopySlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->copy();
            }
        }

    /* _PasteSlot()槽实现 */
    void _MainWindowShadow::_PasteSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->paste();
            }
        }

    /* _PastePlainTextSlot()槽重写 */
    void _MainWindowShadow::_PastePlainTextSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->textCursor()
                    .insertText(qApp->clipboard()->text());
            }
        }

    /* _DeleteSlot()槽实现 */
    void _MainWindowShadow::_DeleteSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->textCursor().deleteChar();
            }
        }

    /* _SelectAllSlot()槽实现 */
    void _MainWindowShadow::_SelectAllSlot()
        {
        if (m_CurrentShadowEditorWindow)
            {
            _NoteEditor* _CurrentEditor
                    = m_CurrentShadowEditorWindow->_GetTextEditor();

            _CurrentEditor->selectAll();
            }
        }

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
