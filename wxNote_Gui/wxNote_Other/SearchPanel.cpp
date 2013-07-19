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

#include "SearchPanel.h"
#include "wxNote_Gui/wxNote_Button/NestingButton.h"

#include <QLineEdit>
#include <QLayout>
#include <QIcon>

//.._SearchPanel类实现

    /* 构造函数实现 */
    _SearchPanel::_SearchPanel(QWidget *_Parent)
        : QWidget(_Parent)
        {
        m_FontFamily = "微软雅黑,12,-1,5,50,0,0,0,0,0";

        m_SearchLineEdit = new QLineEdit;
        m_SearchLineEdit->setPlaceholderText(tr("搜索笔记"));
        m_SearchLineEdit->setStyleSheet("color : rgb(88, 88, 88)");
        m_SearchLineEdit->setFont(QFont(m_FontFamily, 10, 10, true));
        m_SearchLineEdit->setFixedHeight(_SearchLineEditMimimumHeight);

        m_ControlToolButton = new _NestingButton(QIcon(":/wxNote_Icons/beginSearch.png"),
                                                m_SearchLineEdit);

        m_SearchHorizontalLayout = new QHBoxLayout;
        m_SearchHorizontalLayout->addWidget(m_SearchLineEdit);

        connect(m_SearchLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(_SearchLineEditBeginSearchSlot(QString)));
        connect(m_ControlToolButton, SIGNAL(clicked()),
                this, SLOT(_ControlToolButtonClicked()));

        this->setLayout(m_SearchHorizontalLayout);
        this->setFixedHeight(_SearchPanelMinimumHeight);
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _SearchLineEditBeginSearchSlot()槽实现 */
    void _SearchPanel::_SearchLineEditBeginSearchSlot(const QString &_Text)
        {
        if (!_Text.isEmpty())
            {
            m_SearchLineEdit->setStyleSheet("color : black");
            _SetSearchLineIsItalic(false);

            m_ControlToolButton->setIcon(QIcon(":wxNote_Icons/stopSearch.png"));
            m_ControlToolButton->setToolTip(tr("结束搜索"));
            }
        else
            {
            m_SearchLineEdit->setStyleSheet("color : rgb(88, 88, 88)");
            _SetSearchLineIsItalic(true);

            m_ControlToolButton->setIcon(QIcon(":wxNote_Icons/beginSearch.png"));
            m_ControlToolButton->setToolTip(tr("开始搜索"));
            }
        }

    /* _ControlToolButtonClicked()槽实现 */
    void _SearchPanel::_ControlToolButtonClicked()
        {
        if (m_ControlToolButton->toolTip() == tr("结束搜索"))
            {
            m_SearchLineEdit->clear();

            emit _StopFilterSignal();
            // TODO
            }
        else
            emit _BeginFilterSignal();
        }

    /* _SetSearchLineIsItalic()函数实现 */
    void _SearchPanel::_SetSearchLineIsItalic(bool _IsItaic)
        {
        m_SearchLineEdit->setFont(QFont(m_FontFamily, 10, 10, _IsItaic));
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


