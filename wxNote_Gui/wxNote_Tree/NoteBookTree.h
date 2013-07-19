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
#include <QTreeWidget>

class QTreeWidgetItem;
class QAction;

//..该类定义了一个在_MainWindowNormal中显示的笔记列表
class _NoteBookTree : public QTreeWidget
    {
    Q_OBJECT

public:
    _NoteBookTree(QWidget* _Parent = nullptr);

    QTreeWidgetItem* _GetNoteBookRoot() const   { return m_NoteBookRootItem;   }
    QTreeWidgetItem* _GetLabelBookRoot() const  { return m_LabelRootItem;      }
    QTreeWidgetItem* _GetPropertiesRoot() const { return m_PropertiesRootItem; }
    QTreeWidgetItem* _GetTrashRootRoot() const  { return m_TrashRootItem;      }

    QTreeWidgetItem* _GetAllNoteBook() const { return m_AllNoteBookSubItem; }

    void _SetCurrentItemIsNoteBookRoot()
            { setCurrentItem(m_NoteBookRootItem, 0); }

    void _SetCurrentItemIsAllNoteSubRoot()
            { setCurrentItem(m_AllNoteBookSubItem, 0); }

    void _SetCurrentItemIsLabelRoot()
            { setCurrentItem(m_LabelRootItem, 0); }

    void _SetCurrentItemIsPropertiesRoot()
            { setCurrentItem(m_PropertiesRootItem, 0); }

    void _SetCurrentItemIsTrash()
            { setCurrentItem(m_TrashRootItem, 0); }

public slots:
    void _NoteBookCountIncreaseSlot();
    void _NoteBookCountDecreaseSlot();
    void _TrashCountIncreaseSlot();
    void _TrashCountDecreaseSlot();

protected:
    enum { m_ColumnCnt = 1 };
    static int s_AllNotesCnt;
    static int s_TrashCnt;

    void _CreateActionsOnContextMenu();
    void _CreateContextMenu();

    void _RestoreUsersNoteBook();

    /* 上下文菜单中的QAction */
    QAction* m_NewNoteBookAction;
    QAction* m_RenameAction;
    QAction* m_DeleteNoteBookAction;

    /* "笔记"根 */
    QTreeWidgetItem* m_NoteBookRootItem;
    QTreeWidgetItem* m_AllNoteBookSubItem;

    /* "标签"根 */
    QTreeWidgetItem* m_LabelRootItem;

    /* "属性"根 */
    QTreeWidgetItem* m_PropertiesRootItem;

    /* "废纸篓"根 */
    QTreeWidgetItem* m_TrashRootItem;
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
