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

#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Gui/wxNote_Dialog/SetPasswordDialog.h"
#include "wxNote_Gui/wxNote_Dialog/GetPasswordDialog.h"
#include "wxNote_Gui/wxNote_Dialog/CommonDialog.h"
#include "wxNote_Gui/wxNote_Dialog/InsertTableDialog.h"
#include "wxNote_Gui/wxNote_Edit/NoteEditor.h"
#include "wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h"
#include "wxNote_Gui/wxNote_Button/EliminateOrRestorePushButton.h"
#include "wxNote_Gui/wxNote_Other/FontComboBox.h"
#include "wxNote_Gui/wxNote_Other/FontSizeComboBox.h"

#include "wxNote_Item/NoteListItem.h"

#include <QApplication>
#include <QToolBar>
#include <QToolButton>
#include <QIcon>
#include <QMenu>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <QColor>
#include <QCloseEvent>
#include <QFontComboBox>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>
#include <QPushButton>
#include <QLayout>
#include <QStatusBar>
#include <QTextCodec>
#include <QDir>
#include <QActionGroup>

//.._TextEditorWindow类实现

    /* 构造函数实现 */
    _TextEditorWindow::_TextEditorWindow(QWidget *_Parent)
        : QMainWindow(_Parent),
          mb_IsChanged(false),
          mb_IsLocking(false),
          enum_ParentWindowType(wxNote::_Normal),
          enum_NoteRating(wxNote::_NonRating),
          enum_NoteCategories(wxNote::_NonCategories),
          m_IndexInTabWidget(-1)
        {
        using namespace wxNote;

        /////////////////////////////////////////////////////////////////////
        /// 读取用户设置的默认字体和默认字体大小
        ///
        g_Settings.beginGroup("TextEditor");

            m_DefaultFontFamily = g_Settings.value("DefaultFont", QString("微软雅黑")).toString();
            m_DefaultFontSize = g_Settings.value("DefaultFontSize", 8).toInt();
            mb_IsAcceptRichText = g_Settings.value("AcceptRichText").toBool();

        g_Settings.endGroup();

        /////////////////////////////////////////////////////////////////////

        m_SelectFontDialog = new QFontDialog(this);
        m_SelectFontDialog->setCurrentFont(QFont(m_DefaultFontFamily));
        connect(m_SelectFontDialog, SIGNAL(currentFontChanged(QFont)),
                this, SLOT(_FontDialogCurrentFontChangedSlot(QFont)));

        m_SelectColorDialog = new QColorDialog(this);

        m_TextEditor = new _NoteEditor;
        m_TextEditor->setAcceptRichText(mb_IsAcceptRichText);
        m_TextEditor->setFont(QFont(m_DefaultFontFamily));
        setCentralWidget(m_TextEditor);

        _CreateActionsAndSubMenu();
        _CreateWidgetsOnToolBar();
        _CreateToolBar();

        connect(m_TextEditor, SIGNAL(textChanged()),
                this, SLOT(_CurrentEditorTextChangedSlot()));
        connect(m_TextEditor, SIGNAL(cursorPositionChanged()),
                this, SLOT(_SetCurrentTextStatusSlot()));

        /* 如果该笔记被移到废纸篓, 那么由_MainWindowNormal发射该信号 */
        connect(this, SIGNAL(_CurrentNoteBeMove2TrashSignal(bool)),
                this, SLOT(_CurrentNoteBeMove2TrashSlot(bool)));
        /* 如果该笔记被还原, 那么由_MainWindowNormal发射该信号 */
        connect(this, SIGNAL(_CurrentNoteBeRestoreSignal(bool)),
                this, SLOT(_CurrentNoteBeRestroeSlot(bool)));

        setWindowTitle(tr("编辑笔记"));
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
        }

    /* _SetEliminateOrRestoreToolBarHiddewn()函数实现 */
    void _TextEditorWindow
        ::_SetEliminateOrRestoreToolBarHidden(const bool &_IsHidden)
        {
        m_EliminateOrRestoreToolBar->setHidden(_IsHidden);
        }

    /* _SetEditorInfoToolBarHidden()函数实现 */
    void _TextEditorWindow
        ::_SetEditorInfoToolBarHidden(const bool &_IsHidden)
        {
        m_EditorInfoToolBar->setHidden(_IsHidden);
        }

    /* _IsEliminateOrRestoreHidden()函数实现 */
    bool _TextEditorWindow::_IsEliminateOrRestoreHidden() const
        {
        return m_EliminateOrRestoreToolBar->isHidden();
        }

    /* _IsEditorInfoToolBarHidden()函数实现 */
    bool _TextEditorWindow::_IsEditorInfoToolBarHidden() const
        {
        return m_EditorInfoToolBar->isHidden();
        }

    /* _AddAction2_MoveNoteMenu()函数实现 */
    void _TextEditorWindow::_AddAction2_MoveNoteMenu(QAction *_NewAction)
        {
        m_MoveNoteMenu->addAction(_NewAction);
        m_MoveNoteActionGroup->addAction(_NewAction);
        }

    /* _SaveCurrentNoteSlot()槽实现 */
    void _TextEditorWindow::_SaveCurrentNoteSlot()
        {
        if (!mb_IsChanged)
            return;

        /////////////////////////////////////////////////////////////////////
        /// 如果保存笔记时发现笔记内容为空, 那么有可能是用户删除了笔记内容,但是没有删除
        /// 笔记标题,为了不让空笔记污染笔记列表, 所以当保存时, 只要发现笔记内容为空,
        /// 那么就把笔记的标题设置为空, 以便引导程序在用切换笔记本时将空的无用笔记删除.
        ///
        if (m_TextEditor->toPlainText().isEmpty())
            {
            m_NoteTitleLineEdit->clear();

            m_NoteTitleLineEdit->setStyleSheet("color:darkGray");
            m_NoteTitleLineEdit->setFont(
                        QFont("微软雅黑,12,-1,5,50,0,0,0,0,0", 15, 15, true));
            m_NoteTitleLineEdit->setPlaceholderText(tr("在此设置标题..."));

            mb_IsChanged = false;

            _SetCurrentNoteMoveEnabled(!m_NoteTitleLineEdit->text().isEmpty());
            return;
            }

        /////////////////////////////////////////////////////////////////////

        /* 当用户完成笔记编辑后, 如果笔记标题仍然为空的话, 就用正文的第一句话充当标题 */
        int _Cnt = 0;
        QString _ContentOfTextEditor = m_TextEditor->toPlainText().trimmed();
        for (const QChar& _CharElem : _ContentOfTextEditor)
            {
            ++_Cnt;
            if (_CharElem == '\n' || _CharElem == '\t' || _CharElem == '.'
                    || _CharElem == '!' || _CharElem == '?')
                break;
            }

        if (m_NoteTitleLineEdit->text().isEmpty())
            m_NoteTitleLineEdit->setText(m_TextEditor->toPlainText()
                                                .trimmed().left(_Cnt));
        /* 除去标题两头的空格 */
        m_NoteTitleLineEdit->setText(m_NoteTitleLineEdit->text().trimmed());

        /////////////////////////////////////////////////////////////////////
        /// 如果当前笔记内存在图片, 那么使用第一张图片作为该笔记项的图标
        ///
        QString _IconPath = wxNote::_ExtractFirstImagePath(m_TextEditor);

        if (!_IconPath.isEmpty())
            emit _SetCurrentNoteIconSignal(m_NoteTitleLineEdit->text(),
                                           _IconPath);

        /////////////////////////////////////////////////////////////////////

        emit _SetNoteItem_NoteName(m_NoteTitleLineEdit->text());
        emit _BeSavedSignals();

        _EmitPreviewText(m_NoteTitleLineEdit->text());

        _SetCurrentNoteMoveEnabled(!m_NoteTitleLineEdit->text().isEmpty());
        mb_IsChanged = false;   // 保存后, 将编辑器设置为未更改

        _SaveCurrentNote2File();
        }

    /* _LockCurrentNotesSlot()槽实现 */
    bool _TextEditorWindow::_LockCurrentNoteSlot()
        {
        using namespace wxNote;

        g_Settings.beginGroup("TextEditor");

            QString _Password = g_Settings.value("NotePassWord").toString();

        g_Settings.endGroup();

        if (_Password.isEmpty())
            {
            _SetPasswordDialog _SetPassword(this);
            if (_SetPassword.exec())
                {
                _SetEditorNotEnabled();
                _EmitPreviewText(m_NoteTitleLineEdit->text());

                _SaveCurrentNote2File();
                return true;
                }
            else
                return false;
            }
        else
            {
            _SetEditorNotEnabled();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            _SaveCurrentNote2File();
            return true;
            }
        }

    /* _UnlockCurrentNoteSlot()槽实现 */
    void _TextEditorWindow::_UnlockCurrentNoteSlot()
        {
        _GetPasswordDialog _GetPassword(this);

        if (_GetPassword.exec())
            {
            _SetEditorIsEnabled();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            _SaveCurrentNote2File();
            }
        }

    /* _EmitPreviewText()函数实现 */
    void _TextEditorWindow::_EmitPreviewText(const QString &_NoteTitle)
        {
        /* 对当前编辑器的文本进行简化等处理后取前400个字符作为笔记项的预览文本 */
        QString _PreviewText =
                m_TextEditor->toPlainText()
                                .trimmed()
                                .simplified()
                                .left(_PreivewTextMaximunCnt);

        emit _SetNoteListPreview(this, _NoteTitle, _PreviewText);
        }

    /* _GetBindNoteItem()函数实现 */
    _NoteListItem* _TextEditorWindow::_GetBindNoteItem() const
        {
        return m_BindNoteItem;
        }

    /* _SetBindNoteItem()函数实现 */
    void _TextEditorWindow::_SetBindNoteItem(_NoteListItem *_BindNoteItem)
        {
        m_BindNoteItem = _BindNoteItem;
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateActionsAndSubMenu()函数实现 */
    void _TextEditorWindow::_CreateActionsAndSubMenu()
        {
        m_OptionsMenu = new QMenu;

        /* "给笔记评分"子菜单和其中的QAction */
        m_RatingMenu = new QMenu(tr("给笔记评分"));

        m_RatingActionGroup = new QActionGroup(this);
        m_NonRatingAction = new QAction(tr("未打分"), this);
        m_NonRatingAction->setIcon(QIcon(":/wxNote_Icons/nonRating.png"));
        m_NonRatingAction->setShortcut(tr("Alt+0"));
        m_NonRatingAction->setCheckable(true);
        m_NonRatingAction->setChecked(true);
        connect(m_NonRatingAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteRatingIs_NonRating()));

        m_ExcellentAction = new QAction(tr("极好"), this);
        m_ExcellentAction->setIcon(QIcon(":/wxNote_Icons/excellent.png"));
        m_ExcellentAction->setShortcut(tr("Alt+5"));
        m_ExcellentAction->setCheckable(true);
        connect(m_ExcellentAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteRatingIs_Excellent()));

        m_GoodAction = new QAction(wxNote::g_GoodName, this);
        m_GoodAction->setIcon(QIcon(":/wxNote_Icons/good.png"));
        m_GoodAction->setShortcut(tr("Alt+4"));
        m_GoodAction->setCheckable(true);
        connect(m_GoodAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteRatingIs_Good()));

        m_AverageAction = new QAction(wxNote::g_AverageName, this);
        m_AverageAction->setIcon(QIcon(":/wxNote_Icons/average.png"));
        m_AverageAction->setShortcut(tr("Alt+3"));
        m_AverageAction->setCheckable(true);
        connect(m_AverageAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteRatingIs_Average()));

        m_FairAction = new QAction(wxNote::g_FairName, this);
        m_FairAction->setIcon(QIcon(":/wxNote_Icons/fair.png"));
        m_FairAction->setShortcut(tr("Alt+2"));
        m_FairAction->setCheckable(true);
        connect(m_FairAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteRatingIs_Fair()));

        m_PoorAction = new QAction(wxNote::g_PoorName, this);
        m_PoorAction->setIcon(QIcon(":/wxNote_Icons/poor.png"));
        m_PoorAction->setShortcut(tr("Alt+1"));
        m_PoorAction->setCheckable(true);
        connect(m_PoorAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteRatingIs_Poor()));

        m_RatingActionGroup->addAction(m_NonRatingAction);
        m_RatingActionGroup->addAction(m_ExcellentAction);
        m_RatingActionGroup->addAction(m_GoodAction);
        m_RatingActionGroup->addAction(m_AverageAction);
        m_RatingActionGroup->addAction(m_FairAction);
        m_RatingActionGroup->addAction(m_PoorAction);

        m_RatingMenu->addAction(m_NonRatingAction);
        m_RatingMenu->addAction(m_ExcellentAction);
        m_RatingMenu->addAction(m_GoodAction);
        m_RatingMenu->addAction(m_AverageAction);
        m_RatingMenu->addAction(m_FairAction);
        m_RatingMenu->addAction(m_PoorAction);

        /* "给笔记分类"子菜单和其中的QAction */
        m_CategoriesMenu = new QMenu(tr("设置颜色标签"));

        m_CategoriesActionGroup = new QActionGroup(this);
        m_NonCategoriesAction = new QAction(tr("未分类"), this);
        m_NonCategoriesAction->setIcon(QIcon(":/wxNote_Icons/nonCategories.png"));
        m_NonCategoriesAction->setShortcut(tr("Ctrl+0"));
        m_NonCategoriesAction->setCheckable(true);
        m_NonCategoriesAction->setChecked(true);
        connect(m_NonCategoriesAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteCategoriesIs_NonCategories()));

        m_ImportantAction = new QAction(wxNote::g_ImportantName, this);
        m_ImportantAction->setIcon(QIcon(":/wxNote_Icons/important.png"));
        m_ImportantAction->setShortcut(tr("Ctrl+1"));
        m_ImportantAction->setCheckable(true);
        connect(m_ImportantAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteCategoriesIs_Important()));

        m_WorkAction = new QAction(wxNote::g_WorkName, this);
        m_WorkAction->setIcon(QIcon(":/wxNote_Icons/work.png"));
        m_WorkAction->setShortcut(tr("Ctrl+2"));
        m_WorkAction->setCheckable(true);
        connect(m_WorkAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteCategoriesIs_Work()));

        m_PersonalAction = new QAction(wxNote::g_PersonalName, this);
        m_PersonalAction->setIcon(QIcon(":/wxNote_Icons/personal.png"));
        m_PersonalAction->setShortcut(tr("Ctrl+3"));
        m_PersonalAction->setCheckable(true);
        connect(m_PersonalAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteCategoriesIs_Personal()));

        m_ToDoAction = new QAction(wxNote::g_ToDoName, this);
        m_ToDoAction->setIcon(QIcon(":/wxNote_Icons/toDo.png"));
        m_ToDoAction->setShortcut(tr("Ctrl+4"));
        m_ToDoAction->setCheckable(true);
        connect(m_ToDoAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteCategories_ToDo()));

        m_LaterAction = new QAction(wxNote::g_LaterName, this);
        m_LaterAction->setIcon(QIcon(":/wxNote_Icons/later.png"));
        m_LaterAction->setShortcut(tr("Ctrl+5"));
        m_LaterAction->setCheckable(true);
        connect(m_LaterAction, SIGNAL(triggered()),
                this, SLOT(_SetNoteCategoriesIs_Later()));

        m_CategoriesActionGroup->addAction(m_NonCategoriesAction);
        m_CategoriesActionGroup->addAction(m_ImportantAction);
        m_CategoriesActionGroup->addAction(m_WorkAction);
        m_CategoriesActionGroup->addAction(m_PersonalAction);
        m_CategoriesActionGroup->addAction(m_ToDoAction);
        m_CategoriesActionGroup->addAction(m_LaterAction);

        m_CategoriesMenu->addAction(m_NonCategoriesAction);
        m_CategoriesMenu->addAction(m_ImportantAction);
        m_CategoriesMenu->addAction(m_WorkAction);
        m_CategoriesMenu->addAction(m_PersonalAction);
        m_CategoriesMenu->addAction(m_ToDoAction);
        m_CategoriesMenu->addAction(m_LaterAction);

        /* "移动笔记至"子菜单和其中的QAction */
        m_MoveNoteMenu = new QMenu(tr("移动笔记至"));
        m_MoveNoteMenu->setEnabled(false);

        m_MoveNoteActionGroup = new QActionGroup(this);
        connect(m_MoveNoteActionGroup, SIGNAL(triggered(QAction*)),
                this, SLOT(_NoteBooksActionBeClicked(QAction*)));

        m_AllNoteAction = new QAction(wxNote::g_AllNotesName, this);

        m_TrashAction = new QAction(wxNote::g_TrashName, this);
        m_TrashAction->setIcon(QIcon(":/wxNote_Icons/trash.png"));
        connect(m_TrashAction, SIGNAL(triggered()),
                this, SIGNAL(_MoveCurrentNote2TrashSignal()));

        m_MoveNoteActionGroup->addAction(m_AllNoteAction);
        m_MoveNoteActionGroup->addAction(m_TrashAction);

        m_MoveNoteMenu->addAction(m_AllNoteAction);
        m_MoveNoteMenu->addAction(m_TrashAction);

        m_OptionsMenu->addMenu(m_RatingMenu);
        m_OptionsMenu->addMenu(m_CategoriesMenu);
        m_OptionsMenu->addMenu(m_MoveNoteMenu);
        }

    /* _CreateToolButtonOnToolBar()函数实现 */
    void _TextEditorWindow::_CreateWidgetsOnToolBar()
        {
        /* 创建"标题编辑栏" */
        m_NoteTitleLineEdit = new _NoteTitleLineEdit;
        connect(m_NoteTitleLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(_CurrentNoteTitleChangedSlot()));

        /* 创建"选项"按钮 */
        m_OptionsToolButton = new QToolButton;
        m_OptionsToolButton->setIcon(QIcon(":/wxNote_Icons/options.png"));
        m_OptionsToolButton->setMenu(m_OptionsMenu);
        m_OptionsToolButton->setPopupMode(QToolButton::InstantPopup);

        /* 创建"消除或还原"笔记标签 */
        m_EliminateOrRestoreLabel
                = new QLabel(tr("这条笔记已被删除。你只有还原该笔记才能进行编辑。"));

        /* 创建"消除笔记"按钮 */
        m_EliminateNotePushButton = new _EliminateOrRestorePushButton;
        m_EliminateNotePushButton->setText(tr("消除笔记"));
        connect(m_EliminateNotePushButton, SIGNAL(clicked()),
                this, SIGNAL(_EliminateNoteButtonClickedSignal()));

        /* 创建"还原笔记"按钮 */
        m_RestoreNotePushButton = new _EliminateOrRestorePushButton;
        m_RestoreNotePushButton->setText(tr("还原笔记"));
        connect(m_RestoreNotePushButton, SIGNAL(clicked()),
                this, SIGNAL(_RestoreNoteButtonClickedSignal()));

        /* 创建"消除或还原"布局 */
        m_EliminateOrRestoreLayout = new QHBoxLayout;
        m_EliminateOrRestoreLayout->addWidget(m_EliminateOrRestoreLabel);
        m_EliminateOrRestoreLayout->addStretch();
        m_EliminateOrRestoreLayout->addWidget(m_EliminateNotePushButton);
        m_EliminateOrRestoreLayout->addWidget(m_RestoreNotePushButton);
        m_EliminateOrRestoreGroupWidget = new QWidget;
        m_EliminateOrRestoreGroupWidget->setLayout(m_EliminateOrRestoreLayout);

        _CreateFontComboBoxesOnToolBar();

        /* 创建"颜色"按钮 */
        m_SelectColorToolButton = new QToolButton;
        m_SelectColorToolButton->setAutoRaise(true);
        m_SelectColorToolButton->setIcon(QIcon(":/wxNote_Icons/selectColor.png"));
        m_SelectColorToolButton->setToolTip(tr("颜色"));
        connect(m_SelectColorToolButton, SIGNAL(clicked()),
                this, SLOT(_SelectColorSlot()));

        /* 创建"字体"按钮 */
        m_SelectFontToolButton = new QToolButton;
        m_SelectFontToolButton->setAutoRaise(true);
        m_SelectFontToolButton->setIcon(QIcon(":/wxNote_Icons/selectFont.png"));
        m_SelectFontToolButton->setToolTip(tr("字体"));
        connect(m_SelectFontToolButton, SIGNAL(clicked()),
                this, SLOT(_SelectFontSlot()));

        /* 创建"粗体"按钮 */
        m_BoldToolButton = new QToolButton;
        m_BoldToolButton->setAutoRaise(true);
        m_BoldToolButton->setIcon(QIcon(":/wxNote_Icons/bold.png"));
        m_BoldToolButton->setCheckable(true);
        m_BoldToolButton->setToolTip(tr("粗体"));
        connect(m_BoldToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedBoldButtonSlot(bool)));

        /* 创建"斜体"按钮 */
        m_ItalicToolButton = new QToolButton;
        m_ItalicToolButton->setAutoRaise(true);
        m_ItalicToolButton->setIcon(QIcon(":/wxNote_Icons/italic.png"));
        m_ItalicToolButton->setCheckable(true);
        m_ItalicToolButton->setToolTip(tr("斜体"));
        connect(m_ItalicToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedItalicButtonSlot(bool)));

        /* 创建"下划线"按钮 */
        m_UnderlineToolButton = new QToolButton;
        m_UnderlineToolButton->setAutoRaise(true);
        m_UnderlineToolButton->setIcon(QIcon(":/wxNote_Icons/underline.png"));
        m_UnderlineToolButton->setCheckable(true);
        m_UnderlineToolButton->setToolTip(tr("下划线"));
        connect(m_UnderlineToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedUnderlineButtonSlot(bool)));

        /* 创建"左对齐"按钮 */
        m_JustifyLeftToolButton = new QToolButton;
        m_JustifyLeftToolButton->setAutoRaise(true);
        m_JustifyLeftToolButton->setIcon(
                    QIcon(":/wxNote_Icons/format-justify-left.png"));
        m_JustifyLeftToolButton->setToolTip(tr("左对齐"));
        m_JustifyLeftToolButton->setCheckable(true);
        connect(m_JustifyLeftToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedJustifyLeftButtonSlot(bool)));

        /* 创建"居中"按钮 */
        m_JustifyCenterToolButton = new QToolButton;
        m_JustifyCenterToolButton->setAutoRaise(true);
        m_JustifyCenterToolButton->setIcon(
                    QIcon(":/wxNote_Icons/format-justify-center.png"));
        m_JustifyCenterToolButton->setToolTip(tr("居中"));
        m_JustifyCenterToolButton->setCheckable(true);
        connect(m_JustifyCenterToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedJustifyCenterButtonSlot(bool)));

        /* 创建"右对齐"按钮 */
        m_JustifyRightToolButton = new QToolButton;
        m_JustifyRightToolButton->setAutoRaise(true);
        m_JustifyRightToolButton->setIcon(
                    QIcon(":/wxNote_Icons/format-justify-right.png"));
        m_JustifyRightToolButton->setToolTip(tr("右对齐"));
        m_JustifyRightToolButton->setCheckable(true);
        connect(m_JustifyRightToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedJustifyRightButtonSlot(bool)));

        /* 创建"两端对齐"按钮 */
        m_JustifyFillToolButton = new QToolButton;
        m_JustifyFillToolButton->setAutoRaise(true);
        m_JustifyFillToolButton->setIcon(
                    QIcon(":/wxNote_Icons/format-justify-fill.png"));
        m_JustifyFillToolButton->setToolTip(tr("两端对齐"));
        m_JustifyFillToolButton->setCheckable(true);
        connect(m_JustifyFillToolButton, SIGNAL(toggled(bool)),
                this, SLOT(_ClickedJustifyFillButtonSlot(bool)));

        /* 创建"插入表格"按钮 */
        m_InsertTableToolButton = new QToolButton;
        m_InsertTableToolButton->setAutoRaise(true);
        m_InsertTableToolButton->setIcon(
                    QIcon(":/wxNote_Icons/insertTable.png"));
        m_InsertTableToolButton->setToolTip(tr("插入表格"));
        connect(m_InsertTableToolButton, SIGNAL(clicked()),
                this, SLOT(_InsertTableSlot()));

        /* 创建"无序列表"按钮 */
        m_UnorderedListToolButton = new QToolButton;
        m_UnorderedListToolButton->setAutoRaise(true);
        m_UnorderedListToolButton->setIcon(
                    QIcon(":/wxNote_Icons/unorderedList.png"));
        m_UnorderedListToolButton->setToolTip(tr("无序列表"));
        connect(m_UnorderedListToolButton, SIGNAL(clicked()),
                this, SLOT(_InsertUnorderedListSlot()));

        /* 创建"有序列表"按钮 */
        m_OrderedListToolButton = new QToolButton;
        m_OrderedListToolButton->setAutoRaise(true);
        m_OrderedListToolButton->setIcon(
                    QIcon(":/wxNote_Icons/orderedList.png"));
        m_OrderedListToolButton->setToolTip(tr("有序列表"));
        connect(m_OrderedListToolButton, SIGNAL(clicked()),
                this, SLOT(_InsertOrderedListSlot()));

        /* 创建"复选框"按钮 */
        m_CheckBoxToolButton = new QToolButton;
        m_CheckBoxToolButton->setAutoRaise(true);
        m_CheckBoxToolButton->setIcon(
                    QIcon(":/wxNote_Icons/checkBox.png"));
        m_CheckBoxToolButton->setToolTip(tr("插入复选框"));
        connect(m_CheckBoxToolButton, SIGNAL(clicked()),
                this, SLOT(_InsertCheckBoxSlot()));

        /* 创建"锁定"按钮 */
        m_LockCurrentTextEditToolButton = new QToolButton;
        m_LockCurrentTextEditToolButton->setAutoRaise(true);
        m_LockCurrentTextEditToolButton->setIcon(
                    QIcon(":/wxNote_Icons/notlock.png"));
        m_LockCurrentTextEditToolButton->setToolTip(tr("未锁定"));
        connect(m_LockCurrentTextEditToolButton, SIGNAL(clicked()),
                this, SLOT(_ClickedLockedButtonSlot()));

        /* 创建"颜色"标签 */
        m_ColorLabel = new QLabel("<img src=\":/wxNote_Icons/nonCategories.png\">");

        m_ColorLabelLayout = new QHBoxLayout;
        m_ColorLabelLayout->addStretch();
        m_ColorLabelLayout->addWidget(m_ColorLabel);

        m_CategoriesWidget = new QWidget;
        m_CategoriesWidget->setLayout(m_ColorLabelLayout);
        }

    /* _CreateToolBar()函数实现 */
    void _TextEditorWindow::_CreateToolBar()
        {
        m_EditorInfoToolBar = new QToolBar;
        m_EditorInfoToolBar->setFloatable(false);
        m_EditorInfoToolBar->setMovable(false);
        m_EditorInfoToolBar->setAllowedAreas(Qt::TopToolBarArea);
        m_EditorInfoToolBar->addWidget(m_NoteTitleLineEdit);
        m_EditorInfoToolBar->addWidget(m_OptionsToolButton);

        m_EliminateOrRestoreToolBar = new QToolBar;
        m_EliminateOrRestoreToolBar->setStyleSheet("background-color : rgb(242, 237, 218)");

        m_EliminateOrRestoreToolBar->setHidden(true);
        m_EliminateOrRestoreToolBar->setIconSize(QSize(25, 25));
        m_EliminateOrRestoreToolBar->setFloatable(false);
        m_EliminateOrRestoreToolBar->setMovable(false);
        m_EliminateOrRestoreToolBar->setAllowedAreas(Qt::TopToolBarArea);
        m_EliminateOrRestoreToolBar->addWidget(m_EliminateOrRestoreGroupWidget);

        m_EditorOptionsToolBar = new QToolBar;
        m_EditorOptionsToolBar->setIconSize(QSize(20, 20));
        m_EditorOptionsToolBar->setFloatable(false);
        m_EditorOptionsToolBar->setMovable(false);
        m_EditorOptionsToolBar->setAllowedAreas(Qt::TopToolBarArea);

        m_EditorOptionsToolBar->addWidget(m_FontComboBox);
        m_EditorOptionsToolBar->addWidget(m_FontSizeComboBox);
        m_EditorOptionsToolBar->addSeparator();

        m_EditorOptionsToolBar->addWidget(m_SelectColorToolButton);
        m_EditorOptionsToolBar->addWidget(m_SelectFontToolButton);

        m_EditorOptionsToolBar->addSeparator();

        m_EditorOptionsToolBar->addWidget(m_BoldToolButton);
        m_EditorOptionsToolBar->addWidget(m_ItalicToolButton);
        m_EditorOptionsToolBar->addWidget(m_UnderlineToolButton);

        m_EditorOptionsToolBar->addSeparator();

        m_EditorOptionsToolBar->addWidget(m_JustifyLeftToolButton);
        m_EditorOptionsToolBar->addWidget(m_JustifyCenterToolButton);
        m_EditorOptionsToolBar->addWidget(m_JustifyRightToolButton);
        m_EditorOptionsToolBar->addWidget(m_JustifyFillToolButton);

        m_EditorOptionsToolBar->addSeparator();

        m_EditorOptionsToolBar->addWidget(m_InsertTableToolButton);
        m_EditorOptionsToolBar->addWidget(m_UnorderedListToolButton);
        m_EditorOptionsToolBar->addWidget(m_OrderedListToolButton);
        m_EditorOptionsToolBar->addWidget(m_CheckBoxToolButton);

        m_EditorOptionsToolBar->addSeparator();

        m_EditorOptionsToolBar->addWidget(m_LockCurrentTextEditToolButton);

        addToolBar(m_EditorInfoToolBar);
        this->addToolBarBreak(Qt::TopToolBarArea);

        addToolBar(m_EliminateOrRestoreToolBar);
        this->addToolBarBreak(Qt::TopToolBarArea);

        addToolBar(m_EditorOptionsToolBar);
        }

    /* _CreateFontComboBoxesOnToolBar()函数实现 */
    void _TextEditorWindow::_CreateFontComboBoxesOnToolBar()
        {
        /* "创建字体选择框" */
        m_FontComboBox = new _FontComboBox;
        m_FontComboBox->setFont(wxNote::g_StandardFont);
        m_FontComboBox->setCurrentFont(QFont(m_DefaultFontFamily));
        connect(m_FontComboBox, SIGNAL(currentTextChanged(QString)),
                this, SLOT(_SetCurrentFontByFontComboBox(QString)));

        /* "创建字体大小"对话框 */
        m_FontSizeComboBox = new _FontSizeComboBox;
        m_FontSizeComboBox->setFont(wxNote::g_StandardFont);
        m_FontSizeComboBox->setEditable(true);
        connect(m_FontSizeComboBox, SIGNAL(currentTextChanged(QString)),
                this, SLOT(_SetCurrentFontSizeByFontSizeComboBox(QString)));

        for (int _Index = _FontMinSize; _Index <= _FontMaxSize; _Index++)
                m_FontSizeComboBox->addItem(QString::number(_Index), _Index);

        m_FontSizeComboBox->setCurrentText(QString::number(m_DefaultFontSize));
        }

    /* _SetCurrentFontByFontComboBox()槽实现 */
    void _TextEditorWindow
        ::_SetCurrentFontByFontComboBox(const QString &_FontFamily)
        {
        m_FontComboBox->setCurrentText(_FontFamily);
        m_TextEditor->setCurrentFont(QFont(_FontFamily));

        m_TextEditor->setFocus();
        }

    /* _SetCurrentFontSizeByFontSizeComboBox()槽实现 */
    void _TextEditorWindow
        ::_SetCurrentFontSizeByFontSizeComboBox(const QString &_Text)
        {
        int _Size = _Text.toInt();

        int _CurrentPixelSize = m_TextEditor->currentFont().pixelSize();
        int _CurrentPointSize = m_TextEditor->currentFont().pointSize();

        QFont _NewFont(m_TextEditor->currentFont().family());

        if (_CurrentPixelSize >= 0)
            _NewFont.setPixelSize(_Size);

        else if (_CurrentPointSize >= 0)
            _NewFont.setPointSize(_Size);

        m_TextEditor->setCurrentFont(_NewFont);
        m_TextEditor->setFocus();
        }

    /* _FontDialogCurrentFontChangedSlot()槽实现 */
    void _TextEditorWindow
        ::_FontDialogCurrentFontChangedSlot(const QFont &_CurrentFont)
        {
        m_FontComboBox->setCurrentFont(_CurrentFont);
        }

    /* _SelectColorSlot()槽实现 */
    void _TextEditorWindow::_SelectColorSlot()
        {
        QColor _SelectedColor;

        if (m_SelectColorDialog->exec())
            _SelectedColor = m_SelectColorDialog->selectedColor();
        else
            return;

        m_TextEditor->setTextColor(_SelectedColor);
        }

    /* _SelectFontSlot()槽实现 */
    void _TextEditorWindow::_SelectFontSlot()
        {
        m_SelectFontDialog->setCurrentFont(m_FontComboBox->currentFont());

        if (m_SelectFontDialog->exec())
            m_EditorCurrentFont = m_SelectFontDialog->selectedFont();
        else
            return;

        m_TextEditor->setFont(m_EditorCurrentFont);
        }

    /* _ClickedBoldButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedBoldButtonSlot(bool _IsOK)
        {
        if (_IsOK)
            m_TextEditor->setFontWeight(QFont::Bold);
        else
            m_TextEditor->setFontWeight(QFont::Normal);
        }

    /* _ClickedItalicButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedItalicButtonSlot(bool _IsOK)
        {
        m_TextEditor->setFontItalic(_IsOK);
        }

    /* _ClickedUnderlineButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedUnderlineButtonSlot(bool _IsOK)
        {
        m_TextEditor->setFontUnderline(_IsOK);
        }

    /* _ClickedJustifyLeftButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedJustifyLeftButtonSlot(bool _IsOK)
        {
        if (_IsOK)
            {
            m_TextEditor->setAlignment(Qt::AlignLeft);

            m_JustifyCenterToolButton->setChecked(!_IsOK);
            m_JustifyRightToolButton->setChecked(!_IsOK);
            m_JustifyFillToolButton->setChecked(!_IsOK);
            }
        }

    /* _ClickedJustifyCenterButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedJustifyCenterButtonSlot(bool _IsOK)
        {
        if (_IsOK)
            {
            m_TextEditor->setAlignment(Qt::AlignCenter);

            m_JustifyLeftToolButton->setChecked(!_IsOK);
            m_JustifyRightToolButton->setChecked(!_IsOK);
            m_JustifyFillToolButton->setChecked(!_IsOK);
            }
        }

    /* _ClickedJustifyRightButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedJustifyRightButtonSlot(bool _IsOK)
        {
        if (_IsOK)
            {
            m_TextEditor->setAlignment(Qt::AlignRight);

            m_JustifyLeftToolButton->setChecked(!_IsOK);
            m_JustifyCenterToolButton->setChecked(!_IsOK);
            m_JustifyFillToolButton->setChecked(!_IsOK);
            }
        }

    /* _ClickedJustifyFillButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedJustifyFillButtonSlot(bool _IsOK)
        {
        if (_IsOK)
            {
            m_TextEditor->setAlignment(Qt::AlignJustify);

            m_JustifyLeftToolButton->setChecked(!_IsOK);
            m_JustifyCenterToolButton->setChecked(!_IsOK);
            m_JustifyRightToolButton->setChecked(!_IsOK);
            }
        }

    /* _ClickedLockedButtonSlot()槽实现 */
    void _TextEditorWindow::_ClickedLockedButtonSlot()
        {
        if (m_LockCurrentTextEditToolButton->toolTip() == tr("未锁定"))
            {
            _LockCurrentNoteSlot();
            return;
            }

        if (m_LockCurrentTextEditToolButton->toolTip() == tr("已锁定"))
            {
            _UnlockCurrentNoteSlot();
            return;
            }
        }

    /* _CurrentNoteTitleChangedSlot()槽实现 */
    void _TextEditorWindow::_CurrentNoteTitleChangedSlot()
        {
        mb_IsChanged = true; //  当编笔记标题发生改变时, 设置mb_IsChanged为已更改
        }

    /* _CurrentEditorTextChangedSlot()槽实现 */
    void _TextEditorWindow::_CurrentEditorTextChangedSlot()
        {
        if (m_TextEditor->toPlainText().isEmpty())
            {
            m_FontComboBox->setCurrentText(m_DefaultFontFamily);
            m_FontSizeComboBox->setCurrentText(QString::number(m_DefaultFontSize));
            }

        mb_IsChanged = true; //  当编辑器文本发生改变时, 设置mb_IsChanged为已更改
        }

    /* _CurrentNoteBeMove2TrashSlot()槽实现 */
    void _TextEditorWindow::_CurrentNoteBeMove2TrashSlot(bool _IsBeDeleted)
        {
        m_EliminateOrRestoreToolBar->setVisible(_IsBeDeleted);
        m_EditorOptionsToolBar->setHidden(_IsBeDeleted);

        m_TextEditor->setReadOnly(_IsBeDeleted);
        m_NoteTitleLineEdit->setReadOnly(_IsBeDeleted);
        }

    /* _CurrentNoteBeRestoreSlot()槽实现 */
    void _TextEditorWindow::_CurrentNoteBeRestroeSlot(bool _IsBeDeleted)
        {
        m_EliminateOrRestoreToolBar->setHidden(_IsBeDeleted);
        m_EditorOptionsToolBar->setVisible(_IsBeDeleted);

        m_TextEditor->setReadOnly(!_IsBeDeleted);
        m_NoteTitleLineEdit->setReadOnly(!_IsBeDeleted);
        }

    /* _NoteBooksActionBeClicked()槽实现() */
    void _TextEditorWindow::_NoteBooksActionBeClicked(QAction *_TriggeredAction)
        {
        if (_TriggeredAction)
            {
            QString _ActionText = _TriggeredAction->text();

            if (!_ActionText.isEmpty() && (_ActionText != wxNote::g_TrashName))
                emit _CurrentNoteBeMove2OtherNoteBookSignal(_ActionText);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_TriggeredAction)
        }

    /* _SetNoteRatingIs_NonRating()槽实现 */
    void _TextEditorWindow::_SetNoteRatingIs_NonRating()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteRating = wxNote::_NonRating;

            emit _CurrentNoteRatingChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Rating_Current = QString();

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteRatingIs_Excellent()槽实现 */
    void _TextEditorWindow::_SetNoteRatingIs_Excellent()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteRating = wxNote::_Excellent;

            emit _CurrentNoteRatingChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Rating_Current = wxNote::g_ExcellentName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteRatingIs_Good()槽实现 */
    void _TextEditorWindow::_SetNoteRatingIs_Good()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteRating = wxNote::_Good;

            emit _CurrentNoteRatingChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Rating_Current = wxNote::g_GoodName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteRatingIs_Average()槽实现 */
    void _TextEditorWindow::_SetNoteRatingIs_Average()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteRating = wxNote::_Average;

            emit _CurrentNoteRatingChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Rating_Current = wxNote::g_AverageName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteRatingIs_Fair()槽实现 */
    void _TextEditorWindow::_SetNoteRatingIs_Fair()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteRating = wxNote::_Fair;

            emit _CurrentNoteRatingChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Rating_Current = wxNote::g_FairName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteRatingIs_Poor()槽实现 */
    void _TextEditorWindow::_SetNoteRatingIs_Poor()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteRating = wxNote::_Poor;

            emit _CurrentNoteRatingChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Rating_Current = wxNote::g_PoorName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteCategoriesIs_NonCategories()槽实现 */
    void _TextEditorWindow::_SetNoteCategoriesIs_NonCategories()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteCategories = wxNote::_NonCategories;

            emit _CurrentNoteCategoriesChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Categories_Current = QString();

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteCategoriesIs_Important()槽实现 */
    void _TextEditorWindow::_SetNoteCategoriesIs_Important()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteCategories = wxNote::_Important;

            emit _CurrentNoteCategoriesChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Categories_Current = wxNote::g_ImportantName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteCategoriesIs_Work()槽实现 */
    void _TextEditorWindow::_SetNoteCategoriesIs_Work()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteCategories = wxNote::_Work;

            emit _CurrentNoteCategoriesChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Categories_Current = wxNote::g_WorkName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteCategoriesIs_Personal()槽实现 */
    void _TextEditorWindow::_SetNoteCategoriesIs_Personal()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteCategories = wxNote::_Personal;

            emit _CurrentNoteCategoriesChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Categories_Current = wxNote::g_PersonalName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteCategoriesIs_ToDo()槽实现 */
    void _TextEditorWindow::_SetNoteCategories_ToDo()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteCategories = wxNote::_ToDo;

            emit _CurrentNoteCategoriesChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Categories_Current = wxNote::g_ToDoName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetNoteCategoriesIs_Later()槽实现 */
    void _TextEditorWindow::_SetNoteCategoriesIs_Later()
        {
        if (!m_TextEditor->toPlainText().isEmpty())
            {
            enum_NoteCategories = wxNote::_Later;

            emit _CurrentNoteCategoriesChanged();
            _EmitPreviewText(m_NoteTitleLineEdit->text());

            m_Categories_Current = wxNote::g_LaterName;

            _SaveCurrentNote2File();
            }
        }

    /* _SetCurrentNoteMoveEnabled()函数实现 */
    void _TextEditorWindow::_SetCurrentNoteMoveEnabled(bool _IsOK)
        {
        m_MoveNoteMenu->setEnabled(_IsOK);
        }

    /* _InsertTableSlot()槽实现 */
    void _TextEditorWindow::_InsertTableSlot()
        {
        _InsertTableDialog _Dialog(this);

        int _RowCnt = 0;
        int _ColumnCnt = 0;
        int _TableWidth = 100;

        if (_Dialog.exec())
            {
            _RowCnt = _Dialog._GetRowCnt();
            _ColumnCnt = _Dialog._GetColumnCnt();
            _TableWidth = _Dialog._GetTableWidth();

            int _LoopCnt = 0;

            if (_RowCnt && _ColumnCnt)
                {
                QString _TableHTML(wxNote::g_TableHTML_Prefix.arg(_TableWidth));

                for (int _Index = 0; _Index < _RowCnt * _ColumnCnt; _Index++)
                    {
                    if (_LoopCnt % _ColumnCnt)
                        _TableHTML.push_back(wxNote::g_TableHTML_Content);
                    else
                        _TableHTML.push_back("<tr>" + wxNote::g_TableHTML_Content);

                    ++_LoopCnt;
                    }

                m_TextEditor->textCursor().insertHtml(_TableHTML);
                }
            }
        }

    /* _InsertUnorderedListSlot()槽实现 */
    void _TextEditorWindow::_InsertUnorderedListSlot()
        {
        m_TextEditor->textCursor()
                .insertHtml(wxNote::g_UnorderedListHTML_Prefix
                                + wxNote::g_UnorderedListHTML_Content);
        }

    /* _InsertOrderedListSlot()槽实现 */
    void _TextEditorWindow::_InsertOrderedListSlot()
        {
        m_TextEditor->textCursor()
                .insertHtml(wxNote::g_OrderedListHTML_Prefix
                                + wxNote::g_OrderedListHTML_Content);
        }

    /* _InsertCheckBoxSlot()槽实现 */
    void _TextEditorWindow::_InsertCheckBoxSlot()
        {
        m_TextEditor->textCursor().insertHtml(wxNote::g_CheckBoxHTML);
        }

    /* _SetCurrentTextStatusSlot()槽实现
     * 当光标位置改变时, 改变任务栏中各种状态按钮的开关状态 */
    void _TextEditorWindow::_SetCurrentTextStatusSlot()
        {
        /* 更新字体类按钮的选中状态 */
        m_FontComboBox->setCurrentText(m_TextEditor->currentFont().family());
        m_SelectColorDialog->setCurrentColor(m_TextEditor->textColor());

        int _FontPixelSize = m_TextEditor->currentFont().pixelSize();
        int _FontPointSize = m_TextEditor->currentFont().pointSize();

        if (_FontPixelSize >= 0)
            m_FontSizeComboBox->setCurrentText(QString::number(_FontPixelSize));
        else if (_FontPointSize >= 0)
            m_FontSizeComboBox->setCurrentText(QString::number(_FontPointSize));

        m_BoldToolButton->setChecked(
                    m_TextEditor->fontWeight() == QFont::Bold ? true : false);

        m_ItalicToolButton->setChecked(m_TextEditor->fontItalic());

        m_UnderlineToolButton->setChecked(m_TextEditor->fontUnderline());

        /* 更新对齐方式类按钮的选中状态 */
        switch (m_TextEditor->alignment())
            {
        case Qt::AlignLeft:
            m_JustifyLeftToolButton->setChecked(true);
            break;

        case Qt::AlignCenter:
            m_JustifyCenterToolButton->setChecked(true);
            break;

        case Qt::AlignRight:
            m_JustifyRightToolButton->setChecked(true);
            break;

        case Qt::AlignJustify:
            m_JustifyFillToolButton->setChecked(true);
            break;
            }
        }

    /* event()事件重写 */
    bool _TextEditorWindow::event(QEvent *_Event)
        {
        QEvent::Type _EventType = _Event->type();
        switch (_EventType)
            {
        case QEvent::Leave:
            _SaveCurrentNoteSlot();
            return true;

        default:
            QMainWindow::event(_Event);
            return true;
            }

        return false;
        }

    /* keyPressEvet()函数重写 */
    void _TextEditorWindow::keyPressEvent(QKeyEvent *_Event)
        {
        switch(_Event->key())
            {
        case Qt::Key_S:
            if (_Event->modifiers() & Qt::ControlModifier)
                _SaveCurrentNoteSlot();

        default:
            QMainWindow::keyPressEvent(_Event);
            }
        }

    /* _SetToolButtonCheckable()函数实现 */
    void _TextEditorWindow::_SetEditorNotEnabled()
        {
        m_TextEditor->setReadOnly(true);
        m_LockCurrentTextEditToolButton->setToolTip(tr("已锁定"));
        m_LockCurrentTextEditToolButton->setIcon(
                    QIcon(":/wxNote_Icons/locking.png"));

        m_BoldToolButton->setCheckable(false);
        m_ItalicToolButton->setCheckable(false);
        m_UnderlineToolButton->setCheckable(false);
        m_JustifyLeftToolButton->setCheckable(false);
        m_JustifyCenterToolButton->setCheckable(false);
        m_JustifyRightToolButton->setCheckable(false);
        m_JustifyFillToolButton->setCheckable(false);

        m_NoteTitleLineEdit->setReadOnly(true);

        mb_IsLocking = true;

        _SaveCurrentNoteSlot();
        }

    /* _SetToolButtonNotCheckable()函数实现 */
    void _TextEditorWindow::_SetEditorIsEnabled()
        {
        m_TextEditor->setReadOnly(false);
        m_LockCurrentTextEditToolButton->setToolTip(tr("未锁定"));
        m_LockCurrentTextEditToolButton->setIcon(
                    QIcon(":/wxNote_Icons/notlock.png"));

        m_BoldToolButton->setCheckable(true);
        m_ItalicToolButton->setCheckable(true);
        m_UnderlineToolButton->setCheckable(true);
        m_JustifyLeftToolButton->setCheckable(true);
        m_JustifyCenterToolButton->setCheckable(true);
        m_JustifyRightToolButton->setCheckable(true);
        m_JustifyFillToolButton->setCheckable(true);

        m_NoteTitleLineEdit->setReadOnly(false);

        mb_IsLocking = false;
        }

    /* _SimplifyLongTitle()函数实现 */
    QString _TextEditorWindow::_SimplifyLongTitle(const QString &_Title)
        {
        if (_Title.count() >= _SoLongTitleStd)
            return QString(_Title.left(4) + "..." + _Title.right(4));
        else
            return _Title;
        }

    /* _SetJustifyLeftButtonChecked()函数实现 */
    void _TextEditorWindow::_SetJustifyLeftButtonChecked()
        {
        m_JustifyLeftToolButton->setChecked(true);
        }

    /* _SetJustifyCenterButtonChecked()函数实现 */
    void _TextEditorWindow::_SetJustifyCenterButtonChecked()
        {
        m_JustifyCenterToolButton->setChecked(true);
        }

    /* _SetJustifyRightButtonChecked()函数实现 */
    void _TextEditorWindow::_SetJustifyRightButtonChecked()
        {
        m_JustifyRightToolButton->setChecked(true);
        }

    /* _SetJustifyFillButtonChecked()函数实现 */
    void _TextEditorWindow::_SetJustifyFillButtonChecked()
        {
        m_JustifyFillToolButton->setChecked(true);
        }

    /* _SaveCurrentNote2File()函数实现 */
    void _TextEditorWindow::_SaveCurrentNote2File()
        {
        wxNote::_InitializeNoteBooks();

        QString _CurrentNoteBookName = _GetParentNoteBookName_Current();
        QString _CurrentNoteTitle = m_NoteTitleLineEdit->text();

        QString _NoteName = tr("%1.~_%2.~_%3.~_%4.~_%5.~_%6.~_%7")
                                                        .arg(_CurrentNoteBookName)
                                                        .arg(_CurrentNoteTitle)
                                                        .arg(int(mb_IsLocking))
                                                        .arg(int(enum_NoteCategories))
                                                        .arg(int(enum_NoteRating))
                                                        .arg(m_CreateDate.toString(Qt::ISODate))
                                                        .arg(m_CreateTime.toString("hh-mm-ss"));

        QFile _OutFile(tr("%1/%2/%3.html").arg(wxNote::g_LocalFilePath)
                                          .arg((_CurrentNoteBookName == wxNote::g_AllNotesName)
                                                                        ? QString()
                                                                        : _CurrentNoteBookName)
                                          .arg(_NoteName));
        if (_OutFile.open(QFile::WriteOnly))
            {
            QTextStream _Cout(&_OutFile);
            _Cout.setCodec(QTextCodec::codecForLocale());

            QString _NoteContent = m_TextEditor->toHtml();

            _Cout << _NoteContent << endl;
            }

        emit _DeleteNonMatchedNoteFileSignal(_CurrentNoteBookName);
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
