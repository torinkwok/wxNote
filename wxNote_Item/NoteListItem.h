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
#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Global.h"

#include <QListWidgetItem>

class _TextEditorWindow;

class _NoteListItem : public QListWidgetItem
    {
public:
    _NoteListItem(const QIcon& _IconName, const QString& _ItemName);
    _NoteListItem(const QString& _ItemName = "");

    /* 设置存储该笔记项的笔记本的名称 */
    void _SetParentNoteBookName(const QString& _InName)
                { m_ParentNoteBookName_Current = _InName; }

    void _SetNoteRating(const wxNote::_NoteRating& _Rating)
                { enum_NoteRating = _Rating; }
    wxNote::_NoteRating _GetNoteRating() const
                { return enum_NoteRating; }

    void _SetNoteCategories(const wxNote::_NoteCategories& _Categories)
                { enum_NoteCategories = _Categories; }
    wxNote::_NoteCategories _GetNoteCategories() const
                { return enum_NoteCategories; }

    /* 获取存储该笔记项的笔记本的名称 */
    QString _GetParentNoteBookName() const
                { return m_ParentNoteBookName_Current; }

    /* 设置该笔记项被删除前所属的父笔记本的名称 */
    void _SetBeforeDeletedParentNoteBookName(const QString& _InName)
                { m_ParentNoteBookName_BeforeDeleted = _InName; }
    /* 获取该笔记项被删除前所属的父笔记本的名称 */
    QString _GetBeforeDeletedParentNoteBookName() const
                { return m_ParentNoteBookName_BeforeDeleted; }

    /* 将当前笔记项与其对应的编辑窗口进行绑定 */
    void _SetBindTextEW(_TextEditorWindow* _BindEW)
                { m_BindTextEditorWindow = _BindEW; }
    /* 获取与其绑定的编辑窗口 */
    _TextEditorWindow* _GetBindTextEW() const
                { return m_BindTextEditorWindow; }

public slots:
    QString _GetNoteNameSlot() const
                { return m_NoteName; }
    void _SetNoteNameSlot(const QString& _NoteName)
                { m_NoteName = _NoteName; }

    QString _GetTitle();
    void _SetTitle(const QString& _NewTitle);

protected:
    QString m_ParentNoteBookName_Current;       // 用于存储该笔记项当前所属的笔记本的名称
    QString m_ParentNoteBookName_BeforeDeleted; // 用于存储该笔记被删除前所属的笔记本的名称

    QString m_NoteName;

    wxNote::_NoteRating     enum_NoteRating;
    wxNote::_NoteCategories enum_NoteCategories;

    /* 与当前笔记项绑定的编辑窗口 */
    _TextEditorWindow* m_BindTextEditorWindow;
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

