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

#include "wxNote_Gui/wxNote_Window/FinalTabWindow.h"
#include "wxNote_Gui/wxNote_Window/MainWindowNormal.h"
#include "wxNote_Gui/wxNote_Window/MainWindowShadow.h"
#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Gui/wxNote_Tree/NoteBookTree.h"
#include "wxNote_Gui/wxNote_Tree/CategoriesTree.h"
#include "wxNote_Gui/wxNote_Edit/NoteEditor.h"
#include "wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h"
#include "wxNote_Gui/wxNote_List/NoteList.h"
#include "wxNote_Gui/wxNote_Other/NoteEditorSplitter.h"

#include "wxNote_Global.h"

#include <QIcon>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QApplication>
#include <QStringList>
#include <QStackedLayout>

//.._FinalTabWindow类实现

    /* 构造函数实现 */
    _FinalTabWindow::_FinalTabWindow(QWidget* _Parent)
        : QTabWidget(_Parent)
        {
        _CreateNormalMainWindow();

        this->setTabsClosable(true);
        this->setMovable(true);
        this->setElideMode(Qt::ElideMiddle);
        this->addTab(m_NormalMainWindow, m_NormalMainWindow->windowTitle());

        connect(this, SIGNAL(tabCloseRequested(int)),
                this, SLOT(_CloseSpecifiedTabSlot(int)));

        connect(m_NormalMainWindow->_GetNoteList(),
                SIGNAL(_NoteItemBeDCSignal_NonOpened(_MainWindowShadow*,QString,QIcon)),
                this, SLOT(_AddShadowWindow2FinalWindowSlot(_MainWindowShadow*,QString,QIcon)));

        connect(m_NormalMainWindow->_GetNoteList(),
                SIGNAL(_NoteItemBeDCSignal_BeOpened(_TextEditorWindow*)),
                this, SLOT(_OpenExistShadowWindowSlot(_TextEditorWindow*)));

        connect(this, SIGNAL(currentChanged(int)),
                this, SLOT(_ExchangeEditorWindowOwnershipSlot(int)));

        connect(this, SIGNAL(currentChanged(int)),
                this, SLOT(_ChangeMainWindowCurrentTitle(int)));

        connect(m_NormalMainWindow, SIGNAL(_CurrentTitleLineEditTextChangedSignal(const _TextEditorWindow*,QString)),
                this, SLOT(_ChangeMainWindowCurrentTitle(const _TextEditorWindow*,QString)));

        connect(m_NormalMainWindow, SIGNAL(_CurrentTitleLineEditTextChangedSignal(const _TextEditorWindow*,QString)),
                this, SLOT(_ChangeCurrentTabTextSlot(const _TextEditorWindow*,QString)));

        connect(m_NormalMainWindow, SIGNAL(_SetCurrentTabIconSignal(QString,QString)),
                this, SLOT(_ChangeCurrentTabIconSlot(QString,QString)));

        connect(m_NormalMainWindow, SIGNAL(_FinalTabWindowMove2NextTab_BridgeSignal()),
                this, SLOT(_Move2NextTabSlot()));

        connect(m_NormalMainWindow, SIGNAL(_CloseCurrentNoteShadowWindowSignal(const _TextEditorWindow*)),
                this, SLOT(_CloseCurrentNoteShadowWindowSlot(const _TextEditorWindow*)));

        connect(m_NormalMainWindow, SIGNAL(_AdjustTabWindowTitle(QString)),
                this, SLOT(_AdjustCurrentTitle(QString)));

        connect(this->tabBar(), SIGNAL(tabMoved(int,int)),
                this, SLOT(_TabMovedSlot(int,int)));

        setAttribute(Qt::WA_DeleteOnClose);

        _ReadSettings();
        setFont(wxNote::g_StandardFont);
        setWindowTitle(m_NormalMainWindow->windowTitle());
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
        }

    /* _AdjustCurrentTitle(_TextEditorWindow *)函数实现 */
    void _FinalTabWindow::_AdjustCurrentTitle(const _TextEditorWindow *_EditorWindow)
        {
        QString _CurrentNoteBookName =
                m_NormalMainWindow->_GetNoteBookTree()->currentItem()->text(0);

        QString _CurrentNoteTitle = _EditorWindow->_GetTitleLineEdit()->text();

        setWindowTitle(tr("%1 - %2 - wxNote")
                       .arg(_CurrentNoteTitle.isEmpty() ? wxNote::g_NonTitleNoteName
                                                        : _CurrentNoteTitle)
                       .arg(_CurrentNoteBookName));
        }

    /* _AdjustCurrentTitle(QString)函数实现 */
    void _FinalTabWindow::_AdjustCurrentTitle(const QString &_CurrentNoteName)
        {
        setWindowTitle(tr("%1 - wxNote").arg(_CurrentNoteName));
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* event()事件重写 */
    void _FinalTabWindow::keyPressEvent(QKeyEvent *_Event)
        {
        switch (_Event->key())
            {
        case Qt::Key_W:
            if (_Event->modifiers() & Qt::ControlModifier)
                _CloseSpecifiedTabSlot(this->currentIndex());

        default:
            QTabWidget::keyPressEvent(_Event);
            }
        }

      /* _CreateNormalMainWindow()函数实现 */
    void _FinalTabWindow::_CreateNormalMainWindow()
        {
        m_NormalMainWindow = new _MainWindowNormal;

        connect(m_NormalMainWindow, SIGNAL(_MainWindowNormalTitleChangedSignal(QString)),
                this, SLOT(_NormalMainWindowTitleChangedSlot(QString)));

        connect(m_NormalMainWindow, SIGNAL(_MainWindowNormalTitleChangedSignal(QString)),
                this, SLOT(_SetCurrentTabNameSlot(QString)));
        }

    /* _AdjustAllTabIndex()函数实现 */
    void _FinalTabWindow::_AdjustAllTabIndex()
        {
        for (int _Index = 0; _Index < count(); _Index++)
            {
            if (widget(_Index) != m_NormalMainWindow)
                {
                _MainWindowShadow* _CurrentShadowWindow =
                        dynamic_cast<_MainWindowShadow *>(widget(_Index));

                if (_CurrentShadowWindow)
                    _CurrentShadowWindow->_GetShadowEditorWindow()->_SetIndexInTabWidget(_Index);
                else
                    __PTR_INVALID_ERROR_OUTPUT__(_CurrentShadowWindow)
                }
            }
        }

    /* closeEvent()函数实现 */
    void _FinalTabWindow::closeEvent(QCloseEvent *_Event)
        {
        _WriteSettings();
        _Event->accept();
        }

    /* _WriteSettings()槽实现 */
    void _FinalTabWindow::_WriteSettings()
        {
        wxNote::g_Settings.beginGroup("FinalTabWindow");
            wxNote::g_Settings.setValue("Geometry", saveGeometry());
        wxNote::g_Settings.endGroup();
        }

    /* _ReadSettings()函数实现 */
    void _FinalTabWindow::_ReadSettings()
        {
        wxNote::g_Settings.beginGroup("FinalTabWindow");
            restoreGeometry(wxNote::g_Settings.value("Geometry").toByteArray());
        wxNote::g_Settings.endGroup();
        }

    /* _NormalMainWindowTitleChangedSlot()槽实现 */
    void _FinalTabWindow
        ::_NormalMainWindowTitleChangedSlot(const QString &_NewTitle)
        {
        if (!_NewTitle.isEmpty())
            this->setWindowTitle(_NewTitle);
        }

    /* _SetCurrentTabNameSlot()槽实现
     * 当用户切换笔记本列表时, 改变对应的normal window所在tab标题 */
    void _FinalTabWindow::_SetCurrentTabNameSlot(const QString &/*_NewTitle*/)
        {
        _NoteBookTree* _NoteBookTree = m_NormalMainWindow->_GetNoteBookTree();

        QString _TabTextSuffix = " - wxNote";
        QString _CurrentNoteBookName = _NoteBookTree->currentItem()->text(0);

        for (int _Index = 0; _Index < count(); _Index++)
            {
            if (widget(_Index) == m_NormalMainWindow)
                {
                setTabText(_Index, _CurrentNoteBookName + _TabTextSuffix);
                break;
                }
            }
        }

    /* _CloseSpecifiedTabSlot()槽实现 */
    void _FinalTabWindow::_CloseSpecifiedTabSlot(int _Index)
        {
        /* 如果关闭的是normal窗口所在tab, 那么直接退出程序 */
        if (widget(_Index) == m_NormalMainWindow)
            {
            qApp->quit();
            return;
            }

        /* 其他情况下... */
        using namespace wxNote;

        //////////////////////////////////////////////////////////////////////
        /// 将被关闭的shadow窗口中的shadow editor window, 传回m_NormalMainWindow中
        ///
        _MainWindowShadow* _CurrentShadowWindow =
                                _GetShadowWindow_BySpecifiedIndex(_Index);
        _TextEditorWindow* _CurrentShadowEW =
                                _CurrentShadowWindow->_GetShadowEditorWindow();
        if (_CurrentShadowWindow)
            {
            _ReturnSpecifiedEditorOwnership_WhenClose(_CurrentShadowEW);
            _CurrentShadowEW->_SetParentWindowType(wxNote::_Normal);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentShadowWindow)

        //////////////////////////////////////////////////////////////////////

        QList<_TextEditorWindow *>::iterator _Iter =
            std::remove_if(g_ShadowTextEditorList.begin(), g_ShadowTextEditorList.end(),
                           [this, _Index](const _TextEditorWindow* _Elem)->bool
                                {
                                _MainWindowShadow* _CurrentShadowEditor =
                                                    _GetShadowWindow_BySpecifiedIndex(_Index);

                                return _Elem == _CurrentShadowEditor->_GetShadowEditorWindow();
                                });

        if (_Iter != g_ShadowTextEditorList.end())
            {
            (*_Iter)->_SetIndexInTabWidget(-1);

            g_ShadowTextEditorList.erase(_Iter, g_ShadowTextEditorList.end());
            }

        removeTab(_Index);

        _AdjustAllTabIndex();
        }

    /* _OpenExistShadowWindowSlot()槽实现
     * 当_FinalTabWindow中已经有了该笔记的shadow, 调用该槽 */
    void _FinalTabWindow
        ::_OpenExistShadowWindowSlot(_TextEditorWindow *_ShadowEditorWindow)
        {
        if (_ShadowEditorWindow)
            {
            _MainWindowShadow* _CurrentShadowWindow =
                            _GetShadowWindow_BySpecifiedIndex(_ShadowEditorWindow->_GetIndexInTabWidget());
            _TextEditorWindow* _CurrentShadowEditorWindow =
                        _CurrentShadowWindow->_GetShadowEditorWindow();

            if (_CurrentShadowEditorWindow
                    && _CurrentShadowEditorWindow == _ShadowEditorWindow)
                setCurrentWidget(_CurrentShadowWindow);

            _ShadowEditorWindow->_SetParentWindowType(wxNote::_Shadow);

            _AdjustCurrentTitle(_CurrentShadowEditorWindow);
            }
        else
            __PTR_INVALID_ERROR_OUTPUT__(_ShadowEditorWindow)
        }

    /* _AddShadowWindow2FinalWindowSlot()槽实现
     * 如果_FinalTabWindow中还没有该笔记的shadow, 调用该槽 */
    void _FinalTabWindow
        ::_AddShadowWindow2FinalWindowSlot(_MainWindowShadow *_ShadowWindow,
                                           const QString _CurrentNoteName,
                                           const QIcon &_Icon)
        {
        if (!m_NormalMainWindow->_IsSearchLineEditEmpty())
            m_NormalMainWindow->_ClearSearchLineEdit();

        /* 在添加一个新的编辑窗口时, 获取该影子窗口中的_TextEditorWindow,
         * 以便在_ExchangeEditorWindowOwnershipSlot()中返回给m_MainWindowNormal或
         * 其他笔记项 */
        m_CurrentShadowEditorWindow = _ShadowWindow->_GetShadowEditorWindow();

        int _TabIndex = addTab(_ShadowWindow,
                                _CurrentNoteName.isEmpty() ? wxNote::g_NonTitleNoteName
                                                           : _CurrentNoteName);
        setCurrentIndex(_TabIndex);
        setTabIcon(_TabIndex, _Icon);

        wxNote::g_ShadowTextEditorList.push_back(
                                        _ShadowWindow->_GetShadowEditorWindow());

        m_CurrentShadowEditorWindow->_SetParentWindowType(wxNote::_Shadow);
        m_CurrentShadowEditorWindow->_SetIndexInTabWidget(_TabIndex);

        connect(_ShadowWindow, SIGNAL(_ShowOptions_InShadowWindowSignal()),
                this, SLOT(_ShowOptions_InShadowWindowSlot()));
        }

    /* _ExchangeEditorWindowOwnershipSlot()槽实现 */
    void _FinalTabWindow::_ExchangeEditorWindowOwnershipSlot(int _Index)
        {
        if (!m_NormalMainWindow->_IsSearchLineEditEmpty())
            m_NormalMainWindow->_ClearSearchLineEdit();

        /* 如果是从其他的shadow tab切换到m_NormalWindow的tab...
         * 即从shadow窗口切换到normal窗口时(shadow->normal) */
        if (widget(_Index) == m_NormalMainWindow)
            {
            m_NormalMainWindow->_GetOwnershipFromShadowWindow(m_CurrentShadowEditorWindow);

            m_CurrentShadowEditorWindow =
                    m_NormalMainWindow->_GetNoteList()->_GetCurrentItem()->_GetBindTextEW();

            _ReturnAllEditorOwnership2NormalWindow();

            ///////////////////////////////////////////////////////////////////
            /// 控制权移交后, 如果当前笔记项不在当前笔记本中, 那么选中当前笔记本的第一个笔记项
            ///
            _NoteList* _NormalNoteList = m_NormalMainWindow->_GetNoteList();
            _NoteBookTree* _NormalNoteBookTree = m_NormalMainWindow->_GetNoteBookTree();
            _NoteListItem* _CurrentNoteItem = _NormalNoteList->_GetCurrentItem();

            QString _CurrentNoteBookName = _NormalNoteBookTree->currentItem()->text(0);

            QString _CurrentNoteParentNoteBook =
                            _CurrentNoteItem->_GetParentNoteBookName();

            if (_CurrentNoteBookName != _CurrentNoteParentNoteBook)
                {
                _NoteListItem* _FirstNote =
                        m_NormalMainWindow->_GetFirstNoteInSpecifiedNoteBook(_CurrentNoteBookName);

                _NormalNoteList->setCurrentItem(_FirstNote);
                }

            ///////////////////////////////////////////////////////////////////
            }
        else /* 如果是从m_NormalMainWindow的tab切换到其他的shadow tab
              * 即从normal窗口切换到shadow窗口时(normal->shadow)
              * 或者时从shadow窗口切换到shadow窗口(shadow->shadow) */
            {
            _MainWindowShadow* _ShadowDoubleClickedWin =
                                        _GetShadowWindow_BySpecifiedIndex(_Index);

            QString _CurrentTabText = this->tabText(_Index); // 指定索引tab的文本
            QString _CurrentNoteTitle = // 当前笔记的标题
                    m_NormalMainWindow->_GetNoteList()->_GetCurrentItemTitle();

            if (_ShadowDoubleClickedWin)
                {
                if (_CurrentNoteTitle == _CurrentTabText)
                    {
                    m_CurrentShadowEditorWindow =
                            _ShadowDoubleClickedWin->_GetShadowEditorWindow();

                    /* 如果切换到的shadow窗口与normal中的当前笔记是匹配的... */
                    _ShadowDoubleClickedWin->_AddShadowEditorWindow(m_CurrentShadowEditorWindow);

                    m_CurrentShadowEditorWindow->_GetTextEditor()->setFocus();

                    m_CurrentShadowEditorWindow->_SetParentWindowType(wxNote::_Shadow);

                    _AdjustCurrentTitle(_ShadowDoubleClickedWin->_GetShadowEditorWindow());
                    }
                else
                    { /* 如果不是匹配的, 从已打开的shadow窗口中中找到
                       * 标题与当前切换到的tab文本相同的笔记编辑窗口, 然后交换窗口的所有权 */
                    _TextEditorWindow* _MatchedEditorWindow =
                                            _FindMatchedEditorWindow(_Index);

                    if (_MatchedEditorWindow)
                        {
                        _ShadowDoubleClickedWin->_AddShadowEditorWindow(_MatchedEditorWindow);

                        m_CurrentShadowEditorWindow = _MatchedEditorWindow;

                        m_CurrentShadowEditorWindow->_GetTextEditor()->setFocus();

                        _MatchedEditorWindow->_SetParentWindowType(wxNote::_Shadow);

                        _AdjustCurrentTitle(_MatchedEditorWindow);
                        }
                    else
                        __PTR_INVALID_ERROR_OUTPUT__(_MatchedEditorWindow)
                    }
                }
            else
                __PTR_INVALID_ERROR_OUTPUT__(_ShadowDoubleClickedWin)
            }
        }

    /* _ChangeMainWindowCurrentTitle(int)槽实现 */
    void _FinalTabWindow::_ChangeMainWindowCurrentTitle(int _TabIndex)
        {
        if (widget(_TabIndex) != m_NormalMainWindow)
            {
            _MainWindowShadow* _CurrentShadowWindow =
                                    _GetShadowWindow_BySpecifiedIndex(_TabIndex);

            _TextEditorWindow* _CurrentShadowEditorWindow
                    = _CurrentShadowWindow->_GetShadowEditorWindow();

            QString _CurrentNoteName = tabText(_TabIndex);
            QString _CurrentNoteBookName =
                        _CurrentShadowEditorWindow->_GetParentNoteBookName_Current();

            setWindowTitle(tr("%1 - %2 - wxNote")
                           .arg(_CurrentNoteName).arg(_CurrentNoteBookName));
            }
        }

    /* _ChangeMainWindowCurrentTitle(QString)槽实现 */
    void _FinalTabWindow::_ChangeMainWindowCurrentTitle(const _TextEditorWindow */*_CurrentEW*/,
                                                        const QString &_NewTitle)
        {
        QString _CurrentNoteBook =
                m_NormalMainWindow->_GetNoteBookTree()->currentItem()->text(0);

        /////////////////////////////////////////////////////////////////////
        ///     当在m_NormalMainWindow中改变一个拥有shadow窗口的笔记的标题时,
        ///     使该笔记的shadow窗口的tab文本随之同步
        ///
        if (count() > 1)
            {
            _NoteList* _NormalNoteList = m_NormalMainWindow->_GetNoteList();
            _NoteListItem* _CurrentNoteItem = _NormalNoteList->_GetCurrentItem();

            for (_TextEditorWindow* _Elem : wxNote::g_ShadowTextEditorList)
                {
                if (_CurrentNoteItem->_GetBindTextEW() == _Elem)
                    {
                    int _TabIndex = _Elem->_GetIndexInTabWidget();

                    setTabText(_TabIndex, _NewTitle.isEmpty()
                                        ? wxNote::g_NonTitleNoteName : _NewTitle);
                    }
                }
            }

        /////////////////////////////////////////////////////////////////////

        setWindowTitle(tr("%1 - %2 - wxNote")
                       .arg(_NewTitle.isEmpty() ? wxNote::g_NonTitleNoteName
                                                : _NewTitle)
                       .arg(_CurrentNoteBook));
        }

    /* _ChangeCurrentTabTextSlot()槽实现 */
    void _FinalTabWindow::_ChangeCurrentTabTextSlot(const _TextEditorWindow *_CurrentEW,
                                                    const QString &_Text)
        {
        for (int _Index = 0; _Index < count(); _Index++)
            if (widget(_Index) == _CurrentEW)
                setTabText(_Index, _Text.isEmpty()
                                        ? wxNote::g_NonTitleNoteName : _Text);
        }

    /* _ChangeCurrentTabIcon()槽实现 */
    void _FinalTabWindow::_ChangeCurrentTabIconSlot(const QString &_NoteTitle,
                                                    const QString &_IconPath)
        {
        if (count() > 1)
            {
            for (int _Index = 0; _Index < count(); _Index++)
                {
                if (tabText(_Index) == _NoteTitle)
                    {
                    setTabIcon(_Index, QIcon(_IconPath));

                    break;
                    }
                }
            }
        }

    /* _Move2NextTabSlot()槽实现啊 */
    void _FinalTabWindow::_Move2NextTabSlot()
        {
        if (count() > 1)
            {
            int _CurrentIndex = currentIndex();

            if (_CurrentIndex == count() - 1 /* 如果此时到达了tab组的末尾... */)
                setCurrentIndex(0); /* 切换的第一个tab */
            else
                setCurrentIndex(++_CurrentIndex);
            }
        else
            {
            _NoteList* _NormalNoteList = m_NormalMainWindow->_GetNoteList();
            int _CurrentNoteListIndex = _NormalNoteList->currentRow();

            if (_CurrentNoteListIndex == _NormalNoteList->count() - 1)
                _NormalNoteList->setCurrentRow(0);
            else
                _NormalNoteList->setCurrentRow(++_CurrentNoteListIndex);
            }
        }

    /* _CloseCurrentNoteShadowWindowSlot()槽实现
     * 当用户在m_NormalMainWindow中消除一个笔记时, 检查该笔记是否拥有shadow窗口,
     * 如果有, 将shadow窗口关闭 */
    void _FinalTabWindow
        ::_CloseCurrentNoteShadowWindowSlot(const _TextEditorWindow *_EditorWindow)
        {
        int _MatchedTabIndex = _EditorWindow->_GetIndexInTabWidget();

        if (_MatchedTabIndex >= 0)
            _CloseSpecifiedTabSlot(_MatchedTabIndex);
        }

    /* _ShowOptions_InShadowWindowSlot()槽实现 */
    void _FinalTabWindow::_ShowOptions_InShadowWindowSlot()
        {
        setCurrentWidget(m_NormalMainWindow);

        m_NormalMainWindow->_OptionsSlot();
        }

    /* _TabMovedSlot()槽实现
     * 当用户移动tab后, 重新决定每个tab中的shadow编辑窗口的索引 */
    void _FinalTabWindow::_TabMovedSlot(int /*_From*/, int /*_2*/)
        {
        _AdjustAllTabIndex();
        }

    /* _FindMatchedEditorWindow(int)函数实现
     * 通过给定的tab索引查找到匹配的shadow编辑窗口 */
    _TextEditorWindow* _FinalTabWindow::_FindMatchedEditorWindow(int _TabIndex)
        {
        using namespace wxNote;

        if (_TabIndex < 0)
            return nullptr;

        _AdjustAllTabIndex();

        QList<_TextEditorWindow *>::const_iterator _MatchIter =
                std::find_if(g_ShadowTextEditorList.cbegin(), g_ShadowTextEditorList.cend(),
                             [this, _TabIndex](const _TextEditorWindow* _Elem)
                                    {
                                    return _Elem->_GetIndexInTabWidget() == _TabIndex;
                                    });
        return *_MatchIter;
        }

    /* _ReturnAllEditorOwnership2NormalWindow()函数实现
     * 将所有shadow窗口中的编辑窗口控制权全部返回给m_NormalWindow */
    void _FinalTabWindow::_ReturnAllEditorOwnership2NormalWindow()
        {
        using namespace wxNote;

        _NoteList* _NormalNoteList = m_NormalMainWindow->_GetNoteList();

        QStackedLayout* _NormalStackLayout = m_NormalMainWindow->_GetNoteEditorStackedLayout();

        for (int _ShadowIndex = 0; _ShadowIndex < g_ShadowTextEditorList.count(); _ShadowIndex++)
            {
            for (int _ItemIndex = 0; _ItemIndex < _NormalNoteList->count(); _ItemIndex++)
                {
                _TextEditorWindow* _CurrentEW = g_ShadowTextEditorList.at(_ShadowIndex);
                _TextEditorWindow* _CurrentBindEW = _NormalNoteList->_Item(_ItemIndex)->_GetBindTextEW();

                if (_CurrentEW == _CurrentBindEW)
                    {
                    _NormalStackLayout->setCurrentIndex(_ItemIndex);

                    _NoteEditorSplitter* _CurrentSplitter =
                            dynamic_cast<_NoteEditorSplitter *>(_NormalStackLayout->currentWidget());

                    g_ShadowTextEditorList.at(
                                _ShadowIndex)->_SetParentWindowType(wxNote::_Normal);

                    _CurrentSplitter->addWidget(g_ShadowTextEditorList.at(_ShadowIndex));
                    }
                }
            }

        _NormalStackLayout->setCurrentIndex(_NormalNoteList->currentRow());
        }

    /* _ReturnSpecifiedEditorOwnership_WhenClose()函数实现 */
    void _FinalTabWindow
        ::_ReturnSpecifiedEditorOwnership_WhenClose(_TextEditorWindow *_RetEditor)
        {
        using namespace wxNote;

        _NoteList* _NormalNoteList = m_NormalMainWindow->_GetNoteList();
        QStackedLayout* _NormalStackLayout = m_NormalMainWindow->_GetNoteEditorStackedLayout();

        for (int _ItemIndex = 0; _ItemIndex < _NormalNoteList->count(); _ItemIndex++)
            {
            _NoteListItem* _CurrentNoteItem = _NormalNoteList->_Item(_ItemIndex);

            if (_CurrentNoteItem->_GetBindTextEW() == _RetEditor)
                {
                _NormalNoteList->setCurrentRow(_ItemIndex);

                _NoteEditorSplitter* _CurrentSplitter =
                        dynamic_cast<_NoteEditorSplitter *>(_NormalStackLayout->currentWidget());

                _CurrentSplitter->addWidget(_RetEditor);

                _RetEditor->_SetParentWindowType(wxNote::_Normal);
                break;
                }
            }
        }

    /* _GetShadowWindow_BySpecifiedIndex()函数实现
     * 获取与指定索引对应的shadow窗口 */
    _MainWindowShadow* _FinalTabWindow
        ::_GetShadowWindow_BySpecifiedIndex(int _TabIndex)
        {
        _MainWindowShadow* _CurrentShadowWindow =
                dynamic_cast<_MainWindowShadow *>(widget(_TabIndex));

        if (_CurrentShadowWindow)
            return _CurrentShadowWindow;
        else
            {
            __PTR_INVALID_ERROR_OUTPUT__(_CurrentShadowWindow)

            return nullptr;
            }
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
