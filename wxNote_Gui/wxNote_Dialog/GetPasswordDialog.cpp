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

#include "wxNote_Gui/wxNote_Dialog/GetPasswordDialog.h"
#include "wxNote_Gui/wxNote_Dialog/CommonDialog.h"

#include "wxNote_Global.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRegExp>
#include <QLayout>
#include <QValidator>
#include <QMessageBox>
#include <QDialogButtonBox>

//.._GetPasswordDialog类实现

    /* 构造函数实现 */
    _GetPasswordDialog::_GetPasswordDialog(QWidget *_Parent)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
        {
        m_GetPasswordLabel = new QLabel(tr("请输入解锁密码(&I):"));

        m_UnlockFailedLabel = new QLabel("<img src=\":/wxNote_Icons/passwordWrong.png\">"
                                         "\t解锁失败！键入的密码不正确。");
        m_UnlockFailedLabel->setHidden(true);

        QRegExp _PasswordFormat("[A-Za-z0-9,./?_!@#$%^&*()-+=~`]{0,16}");

        m_GetPasswordLineEdit = new QLineEdit;
        m_GetPasswordLineEdit->setValidator(new QRegExpValidator(_PasswordFormat, this));
        m_GetPasswordLineEdit->setEchoMode(QLineEdit::Password);
        m_GetPasswordLabel->setBuddy(m_GetPasswordLineEdit);

        m_VisiblePasswordCheckBox = new QCheckBox(tr("显示密码(&V)"));
        connect(m_VisiblePasswordCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(_VisiblePasswordChangeSlot(bool)));

        m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);
        m_ButtonBox->button(QDialogButtonBox::Ok)->setText(tr("好的"));

        m_ButtonBox->button(QDialogButtonBox::Cancel)->setText(tr("离开"));

        connect(m_ButtonBox, SIGNAL(accepted()), this, SLOT(_ValidatePassword()));
        connect(m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

        QHBoxLayout* _CheckBoxLayout = new QHBoxLayout;
        _CheckBoxLayout->addStretch();
        _CheckBoxLayout->addWidget(m_VisiblePasswordCheckBox);

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_GetPasswordLabel);
        _MainLayout->addWidget(m_GetPasswordLineEdit);
        _MainLayout->addWidget(m_UnlockFailedLabel);
        _MainLayout->addLayout(_CheckBoxLayout);
        _MainLayout->addStretch();
        _MainLayout->addWidget(m_ButtonBox);

        setLayout(_MainLayout);
        setWindowTitle(tr("笔记解锁"));
        setWindowIcon(QIcon(":/wxNote_Icons/notlock.png"));
        setFont(wxNote::g_StandardFont);
        setFixedSize(QSize(sizeHint().width() + 72, 155));
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CheckPasswordWhenClickedOkSlot()槽实现 */
    void _GetPasswordDialog::_VisiblePasswordChangeSlot(bool _IsVisibled)
        {
        m_GetPasswordLineEdit->setEchoMode(_IsVisibled ? QLineEdit::Normal
                                                       : QLineEdit::Password);
        }

    /* _ValidatePassword()槽实现 */
    void _GetPasswordDialog::_ValidatePassword()
        {
        using namespace wxNote;

        g_Settings.beginGroup("TextEditor");
            QString _Password = g_Settings.value("NotePassWord").toString();
        g_Settings.endGroup();

        if (m_GetPasswordLineEdit->text() == _Password)
            accept();
        else
            _SetFailedUnlockLabelHidden(false);
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

