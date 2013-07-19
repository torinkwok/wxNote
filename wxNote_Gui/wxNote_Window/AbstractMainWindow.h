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
#include <QMainWindow>

class _AboutwxNote;
class _wxNote_ToolButton;

class QMenuBar;
class QMenu;
class QAction;
class QToolBar;
class QToolButton;

/* wxNote的主界面基类
 * 其派生类为: _MainWindowNormal和_MainWindowShadow
 * 前者为正常情况下普通的主界面, 后者为当用户双击笔记列表的相应项时显示的主界面
 */
class _AbstractMainWindow : public QMainWindow
    {
    Q_OBJECT

public:
    _AbstractMainWindow(QWidget* _Parent = nullptr);

signals:
    void _MoveNote2TrashSignal();
    void _ThoroughDeleteNoteSignal();

public slots:
    virtual void _UndoSlot() = 0;
    virtual void _RedoSlot() = 0;
    virtual void _CutSlot() = 0;
    virtual void _CopySlot() = 0;
    virtual void _PasteSlot() = 0;
    virtual void _PastePlainTextSlot() = 0;
    virtual void _DeleteSlot() = 0;
    virtual void _SelectAllSlot() = 0;

protected slots:
    virtual void _MoveNote2TrashSlot() = 0;
    virtual void _ThoroughDeleteCurrentNoteSlot() = 0;
    virtual void _RestoreNoteSlot() = 0;
    virtual void _ShareByEmailSlot();
    virtual void _SynchronousSlot();
    virtual void _AboutwxNoteSlot();
    virtual void _OptionsSlot() = 0;

protected:
    virtual void _CreateActionsAndSubMenu();
    virtual void _CreateMainWinMenus();
    virtual void _CreateMainWinToolBar();
    virtual void _CreateToolButtonOnToolBar();

    /* "文件"菜单项中的QMenu或QAction */

    /* "编辑"菜单项中的QMenu或QAction */
    QAction* m_UndoAction;
    QAction* m_RedoAction;
    QAction* m_CutAction;
    QAction* m_CopyAction;
    QAction* m_PasteAction;
    QAction* m_PastePlainTextAction;
    QAction* m_DeleteAction;
    QAction* m_SelectAllAction;

    /* "笔记"菜单项中的QMenu或QAction  */
    QMenu*   m_ShareSubMenu;
    QAction* m_ShareByEmailAction;

    QAction* m_RestoreNoteAction;
    QAction* m_EliminateAction;

    QMenu*   m_DeleteNoteSubMenu;
    QAction* m_Move2TrashAction;
    QAction* m_ThoroughDeleteAction;

    /* "工具"菜单项中的QMenu或QAction */
    QAction* m_SynchronousAction;
    QAction* m_OptionsAction;

    /* "帮助"菜单项中的QMenu或QAction */
    QAction* m_AboutwxNoteAction;

    QMenu* m_FileMenu;
    QMenu* m_EditMenu;
    QMenu* m_ViewMenu;
    QMenu* m_NoteMenu;
    QMenu* m_ToolMenu;
    QMenu* m_HelpMenu;

    /* 工具栏中的按钮 */
    _wxNote_ToolButton* m_SynchronousToolButton;
    _wxNote_ToolButton* m_ShareToolButton;

    _wxNote_ToolButton* m_RestoreNoteToolButton;
    _wxNote_ToolButton* m_EliminateToolButton;

    QToolBar* m_MainWinToolBar;
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
