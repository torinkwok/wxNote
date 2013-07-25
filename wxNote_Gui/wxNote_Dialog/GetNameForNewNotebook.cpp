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

#include "wxNote_Gui/wxNote_Dialog/GetNameForNewNotebook.h"
#include "wxNote_Gui/wxNote_Button/NestingButton.h"

#include "wxNote_Global.h"

#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QToolButton>

//.._GetNameForNewNoteBook类实现

    /* 构造函数实现 */
    _GetNameForNewNoteBookDialog
        ::_GetNameForNewNoteBookDialog(QWidget *_Parent)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
        {
        m_NameLabel = new QLabel(tr("笔记本名称(&N)："));
        m_NameLineEdit = new QLineEdit;
        m_NameLineEdit->setMaxLength(40);
        m_NameLabel->setBuddy(m_NameLineEdit);

        m_BackSpaceToolButton = new _NestingButton(QIcon(":/wxNote_Icons/backSpace.png"),
                                                   m_NameLineEdit);

        m_BackSpaceToolButton->setFixedSize(m_NameLineEdit->sizeHint().height() + 5,
                                            m_NameLineEdit->sizeHint().height());

        connect(m_NameLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(_LineEditTextChanged(QString)));
        connect(m_BackSpaceToolButton, SIGNAL(clicked()),
                this, SLOT(_LineEditBackSpace()));

        m_WaringLabel = new QLabel(tr("<img src=\":/wxNote_Icons/wrongful.png\">"
                                      "\t笔记本名不能包含下列任何字符："
                                      "<p>如 \\ / : * ? &quot; &lt; &gt; |"));
        m_WaringLabel->setHidden(true);

        m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

        QPushButton* _OKButton =
                m_ButtonBox->button(QDialogButtonBox::Ok);
        _OKButton->setText(tr("创建"));
        _OKButton->setDefault(true);
        _OKButton->setEnabled(false);

        QPushButton* _CancelButton =
                m_ButtonBox->button(QDialogButtonBox::Cancel);
        _CancelButton->setText(tr("取消"));

        connect(m_ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_NameLabel);
        _MainLayout->addWidget(m_NameLineEdit);
        _MainLayout->addWidget(m_WaringLabel);
        _MainLayout->addStretch();
        _MainLayout->addWidget(m_ButtonBox);

        setLayout(_MainLayout);
        setFixedSize(250, sizeHint().height() + 70);
        setWindowTitle(tr("新建笔记本"));
        setFont(wxNote::g_StandardFont);
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
        }

    /* _GetLineEditText()函数实现 */
    QString _GetNameForNewNoteBookDialog::_GetLineEditText() const
        {
        return m_NameLineEdit->text();
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _LineEditTextChanged()槽实现 */
    void _GetNameForNewNoteBookDialog
        ::_LineEditTextChanged(const QString &_CurrentText)
        {
        bool _ContainsWrongful(
                _CurrentText.contains('\\') || _CurrentText.contains('/')
                    || _CurrentText.contains(':') || _CurrentText.contains('*')
                    || _CurrentText.contains('?') || _CurrentText.contains('"')
                    || _CurrentText.contains('<') || _CurrentText.contains('>')
                    || _CurrentText.contains('|'));

        m_WaringLabel->setHidden(!_ContainsWrongful);

        m_ButtonBox->button(QDialogButtonBox::Ok)
                        ->setEnabled(!_CurrentText.isEmpty() && !_ContainsWrongful);
        }

    /* _LineEditBackSpace()槽实现 */
    void _GetNameForNewNoteBookDialog::_LineEditBackSpace()
        {
        m_NameLineEdit->backspace();
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
