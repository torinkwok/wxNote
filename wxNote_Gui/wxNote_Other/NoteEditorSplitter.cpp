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

#include "wxNote_Gui/wxNote_Other/NoteEditorSplitter.h"

#include "wxNote_Global.h"

//.._NoteEditorSplitter类实现

    /* 构造函数实现 */
    _NoteEditorSplitter::_NoteEditorSplitter(Qt::Orientation _Orientation,
                                             QWidget *_Parent)
        : QSplitter(_Orientation, _Parent),
          m_ParentNoteName_Current(wxNote::g_NonTitleNoteName)
        {;}

    /* _SetParentNoteName()函数实现 */
    void _NoteEditorSplitter::_SetParentNoteName(const QString &_NoteName)
        {
        m_ParentNoteName_Current = _NoteName;
        }

    /* _GetParentNoteName()函数实现 */
    QString _NoteEditorSplitter::_GetParentNoteName() const
        {
        return m_ParentNoteName_Current;
        }

    /* _SetBeforeDeletedParentNoteName()函数实现 */
    void _NoteEditorSplitter
        ::_SetBeforeDeletedParentNoteName(const QString &_NoteName)
        {
        m_ParentNoteName_BeforeDeleted = _NoteName;
        }

    /* _GetBeforeDeletedParentNoteName()函数实现 */
    QString _NoteEditorSplitter::_GetBeforeDeletedParentNoteName() const
        {
        return m_ParentNoteName_BeforeDeleted;
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

