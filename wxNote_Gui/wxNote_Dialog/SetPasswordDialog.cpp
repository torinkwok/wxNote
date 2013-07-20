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

#include "wxNote_Gui/wxNote_Dialog/SetPasswordDialog.h"

#include "wxNote_Global.h"

#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLayout>
#include <QPushButton>
#include <QIcon>
#include <QMessageBox>
#include <QCheckBox>
#include <QRegExp>
#include <QRegExpValidator>

//.._SetPasswordDialog类实现

    /* 构造函数实现 */
    _SetPasswordDialog::_SetPasswordDialog(QWidget *_Parent)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
        {
        m_SetPasswordLabel = new QLabel(tr("为确保您的笔记不被无意或恶意篡改，"
                                           "<p>请为笔记设置解锁密码(&S)："));

        QRegExp _PasswordFormat("[A-Za-z0-9,./?_!@#$%^&*()-+=~`]{0,16}");

        m_SetPasswordLineEdit = new QLineEdit;
        m_SetPasswordLineEdit->setValidator(new QRegExpValidator(_PasswordFormat, this));
        m_SetPasswordLineEdit->setMaxLength(16);
        m_SetPasswordLineEdit->setEchoMode(QLineEdit::Password);
        m_SetPasswordLabel->setBuddy(m_SetPasswordLineEdit);

        m_ConfirmPasswordLabel = new QLabel(tr("确认密码(&C)："));

        m_ConfirmPasswordLineEdit = new QLineEdit;
        m_ConfirmPasswordLineEdit->setValidator(new QRegExpValidator(_PasswordFormat, this));
        m_ConfirmPasswordLineEdit->setMaxLength(16);
        m_ConfirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
        m_ConfirmPasswordLabel->setBuddy(m_ConfirmPasswordLineEdit);

        /* 只有当用户在m_SetPasswordLineEdit中输入密码之后才可以解禁 */
        m_ConfirmPasswordLineEdit->setEnabled(false);
        connect(m_SetPasswordLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(_ConfirmLineEditEnabled(QString)));

        m_VisiblePasswordCheckBox = new QCheckBox(tr("显示密码(&V)"));
        connect(m_VisiblePasswordCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(_VisiblePasswordChangeSlot(bool)));

        m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);
        m_ButtonBox->button(QDialogButtonBox::Ok)->setText(tr("好的"));

        m_ButtonBox->button(QDialogButtonBox::Cancel)->setText(tr("离开"));

        connect(m_ButtonBox, SIGNAL(accepted()),
                this, SLOT(_CheckPasswordWhenClickedOkSlot()));
        connect(m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

        QHBoxLayout* _CheckBoxLayout = new QHBoxLayout;
        _CheckBoxLayout->addStretch();
        _CheckBoxLayout->addWidget(m_VisiblePasswordCheckBox);

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_SetPasswordLabel);
        _MainLayout->addWidget(m_SetPasswordLineEdit);
        _MainLayout->addWidget(m_ConfirmPasswordLabel);
        _MainLayout->addWidget(m_ConfirmPasswordLineEdit);
        _MainLayout->addLayout(_CheckBoxLayout);
        _MainLayout->addStretch();
        _MainLayout->addWidget(m_ButtonBox);

        setLayout(_MainLayout);
        setWindowTitle(tr("设置笔记解锁密码"));
        setWindowIcon(QIcon(":/wxNote_Icons/locking.png"));
        setFont(wxNote::g_StandardFont);
        setFixedSize(QSize(sizeHint().width() + 24, 230));
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CheckPasswordWhenClickedOkSlot()槽实现 */
    void _SetPasswordDialog::_CheckPasswordWhenClickedOkSlot()
        {
        if (m_SetPasswordLineEdit->text().isEmpty())
            {
            QMessageBox::warning(this
                                 , tr("Warning")
                                 , tr("密码不能为空！请重新输入。")
                                 , QMessageBox::Ok);
            return;
            } else if (m_ConfirmPasswordLineEdit->text().isEmpty())
                {
                QMessageBox::warning(this
                                     , tr("Warning")
                                     , tr("请确认密码无误！")
                                     , QMessageBox::Ok);
                return;
                }

        if (m_SetPasswordLineEdit->text() == m_ConfirmPasswordLineEdit->text())
            {
            using namespace wxNote;

            g_Settings.beginGroup("TextEditor");
                g_Settings.setValue("NotePassWord", m_ConfirmPasswordLineEdit->text());
            g_Settings.endGroup();

            accept();
            }
        else
            {
            QMessageBox::warning(this
                                 , tr("Warning")
                                 , tr("两次密码不一致，请核对后重新设置。")
                                 , QMessageBox::Ok);
            }
        }

    /* _VisiblePasswordChangeSlot()槽实现 */
    void _SetPasswordDialog::_VisiblePasswordChangeSlot(bool _IsVisibled)
        {
        if (_IsVisibled)
            {
            m_SetPasswordLineEdit->setEchoMode(QLineEdit::Normal);
            m_ConfirmPasswordLabel->setHidden(true);
            m_ConfirmPasswordLineEdit->setHidden(true);

            /* 如果用户选择了"显示密码", 那么将不会显示"确认密码"的QLineEdit,
             * 可以隐式地把m_ConfirmPasswordLineEdit中的密码设置为
             * m_SetPasswordLineEdit中的密码, 可以不用用户输入 */
            m_ConfirmPasswordLineEdit->setText(m_SetPasswordLineEdit->text());
            }
        else
            {
            m_SetPasswordLineEdit->setEchoMode(QLineEdit::Password);
            m_ConfirmPasswordLabel->setHidden(false);
            m_ConfirmPasswordLineEdit->setHidden(false);

            /* 如果用户又取消了"显示密码", 那么就把m_ConfirmPasswordLineEdit
             * 中的密码清空, 等待用户输入 */
            m_ConfirmPasswordLineEdit->clear();
            }
        }

    /* _ConfirmLineEditEnabled()槽实现 */
    void _SetPasswordDialog::_ConfirmLineEditEnabled(QString _Text)
        {
        m_ConfirmPasswordLineEdit->setEnabled(!_Text.isEmpty());
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
