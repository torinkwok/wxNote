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
#include "wxNote_Gui/wxNote_Window/MainWindowNormal.h"
#include "wxNote_Gui/wxNote_Dialog/CommonDialog.h"
#include "wxNote_Gui/wxNote_Dialog/GetNameForNewNotebook.h"
#include "wxNote_Gui/wxNote_Dialog/OptionsDialog.h"
#include "wxNote_Gui/wxNote_Button/wxNote_ToolButton.h"
#include "wxNote_Gui/wxNote_Edit/NoteEditor.h"
#include "wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h"
#include "wxNote_Gui/wxNote_List/NoteList.h"
#include "wxNote_Gui/wxNote_Tree/NoteBookTree.h"
#include "wxNote_Gui/wxNote_Tree/CategoriesTree.h"
#include "wxNote_Gui/wxNote_Other/NoteEditorSplitter.h"
#include "wxNote_Gui/wxNote_Other/FontComboBox.h"
#include "wxNote_Gui/wxNote_Other/FontSizeComboBox.h"

#include "wxNote_Item/NoteListItem.h"
#include "wxNote_Global.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QCheckBox>
#include <QToolBar>
#include <QLayout>
#include <QCloseEvent>
#include <QDate>
#include <QTime>
#include <QInputDialog>
#include <QStackedLayout>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QComboBox>
#include <QClipboard>
#include <QDir>
#include <QTextCodec>
#include <QLinearGradient>

#include <algorithm>

//.._MainWindowNormal类实现

    /* 构造函数实现 */
    _MainWindowNormal::_MainWindowNormal(QWidget *_Parent)
        : _AbstractMainWindow(_Parent),
          m_NoteStatus(_NormalStatus),
          m_MatchNoteItemDye(QColor(215, 168, 189), Qt::Dense1Pattern)
        {
        wxNote::_InitializeGlobalFilePath();
        QStringList _Names = wxNote::_InitializeNoteBooks();

        for (const QString& _Elem : _Names)
            wxNote::g_NoteBookNameActionList
                        .push_back(new QAction(_Elem, this));

        _CreateActionsAndSubMenu();
        _CreateToolButtonOnToolBar();
        _CreateMainWinMenus();
        _CreateMainWinToolBar();
        /* _CreateContextMenu(); */

        m_TextEditorStackedLayout = new QStackedLayout;
        m_EditorPanel = new QWidget;

        _Create_wxNoteTabWidget();

        _CreateNoteList();
        _CreateSearchPanel();

        m_RightSplitter = new _NoteEditorSplitter(Qt::Vertical);

        /* m_EditorPanel存在的目的只是为了能够顺利的把
         * _TextEditorStackedLayout放入m_CenterSplitter, 仅此而已 */
        m_EditorPanel->setLayout(m_TextEditorStackedLayout);

        m_NoteListSplitter = new _NoteEditorSplitter(Qt::Vertical);
        m_NoteListSplitter->addWidget(m_SearchPanel);
        m_NoteListSplitter->addWidget(m_NoteList);

        m_CenterSplitter = new _NoteEditorSplitter(Qt::Horizontal);
        m_CenterSplitter->addWidget(m_NoteListSplitter);
        m_CenterSplitter->addWidget(m_EditorPanel);
        m_CenterSplitter->setStretchFactor(1, 1);

        m_MainSplitter = new _NoteEditorSplitter(Qt::Horizontal);
        m_MainSplitter->addWidget(m_wxNoteTabWidget);
        m_MainSplitter->addWidget(m_CenterSplitter);
        m_MainSplitter->setStretchFactor(1, 1);

        setCentralWidget(m_MainSplitter);
        setWindowTitle(tr("%1 - wxNote")
                       .arg(m_NoteBookTree->currentItem()->text(0)));
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
        _ReadSettings();

        _RestoreUserNoteFile();
        }

    /* _GetOwnershipFromShadowWindow()函数实现 */
    void _MainWindowNormal
        ::_GetOwnershipFromShadowWindow(_TextEditorWindow *_ShadowWin)
        {
        if (_ShadowWin)
            {
            QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

            QList<_NoteListItem *> _Notes =
                    m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentNoteBookName,
                                                             m_wxNoteTabWidget);

            for (int _Index = 0; _Index < _Notes.count(); _Index++)
                {
                _TextEditorWindow* _CurrentNoteItemBindEW =
                                    _Notes.at(_Index)->_GetBindTextEW();

                if (_CurrentNoteItemBindEW == _ShadowWin)
                    {
                    m_NoteList->setCurrentRow(_Index);
                    m_TextEditorStackedLayout->setCurrentIndex(_Index);

                    break;
                    }
                }

            _NoteEditorSplitter* _CurrentSplitter =
                    dynamic_cast<_NoteEditorSplitter *>(
                            m_TextEditorStackedLayout->currentWidget());

            _CurrentSplitter->addWidget(_ShadowWin);
            m_TextEditorStackedLayout->setCurrentWidget(_CurrentSplitter);

            _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();

            /* 设置当前笔记的编辑器焦点 */
            if (_CurrentNoteItem)
                wxNote::_GetEWFromGlobalList_BySpecifiedItem(
                            _CurrentNoteItem)->_GetTextEditor()->setFocus();
            else
                __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)

            ////////////////////////////////////////////////////////////
            /// 选中从shadow窗口切换回来的编辑窗口所在的笔记本
            ///
            QString _CurrentNoteParentNoteBook =
                            _CurrentNoteItem->_GetParentNoteBookName();

            _PitchOnSpecifiedNoteBook(_CurrentNoteParentNoteBook);

            ////////////////////////////////////////////////////////////
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_ShadowWin)
        }

    /* _GetFirstNoteInSpecifiedNoteBook()函数实现 */
    _NoteListItem* _MainWindowNormal
        ::_GetFirstNoteInSpecifiedNoteBook(const QString &_NoteBookName)
        {
        _NoteListItem* _FirstNoteItem =
                m_NoteList->_GetNotesInSpecifiedNoteBook(_NoteBookName,
                                                         m_wxNoteTabWidget).at(0);

        return _FirstNoteItem;
        }

    /* _OptionsSlot()槽重写 */
    void _MainWindowNormal::_OptionsSlot()
        {
        using namespace wxNote;

        _OptionsDialog _Option(this);

        connect(_Option._GetAcceptRichTextCheckBox(), SIGNAL(toggled(bool)),
                this, SLOT(_SetAcceptRichTextSlot(bool)));

        if (_Option.exec())
            {
            QString _DefaultFontFamily
                    = _Option._GetFontComboBox()->currentFont().family();
            int     _DefaultFontSize
                    = _Option._GetFontSizeComboBox()->currentText().toInt();

            for (_TextEditorWindow* _Elem : wxNote::g_AllTextEditorList)
                {
                _Elem->_GetFontComboBox()->setCurrentFont(QFont(_DefaultFontFamily));
                _Elem->_GetFontSizeComboBox()->setCurrentText(QString::number(_DefaultFontSize));
                }

            g_Settings.beginGroup("TextEditor");

                g_Settings.setValue("LocalFilePath", _Option._GetLocalFilePathLineEdit()->text());

                g_Settings.setValue("DefaultFont", _DefaultFontFamily);
                g_Settings.setValue("DefaultFontSize", _DefaultFontSize);
                g_Settings.setValue("AcceptRichText", _Option._GetAcceptRichTextCheckBox()->isChecked());

            g_Settings.endGroup();

            if (_Option._GetIsChanged())
                {
                _CommonDialog _WarningDialog(this
                                             , tr("wxNote")
                                             , tr("<font color=darkred><B>wxNote的默认设置已被更改。</font></B>"
                                                "<p>有一些设置必须重启wxNote才能生效。")
                                             , tr("好的"));
                _WarningDialog.exec();
                }
            }
        }

    /* _UndoSlot()槽重写 */
    void _MainWindowNormal::_UndoSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            _CurrentNoteEditor->undo();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _RedoSlot()槽重写 */
    void _MainWindowNormal::_RedoSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            _CurrentNoteEditor->redo();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _CutSlot()槽重写 */
    void _MainWindowNormal::_CutSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            _CurrentNoteEditor->cut();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _CopySlot()槽重写 */
    void _MainWindowNormal::_CopySlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            _CurrentNoteEditor->copy();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _PasteSlot()槽实现 */
    void _MainWindowNormal::_PasteSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            _CurrentNoteEditor->paste();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _PastePlainTextSlot()槽重写 */
    void _MainWindowNormal::_PastePlainTextSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            QTextCursor _TextCursor = _CurrentNoteEditor->textCursor();
            _TextCursor.insertText(qApp->clipboard()->text());
            _TextCursor.movePosition(QTextCursor::End);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _DeleteSlot()槽实现 */
    void _MainWindowNormal::_DeleteSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();

            _CurrentNoteEditor->textCursor().deleteChar();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _SelectAllSlot()槽实现 */
    void _MainWindowNormal::_SelectAllSlot()
        {
        _TextEditorWindow* _CurrentEditorWindow
                        = _GetCurrentTextEditorWindow();

        if (_CurrentEditorWindow)
            {
            _NoteEditor* _CurrentNoteEditor
                    = _CurrentEditorWindow->_GetTextEditor();
            _CurrentNoteEditor->selectAll();
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)
        }

    /* _CurrentNoteNameChangedSlot()槽实现 */
    void _MainWindowNormal::_CurrentNoteNameChangedSlot(const QString &_Title)
        {
        _NoteTitleLineEdit* _CurrentNoteTitleLineEdit =
                       dynamic_cast<_NoteTitleLineEdit *>(sender());

        _TextEditorWindow* _CurrentEditorWindow = nullptr;

        if (_CurrentNoteTitleLineEdit)
            for (_TextEditorWindow* _Elem : wxNote::g_AllTextEditorList)
                if (_Elem->_GetTitleLineEdit() == _CurrentNoteTitleLineEdit)
                    {
                    emit _CurrentTitleLineEditTextChangedSignal(_Elem, _Title);

                    _CurrentEditorWindow = _Elem;
                    break;
                    }

        if (_CurrentEditorWindow != nullptr)
            _CurrentEditorWindow->_GetBindNoteItem()->_SetTitle(_Title.isEmpty()
                                                                    ? wxNote::g_NonTitleNoteName
                                                                    : _Title);
        }

    /* closeEvent()函数重写 */
    void _MainWindowNormal::closeEvent(QCloseEvent *_Event)
        {
        _WriteSettings();
        _Event->accept();
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _RestoreUserNoteFile()函数实现 */
    void _MainWindowNormal::_RestoreUserNoteFile()
        {
        using namespace wxNote;

        QDir _MainDir(g_LocalFilePath);
        QStringList _DirOrFileNames = _MainDir.entryList();

        std::for_each(_DirOrFileNames.begin() + 2, _DirOrFileNames.end(),
                      [this](const QString& _Elem)
                        {
                        /* 如果当前_Elem是一个目录的名称(其他笔记本)... */
                        if (!_Elem.contains(wxNote::g_NoteNameSplitSymbol))
                            {
                            QString _NoteBookPath = _GetSpecifiedNoteBookPath(_Elem);
                            QDir _NoteBookDir(_NoteBookPath);

                            QStringList _NoteNames = _NoteBookDir.entryList();

                            std::for_each(_NoteNames.begin() + 2, _NoteNames.end(),
                                          [this, &_NoteBookPath](const QString& _NoteElem)
                                            {
                                            _RestoreSpecifiedNoteFile(_NoteBookPath,
                                                                      _NoteElem);
                                            });
                            }
                        else    /* 否则就是一个笔记名称(全部笔记)... */
                            _RestoreSpecifiedNoteFile(g_LocalFilePath, _Elem);
                        });

        /* 选中当前笔记本中的第一个笔记项 */
        _NoteListItem* _FirstNoteInCurrentNoteBook =
                _GetFirstNoteInSpecifiedNoteBook(m_NoteBookTree->currentItem()->text(0));

        if (_FirstNoteInCurrentNoteBook)
            m_NoteList->setCurrentItem(_FirstNoteInCurrentNoteBook);
        }

    /* _RestoreSpecifiedNoteFile()函数实现 */
    void _MainWindowNormal
        ::_RestoreSpecifiedNoteFile(const QString &_NoteBookPath,
                                    const QString &_NoteFileName)
        {
        QString _NoteFilePath = tr("%1/%2").arg(_NoteBookPath)
                                           .arg(_NoteFileName);

        QStringList _Splited = _NoteFileName.split(wxNote::g_NoteNameSplitSymbol);

        QFile _NoteFile(_NoteFilePath);
        if (_NoteFile.open(QFile::ReadOnly))
            {
            QTextStream _Cin(&_NoteFile);
            QString _NoteContent(_Cin.readAll());

            QString _NoteBookName = _Splited.at(0);
            QString _CurrentNoteTitle = _Splited.at(1);

            bool _IsLocking = bool(_Splited.at(2).toInt());

            wxNote::_NoteCategories _NoteCategories =
                    wxNote::_NoteCategories(_Splited.at(3).toInt());
            wxNote::_NoteRating _NoteRating =
                    wxNote::_NoteRating(_Splited.at(4).toInt());

            QStringList _CreateDateSplited = _Splited.at(5).split('-');
            int _Year = _CreateDateSplited.at(0).toInt();
            int _Month = _CreateDateSplited.at(1).toInt();
            int _Day = _CreateDateSplited.at(2).toInt();
            QDate _CreateDate(_Year, _Month, _Day);

            QStringList _CreateTimeSplited = _Splited.at(6).split('-');
            int _Hour = _CreateTimeSplited.at(0).toInt();
            int _Minute = _CreateTimeSplited.at(1).toInt();
            int _Second = _CreateTimeSplited.at(2).toInt();
            QTime _CreateTime(_Hour, _Minute, _Second);

            QPair<_NoteListItem *, _TextEditorWindow *> _RetPair = _NewNoteSlot();

            _NoteListItem* _CurrentNoteItem = _RetPair.first;
            _TextEditorWindow* _CurrentEditorWindow = _RetPair.second;

            _CurrentNoteItem->_SetParentNoteBookName(_NoteBookName);
            _CurrentNoteItem->_SetBeforeDeletedParentNoteBookName(_NoteBookName);

            _CurrentEditorWindow->_SetParentNoteBookName_Current(_NoteBookName);
            _CurrentEditorWindow->_SetParentNoteBookName_BeforeDeleted(_NoteBookName);

            _CurrentEditorWindow->_GetTitleLineEdit()->setText(_CurrentNoteTitle);
            if (_IsLocking)
                _CurrentEditorWindow->_LockCurrentNoteSlot(true);

            _CurrentEditorWindow->_SetCreateDate(_CreateDate);
            _CurrentEditorWindow->_SetCreateTime(_CreateTime);

            _CurrentNoteItem->_SetCreateDate(_CreateDate);
            _CurrentNoteItem->_SetCreateTime(_CreateTime);

            _CurrentEditorWindow->_GetTextEditor()->setHtml(_NoteContent);

            _CurrentEditorWindow->_SetNoteCategories(_NoteCategories, true);
            _CurrentEditorWindow->_SetNoteRating(_NoteRating, true);
            _CurrentEditorWindow->_SaveCurrentNoteSlot(true);
            }
        }

    /* _CreateActionsAndSubMenu()函数重写 */
    void _MainWindowNormal::_CreateActionsAndSubMenu()
        {
        m_NewNoteSubMenuOnToolButton = new QMenu;

        /* "文件"菜单项中的QMenu或QAction的创建和设置 */
        m_NewNoteAction = new QAction(tr("新建笔记(&N)"), this);
        m_NewNoteAction->setShortcut(QKeySequence::New);
        connect(m_NewNoteAction, SIGNAL(triggered()),
                this, SLOT(_NewNoteSlot()));

        m_NewNoteBookAction = new QAction(tr("新建笔记本(&B)"), this);
        m_NewNoteBookAction->setShortcut(tr("Insert"));
        connect(m_NewNoteBookAction, SIGNAL(triggered()),
                this, SLOT(_NewNoteBookSlot()));

        m_NewNoteSubMenuOnToolButton->addAction(m_NewNoteBookAction);

        /* "查看"菜单项中的QMenu或QAction的创建和设置 */
        m_DisplayLeftPanelAction = new QAction(tr("显示左面板(&H)"), this);
        m_DisplayLeftPanelAction->setCheckable(true);
        m_DisplayLeftPanelAction->setShortcut(tr("F10"));
        connect(m_DisplayLeftPanelAction, SIGNAL(toggled(bool)),
                this, SLOT(_SetLeftPanelIsHidden(bool)));

        m_DisplayNoteListAction = new QAction(tr("显示笔记列表(&O)"), this);
        m_DisplayNoteListAction->setCheckable(true);
        m_DisplayNoteListAction->setShortcut(tr("F11"));
        connect(m_DisplayNoteListAction, SIGNAL(toggled(bool)),
                this, SLOT(_SetNoteListIsHidden(bool)));

        m_DisplayNotePanelAction = new QAction(tr("显示笔记面板(&W)"), this);
        m_DisplayNotePanelAction->setCheckable(true);
        m_DisplayNotePanelAction->setShortcut(tr("Ctrl+F11"));
        connect(m_DisplayNotePanelAction, SIGNAL(toggled(bool)),
                this, SLOT(_SetNotePanelIsHidden(bool)));

        /* "笔记"菜单项中的QMenu或QAction的创建和设置 */
        m_DeletedNoteBookAction = new QAction(tr("删除笔记本(&N)"), this);
        m_DeletedNoteBookAction->setEnabled(false);
        m_DeletedNoteBookAction->setShortcut(tr("Ctrl+Shift+Delete"));
        connect(m_DeletedNoteBookAction, SIGNAL(triggered()),
                this, SLOT(_ThoroughDeleteNoteBookSlot()));

        m_OneKeyLockAction = new QAction(tr("一键锁定(&K)"), this);
        m_OneKeyLockAction->setEnabled(false);
        m_OneKeyLockAction->setShortcut(tr("Ctrl+Alt+K"));
        connect(m_OneKeyLockAction, SIGNAL(triggered()),
                this, SLOT(_LockAllNotesSlot()));

        /* 将normal窗口中的"编辑"菜单中的QAction全部设置为不可用,
         * 然后由用户的操作状态决定是否解禁 */
        QList<QAction *> _ActionsInEditMenu = m_EditMenu->actions();
        std::for_each(_ActionsInEditMenu.begin(), _ActionsInEditMenu.end(),
                      [](QAction* _Elem)
                            {
                            _Elem->setEnabled(false);
                            });
        }

    /* _CreateMainWinMenus()函数重写 */
    void _MainWindowNormal::_CreateMainWinMenus()
        {
        m_FileMenu->addAction(m_NewNoteAction);
        m_FileMenu->addAction(m_NewNoteBookAction);

        m_ViewMenu->addAction(m_DisplayLeftPanelAction);
        m_ViewMenu->addAction(m_DisplayNoteListAction);
        m_ViewMenu->addAction(m_DisplayNotePanelAction);

        m_NoteMenu->addAction(m_DeletedNoteBookAction);
        m_NoteMenu->addSeparator();
        m_NoteMenu->addAction(m_OneKeyLockAction);
        }

    /* _CreateToolButtonOnToolBar()函数重写 */
    void _MainWindowNormal::_CreateToolButtonOnToolBar()
        {
        /* "新建笔记"按钮 */
        m_NewNoteToolButton = new _wxNote_ToolButton;
        m_NewNoteToolButton->setMenu(m_NewNoteSubMenuOnToolButton);
        m_NewNoteToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        m_NewNoteToolButton->setText(tr("新建笔记"));
        m_NewNoteToolButton->setToolTip(tr("新建笔记"));
        m_NewNoteToolButton->setIcon(
                    QIcon(":/wxNote_Icons/newNote.png"));
        connect(m_NewNoteToolButton, SIGNAL(clicked()),
                this, SLOT(_NewNoteSlot()));

        /* "删除"按钮 */
        m_TrashToolButton = new _wxNote_ToolButton;
        m_TrashToolButton->setEnabled(false);
        m_TrashToolButton->setMenu(m_DeleteNoteSubMenu);
        m_TrashToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        m_TrashToolButton->setText(tr("删除笔记"));
        m_TrashToolButton->setToolTip(tr("删除选择的笔记"));
        m_TrashToolButton->setIcon(
                    QIcon(":/wxNote_Icons/trash.png"));
        connect(m_TrashToolButton, SIGNAL(clicked()),
                this, SLOT(_MoveNote2TrashSlot()));

        /* "一键锁定"按钮 */
        m_OneKeyLockToolButton = new _wxNote_ToolButton;
        m_OneKeyLockToolButton->setEnabled(false);
        m_OneKeyLockToolButton->setText(tr("一键锁定"));
        m_OneKeyLockToolButton->setToolTip(tr("一键锁定全部笔记"));
        m_OneKeyLockToolButton->setIcon(QIcon(":/wxNote_Icons/oneKeyLock.png"));
        connect(m_OneKeyLockToolButton, SIGNAL(clicked()),
                this, SLOT(_LockAllNotesSlot()));
        }

    /* _CreateContextMenu()函数实现 */
    void _MainWindowNormal::_CreateContextMenu()
        {
        // TODO
        }

    /* _CreateMainWinToolBar()函数重写 */
    void _MainWindowNormal::_CreateMainWinToolBar()
        {
        m_MainWinToolBar->addWidget(m_NewNoteToolButton);
        m_MainWinToolBar->addWidget(m_TrashToolButton);

        m_MainWinToolBar->addSeparator();

        m_MainWinToolBar->addWidget(m_EliminateToolButton);
        m_MainWinToolBar->addWidget(m_RestoreNoteToolButton);

        m_MainWinToolBar->addSeparator();

        m_MainWinToolBar->addWidget(m_OneKeyLockToolButton);
        }

    /* _WriteSettings()函数实现 */
    void _MainWindowNormal::_WriteSettings()
        {
        using namespace wxNote;

        g_Settings.beginGroup("MainWindow");

            g_Settings.setValue("Geometry", saveGeometry());
            g_Settings.setValue("RightSplitterStatus", m_RightSplitter->saveState());
            g_Settings.setValue("RightSplitterGeometry", m_RightSplitter->saveGeometry());
            g_Settings.setValue("CenterSplitterStatus", m_CenterSplitter->saveState());
            g_Settings.setValue("CenterSplitterGeometry", m_CenterSplitter->saveGeometry());
            g_Settings.setValue("MainSplitterStatus", m_MainSplitter->saveState());
            g_Settings.setValue("MainSplitterGeometry", m_MainSplitter->saveGeometry());

            g_Settings.setValue("IsLeftPanelActionIsChecked", m_DisplayLeftPanelAction->isChecked());
            g_Settings.setValue("IsNoteListActionChecked", m_DisplayNoteListAction->isChecked());
            g_Settings.setValue("IsNotePanelIsHidden", m_DisplayNotePanelAction->isChecked());

            g_Settings.setValue("UserNoteBookNames", g_UserNoteBookNameList);

        g_Settings.endGroup();
        }

    /* _ReadSettings()函数实现 */
    void _MainWindowNormal::_ReadSettings()
        {
        using namespace wxNote;

        g_Settings.beginGroup("MainWindow");

            restoreGeometry(g_Settings.value("Geometry").toByteArray());
            m_RightSplitter->restoreState(
                        g_Settings.value("RightSplitterStatus").toByteArray());
            m_RightSplitter->restoreState(
                        g_Settings.value("RightSplitterGeometry").toByteArray());
            m_CenterSplitter->restoreState(
                        g_Settings.value("CenterSplitterStatus").toByteArray());
            m_CenterSplitter->restoreGeometry(
                        g_Settings.value("CenterSplitterGeometry").toByteArray());
            m_MainSplitter->restoreState(
                        g_Settings.value("MainSplitterStatus").toByteArray());
            m_MainSplitter->restoreGeometry(
                        g_Settings.value("MainSplitterGeometry").toByteArray());

            bool _IsLeftPanelHiddenActionChecked = g_Settings
                                                    .value("IsLeftPanelActionIsChecked", true)
                                                    .toBool();
            bool _IsNoteListHiddenActionChecked = g_Settings
                                                    .value("IsNoteListActionChecked", true)
                                                    .toBool();
            bool _IsNotePanelHiddenActionChecked = g_Settings
                                                    .value("IsNotePanelIsHidden", true)
                                                    .toBool();
            m_DisplayLeftPanelAction->setChecked(_IsLeftPanelHiddenActionChecked);
            m_DisplayNoteListAction->setChecked(_IsNoteListHiddenActionChecked);
            m_DisplayNotePanelAction->setChecked(_IsNotePanelHiddenActionChecked);

            _SetLeftPanelIsHidden(_IsLeftPanelHiddenActionChecked);
            _SetNoteListIsHidden(_IsNoteListHiddenActionChecked);
            _SetNotePanelIsHidden(_IsNotePanelHiddenActionChecked);

        g_Settings.endGroup();
        }

    /* _SetLeftPanelIsHidden()槽实现 */
    void _MainWindowNormal::_SetLeftPanelIsHidden(bool _IsHidden)
        {
        m_wxNoteTabWidget->setHidden(!_IsHidden);
        }

    /* _SetNoteListIsHidden()槽实现 */
    void _MainWindowNormal::_SetNoteListIsHidden(bool _IsHidden)
        {
        m_NoteListSplitter->setHidden(!_IsHidden);
        }

    /* _SetNotePanelIsHidden()槽实现 */
    void _MainWindowNormal::_SetNotePanelIsHidden(bool _IsHidden)
        {
        m_EditorPanel->setHidden(!_IsHidden);
        }

    /* _Create_wxNoteTabWidget()函数实现 */
    void _MainWindowNormal::_Create_wxNoteTabWidget()
        {
        _CreateNoteBookTree();
        _CreateCategoriesTree();

        m_wxNoteTabWidget = new QTabWidget;
        m_wxNoteTabWidget->addTab(m_NoteBookTree, wxNote::g_NoteBooksListName);
        m_wxNoteTabWidget->addTab(m_CategoriesTree, wxNote::g_CategoriesListName);

        connect(m_wxNoteTabWidget, SIGNAL(currentChanged(int)),
                this, SLOT(_wxNote_TabWidgetChangedSlot(int)));
        }

    /* _CreateNoteBookTree()函数实现 */
    void _MainWindowNormal::_CreateNoteBookTree()
        {
        m_NoteBookTree = new _NoteBookTree(this);

        connect(m_NoteBookTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                this, SLOT(_CurrentNoteBookItemChangedSlot(QTreeWidgetItem*, QTreeWidgetItem*)));

        connect(m_NoteBookTree, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
                this, SLOT(_CurrentNoteBookChangedSlot_WindowTitle()));
        }

    /* _CreateCategoriesTree()函数实现 */
    void _MainWindowNormal::_CreateCategoriesTree()
        {
        m_CategoriesTree = new _CategoriesTree(this);

        connect(m_CategoriesTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                this, SLOT(_CurrentCategoriesItemChangedSlot(QTreeWidgetItem*,QTreeWidgetItem*)));
        }

    /* _IsCurrentNoteListAllHidden()函数实现 */
    bool _MainWindowNormal::_IsCurrentNoteListAllHidden()
        {
        QString _CurrentTabText =
                m_wxNoteTabWidget->tabText(m_wxNoteTabWidget->currentIndex());

        QList<_NoteListItem *> _Notes;

        if (_CurrentTabText == wxNote::g_NoteBooksListName)
            {
            QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

            _Notes = m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentNoteBookName,
                                                              m_wxNoteTabWidget);
            }
        else if (_CurrentTabText == wxNote::g_CategoriesListName)
            {
            QString _CurrentItemText = m_CategoriesTree->currentItem()->text(0);

            if (_CurrentItemText == wxNote::g_ExcellentName)
                _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Excellent);

            else if (_CurrentItemText == wxNote::g_GoodName)
                _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Good);

            else if (_CurrentItemText == wxNote::g_AverageName)
                _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Average);

            else if (_CurrentItemText == wxNote::g_FairName)
                _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Fair);

            else if (_CurrentItemText == wxNote::g_PoorName)
                _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Poor);

            else if (_CurrentItemText == wxNote::g_ImportantName)
                _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Important);

            else if (_CurrentItemText == wxNote::g_WorkName)
                _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Work);

            else if (_CurrentItemText == wxNote::g_PersonalName)
                _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Personal);

            else if (_CurrentItemText == wxNote::g_ToDoName)
                _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_ToDo);

            else if (_CurrentItemText == wxNote::g_LaterName)
                _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Later);
            }

        QList<bool> _VisibleNoteList;

        for (const _NoteListItem* _Elem : _Notes)
            _VisibleNoteList.push_back(_Elem->isHidden());

        bool b_IsAllHidden =
                std::all_of(_VisibleNoteList.begin(), _VisibleNoteList.end(),
                            [](const bool& _Elem)
                                {
                                return _Elem;
                                });
        return b_IsAllHidden;
        }

    /* _ClearEmptyNote()函数实现 */
    void _MainWindowNormal::_ClearEmptyNote(const QString &_NoteBookName)
        {
        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            QString _CurrentNoteTitle =
                    m_NoteList->_GetSpecifiedItemTitle(_Index);

            if (_CurrentNoteTitle == wxNote::g_NonTitleNoteName)
                {
                _NoteListItem* _CurrentNoteItem = m_NoteList->_Item(_Index);

                /* 如果该笔记存在shadow窗口, 那么再删除它的同时将它的shadow窗口关闭 */
                emit _CloseCurrentNoteShadowWindowSignal(_CurrentNoteItem->_GetBindTextEW());

                m_TextEditorStackedLayout->setCurrentIndex(_Index);

                QWidget* _CurrentTextEditorWindow =
                                m_TextEditorStackedLayout->currentWidget();

                _EraseNoteItemFromDeletedList(_CurrentNoteItem);

                _EraseSpecifiedEditorWindowFromGlobalList(_CurrentNoteItem);

                delete _CurrentNoteItem;
                delete _CurrentTextEditorWindow;

                _CurrentNoteItem = nullptr;
                _CurrentTextEditorWindow = nullptr;

                break;
                }
            }

        _RestoreLastPitchOnItem_NoteBookTree(_NoteBookName);

        QListWidgetItem* _NewCurrentItem = m_NoteList->currentItem();

        if (_NewCurrentItem)
            {
            int _NewCurrentIndex = m_NoteList->_GetIndexFromItem(_NewCurrentItem).row();

            m_TextEditorStackedLayout->setCurrentIndex(_NewCurrentIndex);
            }

        _SetDeleteNoteEnabled();
        _SetNoteEditEnabled();
        _SetOneKeyLockEnabled();
        }

    /* _EraseDeletedNoteItem()函数实现 */
    bool _MainWindowNormal
        ::_EraseNoteItemFromDeletedList(const QListWidgetItem *_DeletedItem)
        {
        QList<_NoteListItem *>::iterator _DeletedIter =
                std::remove_if(m_DeletedNoteList.begin(), m_DeletedNoteList.end(),
                               [this, _DeletedItem](const _NoteListItem* _Elem)
                                    {
                                    return _Elem == _DeletedItem;
                                    });

        /* 将被删除的笔记项的指针从_DletedNoteList中移除 */
        if (_DeletedIter != m_DeletedNoteList.end())
            {
            *_DeletedIter = nullptr;
            m_DeletedNoteList.erase(_DeletedIter, m_DeletedNoteList.end());
            return true;
            }

        return false;
        }

    /*_EraseSpecifiedEditorWindowFromGlobalList()函数实现　*/
    void _MainWindowNormal
        ::_EraseSpecifiedEditorWindowFromGlobalList(QListWidgetItem *_NoteItem)
        {
        auto _Iter =
            std::remove_if(wxNote::g_AllTextEditorList.begin(), wxNote::g_AllTextEditorList.end(),
                           [this, _NoteItem](const _TextEditorWindow* _Elem)->bool
                                {
                                _NoteListItem* __CurrentNoteItem =
                                            dynamic_cast<_NoteListItem *>(_NoteItem);

                                 return __CurrentNoteItem->_GetBindTextEW() == _Elem;
                                 });

        if (_Iter != wxNote::g_AllTextEditorList.end())
            {
            *_Iter = nullptr;
            wxNote::g_AllTextEditorList.erase(_Iter, wxNote::g_AllTextEditorList.end());
            }
        }

    /* _EraseCurrentEditorWindowFromGlobalList()函数实现 */
    void _MainWindowNormal::_EraseCurrentEditorWindowFromGlobalList()
        {
        QListWidgetItem* _CurrentNoteItem = m_NoteList->currentItem();

        _EraseSpecifiedEditorWindowFromGlobalList(_CurrentNoteItem);
        }

    /* _SpecifyNoteBookCnt()函数实现
     * 如果给定笔记本不存在或给定笔记本为空, 返回0, 否则返回给定笔记本中的笔记数 */
    int _MainWindowNormal::_SpecifyNoteBookCnt(const QString &_NoteBookName)
        {
        int _NoteCnt = 0;

        /* 如果当前笔记本是"全部笔记"或"笔记本",
         * 那么将返回全部未在废纸篓中的笔记的总数 */
        if (_NoteBookName == wxNote::g_AllNotesName
                || _NoteBookName == wxNote::g_NoteBooksName)
            {
            for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
                {
                _NoteListItem* _CurrentNoteItem = m_NoteList->_Item(_Index);

                if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName)
                    ++_NoteCnt;
                }

            return _NoteCnt;
            }

        /* 如果是别的笔记本... */
        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem =
                    dynamic_cast<_NoteListItem *>(m_NoteList->item(_Index));

            if (_NoteBookName == _CurrentNoteItem->_GetParentNoteBookName())
                ++_NoteCnt;
            }

        return _NoteCnt ? _NoteCnt : 0;
        }

    /* _CurrentNoteBookCnt()函数实现 */
    int _MainWindowNormal::_CurrentNoteBookCnt()
        {
        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

        return _SpecifyNoteBookCnt(_CurrentNoteBookName);
        }

    /* _SetDeleteNoteButton_Action_MenuEnabled()函数实现 */
    void _MainWindowNormal::_SetDeleteNoteButton_Action_MenuEnabled(bool _IsOK)
        {
        m_DeleteNoteSubMenu->setEnabled(_IsOK);
        m_TrashToolButton->setEnabled(_IsOK);
        }

    /* _SetDeleteNoteBookAction_MenuEnabled()函数实现 */
    void _MainWindowNormal::_SetDeleteNoteBookAction_MenuEnabled(bool _IsOK)
        {
        m_DeletedNoteBookAction->setEnabled(_IsOK);
        }

    /* _SetEliminateButton_ActionEnabled()函数实现 */
    void _MainWindowNormal::_SetEliminateButton_ActionEnabled(bool _IsOK)
        {
        m_EliminateAction->setEnabled(_IsOK);
        m_EliminateToolButton->setEnabled(_IsOK);
        }

    /* _SetRestoreButton_ActionEnabled()函数实现 */
    void _MainWindowNormal::_SetRestoreButton_ActionEnabled(bool _IsOK)
        {
        m_RestoreNoteAction->setEnabled(_IsOK);
        m_RestoreNoteToolButton->setEnabled(_IsOK);
        }

    /* _SetNewNoteButtton_ActionEnabled()函数实现 */
    void _MainWindowNormal::_SetNewNoteButton_ActionEnabled(bool _IsOK)
        {
        m_NewNoteAction->setEnabled(_IsOK);
        m_NewNoteToolButton->setEnabled(_IsOK);
        }

    /* _SetOneKeyLockButton_ActionEnabled()函数实现 */
    void _MainWindowNormal::_SetOneKeyLockButton_ActionEnabled(bool _IsOK)
        {
        m_OneKeyLockAction->setEnabled(_IsOK);
        m_OneKeyLockToolButton->setEnabled(_IsOK);
        }

    /* _SetNoteEditActionsEnabled()函数实现 */
    void _MainWindowNormal::_SetNoteEditActionsEnabled(bool _IsOK)
        {
        m_UndoAction->setEnabled(_IsOK);
        m_RedoAction->setEnabled(_IsOK);
        m_CutAction->setEnabled(_IsOK);
        m_CopyAction->setEnabled(_IsOK);
        m_PasteAction->setEnabled(_IsOK);
        m_PastePlainTextAction->setEnabled(_IsOK);
        m_DeleteAction->setEnabled(_IsOK);
        m_SelectAllAction->setEnabled(_IsOK);;
        }

    /* _SetDeleteNoteEnabled()函数实现 */
    void _MainWindowNormal::_SetDeleteNoteEnabled()
        {
        int _wxNoteTabWidgetIndex = m_wxNoteTabWidget->currentIndex();

        QString _CurrentTabText =
                m_wxNoteTabWidget->tabText(_wxNoteTabWidgetIndex);
        QString _CurrentLabelText =
                m_CategoriesTree->currentItem()->text(0);

        bool _IsCurrentCategoriesIsEmpty =
                m_NoteList->_GetNotesInSpecifiedNoteBook(
                        _CurrentLabelText, m_wxNoteTabWidget).isEmpty();

        /* 只有在不在废纸篓下, 并且当前笔记本不为空的时候才能解禁"删除按钮和动作" */
        _SetDeleteNoteButton_Action_MenuEnabled(
                    m_NoteBookTree->currentItem()->text(0) != wxNote::g_TrashName
                        && ((_CurrentTabText == wxNote::g_CategoriesListName)
                                                ? !_IsCurrentCategoriesIsEmpty
                                                : true)
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_NoteBooksName
                        && _SpecifyNoteBookCnt(m_NoteBookTree->currentItem()->text(0)));
        }

    /* _SetDeleteNoteBookEnabled()函数实现 */
    void _MainWindowNormal::_SetDeleteNoteBookEnabled()
        {
        int _wxNoteTabWidgetIndex = m_wxNoteTabWidget->currentIndex();

        QString _CurrentTabText =
                m_wxNoteTabWidget->tabText(_wxNoteTabWidgetIndex);

        /* 只有在拥有可删除笔记本时, 才会解禁 */
        _SetDeleteNoteBookAction_MenuEnabled(
                    !wxNote::g_UserNoteBookNameList.isEmpty()
                        && _CurrentTabText == wxNote::g_NoteBooksListName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_NoteBooksName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_AllNotesName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_TrashName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_LabelName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_PropertiesName);
        }

    /* _SetEliminateNoteEnabled()函数实现 */
    void _MainWindowNormal::_SetEliminateNoteEnabled()
        {
        /* 只有在"废纸篓"下并且废纸篓非空的情况下才能解禁"消除笔记"按钮和动作 */
        _SetEliminateButton_ActionEnabled(
                    m_NoteBookTree->currentItem()->text(0) == wxNote::g_TrashName
                    && _SpecifyNoteBookCnt(wxNote::g_TrashName));
        }

    /* _SetRestoreNoteEnabled()函数实现 */
    void _MainWindowNormal::_SetRestoreNoteEnabled()
        {
        /* 只有在"废纸篓"下并且废纸篓非空的情况下才能解禁"还原笔记"按钮和动作 */
        _SetRestoreButton_ActionEnabled(
                    m_NoteBookTree->currentItem()->text(0) == wxNote::g_TrashName
                    && _SpecifyNoteBookCnt(wxNote::g_TrashName));
        }

    /* _SetNewNoteEnabled()函数实现 */
    void _MainWindowNormal::_SetNewNoteEnabled()
        {
        QString _CurrentTabText =
                m_wxNoteTabWidget->tabText(m_wxNoteTabWidget->currentIndex());

        /* 只有不在废纸篓下才能解禁"新建笔记"按钮和动作 */
        _SetNewNoteButton_ActionEnabled(
                    _CurrentTabText == wxNote::g_NoteBooksListName
                    && m_NoteBookTree->currentItem()->text(0) != wxNote::g_NoteBooksName
                    && m_NoteBookTree->currentItem()->text(0) != wxNote::g_TrashName
                    && m_NoteBookTree->currentItem()->text(0) != wxNote::g_LabelName
                    && m_NoteBookTree->currentItem()->text(0) != wxNote::g_PropertiesName);
        }

    /* _SetOneKeyLockEnabled()函数实现 */
    void _MainWindowNormal::_SetOneKeyLockEnabled()
        {
        /* 只有在普通笔记本下并且笔记本列表不为空时才能解禁"新建笔记"按钮和动作 */
        _SetOneKeyLockButton_ActionEnabled(
                    (m_NoteBookTree->currentItem()->text(0) != wxNote::g_TrashName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_LabelName
                        && m_NoteBookTree->currentItem()->text(0) != wxNote::g_PropertiesName)
                            && _SpecifyNoteBookCnt(m_NoteBookTree->currentItem()->text(0)));
        }

    /* _SetNoteEditEnabled()函数实现 */
    void _MainWindowNormal::_SetNoteEditEnabled()
        {
        QTreeWidgetItem* _CurrentNoteBook = m_NoteBookTree->currentItem();
        QString _CurrentNoteBookName = _CurrentNoteBook->text(0);

        _SetNoteEditActionsEnabled(
                    _CurrentNoteBookName != wxNote::g_TrashName
                        && _CurrentNoteBookName != wxNote::g_LabelName
                        && _CurrentNoteBookName != wxNote::g_PropertiesName
                        && _SpecifyNoteBookCnt(m_NoteBookTree->currentItem()->text(0)));
        }

    /* _MoveNoteFile2OtherOne()函数实现 */
    bool _MainWindowNormal::_MoveNoteFile2OtherOne(const QString &_FromNoteBook,
                                                   const QString &_FileName,
                                                   const QString &_2NoteBook)
        {
        if (_2NoteBook == wxNote::g_TrashName)
            {
            QString _TrashPath = _GetSpecifiedNoteBookPath(wxNote::g_TrashName);

            QDir _TrashDir(_TrashPath);
            if (!_TrashDir.exists())
                _TrashDir.mkpath(_TrashPath);
            }

        QString _OldPath = tr("%1/%2").arg(_GetSpecifiedNoteBookPath(_FromNoteBook))
                                      .arg(_FileName);

        QStringList _Splited = _FileName.split(wxNote::g_NoteNameSplitSymbol);
        _Splited[0] = _2NoteBook;

        QString _NewName = _Splited.join(wxNote::g_NoteNameSplitSymbol);

        bool _IsSuccessful =
                QFile::copy(_OldPath, tr("%1/%2").arg(_GetSpecifiedNoteBookPath(_2NoteBook))
                                                 .arg(_NewName));
        if (_IsSuccessful)
            {
            QFile::remove(_OldPath);

            return true;
            }

        return false;
        }

    /* _wxNote_TabWidgetChangedSlot()槽实现 */
    void _MainWindowNormal::_wxNote_TabWidgetChangedSlot(int _TabIndex)
        {
        _SetNewNoteEnabled();
        _SetDeleteNoteEnabled();
        _SetDeleteNoteBookEnabled();

        QString _CurrentTabText = m_wxNoteTabWidget->tabText(_TabIndex);

        if (_CurrentTabText == wxNote::g_CategoriesListName)
            {
            QString _CurrentLabelText = m_CategoriesTree->currentItem()->text(0);

            _FilterNote_inCategoriesTree(_CurrentLabelText);
            }

        else if (_CurrentTabText == wxNote::g_NoteBooksListName)
            {
            QString _CurrentItemText = m_NoteBookTree->currentItem()->text(0);

            if (_CurrentItemText == wxNote::g_NoteBooksName
                    || _CurrentItemText == wxNote::g_AllNotesName)
                {
                _SetNotesAllVisible_ExceptTrash();

                m_NoteList->_AdjustNoteItemDye(_CurrentItemText, m_wxNoteTabWidget);
                return;
                }

            //////////////////////////////////////////////////////////////////
            /// 当从其他标签切换到"笔记本列表"标签时正确显示相应笔记本中的笔记
            /// 并隐藏其他笔记本中的笔记.
            ///
            QList<_NoteListItem *> _Notes =
                        m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentItemText,
                                                                 m_wxNoteTabWidget);

            _SetNotesAllHidden();

            for (_NoteListItem* _Elem : _Notes)
                {
                int _CurrentItemIndex = m_NoteList->_GetIndexFromItem(_Elem).row();

                _Elem->setHidden(false);

                m_TextEditorStackedLayout->setCurrentIndex(_CurrentItemIndex);
                m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                }

            m_NoteList->_AdjustNoteItemDye(_Notes);

            //////////////////////////////////////////////////////////////////

            _NoteListItem* _FirstNoteItem =
                    _GetFirstNoteInSpecifiedNoteBook(_CurrentItemText);

            if (_FirstNoteItem)
                m_NoteList->setCurrentItem(_FirstNoteItem);
            }
        }

    /* _CurrentNoteBookItemChangedSlot()槽实现 */
    void _MainWindowNormal::_CurrentNoteBookItemChangedSlot(QTreeWidgetItem */*_Current*/,
                                                            QTreeWidgetItem */*_Previous*/)
        {
        _ClearSearchLineEdit();

        _SetDeleteNoteEnabled();
        _SetDeleteNoteBookEnabled();
        _SetEliminateNoteEnabled();
        _SetRestoreNoteEnabled();
        _SetNewNoteEnabled();
        _SetOneKeyLockEnabled();
        _SetNoteEditEnabled();

        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

        if (!_SpecifyNoteBookCnt(_CurrentNoteBookName))
            {
            _SetNotesAllHidden();

            return;
            }

        m_NoteList->_AdjustNoteItemDye(_CurrentNoteBookName,
                                       m_wxNoteTabWidget);

        _ClearEmptyNote(_CurrentNoteBookName);

        /* 当用户选中的是"全部笔记"或"笔记本"时, 则取消对所有笔记项的隐藏 */
        if (_CurrentNoteBookName == wxNote::g_AllNotesName || _CurrentNoteBookName == wxNote::g_NoteBooksName)
            {
            for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
                {
                _NoteListItem* _CurrentNoteItem
                        = dynamic_cast<_NoteListItem *>(m_NoteList->item(_Index));

                if (!_CurrentNoteItem)
                    {
                    __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)

                    return;
                    }

                if (_CurrentNoteItem->_GetParentNoteBookName() == wxNote::g_TrashName)
                    m_NoteList->item(_Index)->setHidden(true);
                else
                    m_NoteList->item(_Index)->setHidden(false);
                }

            /* 如果当前笔记中没有被选中的笔记项, 那么将第一个item设为选中项 */
            if (!_CheckSpecifiedNoteBookHasSelectItem(_CurrentNoteBookName))
                m_NoteList->setCurrentItem(
                            _GetFirstNoteInSpecifiedNoteBook(_CurrentNoteBookName));
            else /* 否则获取当前笔记本中最后一次被选中的笔记项... */
                _RestoreLastPitchOnItem_NoteBookTree(_CurrentNoteBookName);

            return;
            }

        /* 当用户选中的是"标签"或"属性"时, 忽略之 */
        if (_CurrentNoteBookName == wxNote::g_LabelName
                || _CurrentNoteBookName == wxNote::g_PropertiesName)
            return;

        /* 只在"废纸篓"中才显示被删除的笔记, 在别的笔记本中不显示 */
        if (m_NoteBookTree->currentItem() && m_NoteList->currentItem())
            {
            if (m_NoteBookTree->currentItem()->text(0) == wxNote::g_TrashName)
                {
                for (_NoteListItem* _Elem : m_DeletedNoteList)
                    if (_Elem)
                        _Elem->setHidden(false);

                _RestoreLastPitchOnItem_NoteBookTree(wxNote::g_TrashName);
                }
            else
                for (_NoteListItem* _Elem : m_DeletedNoteList)
                    if (_Elem)
                        _Elem->setHidden(true);
            }

        /* 当用户选中的是其他笔记本时, 执行过滤操作 */
        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            _NoteListItem* _CurrentItem
                    = dynamic_cast<_NoteListItem *>(m_NoteList->item(_Index));

            if (!_CurrentItem)
                {
                __PTR_INVALID_ERROR_OUTPUT__(_CurrentItem)

                return;
                }

            /* 所有与当前笔记本名称不符的笔记项都会被过滤掉 */
            if (_CurrentItem->_GetParentNoteBookName()
                    != m_NoteBookTree->currentItem()->text(0))
                {
                _CurrentItem->setHidden(true);

                if (_IsCurrentNoteListAllHidden())
                    /* 如果当前笔记本列表为空, 那么设置编辑窗口为隐藏 */
                    m_TextEditorStackedLayout->currentWidget()->setHidden(true);
                }
            else
                {
                _CurrentItem->setHidden(false);

                _RestoreLastPitchOnItem_NoteBookTree(_CurrentNoteBookName);
                }
            }

         /* 如果当前笔记中没有被选中的笔记项, 那么将第一个item设为选中项 */
        if (!_CheckSpecifiedNoteBookHasSelectItem(_CurrentNoteBookName))
            m_NoteList->setCurrentItem(
                        _GetFirstNoteInSpecifiedNoteBook(_CurrentNoteBookName));
        else /* 否则获取当前笔记本中最后一次被选中的笔记项... */
            _RestoreLastPitchOnItem_NoteBookTree(_CurrentNoteBookName);
        }

    /* _CurrentCategoriesItemChangedSlot()槽实现 */
    void _MainWindowNormal::_CurrentCategoriesItemChangedSlot(QTreeWidgetItem */*_Current*/,
                                                              QTreeWidgetItem */*_Previous*/)
        {
        _SetDeleteNoteEnabled();

        QString _CurrentLabelName = m_CategoriesTree->currentItem()->text(0);

        _FilterNote_inCategoriesTree(_CurrentLabelName);

        _RestoreLastPitchOnItem_CategoriesTree(_CurrentLabelName);
        }

    /* _CurrentNoteRatingChangeSlot()槽实现 */
    void _MainWindowNormal::_CurrentNoteRatingChangedSlot()
        {
        _NoteListItem* _CurrentNoteItem =
                dynamic_cast<_NoteListItem *>(m_NoteList->currentItem());

        if (_CurrentNoteItem)
            {
            wxNote::_NoteRating _CurrentNoteRating =
                    wxNote::_GetEWFromGlobalList_BySpecifiedItem(
                                        _CurrentNoteItem)->_GetNoteRating();

            _CurrentNoteItem->_SetNoteRating(_CurrentNoteRating);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)
        }

    /* _CurrentNoteCategoriesChangeSlot()槽实现 */
    void _MainWindowNormal::_CurrentNoteCategoriesChangedSlot()
        {
        _NoteListItem* _CurrentNoteItem =
                dynamic_cast<_NoteListItem *>(m_NoteList->currentItem());

        if (_CurrentNoteItem)
            {
            wxNote::_NoteCategories _CurrentNoteCategories =
                    wxNote::_GetEWFromGlobalList_BySpecifiedItem(
                                        _CurrentNoteItem)->_GetNoteCategories();

            _CurrentNoteItem->_SetNoteCategories(_CurrentNoteCategories);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)
        }

    /* _SetCurrentNoteIconSlot()槽实现 */
    void _MainWindowNormal::_SetCurrentNoteIconSlot(const QString &/*_TextTitle*/,
                                                    const QString &_IconPath)
        {
        m_NoteList->_GetCurrentItem()->setIcon(QIcon(_IconPath));
        }

    /* _NoteTitleLineEditTextChanged()槽重写 */
    void _MainWindowNormal::_NoteTitleLineEditTextChanged(const QString &_Text)
        {
        QString _CurrentDate = QDate::currentDate().toString(Qt::ISODate);

        _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();
        _CurrentNoteItem->_SetNoteNameSlot(_Text.isEmpty()
                                                ? wxNote::g_NonTitleNoteName : _Text);

        QLabel* _DisplayWidget
                = dynamic_cast<QLabel *>(m_NoteList->itemWidget(_CurrentNoteItem));

        _DisplayWidget->setText(tr("<I><p style=\" margin-top:5px;"
                                   "margin-bottom:0px;"
                                   "margin-left:7px;"
                                   "margin-right:0px;"
                                   "-qt-block-indent:0;"
                                   "text-indent:0px;\">"
                                   "<span style=\" font-family:'微软雅黑';\">")
                                   + (_Text.isEmpty() ? wxNote::g_NonTitleNoteName : _Text)
                                   + "</span></p></I>"
                                   "<p style=\" margin-top:2px;"
                                   "margin-bottom:0px;"
                                   "margin-left:7px;"
                                   "margin-right:0px;"
                                   "-qt-block-indent:0;"
                                   "text-indent:0px;\">"
                                   "<span style=\" font-family:'微软雅黑';color:#0a95d8;\">"
                                 + _CurrentDate + "</span></p>");

        _NoteEditorSplitter* _CurrentSplitter
                = dynamic_cast<_NoteEditorSplitter *>(
                    m_TextEditorStackedLayout->currentWidget());

        if (_CurrentSplitter)
            _CurrentSplitter->_SetParentNoteName(_Text.isEmpty() ? wxNote::g_NonTitleNoteName
                                                                 : _Text);
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentSplitter)
        }

    /* _MainWindowCurrentTitleChanged()槽实现 */
    void _MainWindowNormal::_MainWindowCurrentTitleChanged(const QString &_Text)
        {
        this->setWindowTitle(tr("%1%2 - wxNote")
                             .arg(_Text.isEmpty() ? "" : (_Text + " - "))
                             .arg(m_NoteBookTree->currentItem()->text(0)));
        }

    /* _CurrentTitleLineEditTextChangedSlot()槽实现 */
    void _MainWindowNormal
        ::_CurrentTitleLineEditTextChangedSlot(const QString &_Text)
        {
        this->setWindowTitle(
                    tr("%1 - %2 - wxNote")
                    .arg(_Text.isEmpty() ? wxNote::g_NonTitleNoteName : _Text)
                    .arg(m_NoteBookTree->currentItem()->text(0)));
        }

    /* _SetAcceptRichTextSlot()函数实现 */
    void _MainWindowNormal::_SetAcceptRichTextSlot(bool _IsOK)
        {
        using namespace wxNote;

        for (_TextEditorWindow* _Elem : wxNote::g_AllTextEditorList)
            if (_Elem)
                _Elem->_GetTextEditor()->setAcceptRichText(_IsOK);
        }

    /* _NewNoteBookSlot()函数实现 */
    void _MainWindowNormal::_NewNoteBookSlot()
        {
        _GetNameForNewNoteBookDialog _GetNameDialog(this);

        if (_GetNameDialog.exec())
            {
            QString _NewNoteBookName = _GetNameDialog._GetLineEditText();

            /* 防止用户创建相同名称的笔记本 */
            for (const QString& _Elem : wxNote::g_UserNoteBookNameList)
                {
                if (_NewNoteBookName == _Elem)
                    {
                    _CommonDialog _WarningDialog(
                                this, tr("wxNote"),
                                tr("<font color=darkred><B>笔记本“%1”已存在。</font></B>"
                                   "<p>请指定一个不同的笔记本名称，并重试。")
                                   .arg(_NewNoteBookName),
                                tr("OK"));
                    _WarningDialog.exec();
                    return;
                    }
                }

            /* 创建一个新项 */
            QTreeWidgetItem* _NewNoteItem =
                    new QTreeWidgetItem(m_NoteBookTree->_GetNoteBookRoot(),
                                        QStringList() << _NewNoteBookName);

            _NewNoteItem->setIcon(0, QIcon(":/wxNote_Icons/noteBook.png"));
            m_NoteBookTree->_GetNoteBookRoot()->addChild(_NewNoteItem);

            QString _Path = wxNote::g_LocalFilePath + '/' + _NewNoteBookName;
            QDir _Dir(_Path);
            if (!_Dir.exists())
                _Dir.mkpath(_Path);

            m_NoteBookTree->setCurrentItem(_NewNoteItem, 0);

            wxNote::g_UserNoteBookNameList.push_back(_NewNoteBookName);
            m_NoteBookTree->_NoteBookCountIncreaseSlot();

            _ClearSearchLineEdit();

            QAction* _NewNoteBookNameAction = new QAction(_NewNoteBookName, this);

            wxNote::g_NoteBookNameActionList.push_back(_NewNoteBookNameAction);

            for (_TextEditorWindow* _Elem : wxNote::g_AllTextEditorList)
                {
                QMenu* _MoveNoteMenu = _Elem->_GetMoveNoteMenu();

                if (!_MoveNoteMenu->actions().contains(_NewNoteBookNameAction))
                    _Elem->_AddAction2_MoveNoteMenu(_NewNoteBookNameAction);
                }

            _SetDeleteNoteBookEnabled();
            }
        }

    /* _CreateNoteList()函数实现 */
    void _MainWindowNormal::_CreateNoteList()
        {
        m_NoteList = new _NoteList;
        m_NoteList->setIconSize(QSize(70, 70));

        connect(m_NoteList, SIGNAL(currentRowChanged(int)),
                m_TextEditorStackedLayout, SLOT(setCurrentIndex(int)));

        connect(m_NoteList, SIGNAL(currentRowChanged(int)),
                this, SLOT(_RecordLastPitchOnNoteItemSlot_NoteBookTree(int)));

        connect(m_NoteList, SIGNAL(currentRowChanged(int)),
                this, SLOT(_RecordLastPitchOnNoteItemSlot_Categories(int)));

        connect(m_NoteList, SIGNAL(currentRowChanged(int)),
                this, SLOT(_CurrentNoteChangedSlot_WindowTitle()));
        }

    /* _CreateSearchPanel()函数实现 */
    void _MainWindowNormal::_CreateSearchPanel()
        {
        m_SearchPanel = new _SearchPanel;

        connect(m_SearchPanel->_GetSearchLineEdit(), SIGNAL(textChanged(QString)),
                this, SLOT(_BeginFilterSlot(QString)));
        }

    /* _NewNoteSlot()槽重写 */
    QPair<_NoteListItem*, _TextEditorWindow *>
        _MainWindowNormal::_NewNoteSlot()
        {
        /* 遍历笔记列表, 如果当前笔记列表中存在"无标题笔记", 那么将不会重复创建 */
        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            if (!m_NoteList->item(_Index)->isHidden()
                    && m_NoteList->_Item(_Index)->_GetNoteNameSlot() == wxNote::g_NonTitleNoteName)

                return qMakePair(new _NoteListItem(),
                                 new _TextEditorWindow());
            }

        QString _CurrentDate = QDate::currentDate().toString(Qt::ISODate);

        QLabel* _DisplayLabel = new QLabel(tr("<I><p style=\" margin-top:5px;"
                                              "margin-bottom:0px;"
                                              "margin-left:8px;"
                                              "margin-right:8px;"
                                              "-qt-block-indent:0;"
                                              "text-indent:0px;\">"
                                              "<span style=\" font-family:'微软雅黑';\">"
                                              "无标题笔记</span></p></I>"
                                              "<p style=\" margin-top:2px;"
                                              "margin-bottom:0px;"
                                              "margin-left:7px;"
                                              "margin-right:7px;"
                                              "-qt-block-indent:0;"
                                              "text-indent:0px;\">"
                                              "<span style=\" font-family:'微软雅黑';color:#0a95d8;\">")
                                            + _CurrentDate + "</span></p>");

        _DisplayLabel->setAlignment(Qt::AlignTop);
        _DisplayLabel->setWordWrap(true);

        _NoteListItem* _Item = new _NoteListItem;
        _Item->_SetNoteNameSlot(wxNote::g_NonTitleNoteName);

        /* 存储该新建笔记所属的笔记本的名称 */
        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

        if (m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentNoteBookName, m_wxNoteTabWidget).count() % 2)
            _Item->setBackground(QBrush(QColor(240, 240, 240), Qt::Dense3Pattern));

        /* 将该新建笔记与其所属的笔记本的名称绑定到一起 */
        QPair<_NoteListItem*, QString> _ItemGroup = qMakePair(_Item, _CurrentNoteBookName);

        m_NoteList->_AddItem(_ItemGroup);
        m_NoteList->setCurrentItem(_Item);
        m_NoteList->setItemWidget(_Item, _DisplayLabel);

        _NoteEditorSplitter* _RightSplitter = new _NoteEditorSplitter(Qt::Vertical);

        _TextEditorWindow* _EditWindow = new _TextEditorWindow;
        _EditWindow->_SetParentNoteBookName_Current(m_NoteBookTree->currentItem()->text(0));
        _EditWindow->_SetParentNoteBookName_BeforeDeleted(m_NoteBookTree->currentItem()->text(0));

        _Item->_SetBindTextEW(_EditWindow);
        _EditWindow->_SetBindNoteItem(_Item);

        for (QAction* _Elem : wxNote::g_NoteBookNameActionList)
            _EditWindow->_AddAction2_MoveNoteMenu(_Elem);

        wxNote::g_AllTextEditorList.push_back(_EditWindow);

        QDate _CreateDate = QDate::currentDate();
        QTime _CreateTime = QTime::currentTime();
        _Item->_SetCreateDate(_CreateDate);
        _Item->_SetCreateTime(_CreateTime);
        _EditWindow->_SetCreateDate(_CreateDate);
        _EditWindow->_SetCreateTime(_CreateTime);

        connect(_EditWindow, SIGNAL(_DeleteNonMatchedNoteFileSignal(QString)),
                this, SLOT(_DeleteNonMatchedNoteFile(QString)));

        connect(_EditWindow, SIGNAL(_EliminateNoteButtonClickedSignal()),
                this, SLOT(_ThoroughDeleteCurrentNoteSlot()));

        connect(_EditWindow, SIGNAL(_RestoreNoteButtonClickedSignal()),
                this, SLOT(_RestoreNoteSlot()));

        connect(_EditWindow, SIGNAL(_SetNoteListPreview(const _TextEditorWindow*,QString,QString)),
                m_NoteList, SLOT(_SetCurrentNoteItemPreview(const _TextEditorWindow*,QString,QString)));

        connect(_EditWindow, SIGNAL(_SetNoteItem_NoteName(QString)),
                m_NoteList, SLOT(_SetCurrentNoteItem_NoteName(QString)));

        connect(_EditWindow, SIGNAL(_BeSavedSignals()),
                this, SLOT(_SaveCurrentNoteContentSlot()));

        connect(_EditWindow, SIGNAL(_MoveCurrentNote2TrashSignal()),
                this, SLOT(_MoveNote2TrashSlot()));

        connect(_EditWindow->_GetTitleLineEdit(), SIGNAL(textChanged(QString)),
                this, SLOT(_CurrentTitleLineEditTextChangedSlot(QString)));

        connect(_EditWindow->_GetTitleLineEdit(), SIGNAL(textChanged(QString)),
                this, SLOT(_CurrentNoteNameChangedSlot(QString)));

        connect(_EditWindow, SIGNAL(_CurrentNoteRatingChanged()),
                this, SLOT(_CurrentNoteRatingChangedSlot()));

        connect(_EditWindow, SIGNAL(_CurrentNoteCategoriesChanged()),
                this, SLOT(_CurrentNoteCategoriesChangedSlot()));

        connect(_EditWindow, SIGNAL(_CurrentNoteBeMove2OtherNoteBookSignal(QString)),
                this, SLOT(_MoveCurrentNote2OtherNoteBook(QString)));

        connect(_EditWindow, SIGNAL(_SetCurrentNoteIconSignal(QString,QString)),
                this, SLOT(_SetCurrentNoteIconSlot(QString,QString)));

        connect(_EditWindow, SIGNAL(_SetCurrentNoteIconSignal(QString,QString)),
                this, SLOT(_SetCurrentNoteIconSlot(QString,QString)));

        connect(_EditWindow, SIGNAL(_SetCurrentNoteIconSignal(QString,QString)),
                this, SIGNAL(_SetCurrentTabIconSignal(QString,QString)));

        _RightSplitter->addWidget(_EditWindow);
        _RightSplitter->_SetParentNoteName(_EditWindow->_GetTitleLineEdit()->text());
        _RightSplitter->_SetBeforeDeletedParentNoteName(_EditWindow->_GetTitleLineEdit()->text());

        connect(_EditWindow->_GetTitleLineEdit(), SIGNAL(textChanged(QString)),
                this, SLOT(_NoteTitleLineEditTextChanged(QString)));

        connect(_EditWindow->_GetTitleLineEdit(), SIGNAL(textChanged(QString)),
                this, SLOT(_MainWindowCurrentTitleChanged(QString)));

        connect(_EditWindow->_GetTextEditor(), SIGNAL(_FinalTabWindowMove2NextTabSignal()),
                this, SIGNAL(_FinalTabWindowMove2NextTab_BridgeSignal()));

        m_TextEditorStackedLayout->addWidget(_RightSplitter);

        m_TextEditorStackedLayout->setCurrentIndex(m_NoteList->currentIndex().row());

        /* 设置新建笔记的编辑器焦点 */
        if (_Item)
            wxNote::_GetEWFromGlobalList_BySpecifiedItem(_Item)->_GetTextEditor()->setFocus();
        else
            __PTR_INVALID_ERROR_OUTPUT__(_Item)

        _ClearSearchLineEdit();

        _SetDeleteNoteEnabled();
        _SetOneKeyLockEnabled();
        _SetNoteEditEnabled();

        return qMakePair(_Item, _EditWindow);
        }

    /* _MoveNote2TrashSlot()槽实现 */
    void _MainWindowNormal::_MoveNote2TrashSlot()
        {
        _NoteListItem* _CurrentNoteItem =
                dynamic_cast<_NoteListItem *>(m_NoteList->currentItem());

        _TextEditorWindow* _CurrentEditorWindow = _CurrentNoteItem->_GetBindTextEW();

        QString _CurrentNoteTitle = _CurrentNoteItem->_GetTitle();

        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);
        QString _CurrentCategoriesName = m_CategoriesTree->currentItem()->text(0);

        ///////////////////////////////////////////////////////////////////////
        /// 如果被移到废纸篓的笔记是一个空笔记,
        /// 那么将直接彻底删除该笔记项和该笔记的编辑窗口, 不留后患
        ///
        if ((_CurrentNoteTitle == wxNote::g_NonTitleNoteName)
                && _CurrentEditorWindow->_GetTitleLineEdit()->text().isEmpty())
            {
            /* 如果该笔记存在shadow窗口, 那么再删除该笔记的同时关闭它的shadow窗口 */
            emit _CloseCurrentNoteShadowWindowSignal(_CurrentEditorWindow);

            _EraseNoteItemFromDeletedList(m_NoteList->currentItem());
            _EraseCurrentEditorWindowFromGlobalList();

            m_TextEditorStackedLayout->setCurrentIndex(m_NoteList->row(0));
            QWidget* _CurrentSplitter = m_TextEditorStackedLayout->currentWidget();

            delete _CurrentNoteItem;
            delete _CurrentSplitter;

            _CurrentNoteItem = nullptr;
            _CurrentSplitter = nullptr;
            _CurrentEditorWindow = nullptr;

            _SetDeleteNoteEnabled();
            _SetOneKeyLockEnabled();
            _SetNoteEditEnabled();

            if (_IsCurrentNoteListAllHidden())
                _SetNotesAllHidden();

            m_NoteList->_AdjustNoteItemDye(_CurrentNoteBookName,
                                           m_wxNoteTabWidget);
            return;
            }

        ///////////////////////////////////////////////////////////////////////

        /* 验证当前笔记是否被锁定, 如果被锁定, wxNote将拒绝用户的移除操作 */
        if (_CurrentEditorWindow && _CurrentEditorWindow->_IsLocking())
            {
            _CommonDialog _MustUnlockDialog(
                        this
                        , tr("删除笔记")
                        , tr("<font color=darkred><B>当前笔记已被锁定。</font></B>"
                           "<p>为了保护您的笔记安全，"
                           "<p>必须对锁定笔记进行解锁后才能进行操作")
                        , tr("好的"));

            _MustUnlockDialog.exec();
            return;
            }

        if (_CurrentNoteItem)
            {
            /* 首先将该笔记移到trash文件夹中 */
            _MoveNoteFile2OtherOne(_CurrentNoteItem->_GetParentNoteBookName()
                                   , wxNote::_GetMatchedNoteFile_byNoteItem(_CurrentNoteItem)
                                   , wxNote::g_TrashName);

            emit _CloseCurrentNoteShadowWindowSignal(_CurrentEditorWindow);

            _EraseLastPitchOnItem_inNoteBookList(_CurrentNoteBookName);
            _EraseLastPitchOnItem_inCategoriesList(_CurrentCategoriesName);

            _CurrentEditorWindow->_SetCategoriesMenuEnabled(false);
            _CurrentEditorWindow->_SetMoveNoteMenuEnabled(false);

            _CurrentNoteItem->setHidden(true);

            _CurrentNoteItem->_SetParentNoteBookName(wxNote::g_TrashName);

            m_DeletedNoteList.push_back(_CurrentNoteItem);

            m_NoteBookTree->_TrashCountIncreaseSlot();

            _TextEditorWindow* _CurrentEditorWindow =
                            wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

            if (_CurrentEditorWindow)
                {
                _CurrentEditorWindow->_SetParentNoteBookName_Current(wxNote::g_TrashName);

                emit _CurrentEditorWindow->_CurrentNoteBeMove2TrashSignal(true);
                }
            else
                __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)

            m_NoteList->_AdjustNoteItemDye(_CurrentNoteBookName,
                                           m_wxNoteTabWidget);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)

        if (_IsCurrentNoteListAllHidden()
                && m_TextEditorStackedLayout->currentWidget())
            {
            _SetNotesAllHidden();
            _SetDeleteNoteEnabled();
            _SetOneKeyLockEnabled();

            return;
            }

        int _wxNoteTabWidgetIndex = m_wxNoteTabWidget->currentIndex();

        if (m_wxNoteTabWidget->tabText(_wxNoteTabWidgetIndex)
                    == wxNote::g_NoteBooksListName)
            _RestoreLastPitchOnItem_NoteBookTree(_CurrentNoteBookName);

        else if (m_wxNoteTabWidget->tabText(_wxNoteTabWidgetIndex)
                    == wxNote::g_CategoriesListName)
            _RestoreLastPitchOnItem_CategoriesTree(_CurrentCategoriesName);

        _SetDeleteNoteEnabled();
        _SetOneKeyLockEnabled();

        m_NoteList->_AdjustNoteItemDye(_CurrentNoteBookName,
                                       m_wxNoteTabWidget);
        }

    /* _ThoroughDeleteCurrentNoteSlot()槽重写 */
    void _MainWindowNormal::_ThoroughDeleteCurrentNoteSlot()
        {
        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

        _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();

        /* 只有在当前笔记列表不为空的情况下才会响应Delete按钮 */
        if (!_IsCurrentNoteListAllHidden())
            {
            _TextEditorWindow* _CurrentEditorWindow =
                            wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

            QString _CurrentNoteTitle = _CurrentEditorWindow->_GetTitleLineEdit()->text();

            /* 验证当前笔记是否被锁定, 如果被锁定, wxNote将拒绝用户的删除操作 */
            if (_CurrentEditorWindow
                    &&_CurrentEditorWindow->_IsLocking())
                {
                _CommonDialog _MustUnlockDialog(
                            this
                            , tr("删除笔记")
                            , tr("<font color=darkred><B>当前笔记已被锁定。</font></B>"
                               "<p>为了保护您的笔记安全，"
                               "<p>必须对锁定笔记进行解锁后才能进行操作")
                            , tr("好的"));

                _MustUnlockDialog.exec();
                return;
                }

            _CommonDialog _QuestionDialog(
                        this
                        , tr("删除笔记")
                        , tr("<font color=darkred><B>你即将永远删除该笔记。</font></B>"
                           "<p>如果你删除这条笔记，它将永远消失。"
                           "<p>你确定要删除这条笔记吗？")
                        , tr("删除笔记")
                        , tr("离开"));

            if (_QuestionDialog.exec())
                {
                _DeleteSavedContext(_CurrentNoteTitle);
                _EraseLastPitchOnItem_inNoteBookList(_CurrentNoteBookName);

                emit _CloseCurrentNoteShadowWindowSignal(_CurrentEditorWindow);

                _EraseNoteItemFromDeletedList(m_NoteList->currentItem());
                _EraseCurrentEditorWindowFromGlobalList();

                QString _NoteParentBook = _CurrentNoteItem->_GetParentNoteBookName();
                wxNote::_Deleted_SpecifiedNoteFile(_GetSpecifiedNoteBookPath(_NoteParentBook),
                                                   _CurrentNoteItem);

                delete m_TextEditorStackedLayout->currentWidget();
                delete m_NoteList->currentItem();

                /* 保证笔记项的索引与栈布局的索引正确对应 */
                m_TextEditorStackedLayout->setCurrentIndex(m_NoteList->currentRow());

                m_NoteList->_AdjustNoteItemDye(_CurrentNoteBookName,
                                               m_wxNoteTabWidget);
                }

            if (_IsCurrentNoteListAllHidden()
                    && m_TextEditorStackedLayout->currentWidget())
                m_TextEditorStackedLayout->currentWidget()->setHidden(true);
            }

        if (_IsCurrentNoteListAllHidden())
            _EraseLastPitchOnItem_inNoteBookList(_CurrentNoteBookName);

        _SetEliminateNoteEnabled();
        _SetRestoreNoteEnabled();
        _SetDeleteNoteEnabled();
        _SetOneKeyLockEnabled();
        _SetNoteEditEnabled();
        }

    /* _RestoreNoteSlot()槽实现 */
    void _MainWindowNormal::_RestoreNoteSlot()
        {
        _NoteListItem* _CurrentNoteItem =
                dynamic_cast<_NoteListItem *>(m_NoteList->currentItem());

        _TextEditorWindow* _CurrentEditorWindow =
                wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

        if (_CurrentNoteItem)
            {
            _MoveNoteFile2OtherOne(wxNote::g_TrashName,
                                   wxNote::_GetMatchedNoteFile_byNoteItem(_CurrentNoteItem),
                                   _CurrentNoteItem->_GetBindTextEW()->_GetParentNoteBookName_BeforeDeleted());

            QString _BeforeDeletedParentNoteBookName =
                    _CurrentNoteItem->_GetBeforeDeletedParentNoteBookName();

            _CurrentNoteItem->_SetParentNoteBookName(
                                        _BeforeDeletedParentNoteBookName);

            for (QPair<QString, _NoteListItem *>& _Elem : m_LastPitchOnItemList_NoteBookTree)
                if (_Elem.first == wxNote::g_TrashName)
                    _Elem.first = _BeforeDeletedParentNoteBookName;

            _CurrentEditorWindow->_SetCategoriesMenuEnabled(true);
            _CurrentEditorWindow->_SetMoveNoteMenuEnabled(true);

            _CurrentNoteItem->setHidden(true);
            m_TextEditorStackedLayout->currentWidget()->setHidden(true);

            if (_SpecifyNoteBookCnt(wxNote::g_TrashName))
                {
                for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
                    {
                    if (!m_NoteList->item(_Index)->isHidden())
                        {
                        m_NoteList->setCurrentRow(_Index);
                        break;
                        }
                    }
                }

            /* 笔记成功恢复后正确调整编辑窗口的工具栏 */
            _TextEditorWindow* _CurrentEditorWindow =
                            wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);
            if (_CurrentEditorWindow)
                emit _CurrentEditorWindow->_CurrentNoteBeRestoreSignal(true);
            else
                {
                __PTR_INVALID_ERROR_OUTPUT__(_CurrentEditorWindow)

                return;
                }

            _SetEliminateNoteEnabled();
            _SetRestoreNoteEnabled();
            _SetDeleteNoteEnabled();
            _SetNoteEditEnabled();

            m_NoteList->_AdjustNoteItemDye(wxNote::g_TrashName,
                                           m_wxNoteTabWidget);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)
        }

    /* _MoveCurrentNote2OtherNoteBook()槽实现 */
    void _MainWindowNormal::_MoveCurrentNote2OtherNoteBook(const QString &_NoteBookName)
        {
        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

        if (_NoteBookName != _CurrentNoteBookName)
            {
            _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();

            if (_CurrentNoteItem)
                {
                _TextEditorWindow* _CurrentEditorWindow =
                        wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

                _MoveNoteFile2OtherOne(_CurrentNoteItem->_GetParentNoteBookName()
                                       , wxNote::_GetMatchedNoteFile_byNoteItem(_CurrentNoteItem)
                                       , _NoteBookName);

                m_TextEditorStackedLayout->setCurrentIndex(
                            m_NoteList->_GetIndexFromItem(_CurrentNoteItem).row());
                QWidget* _CurrentSplitter =
                        m_TextEditorStackedLayout->currentWidget();

                _CurrentNoteItem->_SetParentNoteBookName(_NoteBookName);
                _CurrentNoteItem->_SetBeforeDeletedParentNoteBookName(_NoteBookName);

                if (_CurrentNoteBookName != wxNote::g_AllNotesName
                        && _CurrentNoteBookName != wxNote::g_NoteBooksName)
                    {
                    _CurrentNoteItem->setHidden(true);
                    _CurrentSplitter->setHidden(true);

                    _CurrentEditorWindow->_SetParentNoteBookName_Current(_NoteBookName);
                    _CurrentEditorWindow->_SetParentNoteBookName_BeforeDeleted(_NoteBookName);

                    _NoteListItem* _FirstItem =
                            _GetFirstNoteInSpecifiedNoteBook(_CurrentNoteBookName);
                    if (_FirstItem)
                        m_NoteList->setCurrentItem(_FirstItem);

                    if (_IsCurrentNoteListAllHidden())
                        _EraseLastPitchOnItem_inNoteBookList(_CurrentNoteBookName);
                    }
                }
            else
                __PTR_INVALID_ERROR_OUTPUT__(_CurrentNoteItem)
            }
        }

    /* _RenameNoteBookSlot()槽实现 */
    void _MainWindowNormal::_RenameNoteBookSlot()
        {
        // TODO
        }

    /* _ThoroughDeleteNoteBookSlot()槽实现 */
    void _MainWindowNormal::_ThoroughDeleteNoteBookSlot()
        {
        using namespace wxNote;

        QString _DeletedNoteBookName = m_NoteBookTree->currentItem()->text(0);

        _CommonDialog _QuestionDialog(
                    this
                    , tr("删除笔记本")
                    , tr("<font color=darkred><B>确认删除笔记本“%1”？</font></B>"
                         "<p>如果你删除这个笔记本，该笔记本中的笔记不会被删除，而是会移动到默认笔记本中。"
                         "<p>你确定要删除这个笔记本吗？").arg(_DeletedNoteBookName)
                    , tr("删除笔记本")
                    , tr("离开"));

        if (_QuestionDialog.exec())
            {
            _EraseNoteBookName_WhenDeleted(_DeletedNoteBookName);

            /* 将被删除的笔记本中的笔记都移到默认笔记下 */
            for (int _DeleteIndex = 0; _DeleteIndex < m_NoteList->count(); _DeleteIndex++)
                {
                _NoteListItem* _Item
                        = dynamic_cast<_NoteListItem *>(m_NoteList->item(_DeleteIndex));

                if (_Item)
                    {
                    if (_Item->_GetParentNoteBookName() == _DeletedNoteBookName)
                        {
                        _Item->_SetParentNoteBookName(
                                    m_NoteBookTree->_GetAllNoteBook()->text(0));
                        _Item->_SetBeforeDeletedParentNoteBookName(
                                    m_NoteBookTree->_GetAllNoteBook()->text(0));
                        }
                    }
                else
                    {
                    __PTR_INVALID_ERROR_OUTPUT__(_Item)

                    return;
                    }
                }

            //////////////////////////////////////////////////////////////////////
            /// 将被删除的笔记本的"Name action"从g_NoteBookNameActionsList和编辑窗口
            /// 的子菜单中移除
            ///
            QAction* _BeDeletedAction_Shadow = nullptr;
            for (QAction* _Elem : g_NoteBookNameActionList)
                {
                if (_Elem && _Elem->text() == _DeletedNoteBookName)
                    {
                    _BeDeletedAction_Shadow = _Elem;

                    delete _Elem;
                    _Elem = nullptr;

                    break;
                    }
                }

            auto _DeletedActionIter =
                std::remove_if(g_NoteBookNameActionList.begin(),
                               g_NoteBookNameActionList.end(),
                               [_BeDeletedAction_Shadow](const QAction* _Elem)
                                    {
                                    return _Elem == _BeDeletedAction_Shadow;
                                    });

            if (_DeletedActionIter != g_NoteBookNameActionList.end())
                {
                *_DeletedActionIter = nullptr;

                g_NoteBookNameActionList
                        .erase(_DeletedActionIter, g_NoteBookNameActionList.end());
                }

            //////////////////////////////////////////////////////////////////////

            _EraseLastPitchOnItem_inNoteBookList(_DeletedNoteBookName);

            QString _Path = _GetSpecifiedNoteBookPath(_DeletedNoteBookName);
            QDir _Dir(_Path);

            if (_Dir.exists())
                {
                QStringList _NoteFileNames = _Dir.entryList();

                for (const QString& _Elem : _NoteFileNames)
                    _MoveNoteFile2OtherOne(_DeletedNoteBookName
                                           , _Elem
                                           , wxNote::g_AllNotesName);

                _Dir.removeRecursively();
                }

            delete m_NoteBookTree->currentItem();

            m_NoteBookTree->_NoteBookCountDecreaseSlot();

            _SetDeleteNoteBookEnabled();
            }
        }

    /* _LockAllNotesSlot()槽实现 */
    void _MainWindowNormal::_LockAllNotesSlot()
        {
        for (_TextEditorWindow* _Elem : wxNote::g_AllTextEditorList)
            if (_Elem && !_Elem->_IsLocking())
                if (!_Elem->_LockCurrentNoteSlot())
                    break;
        }

    /* _BeginFilterSlot()槽实现 */
    void _MainWindowNormal::_BeginFilterSlot(QString _SearchText)
        {
        using namespace wxNote;

        _StopFilterSlot();

        if (!_SearchText.isEmpty())
            {
            QString _CurrentNoteBookName =
                    m_NoteBookTree->currentItem()->text(0);

            QList<_NoteListItem *> _NoteInCurrentNoteBook =
                            m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentNoteBookName,
                                                                     m_wxNoteTabWidget);

            QLineEdit* _SearchLineEdit = m_SearchPanel->_GetSearchLineEdit();

            /* 用于保存第一个匹配成功的笔记项 */
            _NoteListItem* _FirstMatchedItem = nullptr;

            for (_NoteListItem* _Elem : _NoteInCurrentNoteBook)
                {
                _TextEditorWindow* _CurrentEditorWindow =
                        _GetEWFromGlobalList_BySpecifiedItem(_Elem);
                _NoteEditor* _CurrentNoteEditor = _CurrentEditorWindow->_GetTextEditor();

                QString _CurrentNoteContext = _CurrentNoteEditor->toHtml();
                QString _CurrentNoteContext_Simple = _CurrentNoteEditor->toPlainText();

                /* 如果当前内容中含有被搜索的文本 */
                if (_CurrentNoteContext_Simple.contains(_SearchText))
                    {
                    _SearchLineEdit->setStyleSheet(g_SearchLineEditStyleSheet_Found);

                    if (_FirstMatchedItem == nullptr)
                        /* 设置为第一个匹配的笔记项... */
                        _FirstMatchedItem = _Elem;

                    _CurrentNoteContext.replace(_SearchText,
                                                g_MatchTextMark_Prefix +
                                                _SearchText +
                                                g_MatchTextMark_Suffix, Qt::CaseSensitive);

                    _CurrentNoteEditor->setHtml(_CurrentNoteContext);

                    _Elem->setBackground(m_MatchNoteItemDye);

                    if (!_CurrentEditorWindow->_IsLocking())
                        {
                        _NoteTitleLineEdit* _CurrentNoteTitleLineEdit
                                    = _CurrentEditorWindow->_GetTitleLineEdit();

                        /* 当笔记处于搜索状态时, 不允许用户编辑笔记 */
                        _CurrentNoteTitleLineEdit->setReadOnly(true);
                        _CurrentNoteEditor->setReadOnly(true);
                        }
                #if 0   // BUG! 实现编辑器当前位置为搜索文本所在位置
                    std::string _CurrentNoteContext_StdString = _CurrentNoteBookName.toStdString();
                    int _Position =
                            _CurrentNoteContext_StdString.find_first_of(_SearchText.toStdString(), 0);

                    _CurrentNoteEditor->textCursor().setPosition(_Position);
                #endif
                    }
                else
                    _SearchLineEdit->setStyleSheet(g_SearchLineEditStyleSheet_NotFound);
                }

            if (_FirstMatchedItem)
                /* 设置第一个匹配笔记项为当前笔记 */
                m_NoteList->setCurrentItem(_FirstMatchedItem);

            m_NoteStatus = _FilterStatus;
            }
        else
            _StopFilterSlot();
        }

    /* _StopFilterSlot()槽实现 */
    void _MainWindowNormal::_StopFilterSlot()
        {
        using namespace wxNote;

        for (_TextEditorWindow* _Elem : g_AllTextEditorList)
            {
            for (const QPair<QString, QString>& _ContextElem : m_FirstHandContextList)
                {
                _NoteEditor* _CurrentNoteEditor = _Elem->_GetTextEditor();
                QString _CurrentNoteTitle = _Elem->_GetTitleLineEdit()->text();

                if (_ContextElem.first == _CurrentNoteTitle)
                    _Elem->_GetTextEditor()->setHtml(_ContextElem.second);

                if (!_Elem->_IsLocking())
                    {
                    _NoteTitleLineEdit* _CurrentNoteTitleLineEdit
                                = _Elem->_GetTitleLineEdit();

                    /* 当笔记结束搜索状态时, 用户可以编辑该笔记 */
                    _CurrentNoteTitleLineEdit->setReadOnly(false);
                    _CurrentNoteEditor->setReadOnly(false);
                    }
                }
            }

        QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

        m_NoteList->_AdjustNoteItemDye(_CurrentNoteBookName, m_wxNoteTabWidget);

        m_NoteStatus = _NormalStatus;
        }

    /* _SaveCurrentNoteContentSlot()槽实现 */
    void _MainWindowNormal::_SaveCurrentNoteContentSlot()
        {
        using namespace wxNote;

        _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();
        _TextEditorWindow* _CurrentEditorWindow =
                _GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

        QString _CurrentNoteTitle = _CurrentEditorWindow->_GetTitleLineEdit()->text();

        for (QPair<QString, QString>& _Elem : m_FirstHandContextList)
            {
            if (_Elem.first == _CurrentNoteTitle)
                {
                if (m_SearchPanel->_GetSearchLineEdit()->text().isEmpty())
                    _Elem.second = _CurrentEditorWindow->_GetTextEditor()->toHtml();
                else
                    return;
                }
            }

        m_FirstHandContextList.push_back(
                    qMakePair(_CurrentNoteTitle,
                              _CurrentEditorWindow->_GetTextEditor()->toHtml()));
        }

    /* _RecordLastPitchOnNoteItemSlot_NoteBookTree()槽实现 */
    void _MainWindowNormal::_RecordLastPitchOnNoteItemSlot_NoteBookTree(int /*_Row*/)
        {
        int _wxNoteTabWidgetCurrentIndex = m_wxNoteTabWidget->currentIndex();

        if (m_wxNoteTabWidget->tabText(_wxNoteTabWidgetCurrentIndex)
                    == wxNote::g_NoteBooksListName)
            {
            QString _CurrentNoteBookName = m_NoteBookTree->currentItem()->text(0);

            auto _Iter =
                std::find_if(m_LastPitchOnItemList_NoteBookTree.begin(), m_LastPitchOnItemList_NoteBookTree.end(),
                             [&_CurrentNoteBookName](const QPair<QString, _NoteListItem *>& _Elem)
                                    {
                                    return _Elem.first == _CurrentNoteBookName;
                                    });

            _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();
            if (_Iter != m_LastPitchOnItemList_NoteBookTree.end())
                {
                for (QPair<QString, _NoteListItem *>& _Elem : m_LastPitchOnItemList_NoteBookTree)
                    if (_Elem.first == _CurrentNoteBookName)
                        _Elem.second = _CurrentNoteItem;
                }
            else
                m_LastPitchOnItemList_NoteBookTree.push_back(
                            qMakePair(_CurrentNoteBookName, m_NoteList->_GetCurrentItem()));
            }
        }

    /* _RecordLastPitchOnNoteItemSlot_Categories()槽实现 */
    void _MainWindowNormal::_RecordLastPitchOnNoteItemSlot_Categories(int /*_Row*/)
        {
        int _wxNoteTabWidgetCurrentIndex = m_wxNoteTabWidget->currentIndex();

        if (m_wxNoteTabWidget->tabText(_wxNoteTabWidgetCurrentIndex)
                    == wxNote::g_CategoriesListName)
            {
            QString _CurrentCategoriesName = m_CategoriesTree->currentItem()->text(0);

            auto _Iter =
                std::find_if(m_LastPitchOnItemList_CategoriesTree.begin(), m_LastPitchOnItemList_CategoriesTree.end(),
                             [&_CurrentCategoriesName](const QPair<QString, _NoteListItem *>& _Elem)
                                    {
                                    return _Elem.first == _CurrentCategoriesName;
                                    });

            _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();
            if (_Iter != m_LastPitchOnItemList_CategoriesTree.end())
                {
                for (QPair<QString, _NoteListItem *>& _Elem : m_LastPitchOnItemList_CategoriesTree)
                    if (_Elem.first == _CurrentCategoriesName)
                        _Elem.second = _CurrentNoteItem;
                }
            else
                m_LastPitchOnItemList_CategoriesTree.push_back(
                            qMakePair(_CurrentCategoriesName, m_NoteList->_GetCurrentItem()));
            }
        }


    /* _DeleteNonMatchedNoteFile()函数实现 */
    void _MainWindowNormal
        ::_DeleteNonMatchedNoteFile(const QString &_CurrentNoteBook)
        {
        QString _CurrentPath = _GetSpecifiedNoteBookPath(_CurrentNoteBook);

        QList<_NoteListItem *> _Notes =
                m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentNoteBook,
                                                         m_wxNoteTabWidget);

        wxNote::_Deleted_LoseHome_NoteFile(_CurrentPath, _Notes);
        }

    /* _CurrentNoteChangedSlot_WindowTitle()槽实现 */
    void _MainWindowNormal::_CurrentNoteChangedSlot_WindowTitle()
        {
        if (m_NoteList->currentItem())
            {
            setWindowTitle(tr("%1 - %2 - wxNote")
                           .arg(m_NoteList->_GetCurrentItem()->_GetNoteNameSlot())
                           .arg(m_NoteBookTree->currentItem()->text(0)));

            emit _MainWindowNormalTitleChangedSignal(this->windowTitle());
            }
        else
            {
            setWindowTitle(tr("%1 - wxNote")
                           .arg(m_NoteBookTree->currentItem()->text(0)));

            emit _MainWindowNormalTitleChangedSignal(this->windowTitle());
            }
        }

    /* _CurrentNoteBookChangedSlot_WindowTitle()槽实现 */
    void _MainWindowNormal::_CurrentNoteBookChangedSlot_WindowTitle()
        {
        if (_IsCurrentNoteListAllHidden()
                || m_NoteBookTree->currentItem()->text(0) == wxNote::g_LabelName
                || m_NoteBookTree->currentItem()->text(0) == wxNote::g_PropertiesName)
            {
            setWindowTitle(tr("%1 - wxNote")
                           .arg(m_NoteBookTree->currentItem()->text(0)));

            emit _MainWindowNormalTitleChangedSignal(this->windowTitle());
            }
        else
            {
            if (m_NoteList->currentItem())
                {
                setWindowTitle(tr("%1 - %2 - wxNote")
                               .arg(m_NoteList->_GetCurrentItem()->_GetNoteNameSlot())
                               .arg(m_NoteBookTree->currentItem()->text(0)));

                emit _MainWindowNormalTitleChangedSignal(this->windowTitle());
                }
            }
        }

    /* _DeleteSavedContext()函数实现 */
    void _MainWindowNormal::_DeleteSavedContext(const QString &_NoteTitle)
        {
        auto _Iter =
            std::remove_if(m_FirstHandContextList.begin(), m_FirstHandContextList.end(),
                           [&_NoteTitle](const QPair<QString, QString>& _Elem)
                                {
                                return _Elem.first == _NoteTitle;
                                });

        if (_Iter != m_FirstHandContextList.end())
            m_FirstHandContextList.erase(_Iter, m_FirstHandContextList.end());
        }

    /* _EraseNoteBookName_WhenDeleted()函数实现 */
    void _MainWindowNormal
        ::_EraseNoteBookName_WhenDeleted(const QString &_NoteBookName)
        {
        using namespace wxNote;

        auto _Iter =
                std::remove_if(g_UserNoteBookNameList.begin(), g_UserNoteBookNameList.end(),
                               [&_NoteBookName](const QString& _Elem)
                                    {
                                    return _Elem == _NoteBookName;
                                    });

        if (_Iter != g_UserNoteBookNameList.end())
            g_UserNoteBookNameList.erase(_Iter, g_UserNoteBookNameList.end());
        }

    /* _EraseLastPitchOnItem_inNoteBookList()函数实现 */
    void _MainWindowNormal
        ::_EraseLastPitchOnItem_inNoteBookList(const QString &_NoteBookName)
        {
        auto _PitchOnIter =
            std::remove_if(m_LastPitchOnItemList_NoteBookTree.begin(), m_LastPitchOnItemList_NoteBookTree.end(),
                           [&_NoteBookName](const QPair<QString, _NoteListItem *>& _Elem)
                                {
                                return _Elem.first == _NoteBookName;
                                });

        if (_PitchOnIter != m_LastPitchOnItemList_NoteBookTree.end())
            m_LastPitchOnItemList_NoteBookTree.erase(_PitchOnIter, m_LastPitchOnItemList_NoteBookTree.end());
        }

    /* _EraseLastPitchOnItem_inCategoriesList()函数实现 */
    void _MainWindowNormal
        ::_EraseLastPitchOnItem_inCategoriesList(const QString &_CategoriesName)
        {
        auto _PitchOnIter =
            std::remove_if(m_LastPitchOnItemList_CategoriesTree.begin(), m_LastPitchOnItemList_CategoriesTree.end(),
                           [&_CategoriesName](const QPair<QString, _NoteListItem* >& _Elem)
                                {
                                return _Elem.first == _CategoriesName;
                                });

        if (_PitchOnIter != m_LastPitchOnItemList_CategoriesTree.end())
            m_LastPitchOnItemList_CategoriesTree.erase(_PitchOnIter, m_LastPitchOnItemList_CategoriesTree.end());
        }

    /* _GetLastPitchOnItem_inNoteBookTree()函数实现
     * 该函数用于获取指定笔记本中最后一次被选中的笔记项 */
    _NoteListItem* _MainWindowNormal
        ::_GetLastPitchOnItem_inNoteBookTree(const QString &_NoteBookName)
        {
        QList<_NoteListItem *> _Notes =
                m_NoteList->_GetNotesInSpecifiedNoteBook(_NoteBookName,
                                                         m_wxNoteTabWidget);
        if (_Notes.isEmpty())
            return nullptr;

        auto _Iter =
                std::find_if(m_LastPitchOnItemList_NoteBookTree.begin(), m_LastPitchOnItemList_NoteBookTree.end(),
                             [&_NoteBookName](const QPair<QString, _NoteListItem*>& _Elem)
                                {
                                return _Elem.first == _NoteBookName;
                                });

        if (_Iter != m_LastPitchOnItemList_NoteBookTree.end())
            return _Iter->second;
        else
            return nullptr;
        }

    /* _GetLastPitchOnItem_inCategoriesTree()函数实现 */
    _NoteListItem* _MainWindowNormal
        ::_GetLastPitchOnItem_inCategoriesTree(const QString &_CategoriesName)
        {
        auto _Iter =
                std::find_if(m_LastPitchOnItemList_CategoriesTree.begin(), m_LastPitchOnItemList_CategoriesTree.end(),
                             [&_CategoriesName](const QPair<QString, _NoteListItem*>& _Elem)
                                {
                                return _Elem.first == _CategoriesName;
                                });

        if (_Iter != m_LastPitchOnItemList_CategoriesTree.end())
            return _Iter->second;
        else
            return nullptr;
        }

    /* _GetCurrentTextEditorWindow()函数实现 */
    _TextEditorWindow* _MainWindowNormal::_GetCurrentTextEditorWindow() const
        {
        _NoteListItem* _CurrentNoteItem = m_NoteList->_GetCurrentItem();

        _TextEditorWindow* _CurrentEditorWindow =
                wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

        return _CurrentEditorWindow ? _CurrentEditorWindow : nullptr;
        }

    /* _RestoreLastPitchOnItem_NoteBookTree()函数实现 */
    void _MainWindowNormal
        ::_RestoreLastPitchOnItem_NoteBookTree(const QString &_NoteBookName)
        {
        int _wxNoteTabWidgetCurrentIndex = m_wxNoteTabWidget->currentIndex();

        if (m_wxNoteTabWidget->tabText(_wxNoteTabWidgetCurrentIndex)
                    == wxNote::g_NoteBooksListName)
            {
            _NoteListItem* _LastPitchOnItem =
                    _GetLastPitchOnItem_inNoteBookTree(_NoteBookName);

            if (_LastPitchOnItem)
                {
                m_NoteList->setCurrentItem(_LastPitchOnItem);

                QWidget* _CurrentWidget =
                        m_TextEditorStackedLayout->currentWidget();
                if (_CurrentWidget)
                    m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                }
            else
                {
                QListWidgetItem* _FirstNoteItem =
                        _GetFirstNoteInSpecifiedNoteBook(_NoteBookName);

                if (_FirstNoteItem)
                    {
                    m_NoteList->setCurrentItem(_FirstNoteItem);
                    m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                    }
                }
            }
        }

    /* _RestoreLastPitchOnItem_CategoriesTree()函数实现 */
    void _MainWindowNormal
        ::_RestoreLastPitchOnItem_CategoriesTree(const QString &_CategoriesName)
        {
        int _wxNoteTabWidgetCurrentIndex = m_wxNoteTabWidget->currentIndex();

        if (m_wxNoteTabWidget->tabText(_wxNoteTabWidgetCurrentIndex)
                    == wxNote::g_CategoriesListName)
            {
            _NoteListItem* _LastPitchOnItem =
                    _GetLastPitchOnItem_inCategoriesTree(_CategoriesName);

            if (_LastPitchOnItem)
                {
                m_NoteList->setCurrentItem(_LastPitchOnItem);

                QWidget* _CurrentWidget =
                        m_TextEditorStackedLayout->currentWidget();
                if (_CurrentWidget)
                    m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                }
            else
                {
                QList<_NoteListItem *> _Notes;

                if (_CategoriesName == wxNote::g_ExcellentName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Excellent);

                else if (_CategoriesName == wxNote::g_GoodName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Good);

                else if (_CategoriesName == wxNote::g_AverageName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Average);

                else if (_CategoriesName == wxNote::g_FairName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Fair);

                else if (_CategoriesName == wxNote::g_PoorName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedRating(wxNote::_Poor);

                else if (_CategoriesName == wxNote::g_ImportantName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Important);

                else if (_CategoriesName == wxNote::g_WorkName)
                    _Notes = m_NoteList-> _GetNotesInSpecifiedCategories(wxNote::_Work);

                else if (_CategoriesName == wxNote::g_PersonalName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Personal);

                else if (_CategoriesName == wxNote::g_ToDoName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_ToDo);

                else if (_CategoriesName == wxNote::g_LaterName)
                    _Notes = m_NoteList->_GetNotesInSpecifiedCategories(wxNote::_Later);

                if (!_Notes.isEmpty())
                    m_NoteList->setCurrentItem(_Notes.first());
                }
            }
        }

    /* _SetNotesAllHidden()函数实现 */
    void _MainWindowNormal::_SetNotesAllHidden()
        {
        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            m_NoteList->item(_Index)->setHidden(true);

            m_TextEditorStackedLayout->setCurrentIndex(_Index);
            m_TextEditorStackedLayout->currentWidget()->setHidden(true);
            }

        emit _AdjustTabWindowTitle(m_NoteBookTree->currentItem()->text(0));
        }

    /* _SetNotesAllVisible_ExceptTrash()函数实现 */
    void _MainWindowNormal::_SetNotesAllVisible_ExceptTrash()
        {
        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem = m_NoteList->_Item(_Index);

            if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName)
                {
                m_NoteList->item(_Index)->setHidden(false);
                m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                }
            }
        }

    /* _FilterNote_ByCategories()函数实现 */
    void _MainWindowNormal::_FilterNote_ByCategories(
            const wxNote::_NoteCategories &_NoteCategories)
        {
        _SetNotesAllHidden();

        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem = m_NoteList->_Item(_Index);

            _TextEditorWindow* _CurrentEditorWindow =
                    wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

            if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName
                    && _CurrentEditorWindow->_GetNoteCategories() == _NoteCategories)
                {
                _CurrentNoteItem->setHidden(false);

                m_TextEditorStackedLayout->setCurrentIndex(_Index);
                m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                }
            }

        m_NoteList->_AdjustNoteItemDye(
                m_NoteList->_GetNotesInSpecifiedCategories(_NoteCategories));

        m_TextEditorStackedLayout->
                setCurrentIndex(m_NoteList->currentRow());
        }

    /* _FilterNote_ByRating()函数实现 */
    void _MainWindowNormal
        ::_FilterNote_ByRating(const wxNote::_NoteRating &_NoteRating)
        {
        _SetNotesAllHidden();

        for (int _Index = 0; _Index < m_NoteList->count(); _Index++)
            {
            _NoteListItem* _CurrentNoteItem = m_NoteList->_Item(_Index);

            _TextEditorWindow* _CurrentEditorWindow =
                    wxNote::_GetEWFromGlobalList_BySpecifiedItem(_CurrentNoteItem);

            if (_CurrentNoteItem->_GetParentNoteBookName() != wxNote::g_TrashName
                    &&_CurrentEditorWindow->_GetNoteRating() == _NoteRating)
                {
                _CurrentNoteItem->setHidden(false);

                m_TextEditorStackedLayout->setCurrentIndex(_Index);
                m_TextEditorStackedLayout->currentWidget()->setHidden(false);
                }
            }

        m_NoteList->_AdjustNoteItemDye(
                m_NoteList->_GetNotesInSpecifiedRating(_NoteRating));

        m_TextEditorStackedLayout->
                setCurrentIndex(m_NoteList->currentRow());
        }

    /* _FilterNote_inCategoriesTree()函数实现 */
    void _MainWindowNormal
        ::_FilterNote_inCategoriesTree(const QString _CurrentLabelName)
        {
        /* "评分" */
        if (_CurrentLabelName == wxNote::g_RatingListName)
            _SetNotesAllHidden();

        else if (_CurrentLabelName == wxNote::g_ExcellentName)
            _FilterNote_ByRating(wxNote::_Excellent);

        else if (_CurrentLabelName == wxNote::g_GoodName)
            _FilterNote_ByRating(wxNote::_Good);

        else if (_CurrentLabelName == wxNote::g_AverageName)
            _FilterNote_ByRating(wxNote::_Average);

        else if (_CurrentLabelName == wxNote::g_FairName)
            _FilterNote_ByRating(wxNote::_Fair);

        else if (_CurrentLabelName == wxNote::g_PoorName)
            _FilterNote_ByRating(wxNote::_Poor);

        /* "颜色标签" */
        else if (_CurrentLabelName == wxNote::g_ColorLabelListName)
            _SetNotesAllHidden();

        else if (_CurrentLabelName == wxNote::g_ImportantName)
            _FilterNote_ByCategories(wxNote::_Important);

        else if (_CurrentLabelName == wxNote::g_WorkName)
            _FilterNote_ByCategories(wxNote::_Work);

        else if (_CurrentLabelName == wxNote::g_PersonalName)
            _FilterNote_ByCategories(wxNote::_Personal);

        else if (_CurrentLabelName == wxNote::g_ToDoName)
            _FilterNote_ByCategories(wxNote::_ToDo);

        else if (_CurrentLabelName == wxNote::g_LaterName)
            _FilterNote_ByCategories(wxNote::_Later);

        bool _IsEmpty =
                m_NoteList->_GetNotesInSpecifiedNoteBook(_CurrentLabelName,
                                                         m_wxNoteTabWidget)
                                                        .isEmpty();
        if (_IsEmpty /* 如果当前标签中没有笔记.. */)
            _SetNotesAllHidden();
        else    /* 否则还原该标签中最后一次被选中的笔记项... */
            _RestoreLastPitchOnItem_CategoriesTree(_CurrentLabelName);
        }

    /* _PitchOnSpecifiedNoteBook()函数实现 */
    void _MainWindowNormal
        ::_PitchOnSpecifiedNoteBook(const QString &_NoteBookName)
        {
        QTreeWidgetItem* _NoteBookRootItem = m_NoteBookTree->_GetNoteBookRoot();

        for (int _Index = 0; _Index < _NoteBookRootItem->childCount(); _Index++)
            {
            QTreeWidgetItem* _ChildItem = _NoteBookRootItem->child(_Index);

            if (_ChildItem->text(0) == _NoteBookName)
                {
                m_NoteBookTree->setCurrentItem(_ChildItem);

                _RestoreLastPitchOnItem_NoteBookTree(_NoteBookName);
                break;
                }
            }
        }

    /* _CheckSpecifiedNoteBookHasSelectItem()函数实现 */
    bool _MainWindowNormal
        ::_CheckSpecifiedNoteBookHasSelectItem(const QString _NoteBookName)
        {
        QList<_NoteListItem *> _Notes =
                m_NoteList->_GetNotesInSpecifiedNoteBook(_NoteBookName,
                                                         m_wxNoteTabWidget);

        QList<bool> _AllPredicates;
        for (const _NoteListItem* _Elem : _Notes)
            _AllPredicates.push_back(_Elem->isSelected());

        bool b_HasSelect =
                std::any_of(_AllPredicates.begin(), _AllPredicates.end(),
                            [](bool _Elem)
                                {
                                return _Elem;
                                });
        return b_HasSelect;
        }

    /* _GetSpecifiedNoteBookPath()函数实现 */
    QString _MainWindowNormal
        ::_GetSpecifiedNoteBookPath(const QString &_NoteBookName)
        {
        QString _CurrentPath = tr("%1%2").arg(wxNote::g_LocalFilePath)
                                          .arg((_NoteBookName == wxNote::g_AllNotesName)
                                                              ? QString()
                                                              : '/' + _NoteBookName);
        return _CurrentPath;
        }
#if 0
    void _MainWindowNormal::_SynchronousSlot()
        {
        cout << wxNote::_GetMatchedNoteFile_byNoteItem(m_NoteList->_GetCurrentItem())
             << endl;
        }
#endif

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
