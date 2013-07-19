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

class _CategoriesTree : public QTreeWidget
    {
    Q_OBJECT

public:
    _CategoriesTree(QWidget* _Parent = nullptr);

    QTreeWidgetItem* _GetColorLabelRootItem() const
                        { return m_ColorLabelRootItem; }
    QTreeWidgetItem* _GetImportantItem() const
                        { return m_ImportantItem; }
    QTreeWidgetItem* _GetWorkItem() const
                        { return m_WorkItem; }
    QTreeWidgetItem* _GetPersonalItem() const
                        { return m_PersonalItem; }
    QTreeWidgetItem* _GetToDoItem() const
                        { return m_ToDoItem; }
    QTreeWidgetItem* _GetLaterItem() const
                        { return m_LaterItem; }

protected:
    /* "评分" */
    QTreeWidgetItem* m_RatingRootItem;

    QTreeWidgetItem* m_ExcellentItem;
    QTreeWidgetItem* m_GoodItem;
    QTreeWidgetItem* m_AverageItem;
    QTreeWidgetItem* m_FairItem;
    QTreeWidgetItem* m_PoorItem;

    /* "颜色标签" */
    QTreeWidgetItem* m_ColorLabelRootItem;

    QTreeWidgetItem* m_ImportantItem;
    QTreeWidgetItem* m_WorkItem;
    QTreeWidgetItem* m_PersonalItem;
    QTreeWidgetItem* m_ToDoItem;
    QTreeWidgetItem* m_LaterItem;
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

