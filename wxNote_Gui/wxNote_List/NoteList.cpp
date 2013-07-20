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

#include "wxNote_Gui/wxNote_Window/MainWindowNormal.h"
#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Gui/wxNote_Window/MainWindowShadow.h"
#include "wxNote_Gui/wxNote_Dialog/CommonDialog.h"
#include "wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h"
#include "wxNote_Gui/wxNote_List/NoteList.h"
#include "wxNote_Gui/wxNote_Tree/CategoriesTree.h"

#include "wxNote_Item/NoteListItem.h"
#include "wxNote_Global.h"

#include <QMouseEvent>
#include <QPair>
#include <QDate>
#include <QTime>
#include <QLabel>

#include <algorithm>

//.._NoteList类实现

    /* 构造函数实现 */
    _NoteList::_NoteList(QWidget *_Parent)
        : QListWidget(_Parent)
        {;}

    /* _AddItem()函数实现 */
    void _NoteList::_AddItem(const QPair<_NoteListItem *, QString> _ItemGroup)
        {
        _ItemGroup.first->_SetParentNoteBookName(_ItemGroup.second);
        _ItemGroup.first->_SetBeforeDeletedParentNoteBookName(_ItemGroup.second);

        this->addItem(_ItemGroup.first);
        }

    /* _HiddenCount()函数实现 */
    int _NoteList::_HiddenCount()
        {
        int _HiddenCnt = 0;
        for (int _Index = 0; _Index < this->count(); _Index++)
            if (this->currentItem()
                    && this->currentItem()->isHidden())
                ++_HiddenCnt;

        return _HiddenCnt;
        }

    /* _GetIndexFromIndex()函数实现 */
    QModelIndex _NoteList::_GetIndexFromItem(QListWidgetItem *_Item) const
        {
        return this->indexFromItem(_Item);
        }

    /* _GetCurrentItem()函数实现 */
    _NoteListItem* _NoteList::_GetCurrentItem() const
        {
        _NoteListItem* _Temp = dynamic_cast<_NoteListItem *>(currentItem());

        return _Temp ? _Temp : nullptr;
        }

    /* _Item()函数实现 */
    _NoteListItem* _NoteList::_Item(int _Index) const
        {
        _NoteListItem* _CurrentNoteItem =
                dynamic_cast<_NoteListItem *>(this->item(_Index));

        return _CurrentNoteItem ? _CurrentNoteItem : nullptr;
        }

    /* _GetSpecifiedItemTitle()函数实现 */
    QString _NoteList::_GetSpecifiedItemTitle(int _Index)
        {
        return _Item(_Index)->_GetNoteNameSlot();
        }

    /* _GetCurrentItemTitle()函数实现 */
    QString _NoteList::_GetCurrentItemTitle()
        {
        int _CurrentIndex = this->currentRow();

        return _GetSpecifiedItemTitle(_CurrentIndex);
        }

    /* _GetNotesInSpecifiedNoteBook()函数实现
     * 获取给定笔记本中的笔记的列表 */
    QList<_NoteListItem *> _NoteList
        ::_GetNotesInSpecifiedNoteBook(const QString &_CurrentLabelName,
                                       const QTabWidget *wxNote_TabWidget)
        {
        QList<_NoteListItem *> _Notes;

        int _wxNoteTabWidgetIndex = wxNote_TabWidget->currentIndex();

        QString _CurrentTabText =
                wxNote_TabWidget->tabText(_wxNoteTabWidgetIndex);

        if (_CurrentTabText == wxNote::g_NoteBooksListName)
            {
            /* 如果当前笔记本是"全部笔记"或"笔记本"
             * 那么将返回全部未在废纸篓中的笔记项 */
            if (_CurrentLabelName == wxNote::g_AllNotesName
                    || _CurrentLabelName == wxNote::g_NoteBooksName)
                _Notes = _GetAllNotes_ExceptTrash();
            else
                {
                for (int _Index = 0; _Index < count(); _Index++)
                    {
                    _NoteListItem* _CurrentNoteItem = _Item(_Index);

                    if (_CurrentLabelName == _CurrentNoteItem->_GetParentNoteBookName())
                        _Notes.push_back(_CurrentNoteItem);
                    }
                }
            }

        else if (_CurrentTabText == wxNote::g_CategoriesListName)
            {
            /* "评分" */
            if (_CurrentLabelName == wxNote::g_ExcellentName)
                _Notes = _GetNotesInSpecifiedRating(wxNote::_Excellent);

            else if (_CurrentLabelName == wxNote::g_GoodName)
                _Notes = _GetNotesInSpecifiedRating(wxNote::_Good);

            else if (_CurrentLabelName == wxNote::g_AverageName)
                _Notes = _GetNotesInSpecifiedRating(wxNote::_Average);

            else if (_CurrentLabelName == wxNote::g_FairName)
                _Notes = _GetNotesInSpecifiedRating(wxNote::_Fair);

            else if (_CurrentLabelName == wxNote::g_PoorName)
                _Notes = _GetNotesInSpecifiedRating(wxNote::_Poor);

            /* "颜色标签" */
            else if (_CurrentLabelName == wxNote::g_ImportantName)
                _Notes = _GetNotesInSpecifiedCategories(wxNote::_Important);

            else if (_CurrentLabelName == wxNote::g_WorkName)
                _Notes = _GetNotesInSpecifiedCategories(wxNote::_Work);

            else if (_CurrentLabelName == wxNote::g_PersonalName)
                _Notes = _GetNotesInSpecifiedCategories(wxNote::_Personal);

            else if (_CurrentLabelName == wxNote::g_ToDoName)
                _Notes = _GetNotesInSpecifiedCategories(wxNote::_ToDo);

            else if (_CurrentLabelName == wxNote::g_LaterName)
                _Notes = _GetNotesInSpecifiedCategories(wxNote::_Later);
            }

        return _Notes;
        }

    /* _GetNotesInSpecifiedCategories()函数实现 */
    QList<_NoteListItem *> _NoteList
        ::_GetNotesInSpecifiedCategories(const wxNote::_NoteCategories _Cate_)
        {
        QList<_NoteListItem *> _Notes;

        for (int _Index = 0; _Index < count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem = _Item(_Index);

            if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName
                    && _CurrentNoteItem->_GetNoteCategories() == _Cate_)
                _Notes.push_back(_CurrentNoteItem);
            }

        return _Notes;
        }

    /* _GetNotesInSpecifiedRating()函数实现 */
    QList<_NoteListItem *> _NoteList
        ::_GetNotesInSpecifiedRating(const wxNote::_NoteRating _Rating)
        {
        QList<_NoteListItem *> _Notes;

        for (int _Index = 0; _Index < count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem = _Item(_Index);

            if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName
                    && _CurrentNoteItem->_GetNoteRating() == _Rating)
                _Notes.push_back(_CurrentNoteItem);
            }

        return _Notes;
        }

    /* _GetAllNotes()函数实现
     * 返回全部笔记项(包括在废纸篓中的笔记) */
    QList<_NoteListItem *> _NoteList::_GetAllNotes() const
        {
        QList<_NoteListItem *> _AllNotes;

        for (int _Index = 0; _Index < count(); _Index++)
            _AllNotes.push_back(_Item(_Index));

        return _AllNotes;
        }

    /* _GetAllNotes_ExceptTrash()函数实现
     * 返回全部笔记中不包括在废纸篓中的笔记 */
    QList<_NoteListItem *> _NoteList::_GetAllNotes_ExceptTrash() const
        {
        QList<_NoteListItem *> _Notes;

        for (int _Index = 0; _Index < count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem = _Item(_Index);

            if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName)
                _Notes.push_back(_CurrentNoteItem);
            }

        return _Notes;
        }

    /* _AdjustNoteItemDye(QString)函数实现 */
    void _NoteList::_AdjustNoteItemDye(const QString &_CurrentNoteBookName,
                                       const QTabWidget *wxNote_TabWidget)
        {
        QList<_NoteListItem *> _Notes;

        int _TabWidgetIndex = wxNote_TabWidget->currentIndex();

        QString _CurrentTabText = wxNote_TabWidget->tabText(_TabWidgetIndex);

        if (_CurrentTabText == wxNote::g_NoteBooksListName)
            {
            if (_CurrentNoteBookName == wxNote::g_AllNotesName
                    || _CurrentNoteBookName == wxNote::g_NoteBooksName)
                _Notes = _GetAllNotes_ExceptTrash();
            else
                _Notes = _GetNotesInSpecifiedNoteBook(_CurrentNoteBookName,
                                                      wxNote_TabWidget);
            }

        else if (_CurrentTabText == wxNote::g_CategoriesListName)
            {
            _CategoriesTree* __CategoriesTree =
                    dynamic_cast<_CategoriesTree *>(wxNote_TabWidget->widget(1));

            if (__CategoriesTree)
                {
                QString _CurrentItemText = __CategoriesTree->currentItem()->text(0);

                if (_CurrentItemText == wxNote::g_ExcellentName)
                    _Notes = _GetNotesInSpecifiedRating(wxNote::_Excellent);

                else if (_CurrentItemText == wxNote::g_GoodName)
                    _Notes = _GetNotesInSpecifiedRating(wxNote::_Good);

                else if (_CurrentItemText == wxNote::g_AverageName)
                    _Notes = _GetNotesInSpecifiedRating(wxNote::_Average);

                else if (_CurrentItemText == wxNote::g_FairName)
                    _Notes = _GetNotesInSpecifiedRating(wxNote::_Fair);

                else if (_CurrentItemText == wxNote::g_PoorName)
                    _Notes = _GetNotesInSpecifiedRating(wxNote::_Poor);

                else if (_CurrentItemText == wxNote::g_ImportantName)
                    _Notes = _GetNotesInSpecifiedCategories(wxNote::_Important);

                else if (_CurrentItemText == wxNote::g_WorkName)
                    _Notes = _GetNotesInSpecifiedCategories(wxNote::_Work);

                else if (_CurrentItemText == wxNote::g_PersonalName)
                    _Notes = _GetNotesInSpecifiedCategories(wxNote::_Personal);

                else if (_CurrentItemText == wxNote::g_ToDoName)
                    _Notes = _GetNotesInSpecifiedCategories(wxNote::_ToDo);

                else if (_CurrentItemText == wxNote::g_LaterName)
                    _Notes = _GetNotesInSpecifiedCategories(wxNote::_Later);
                }
            else
                __PTR_INVALID_ERROR_OUTPUT__(__CategoriesTree)
            }

        _AdjustNoteItemDye(_Notes);
        }

    /* _AdjustNoteItemDye(QList<_NoteListItem *>)函数实现 */
    void _NoteList::_AdjustNoteItemDye(const QList<_NoteListItem *> _Notes)
        {
        for (int _Index = 0; _Index < _Notes.count(); _Index++)
            {
            if (_Index % 2)
                _Notes.at(_Index)->setBackground(
                            QBrush(QColor(240, 240, 240), Qt::Dense3Pattern));
            else
                _Notes.at(_Index)->setBackground(
                            QBrush(QColor(255, 255, 255)));
            }
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _SetCurrentNoteItemPreview()槽实现 */
    void _NoteList::_SetCurrentNoteItemPreview(const _TextEditorWindow *_EditorWindow,
                                               const QString &/*_NoteTitle*/,
                                               const QString &_PreviewText)
        {
        if (_PreviewText.isEmpty())
            return;

        _NoteListItem* _CurrentNoteItem = new _NoteListItem;
        for (int _Index = 0; _Index < count(); _Index++)
            {
            _NoteListItem* _NoteItem = _Item(_Index);

            if (_NoteItem->_GetBindTextEW() == _EditorWindow)
                {
                _CurrentNoteItem = _NoteItem;
                break;
                }
            }

        _TextEditorWindow* _CurrentEditorWindow = _CurrentNoteItem->_GetBindTextEW();

        QString _CurrentNoteName =
                _CurrentEditorWindow->_GetTitleLineEdit()->text();
        QString _CurrentDate = QDate::currentDate().toString(Qt::ISODate);

        QLabel* _DisplayWidget = dynamic_cast<QLabel *>(itemWidget(_CurrentNoteItem));
        _DisplayWidget->clear();

        QString _ColorLabel = _GetCurrentNoteCategories(_CurrentEditorWindow);
        QString _RatingLabel = _GetCurrentNoteRating(_CurrentEditorWindow);

        _DisplayWidget->setText(tr("<p style=\" margin-top:5px; margin-bottom:0px; margin-left:8px; margin-right:8px; -qt-block-indent:0; text-indent:0px;\">"
                                   "<span style=\" \">"
                                   "%1<span style=\" \">   </span>"
                                   "%2<span style=\" \">   </span>"
                                   "%3<span style=\" \">   </span>"
                                   "%4</span></p>"

                                   "<p style=\" margin-top:9px; margin-bottom:0px; margin-left:8px; margin-right:8px; -qt-block-indent:0; text-indent:0px;\">"
                                   "<span style=\"color:#0a95d8;\">%5</span>"
                                   "<span style=\" \">   </span><span style=\"color:#6b6b6b;\">%6</span></p>")
                                .arg(_CurrentNoteName)
                                .arg(_RatingLabel)
                                .arg(_ColorLabel)
                                .arg(_CurrentEditorWindow->_IsLocking() ? wxNote::g_Locking_Mini
                                                                        : QString())
                                .arg(_CurrentDate)
                                .arg(_PreviewText));
        }

    /* _SetCurrentNoteItem_NoteName()槽实现 */
    void _NoteList::_SetCurrentNoteItem_NoteName(const QString &_NoteName)
        {
        _GetCurrentItem()->_SetNoteNameSlot(_NoteName);
        }

    /* mouseDoubleClickEvent()事件重写 */
    void _NoteList::mouseDoubleClickEvent(QMouseEvent */*_Event*/)
        {
        if (this->count() /* 只有当笔记列表中存在至少一个笔记项时才会打开独立的编辑窗口 */)
            {
            if (!_IsCurrentNoteListAllHidden())
                {
                _NoteListItem* _CurrentNoteItem = _GetCurrentItem();

                QString _CurrentNoteName = _CurrentNoteItem->_GetNoteNameSlot();

                /* 如果当前笔记已被删除, 将拒绝用户打开该笔记的shadow窗口的请求 */
                if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName)
                    {
                    _TextEditorWindow* _CurrentTextEditorWindow = _GetCurrentNoteEditorWindow();

                    /* 如果当前的tab窗口中已经有了该笔记的编辑窗口, 那么直接切换到该窗口 */
                    if (_HasSpecifiedShadowWindow(_CurrentTextEditorWindow))
                        emit _NoteItemBeDCSignal_BeOpened(_CurrentTextEditorWindow);
                    else
                        {
                        /* 否则创建新的tab窗口 */
                        _MainWindowShadow* _NewWindow
                                        = new _MainWindowShadow(_CurrentTextEditorWindow, this);

                        emit _NoteItemBeDCSignal_NonOpened(_NewWindow,
                                                           _CurrentNoteName,
                                                           _GetCurrentItem()->icon());
                        emit _SetTabIconSignal(_GetCurrentItem()->icon());
                        }
                    }
                else
                    {
                    _CommonDialog _WarningDialog(this
                                                 , tr("wxNote")
                                                 , tr("<font color=darkred><B>笔记“%1”已被删除。</font></B>"
                                                    "<p>无法打开一个被删除的笔记的编辑窗口，"
                                                    "<p>必须还原笔记后才能进进行该操作。")
                                                    .arg(_CurrentNoteName.count() < _SoLongTitleStd
                                                                    ? _CurrentNoteName
                                                                    : _CurrentNoteName.left(4)
                                                                      + "..." + _CurrentNoteName.right(4))
                                                 , tr("OK"));
                    _WarningDialog.exec();
                    return;
                    }
                }
            }
        }

    /* _GetCurrentNoteEditorWindow()函数实现 */
    _TextEditorWindow* _NoteList::_GetCurrentNoteEditorWindow() const
        {
        QList<_TextEditorWindow *>::const_iterator _Iter =
                std::find_if(wxNote::g_AllTextEditorList.cbegin(), wxNote::g_AllTextEditorList.cend(),
                             [this](const _TextEditorWindow* _Elem)
                                    {
                                    _NoteListItem* _CurrentNoteItem = _GetCurrentItem();

                                    if (_Elem)
                                        return _CurrentNoteItem->_GetBindTextEW() == _Elem;
                                    else
                                        return false;
                                    });
        return *_Iter;
        }

    /* _IsCurrentNoteListAllHidden()函数实现 */
    bool _NoteList::_IsCurrentNoteListAllHidden()
        {
        QList<bool> _IsHiddenList;
        for (int _Index = 0; _Index < this->count(); _Index++)
            _IsHiddenList.push_back(dynamic_cast<_NoteListItem *>(
                                        this->item(_Index))->isHidden());

        /* 与_MainWindowNormal::_IsCurrentNoteListAllHidden()
         * 函数中的算法相同, 用于判断当前笔记列表中的笔记项是否为全部隐藏
         * 如果为全部隐藏, 意味着当前笔记本中没有任何笔记, 那也就没有必要弹出该窗口了 */
        bool b_IsAllHidden
                = std::all_of(_IsHiddenList.begin(), _IsHiddenList.end(),
                              [](const bool& _Elem)
                                    {
                                    return _Elem;
                                    });
        return b_IsAllHidden;
        }

    /* _HasSpecifiedShadowWindow()函数实现 */
    bool _NoteList
        ::_HasSpecifiedShadowWindow(const _TextEditorWindow *_ShadowWindow)
        {
        using namespace wxNote;

        for (int _Index = 0; _Index < g_ShadowTextEditorList.count(); _Index++)
            if (_ShadowWindow == g_ShadowTextEditorList.at(_Index))
                return true;

        return false;
        }

    /* _GetCurrentNoteCategories()函数实现 */
    QString _NoteList
        ::_GetCurrentNoteCategories(const _TextEditorWindow *_CurrentEW)
        {
        QString _ColorLabel;
        switch (_CurrentEW->_GetNoteCategories())
            {
        case wxNote::_Important:
            _ColorLabel = wxNote::g_Important_Mini;
            break;

        case wxNote::_Work:
            _ColorLabel = wxNote::g_Work_Mini;
            break;

        case wxNote::_Personal:
            _ColorLabel = wxNote::g_Personal_Mini;
            break;

        case wxNote::_ToDo:
            _ColorLabel = wxNote::g_ToDO_Mini;
            break;

        case wxNote::_Later:
            _ColorLabel = wxNote::g_Later_Mini;
            break;

        default:
            _ColorLabel = QString();
            }

        return _ColorLabel;
        }

    /* _GetCurrentNoteRating()函数实现 */
    QString _NoteList
        ::_GetCurrentNoteRating(const _TextEditorWindow *_CurrentEW)
        {
        QString _RatingLabel;
        switch (_CurrentEW->_GetNoteRating())
            {
        case wxNote::_Excellent:
            _RatingLabel = wxNote::g_Excellent_Mini;
            break;

        case wxNote::_Good:
            _RatingLabel = wxNote::g_Good_Mini;
            break;

        case wxNote::_Average:
            _RatingLabel = wxNote::g_Average_Mini;
            break;

        case wxNote::_Fair:
            _RatingLabel = wxNote::g_Fair_Mini;
            break;

        case wxNote::_Poor:
            _RatingLabel = wxNote::g_Poor_Mini;
            break;

        default:
            _RatingLabel = QString();
            }

        return _RatingLabel;
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

