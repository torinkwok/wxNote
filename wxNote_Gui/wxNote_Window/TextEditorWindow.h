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
#include "wxNote_Global.h"

#include <QMainWindow>
#include <QFont>
#include <QMenu>
#include <QStringList>

#ifndef QT_KEYPAD_NAVIGATION
    #define QT_KEYPAD_NAVIGATION
#endif

class _NoteTitleLineEdit;
class _CommonDialog;
class _EliminateOrRestorePushButton;
class _NoteEditor;
class _FontComboBox;
class _FontSizeComboBox;
class _EditorWindowStatusBar;
class _NoteListItem;

class QToolBar;
class QToolButton;
class QFontDialog;
class QColorDialog;
class QFontComboBox;
class QComboBox;
class QLabel;
class QComboBox;
class QPushButton;
class QHBoxLayout;
class QActionGroup;
class QStatusBar;

class _TextEditorWindow : public QMainWindow
    {
    Q_OBJECT

public:
    _TextEditorWindow(QWidget* _Parent = nullptr);

    _NoteEditor* _GetTextEditor() const
                    { return m_TextEditor; }
    _NoteTitleLineEdit* _GetTitleLineEdit() const
                    { return m_NoteTitleLineEdit; }

    bool _IsLocking() const
                    { return mb_IsLocking; }
    QToolButton* _GetLockButton() const
                    { return m_LockCurrentTextEditToolButton; }

    /* 设置该笔记的编辑窗口当前所属的笔记本名称 */
    void _SetParentNoteBookName_Current(const QString& _NoteBookName)
                    { m_ParentNoteBookName_Current = _NoteBookName; }
    /* Getter */
    QString _GetParentNoteBookName_Current() const
                    { return m_ParentNoteBookName_Current; }

    /* 设置该笔记的编辑窗口被删除前所属的的笔记本名称 */
    void _SetParentNoteBookName_BeforeDeleted(const QString& _NoteBookName)
                    { m_ParentNoteBookName_BeforeDeleted = _NoteBookName; }
    /* Getter */
    QString _GetParentNoteBookName_BeforeDeleted() const
                    { return m_ParentNoteBookName_BeforeDeleted; }

    QString _GetCategoriesName_Current() const
                    { return m_Categories_Current; }
    void _SetCategoriedName_Current(const QString& _CategoriesName)
                    { m_Categories_Current = _CategoriesName; }

    QString _GetRatingName_Current() const
                    { return m_Rating_Current; }
    void _SetRatingName_Current(const QString& _RatingName)
                    { m_Rating_Current = _RatingName; }

    void _SetParentWindowType(wxNote::_ParentWindowType _ParentWindowType)
                    { enum_ParentWindowType = _ParentWindowType; }
    wxNote::_ParentWindowType _GetParentWindowType() const
                    { return enum_ParentWindowType; }

    QMenu* _GetCategoriesMenu() const
                    { return m_CategoriesMenu; }
    QMenu* _GetMoveNoteMenu() const
                    { return m_MoveNoteMenu; }

    wxNote::_NoteCategories _GetNoteCategories() const
                    { return enum_NoteCategories; }
    wxNote::_NoteRating _GetNoteRating() const
                    { return enum_NoteRating; }

    _FontComboBox* _GetFontComboBox() const
                    { return m_FontComboBox; }
    _FontSizeComboBox* _GetFontSizeComboBox() const
                    { return m_FontSizeComboBox; }

    void _SetIndexInTabWidget(int _Index)
                    { m_IndexInTabWidget = _Index; }
    int _GetIndexInTabWidget() const
                    { return m_IndexInTabWidget; }

    /* 将当前编辑窗口与其对应的笔记项进行绑定 */
    void _SetBindNoteItem(_NoteListItem *_BindNoteItem);
    /* 获取与当前编辑窗口绑定的笔记项 */
    _NoteListItem* _GetBindNoteItem() const;

    bool _GetIsAcceptRichText() const
                    { return mb_IsAcceptRichText; }
    void _SetIsAcceptRichText(const bool& _IsAccept)
                    { mb_IsAcceptRichText = _IsAccept; }

    void _SetCategoriesMenuEnabled(bool _IsOK)
                    { _GetCategoriesMenu()->setEnabled(_IsOK); }
    void _SetMoveNoteMenuEnabled(bool _IsOK)
                    { _GetMoveNoteMenu()->setEnabled(_IsOK); }

    void _SetEliminateOrRestoreToolBarHidden(const bool& _IsHidden);
    void _SetEditorInfoToolBarHidden(const bool& _IsHidden);

    bool _IsEliminateOrRestoreHidden() const;
    bool _IsEditorInfoToolBarHidden() const;

    void _AddAction2_MoveNoteMenu(QAction* _NewAction);

    void _EmitPreviewText(const QString& _NoteTitle);

public slots:
    void _SetNoteRatingIs_NonRating();
    void _SetNoteRatingIs_Excellent();
    void _SetNoteRatingIs_Good();
    void _SetNoteRatingIs_Average();
    void _SetNoteRatingIs_Fair();
    void _SetNoteRatingIs_Poor();

    void _SetNoteCategoriesIs_NonCategories();
    void _SetNoteCategoriesIs_Important();
    void _SetNoteCategoriesIs_Work();
    void _SetNoteCategoriesIs_Personal();
    void _SetNoteCategories_ToDo();
    void _SetNoteCategoriesIs_Later();

    void _SaveCurrentNoteSlot();

    bool _LockCurrentNoteSlot();
    void _UnlockCurrentNoteSlot();

signals:
    void _CurrentNoteBeMove2TrashSignal(bool _IsBeDeleted);
    void _CurrentNoteBeRestoreSignal(bool _IsRestored);

    void _EliminateNoteButtonClickedSignal();
    void _RestoreNoteButtonClickedSignal();

    void _SetCurrentNoteIconSignal(const QString& _NoteTitle,
                                   const QString& _IconPath);
    void _CurrentTabIconChangedSignal(const QString& _NoteTitle,
                                      const QString& _IconPath);
    void _CurrentTabTextChanged(const QString& _Text);
    void _SetNoteListPreview(const _TextEditorWindow* _TextEditorWindow,
                             const QString _NoteTitle,
                             const QString& _PreviewText);
    void _SetNoteItem_NoteName(const QString& _NoteName);

    /* 由_MainWindowNormal捕获 */
    void _BeSavedSignals();

    void _MoveCurrentNote2TrashSignal();

    void _CurrentNoteRatingChanged();
    void _CurrentNoteCategoriesChanged();

    void _CurrentNoteBeMove2OtherNoteBookSignal(const QString& _NoteBookName);

    void _DeleteNonMatchedNoteFileSignal(const QString& _CurrentNoteBook);

protected slots:
    void _SetCurrentFontByFontComboBox(const QString& _FontFamily);
    void _SetCurrentFontSizeByFontSizeComboBox(const QString& _Text);

    void _FontDialogCurrentFontChangedSlot(const QFont& _CurrentFont);
    void _SelectColorSlot();
    void _SelectFontSlot();

    void _ClickedBoldButtonSlot(bool _IsOK);
    void _ClickedItalicButtonSlot(bool _IsOK);
    void _ClickedUnderlineButtonSlot(bool _IsOK);

    void _ClickedLockedButtonSlot();

    void _ClickedJustifyLeftButtonSlot(bool _IsOK);
    void _ClickedJustifyCenterButtonSlot(bool _IsOK);
    void _ClickedJustifyRightButtonSlot(bool _IsOK);
    void _ClickedJustifyFillButtonSlot(bool _IsOK);

    void _CurrentNoteTitleChangedSlot();
    void _CurrentEditorTextChangedSlot();

    void _CurrentNoteBeMove2TrashSlot(bool _IsBeDeleted);
    void _CurrentNoteBeRestroeSlot(bool _IsBeDeleted);

    void _NoteBooksActionBeClicked(QAction* _TriggeredAction);

    /* 当光标位置改变时, 改变任务栏中各种状态按钮的开关状态 */
    void _SetCurrentNoteMoveEnabled(bool _IsOK);

    void _InsertTableSlot();
    void _InsertUnorderedListSlot();
    void _InsertOrderedListSlot();
    void _InsertCheckBoxSlot();

    void _SetCurrentTextStatusSlot();

protected:
    enum { _FontMinSize = 8, _FontMaxSize = 96 };
    enum { _SoLongTitleStd = 13 };
    enum { _PreivewTextMaximunCnt = 500 };
    bool mb_IsChanged;  // 用于判断当前笔记是否被更改
    bool mb_IsLocking;  // 用于判断当前笔记是否被锁定

    bool event(QEvent *_Event);
    void keyPressEvent(QKeyEvent *_Event);

    void _CreateActionsAndSubMenu();
    void _CreateWidgetsOnToolBar();
    void _CreateToolBar();

    void _CreateFontComboBoxesOnToolBar();

    void _SetEditorNotEnabled();
    void _SetEditorIsEnabled();

    QString _SimplifyLongTitle(const QString& _Title);

    void _SetJustifyLeftButtonChecked();
    void _SetJustifyCenterButtonChecked();
    void _SetJustifyRightButtonChecked();
    void _SetJustifyFillButtonChecked();

    QString    m_ParentNoteBookName_Current;
    QString    m_ParentNoteBookName_BeforeDeleted;
    QString    m_Categories_Current;
    QString    m_Rating_Current;

    wxNote::_ParentWindowType    enum_ParentWindowType;

    _NoteEditor* m_TextEditor;
    QFont        m_EditorCurrentFont;

    /* 与当前编辑窗口绑定的笔记项 */
    _NoteListItem* m_BindNoteItem;

    /* On m_EditorInfoToolBar */
    _NoteTitleLineEdit* m_NoteTitleLineEdit;

    //////////////////////////////////////////////////////////////
    /// 创建并实现"操作"按钮中的子菜单和动作
    ///
    wxNote::_NoteRating     enum_NoteRating;
    wxNote::_NoteCategories enum_NoteCategories;

    QToolButton*        m_OptionsToolButton;
    QMenu*              m_OptionsMenu;

    /* "给笔记评分"子菜单 */
    QMenu*        m_RatingMenu;
    QActionGroup* m_RatingActionGroup;
    QAction*      m_NonRatingAction;
    QAction*      m_ExcellentAction;
    QAction*      m_GoodAction;
    QAction*      m_AverageAction;
    QAction*      m_FairAction;
    QAction*      m_PoorAction;

    /* "给笔记分类"子菜单 */
    QMenu*        m_CategoriesMenu;
    QActionGroup* m_CategoriesActionGroup;
    QAction*      m_NonCategoriesAction;
    QAction*      m_ImportantAction;
    QAction*      m_WorkAction;
    QAction*      m_PersonalAction;
    QAction*      m_ToDoAction;
    QAction*      m_LaterAction;

    /* "移动笔记至"子菜单 */
    QMenu*        m_MoveNoteMenu;
    QActionGroup* m_MoveNoteActionGroup;
    QAction*      m_AllNoteAction;
    QAction*      m_TrashAction;

    ///////////////////////////////////////////////////////////////

    /* On m_EliminateOrRestoreToolBar */
    QLabel*      m_EliminateOrRestoreLabel;
    QHBoxLayout* m_EliminateOrRestoreLayout;
    QWidget*     m_EliminateOrRestoreGroupWidget;

    _EliminateOrRestorePushButton* m_EliminateNotePushButton;
    _EliminateOrRestorePushButton* m_RestoreNotePushButton;

    /* On m_EditorOptionsToolBar */
    _FontComboBox* m_FontComboBox;
    _FontSizeComboBox*     m_FontSizeComboBox;

    QString m_DefaultFontFamily;
    int     m_DefaultFontSize;
    bool    mb_IsAcceptRichText;

    int     m_IndexInTabWidget;

    QToolButton* m_SelectColorToolButton;
    QToolButton* m_SelectFontToolButton;

    QToolButton* m_BoldToolButton;
    QToolButton* m_ItalicToolButton;
    QToolButton* m_UnderlineToolButton;

    QToolButton* m_JustifyLeftToolButton;
    QToolButton* m_JustifyCenterToolButton;
    QToolButton* m_JustifyRightToolButton;
    QToolButton* m_JustifyFillToolButton;

    QToolButton* m_InsertTableToolButton;
    QToolButton* m_UnorderedListToolButton;
    QToolButton* m_OrderedListToolButton;
    QToolButton* m_CheckBoxToolButton;

    QToolButton* m_LockCurrentTextEditToolButton;

    QFontDialog* m_SelectFontDialog;
    QColorDialog*m_SelectColorDialog;

    QLabel*      m_ColorLabel;
    QHBoxLayout* m_ColorLabelLayout;
    QWidget*     m_CategoriesWidget;

    QToolBar*    m_EditorInfoToolBar;
    QToolBar*    m_EliminateOrRestoreToolBar;
    QToolBar*    m_EditorOptionsToolBar;
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
