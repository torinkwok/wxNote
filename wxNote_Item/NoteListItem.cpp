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

#include "wxNote_Item/NoteListItem.h"

//.._NoteListItem类实现

    /* 构造函数实现 */
    _NoteListItem::_NoteListItem(const QIcon &_IconName,
                                 const QString &_ItemName)
        : QListWidgetItem(_IconName, _ItemName),
          m_ParentNoteBookName_Current(""),
          enum_NoteRating(wxNote::_NonRating),
          enum_NoteCategories(wxNote::_NonCategories)
        {
        cout << enum_NoteCategories << endl;
        cout << enum_NoteRating << endl << endl;

        setFont(QFont("微软雅黑,12,-1,5,50,0,0,0,0,0"));
        setSizeHint(QSize(200, 80));
        setTextColor(QColor(50, 51, 46));
        }

    _NoteListItem::_NoteListItem(const QString &_ItemName)
        : QListWidgetItem(_ItemName)
        {
        setFont(QFont("微软雅黑,12,-1,5,50,0,0,0,0,0"));
        setSizeHint(QSize(200, 80));
        setTextColor(QColor(50, 51, 46));
        }

    /* _GetTitle()槽实现 */
    QString _NoteListItem::_GetTitle()
        {
        return _GetNoteNameSlot();
        }

    /* _SetTitle()槽实现 */
    void _NoteListItem::_SetTitle(const QString &_NewTitle)
        {
        _SetNoteNameSlot(_NewTitle);
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

