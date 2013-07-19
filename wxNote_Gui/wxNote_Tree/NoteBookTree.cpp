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

#include "wxNote_Gui/wxNote_Tree/NoteBookTree.h"

#include "wxNote_Global.h"

#include <QTreeWidgetItem>
#include <QStringList>
#include <QAction>

//.._NoteBookTree类实现

    int _NoteBookTree::s_AllNotesCnt = 0;
    int _NoteBookTree::s_TrashCnt = 0;

    /* 构造函数实现 */
    _NoteBookTree::_NoteBookTree(QWidget *_Parent)
        : QTreeWidget(_Parent)
        {
        setColumnCount(m_ColumnCnt);

        /* "笔记"根 */
        m_NoteBookRootItem = new QTreeWidgetItem(this, QStringList() << wxNote::g_NoteBooksName);
        m_NoteBookRootItem->setIcon(0, QIcon(":/wxNote_Icons/noteBook.png"));

        m_AllNoteBookSubItem = new QTreeWidgetItem(m_NoteBookRootItem,
                                               QStringList() << wxNote::g_AllNotesName);
        m_AllNoteBookSubItem->setIcon(0, QIcon(":/wxNote_Icons/allNotes.png"));

        /* "标签"根 */
        m_LabelRootItem = new QTreeWidgetItem(this, QStringList() << wxNote::g_LabelName);
        m_LabelRootItem->setIcon(0, QIcon(":/wxNote_Icons/label.png"));

        /* "属性"根 */
        m_PropertiesRootItem = new QTreeWidgetItem(this, QStringList() << wxNote::g_PropertiesName);
        m_PropertiesRootItem->setIcon(0, QIcon(":/wxNote_Icons/property.png"));

        /* "废纸篓"根 */
        m_TrashRootItem = new QTreeWidgetItem(this, QStringList() << wxNote::g_TrashName);
        m_TrashRootItem->setIcon(0, QIcon(":/wxNote_Icons/trash.png"));

        // _CreateContextMenu()存在BUG!

        setCurrentItem(m_AllNoteBookSubItem);
        setIconSize(QSize(30,30));
        setHeaderLabels(QStringList() << wxNote::g_NoteBooksName);
        setHeaderHidden(true);
        }

    /* _NoteBookCountIncreaseSlot()槽实现 */
    void _NoteBookTree::_NoteBookCountIncreaseSlot()
        {
        m_AllNoteBookSubItem->setText(0, wxNote::g_AllNotesName);
        }

    /* _NoteBookCountDecreaseSlot()槽实现 */
    void _NoteBookTree::_NoteBookCountDecreaseSlot()
        {
        m_AllNoteBookSubItem->setText(0, wxNote::g_AllNotesName);
        }

    /* _TrashCountIncreaseSlot()槽实现 */
    void _NoteBookTree::_TrashCountIncreaseSlot()
        {
        m_TrashRootItem->setText(0, wxNote::g_TrashName);
        }

    /* _TrashCountDecreaseSlot()槽实现 */
    void _NoteBookTree::_TrashCountDecreaseSlot()
        {
        m_TrashRootItem->setText(0, wxNote::g_TrashName);
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateActionOnContextMenu()函数实现 */
    void _NoteBookTree::_CreateActionsOnContextMenu()
        {
        /* "新建笔记本"动作 */
        m_NewNoteBookAction = new QAction(tr("新建笔记本(&C)"), this);
        m_NewNoteBookAction->setShortcut(tr("Insert"));
        m_NewNoteBookAction->setToolTip(tr("新建笔记本"));

        /* "重命名 笔记本"动作 */
        m_RenameAction = new QAction(tr("重命名(&R)"), this);
        m_RenameAction->setShortcut(tr("F2"));
        m_RenameAction->setToolTip(tr("重命名笔记本"));

        /* "删除笔记本" 动作 */
        m_DeleteNoteBookAction = new QAction(tr("删除(&D)..."), this);
        m_DeleteNoteBookAction->setShortcut(QKeySequence::Delete);
        m_DeleteNoteBookAction->setToolTip(tr("删除笔记本"));
        }

    /* _CreateContextMenu()函数实现 */
    void _NoteBookTree::_CreateContextMenu()
        {
        addAction(m_NewNoteBookAction);
        addAction(m_RenameAction);
        addAction(m_DeleteNoteBookAction);
        setContextMenuPolicy(Qt::ActionsContextMenu);
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
