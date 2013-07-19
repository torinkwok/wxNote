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

#include "wxNote_Gui/wxNote_Window/AbstractMainWindow.h"
#include "wxNote_Gui/wxNote_Dialog/AboutDialog.h"
#include "wxNote_Gui/wxNote_Button/wxNote_ToolButton.h"

#include "wxNote_Global.h"

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QAction>

//.._MainWindow类实现

    /* 构造函数实现 */
    _AbstractMainWindow::_AbstractMainWindow(QWidget *_Parent)
        : QMainWindow(_Parent)
        {
        _CreateActionsAndSubMenu();
        _CreateMainWinMenus();
        _CreateMainWinToolBar();

        setAttribute(Qt::WA_DeleteOnClose);
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateActions()函数实现 */
    void _AbstractMainWindow::_CreateActionsAndSubMenu()
        {
        /* "编辑"菜单项中的QMenu或QAction的创建和设置 */
        m_UndoAction = new QAction(tr("撤销(&U)"), this);
        m_UndoAction->setShortcut(QKeySequence::Undo);
        m_UndoAction->setToolTip(tr("撤销编辑"));
        connect(m_UndoAction, SIGNAL(triggered()), this, SLOT(_UndoSlot()));

        m_RedoAction = new QAction(tr("重做(&R)"), this);
        m_RedoAction->setShortcut(QKeySequence::Redo);
        m_RedoAction->setToolTip(tr("重做编辑"));
        connect(m_RedoAction, SIGNAL(triggered()), this, SLOT(_RedoSlot()));

        m_CutAction = new QAction(tr("剪切(&T)"), this);
        m_CutAction->setShortcut(QKeySequence::Cut);
        m_CutAction->setToolTip(tr("剪切选中文本"));
        connect(m_CutAction, SIGNAL(triggered()), this, SLOT(_CutSlot()));

        m_CopyAction = new QAction(tr("复制(&C)"), this);
        m_CopyAction->setShortcut(QKeySequence::Copy);
        m_CopyAction->setToolTip(tr("复制选中文本"));
        connect(m_CopyAction, SIGNAL(triggered()), this, SLOT(_CopySlot()));

        m_PasteAction = new QAction(tr("粘贴(&P)"), this);
        m_PasteAction->setShortcut(QKeySequence::Paste);
        m_PasteAction->setToolTip(tr("粘贴文本"));
        connect(m_PasteAction, SIGNAL(triggered()), this, SLOT(_PasteSlot()));

        m_PastePlainTextAction = new QAction(tr("粘贴为文本(&V)"), this);
        m_PastePlainTextAction->setShortcut(tr("Ctrl+Shift+V"));
        m_PastePlainTextAction->setToolTip(tr("粘贴为非富文本"));
        connect(m_PastePlainTextAction, SIGNAL(triggered()),
                this, SLOT(_PastePlainTextSlot()));

        m_DeleteAction = new QAction(tr("删除(&D)"), this);
        m_DeleteAction->setShortcut(QKeySequence::Delete);
        m_DeleteAction->setToolTip(tr("删除选中文本"));
        connect(m_DeleteAction, SIGNAL(triggered()), this, SLOT(_DeleteSlot()));

        m_SelectAllAction = new QAction(tr("选择全部(&S)"), this);
        m_SelectAllAction->setShortcut(QKeySequence::SelectAll);
        m_SelectAllAction->setToolTip(tr("选中所有"));
        connect(m_SelectAllAction, SIGNAL(triggered()), this, SLOT(_SelectAllSlot()));

        /* "笔记"菜单项中的QMenu或QAction的创建和设置 */
        m_ShareSubMenu = new QMenu(tr("分享"));
        m_ShareByEmailAction = new QAction(tr("通过电子邮件发送...(&E)"), this);
        m_ShareSubMenu->addAction(m_ShareByEmailAction);
        connect(m_ShareByEmailAction, SIGNAL(triggered()),
                this, SLOT(_ShareByEmailSlot()));

        m_DeleteNoteSubMenu = new QMenu(tr("删除笔记(&O)"));
        m_DeleteNoteSubMenu->setEnabled(false);
        m_Move2TrashAction = new QAction(tr("移动笔记到废纸篓"), this);
        m_Move2TrashAction->setToolTip(tr("移动笔记到废纸篓"));
        connect(m_Move2TrashAction, SIGNAL(triggered()),
                this, SLOT(_MoveNote2TrashSlot()));

        m_ThoroughDeleteAction = new QAction(tr("彻底删除笔记"), this);
        m_ThoroughDeleteAction->setShortcut(tr("Ctrl+Delete"));
        m_ThoroughDeleteAction->setToolTip(tr("彻底删除笔记"));
        connect(m_ThoroughDeleteAction, SIGNAL(triggered()),
                this, SLOT(_ThoroughDeleteCurrentNoteSlot()));

        m_DeleteNoteSubMenu->addAction(m_Move2TrashAction);
        m_DeleteNoteSubMenu->addAction(m_ThoroughDeleteAction);

        m_RestoreNoteAction = new QAction(tr("还原笔记(&R)"), this);
        m_RestoreNoteAction->setEnabled(false);
        connect(m_RestoreNoteAction, SIGNAL(triggered()),
                this, SLOT(_RestoreNoteSlot()));

        m_EliminateAction = new QAction(tr("消除笔记(&E)"), this);
        m_EliminateAction->setEnabled(false);
        connect(m_EliminateAction, SIGNAL(triggered()),
                this, SLOT(_ThoroughDeleteCurrentNoteSlot()));

        /* "工具"菜单项中的QMenu或QAction的创建和设置 */
        m_SynchronousAction = new QAction(tr("同步(&S)"), this);
        m_SynchronousAction->setShortcut(tr("F9"));
        m_SynchronousAction->setToolTip(tr("同步"));
        connect(m_SynchronousAction, SIGNAL(triggered()),
                this, SLOT(_SynchronousSlot()));

        m_OptionsAction = new QAction(tr("选项(&O)"), this);
        m_OptionsAction->setToolTip(tr("选项"));
        connect(m_OptionsAction, SIGNAL(triggered()),
                this, SLOT(_OptionsSlot()));

        /* "帮助"菜单项中的QMenu或QAction的创建和设置 */
        m_AboutwxNoteAction = new QAction(tr("关于(&A)"), this);
        m_AboutwxNoteAction->setShortcut(tr("Alt+F1"));
        m_AboutwxNoteAction->setToolTip(tr("关于wxNote"));
        connect(m_AboutwxNoteAction, SIGNAL(triggered()),
                this, SLOT(_AboutwxNoteSlot()));
        }

    /* _CreateMainWinMenus()函数实现 */
    void _AbstractMainWindow::_CreateMainWinMenus()
        {
        /* "文件"菜单项的创建与设置 */
        m_FileMenu = menuBar()->addMenu(tr("文件(&F)"));

        /* "编辑"菜单项的创建与设置 */
        m_EditMenu = menuBar()->addMenu(tr("编辑(&E)"));
        m_EditMenu->addAction(m_UndoAction);
        m_EditMenu->addAction(m_RedoAction);
        m_EditMenu->addSeparator();
        m_EditMenu->addAction(m_CutAction);
        m_EditMenu->addAction(m_CopyAction);
        m_EditMenu->addAction(m_PasteAction);
        m_EditMenu->addAction(m_PastePlainTextAction);
        m_EditMenu->addSeparator();
        m_EditMenu->addAction(m_DeleteAction);
        m_EditMenu->addAction(m_SelectAllAction);

        /* "查看"菜单项的创建与设置 */
        m_ViewMenu = menuBar()->addMenu(tr("查看(&V)"));

        /* "笔记"菜单项的创建与设置 */
        m_NoteMenu = menuBar()->addMenu(tr("笔记(&N)"));
        m_NoteMenu->addMenu(m_ShareSubMenu);
        m_NoteMenu->addSeparator();
        m_NoteMenu->addAction(m_EliminateAction);
        m_NoteMenu->addAction(m_RestoreNoteAction);
        m_NoteMenu->addMenu(m_DeleteNoteSubMenu);

        /* "工具"菜单项的创建与设置 */
        m_ToolMenu = menuBar()->addMenu(tr("工具(&T)"));
        m_ToolMenu->addAction(m_SynchronousAction);
        m_ToolMenu->addSeparator();
        m_ToolMenu->addAction(m_OptionsAction);

        /* "帮助"菜单项的创建与设置 */
        m_HelpMenu = menuBar()->addMenu(tr("帮助(&H)"));
        m_HelpMenu->addAction(m_AboutwxNoteAction);
        }

    /* _CreateMainWinToolBar()函数实现 */
    void _AbstractMainWindow::_CreateMainWinToolBar()
        {
        _CreateToolButtonOnToolBar();
        m_MainWinToolBar = addToolBar(tr("功能"));
        m_MainWinToolBar->setIconSize(QSize(25, 30));
        m_MainWinToolBar->setMovable(false);

        m_MainWinToolBar->addWidget(m_SynchronousToolButton);
        m_MainWinToolBar->addWidget(m_ShareToolButton);
        m_MainWinToolBar->addSeparator();
        }

    /* _CreateToolButtonOnToolBar()函数实现 */
    void _AbstractMainWindow::_CreateToolButtonOnToolBar()
        {
        /* "同步"按钮 */
        m_SynchronousToolButton = new _wxNote_ToolButton;
        m_SynchronousToolButton->setText(tr("同步"));
        m_SynchronousToolButton->setToolTip(tr("同步笔记"));
        m_SynchronousToolButton->setIcon(
                    QIcon(":/wxNote_Icons/synchronous.png"));
        connect(m_SynchronousToolButton, SIGNAL(clicked()),
                this, SLOT(_SynchronousSlot()));

        /* "分享"按钮 */
        m_ShareToolButton = new _wxNote_ToolButton;
        m_ShareToolButton->setMenu(m_ShareSubMenu);
        m_ShareToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        m_ShareToolButton->setText(tr("分享"));
        m_ShareToolButton->setToolTip(tr("分享笔记给你的朋友"));
        m_ShareToolButton->setIcon(
                    QIcon(":/wxNote_Icons/share.png"));
        connect(m_ShareToolButton, SIGNAL(clicked()),
                this, SLOT(_ShareByEmailSlot()));

        /* "还原笔记"按钮 */
        m_RestoreNoteToolButton = new _wxNote_ToolButton;
        m_RestoreNoteToolButton->setEnabled(false);
        m_RestoreNoteToolButton->setText(tr("还原笔记"));
        m_RestoreNoteToolButton->setToolTip(tr("还原被移动到废纸篓中的笔记"));
        m_RestoreNoteToolButton->setIcon(
                    QIcon(":/wxNote_Icons/restore.png"));
        connect(m_RestoreNoteToolButton, SIGNAL(clicked()),
                this, SLOT(_RestoreNoteSlot()));

        /* "消除笔记"按钮 */
        m_EliminateToolButton = new _wxNote_ToolButton;
        m_EliminateToolButton->setEnabled(false);
        m_EliminateToolButton->setText(tr("消除笔记"));
        m_EliminateToolButton->setToolTip(tr("彻底消除被移到废纸篓中的笔记"));
        m_EliminateToolButton->setIcon(
                    QIcon(":/wxNote_Icons/eliminate.png"));
        connect(m_EliminateToolButton, SIGNAL(clicked()),
                this, SLOT(_ThoroughDeleteCurrentNoteSlot()));
        }

    //..protected slots部分

    /* _ShareByEmailSlot()槽实现 */
    void _AbstractMainWindow::_ShareByEmailSlot()
        {
        // TODO
        }

    /* _SynchronousSlot()槽实现 */
    void _AbstractMainWindow::_SynchronousSlot()
        {
        m_SynchronousToolButton->setIcon(
                    QIcon(":/wxNote_Icons/synchronousFailed.png"));
        m_SynchronousToolButton->setToolTip(tr("同步笔记（暂不可用）"));

        _AboutwxNote _2User;
        _2User._GetTabWidget()->setCurrentIndex(3);
        _2User.exec();
        }

    /* _AboutwxNote()槽实现 */
    void _AbstractMainWindow::_AboutwxNoteSlot()
        {
        _AboutwxNote _AboutDialog(this);
        _AboutDialog.exec();
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
