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
#include <QDialog>

class QTabWidget;
class QLabel;
class QGroupBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QCheckBox;
class QDialogButtonBox;

class _FontComboBox;
class _FontSizeComboBox;

class _OptionsDialog : public QDialog
    {
    Q_OBJECT

public:
    _OptionsDialog(QWidget* _Parent = nullptr);

    _FontComboBox* _GetFontComboBox() const
                    { return m_FontComboBox; }
    _FontSizeComboBox* _GetFontSizeComboBox() const
                    { return m_FontSizeComboBox; }
    QCheckBox* _GetAcceptRichTextCheckBox() const
                    { return m_AcceptRichTextCheckBox; }
    bool _GetIsChanged() const
                    { return mb_IsChanged; }

protected slots:
    void _SetCurrentSettingsChanged();

protected:
    enum { _FontMinSize = 8, _FontMaxSize = 96 };

    void _CreateNoteOptionsPage();

    QTabWidget*       m_TabWidgetOnDialog;
    QDialogButtonBox* m_ButtonBox;

    bool mb_IsChanged;

    /* "笔记"选项 */
    QWidget* m_NoteOptionPage;

    QLabel*  m_NoteEditorFontLabel;

    _FontComboBox*     m_FontComboBox;
    _FontSizeComboBox* m_FontSizeComboBox;

    QGroupBox*   m_FontGroupBox;
    QHBoxLayout* m_ComboBoxLayout;
    QVBoxLayout* m_FontLayout;

    QCheckBox*   m_AcceptRichTextCheckBox;
    QGroupBox*   m_AcceptGroupBox;
    QVBoxLayout* m_AcceptLayout;
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

