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
#include <QTabWidget>

class _MainWindowNormal;
class _TextEditorWindow;
class _MainWindowShadow;

class _FinalTabWindow : public QTabWidget
    {
    Q_OBJECT

public:
    _FinalTabWindow(QWidget* _Parent = nullptr);

protected slots:
    void _WriteSettings();
    void _ReadSettings();

    void _NormalMainWindowTitleChangedSlot(const QString& _NewTitle);
    void _SetCurrentTabNameSlot(const QString& _NewTitle);

    void _CloseSpecifiedTabSlot(int _Index);

    /* 当_FinalTabWindow中已经有了该笔记的shadow, 调用该槽 */
    void _OpenExistShadowWindowSlot(_TextEditorWindow* _ShadowEditorWindow);

    /* 如果_FinalTabWindow中还没有该笔记的shadow, 调用该槽 */
    void _AddShadowWindow2FinalWindowSlot(_MainWindowShadow* _ShadowWindow,
                                          const QString _CurrentNoteName,
                                          const QIcon& _Icon);

    void _ExchangeEditorWindowOwnershipSlot(int _Index);

    void _ChangeMainWindowCurrentTitle(int _TabIndex);
    void _ChangeMainWindowCurrentTitle(const _TextEditorWindow* _CurrentEW,
                                       const QString& _NewTitle);
    void _ChangeCurrentTabTextSlot(const _TextEditorWindow* _CurrentEW,
                                   const QString& _Text);
    void _ChangeCurrentTabIconSlot(const QString& _NoteTitle,
                                   const QString& _IconPath);

    void _Move2NextTabSlot();

    void _CloseCurrentNoteShadowWindowSlot(const _TextEditorWindow* _EditorWindow);

    void _ShowOptions_InShadowWindowSlot();

    void _TabMovedSlot(int _From, int _2);

    void _AdjustCurrentTitle(const _TextEditorWindow* _EditorWindow);
    void _AdjustCurrentTitle(const QString& _CurrentNoteName);

protected:
    void keyPressEvent(QKeyEvent *_Event);
    void closeEvent(QCloseEvent *_Event);

    void _CreateNormalMainWindow();
    void _AdjustAllTabIndex();

    /* 从wxNote::g_ShadowTextEditorList中找到
     * 标题与给定索引的tab的文本相同的笔记编辑窗口, 并返回该窗口的指针 */
    _TextEditorWindow* _FindMatchedEditorWindow(int _TabIndex);

    /* 将所有shadow窗口中的编辑窗口控制权全部返回给m_NormalWindow */
    void _ReturnAllEditorOwnership2NormalWindow();

    /* 将指定的shadow窗口中的编辑窗口控制器返回给m_NormalWindow */
    void _ReturnSpecifiedEditorOwnership_WhenClose(_TextEditorWindow* _RetEditor);

    _MainWindowShadow* _GetShadowWindow_BySpecifiedIndex(int _TabIndex);

    _MainWindowNormal* m_NormalMainWindow;

    /* 用于追踪整个程序中的当前shadow编辑窗口 */
    _TextEditorWindow* m_CurrentShadowEditorWindow;
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
