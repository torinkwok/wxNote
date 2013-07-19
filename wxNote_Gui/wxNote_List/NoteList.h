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
#include <QListWidget>

#include "wxNote_Item/NoteListItem.h"

class _NoteListItem;
class _TextEditorWindow;
class _MainWindowShadow;

class QTabWidget;

//..该类定义了一个在_MainWindowNormal中显示的笔记列表
class _NoteList : public QListWidget
    {
    Q_OBJECT

public:
    _NoteList(QWidget* _Parent = nullptr);

    void _AddItem(const QPair<_NoteListItem*, QString> _ItemGroup);

    int _HiddenCount();

    QModelIndex _GetIndexFromItem(QListWidgetItem* _Item) const;

    _NoteListItem* _GetCurrentItem() const;
    _NoteListItem* _Item(int _Index) const;

    QString _GetSpecifiedItemTitle(int _Index);
    QString _GetCurrentItemTitle();

    QList<_NoteListItem *>
        _GetNotesInSpecifiedNoteBook(const QString& _CurrentLabelName,
                                     const QTabWidget* wxNote_TabWidget);
    QList<_NoteListItem *>
        _GetNotesInSpecifiedCategories(const wxNote::_NoteCategories);
    QList<_NoteListItem *>
        _GetNotesInSpecifiedRating(const wxNote::_NoteRating);

    QList<_NoteListItem *> _GetAllNotes() const;
    QList<_NoteListItem *> _GetAllNotes_ExceptTrash() const;

    void _AdjustNoteItemDye(const QString& _CurrentNoteBookName,
                            const QTabWidget* wxNote_TabWidget);
    void _AdjustNoteItemDye(const QList<_NoteListItem *> _Notes);

signals:
    /* 当_FinalTabWindow中已经有了该笔记的shadow, 发射该信号 */
    void _NoteItemBeDCSignal_NonOpened(_MainWindowShadow* _EditWindow,
                                       const QString& _EditWindowTitle,
                                       const QIcon& _Icon);
    /* 如果_FinalTabWindow中还没有该笔记的shadow, 发射该信号 */
    void _NoteItemBeDCSignal_BeOpened(_TextEditorWindow* _EditorWindow);

    void _SetTabIconSignal(const QIcon& _Icon);

protected slots:
    void _SetCurrentNoteItemPreview(const _TextEditorWindow* _EditorWindow,
                                    const QString& _NoteTitle,
                                    const QString& _PreviewText);

    void _SetCurrentNoteItem_NoteName(const QString& _NoteName);

protected:
    enum { _SoLongTitleStd = 13 };

    void mouseDoubleClickEvent(QMouseEvent *_Event);
    _TextEditorWindow* _GetCurrentNoteEditorWindow() const;

    bool _IsCurrentNoteListAllHidden();

    bool _HasSpecifiedShadowWindow(
                    const _TextEditorWindow* _ShadowWindow);

    QString _GetCurrentNoteCategories(const _TextEditorWindow* _CurrentEW);
    QString _GetCurrentNoteRating(const _TextEditorWindow* _CurrentEW);
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

