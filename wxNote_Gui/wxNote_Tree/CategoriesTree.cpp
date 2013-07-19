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

#include "wxNote_Gui/wxNote_Tree/CategoriesTree.h"

#include "wxNote_Global.h"

#include <QTreeWidgetItem>

//.._CategoriesTree类实现

    /* 构造函数实现 */
    _CategoriesTree::_CategoriesTree(QWidget *_Parent)
        : QTreeWidget(_Parent)
        {
        /* "评分" */
        m_RatingRootItem = new QTreeWidgetItem(this, QStringList() << wxNote::g_RatingListName);
        m_RatingRootItem->setIcon(0, QIcon(":/wxNote_Icons/rating.png"));
        m_RatingRootItem->setExpanded(true);

        m_ExcellentItem = new QTreeWidgetItem(
                    m_RatingRootItem, QStringList() << wxNote::g_ExcellentName);
        m_ExcellentItem->setIcon(0, QIcon(":/wxNote_Icons/excellent.png"));

        m_GoodItem = new QTreeWidgetItem(
                    m_RatingRootItem, QStringList() << wxNote::g_GoodName);
        m_GoodItem->setIcon(0, QIcon(":/wxNote_Icons/good.png"));

        m_AverageItem = new QTreeWidgetItem(
                    m_RatingRootItem, QStringList() << wxNote::g_AverageName);
        m_AverageItem->setIcon(0, QIcon(":/wxNote_Icons/average.png"));

        m_FairItem = new QTreeWidgetItem(
                    m_RatingRootItem, QStringList() << wxNote::g_FairName);
        m_FairItem->setIcon(0, QIcon(":/wxNote_Icons/fair.png"));

        m_PoorItem = new QTreeWidgetItem(
                    m_RatingRootItem, QStringList() << ("差"));
        m_PoorItem->setIcon(0, QIcon(":/wxNote_Icons/poor.png"));

        /* "颜色标签" */
        m_ColorLabelRootItem = new QTreeWidgetItem(
                                this, QStringList() << wxNote::g_ColorLabelListName);
        m_ColorLabelRootItem->setIcon(0, QIcon(":/wxNote_Icons/colorLabel.png"));
        m_ColorLabelRootItem->setExpanded(true);

        m_ImportantItem = new QTreeWidgetItem(m_ColorLabelRootItem,
                                              QStringList() << wxNote::g_ImportantName);
        m_ImportantItem->setIcon(0, QIcon(":/wxNote_Icons/important.png"));

        m_WorkItem = new QTreeWidgetItem(m_ColorLabelRootItem,
                                         QStringList() << wxNote::g_WorkName);
        m_WorkItem->setIcon(0, QIcon(":/wxNote_Icons/work.png"));

        m_PersonalItem = new QTreeWidgetItem(m_ColorLabelRootItem,
                                             QStringList() << wxNote::g_PersonalName);
        m_PersonalItem->setIcon(0, QIcon(":/wxNote_Icons/personal.png"));

        m_ToDoItem = new QTreeWidgetItem(m_ColorLabelRootItem,
                                         QStringList() << wxNote::g_ToDoName);
        m_ToDoItem->setIcon(0, QIcon(":/wxNote_Icons/toDo.png"));

        m_LaterItem = new QTreeWidgetItem(m_ColorLabelRootItem,
                                          QStringList() << wxNote::g_LaterName);
        m_LaterItem->setIcon(0, QIcon(":/wxNote_Icons/later.png"));

        setCurrentItem(m_RatingRootItem);
        setHeaderHidden(true);
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

