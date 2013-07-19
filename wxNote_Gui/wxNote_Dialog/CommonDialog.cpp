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

#include "wxNote_Gui/wxNote_Dialog/CommonDialog.h"

#include "wxNote_Global.h"

#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLayout>

//.._CommonDialog类实现

    /* 构造函数实现 */
    _CommonDialog::_CommonDialog(QWidget *_Parent,
                                 const QString &_Title,
                                 const QString &_Label,
                                 const QString &_OKButtonName,
                                 const QString &_CancelButtonName)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
        {
        m_LogoLabel = new QLabel("<img src=\":/wxNote_Icons/wxNoteicon.png\">");
        m_QuestionLabel = new QLabel(_Label);

        m_OKButton = new QPushButton(_OKButtonName);
        m_CancelButton = new QPushButton(_CancelButtonName);

        connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(m_CancelButton, SIGNAL(clicked()), this, SLOT(reject()));

        QHBoxLayout* _BottomLayout = new QHBoxLayout;
        _BottomLayout->addStretch();
        _BottomLayout->addWidget(m_OKButton);

        if (!_CancelButtonName.isEmpty() /* 如果用户没有指定Cancel按钮的名称... */)
             /* 代表用户不想要这个按钮在对话框中显示出来 */
            _BottomLayout->addWidget(m_CancelButton);

        QVBoxLayout* _RightLayout = new QVBoxLayout;
        _RightLayout->addWidget(m_QuestionLabel);
        _RightLayout->addLayout(_BottomLayout);

        QHBoxLayout* _MainLayout = new QHBoxLayout;
        _MainLayout->addWidget(m_LogoLabel);
        _MainLayout->addLayout(_RightLayout);

        setLayout(_MainLayout);
        setFixedSize(sizeHint());
        setWindowTitle(_Title);
        setFont(wxNote::g_StandardFont);
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
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
