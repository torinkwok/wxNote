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
#include "wxNote_Gui/wxNote_Window/AbstractMainWindow.h"
#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Gui/wxNote_Other/SearchPanel.h"

#include "wxNote_Global.h"

#include <QLineEdit>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QMap>

class _NoteBookTree;
class _NoteList;
class _CategoriesTree;
class _NoteListItem;
class _TextEditorWindow;
class _NoteTitleLineEdit;
class _NoteEditorSplitter;
class _wxNote_ToolButton;

class QLineEdit;
class QStackedLayout;
class QAction;
class QMenu;
class QToolButton;
class QTreeWidgetItem;
class QListWidget;
class QListWidgetItem;
class QHBoxLayout;
class QBrush;
class QTabWidget;

/* wxNote正常情况下显示的主窗口
 * 该类派生自_MainWindow
 */

class _MainWindowNormal : public _AbstractMainWindow
    {
    Q_OBJECT

public:
    _MainWindowNormal(QWidget* _Parent = nullptr);

    _NoteList* _GetNoteList() const
                        { return m_NoteList; }
    _NoteBookTree* _GetNoteBookTree() const
                        { return m_NoteBookTree; }

    QStackedLayout* _GetNoteEditorStackedLayout() const
                        { return m_TextEditorStackedLayout; }

    /* 该函数供_FinalTabWindow::_ExchangeEditorWindowOwnershipSlot()使用,
     * 用来在切换tab时重新将_MainWindowShadow中的共享shadow编辑窗口
     * 添加到当前的主编辑窗口中 */
    void _GetOwnershipFromShadowWindow(_TextEditorWindow* _ShadowWin);

    bool _IsSearchLineEditEmpty() const
            { return m_SearchPanel->_GetSearchLineEdit()->text().isEmpty(); }

    void _ClearSearchLineEdit()
            { m_SearchPanel->_GetSearchLineEdit()->clear(); }

    QList<QPair<QString, _NoteListItem *>> _GetLastPitchOnItemList() const
            { return m_LastPitchOnItemList_NoteBookTree; }

    _NoteListItem* _GetFirstNoteInSpecifiedNoteBook(const QString& _NoteBookName);

    void closeEvent(QCloseEvent *_Event);

signals:
    void _MainWindowNormalTitleChangedSignal(const QString& _NewTitle);

    /* 由_FinalTabWindow捕获该signal */
    void _CurrentTitleLineEditTextChangedSignal(const _TextEditorWindow*,
                                                const QString& _Text);

    void _FinalTabWindowMove2NextTab_BridgeSignal();

    void _CloseCurrentNoteShadowWindowSignal(const _TextEditorWindow* _EditorWindow);

    void _EliminateCurrentShadowWindowSignal();

    void _SetCurrentTabIconSignal(const QString& _NoteTitle,
                                  const QString& _IconPath);

    void _AdjustTabWindowTitle(const QString& _CurrentNoteBookName);

public slots:
    void _OptionsSlot();

    void _UndoSlot();
    void _RedoSlot();
    void _CutSlot();
    void _CopySlot();
    void _PasteSlot();
    void _PastePlainTextSlot();
    void _DeleteSlot();
    void _SelectAllSlot();

    void _CurrentNoteNameChangedSlot(const QString& _Title);

protected slots:
    void _WriteSettings();
    void _ReadSettings();

    void _SetLeftPanelIsHidden(bool _IsHidden);
    void _SetNoteListIsHidden(bool _IsHidden);
    void _SetNotePanelIsHidden(bool _IsHidden);

    void _wxNote_TabWidgetChangedSlot(int _TabIndex);

    void _CurrentNoteBookItemChangedSlot(QTreeWidgetItem* _Current,
                                         QTreeWidgetItem* _Previous);
    void _CurrentCategoriesItemChangedSlot(QTreeWidgetItem* _Current,
                                           QTreeWidgetItem* _Previous);

    void _CurrentNoteRatingChangedSlot();
    void _CurrentNoteCategoriesChangedSlot();

    void _CurrentNoteBookChangedSlot_WindowTitle();
    void _CurrentNoteChangedSlot_WindowTitle();

    void _SetCurrentNoteIconSlot(const QString& _TextTitle,
                                 const QString& _IconPath);
    void _NoteTitleLineEditTextChanged(const QString &_Text);
    void _MainWindowCurrentTitleChanged(const QString &_Text);
    void _CurrentTitleLineEditTextChangedSlot(const QString &_Text);

    void _SetAcceptRichTextSlot(bool _IsOK);

    /* 针对单个笔记操作的slot */
    void _NewNoteSlot();
    void _MoveNote2TrashSlot();
    void _ThoroughDeleteCurrentNoteSlot();
    void _RestoreNoteSlot();

    void _MoveCurrentNote2OtherNoteBook(const QString& _NoteBookName);

    /* 针对单个笔记本操作的slot  */
    void _NewNoteBookSlot();
    void _RenameNoteBookSlot();
    void _ThoroughDeleteNoteBookSlot();

    void _LockAllNotesSlot();

    void _BeginFilterSlot(QString _SearchText);
    void _StopFilterSlot();

    void _SaveCurrentNoteContextSlot();

    void _RecordLastPitchOnNoteItemSlot_NoteBookTree(int _Row);
    void _RecordLastPitchOnNoteItemSlot_Categories(int _Row);

protected:
    void _CreateActionsAndSubMenu();
    void _CreateMainWinMenus();
    void _CreateMainWinToolBar();
    void _CreateToolButtonOnToolBar();
    void _CreateContextMenu();

    void _Create_wxNoteTabWidget();
    void _CreateNoteBookTree();
    void _CreateCategoriesTree();

    void _CreateNoteList();
    void _CreateSearchPanel();

    bool _IsCurrentNoteListAllHidden();

    bool _EraseNoteItemFromDeletedList(const QListWidgetItem* _DeletedItem);
    void _EraseSpecifiedEditorWindowFromGlobalList(QListWidgetItem* _NoteItem);
    void _EraseCurrentEditorWindowFromGlobalList();

    int  _SpecifyNoteBookCnt(const QString& _NoteBookName);
    int  _CurrentNoteBookCnt();

    void _SetDeleteNoteButton_Action_MenuEnabled(bool _IsOK);
    void _SetDeleteNoteBookAction_MenuEnabled(bool _IsOK);
    void _SetEliminateButton_ActionEnabled(bool _IsOK);
    void _SetRestoreButton_ActionEnabled(bool _IsOK);
    void _SetNewNoteButton_ActionEnabled(bool _IsOK);
    void _SetOneKeyLockButton_ActionEnabled(bool _IsOK);
    void _SetNoteEditActionsEnabled(bool _IsOK);

    void _SetDeleteNoteEnabled();
    void _SetDeleteNoteBookEnabled();
    void _SetEliminateNoteEnabled();
    void _SetRestoreNoteEnabled();
    void _SetNewNoteEnabled();
    void _SetOneKeyLockEnabled();
    void _SetNoteEditEnabled();

    void _DeleteSavedContext(const QString& _NoteTitle);
    void _EraseNoteBookName_WhenDeleted(const QString& _NoteBookName);
    void _EraseLastPitchOnItem_inNoteBookList(const QString& _NoteBookName);
    void _EraseLastPitchOnItem_inCategoriesList(const QString& _CategoriesName);

    /* 该函数用于获取指定笔记本中最后一次被选中的笔记项 */
    _NoteListItem*
        _GetLastPitchOnItem_inNoteBookTree(const QString& _NoteBookName);
    _NoteListItem*
        _GetLastPitchOnItem_inCategoriesTree(const QString& _CategoriesName);

    _TextEditorWindow* _GetCurrentTextEditorWindow() const;

    void _RestoreLastPitchOnItem_NoteBookTree(const QString& _NoteBookName);
    void _RestoreLastPitchOnItem_CategoriesTree(const QString& _CategoriesName);

    void _SetNotesAllHidden();
    void _SetNotesAllVisible_ExceptTrash();

    void _FilterNote_ByCategories(
            const wxNote::_NoteCategories& _NoteCategories);
    void _FilterNote_ByRating(
            const wxNote::_NoteRating& _NoteRating);

    void _FilterNote_inCategoriesTree(const QString _CurrentLabelName);

    void _PitchOnSpecifiedNoteBook(const QString& _NoteBookName);

    bool _CheckSpecifiedNoteBookHasSelectItem(const QString _NoteBookName);

    enum enum_NoteStatus { _NormalStatus, _FilterStatus } m_NoteStatus;

    QBrush  m_MatchNoteItemDye;

    /* 存储每个笔记本中最后一个被选中的笔记项 */
    QList<QPair<QString, _NoteListItem *>> m_LastPitchOnItemList_NoteBookTree;
    QList<QPair<QString, _NoteListItem *>> m_LastPitchOnItemList_CategoriesTree;

    QList<QPair<QString, QString>> m_FirstHandContextList;

    /* 用于存储被移动到废纸篓的笔记项 */
    QList<_NoteListItem *> m_DeletedNoteList;

    ///////////////////////////////////////////////
    /// wxNote TabWidget中的窗口部件
    ///
    QTabWidget*          m_wxNoteTabWidget;

    _NoteBookTree*       m_NoteBookTree;
    _CategoriesTree*     m_CategoriesTree;

    ///////////////////////////////////////////////

    _NoteList*           m_NoteList;

    _SearchPanel* m_SearchPanel;     // 搜索面板

    _NoteEditorSplitter* m_LineEditSplitter;
    _NoteEditorSplitter* m_RightSplitter;
    _NoteEditorSplitter* m_NoteListSplitter;
    _NoteEditorSplitter* m_CenterSplitter;
    _NoteEditorSplitter* m_MainSplitter;

    QStackedLayout*      m_TextEditorStackedLayout;
    QWidget*             m_EditorPanel;

    /* "文件"菜单项中的QMenu或QAction */
    QAction* m_NewNoteAction;
    QAction* m_NewNoteBookAction;
    QMenu*   m_NewNoteSubMenuOnToolButton;

    /* "查看"菜单项中的QMenu或QAction */
    QAction* m_DisplayLeftPanelAction;
    QAction* m_DisplayNoteListAction;
    QAction* m_DisplayNotePanelAction;

    /* "笔记"菜单项中的QMenu或QAction */
    QAction* m_DeletedNoteBookAction;
    QAction* m_OneKeyLockAction;

    _wxNote_ToolButton* m_NewNoteToolButton;
    _wxNote_ToolButton* m_TrashToolButton;
    _wxNote_ToolButton* m_OneKeyLockToolButton;
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
