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

#include "wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h"

#include <QFont>
#include <QFocusEvent>

//.._NoteTitleLineEdit类实现

    /* 构造函数实现 */
    _NoteTitleLineEdit::_NoteTitleLineEdit(QWidget *_Parent)
        : QLineEdit(_Parent)
        {
        m_FontFamily = "微软雅黑,12,-1,5,50,0,0,0,0,0";

        setStyleSheet("color:darkGray");
        setFont(QFont(m_FontFamily, 15, 15, true));
        setCursorMoveStyle(Qt::LogicalMoveStyle);
        setFrame(false);
        setPlaceholderText(tr("在此设置标题..."));

        connect(this, SIGNAL(textChanged(QString)),
                this, SLOT(_CurrentTextChanged(QString)));
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _SetIsItalic()函数实现 */
    void _NoteTitleLineEdit::_SetIsItalic(bool _IsItaic)
        {
        setFont(QFont(m_FontFamily, 15, 15, _IsItaic));
        }

    /* _CurrentTextChaned()槽实现 */
    void _NoteTitleLineEdit::_CurrentTextChanged(const QString &_Text)
        {
        if (!_Text.isEmpty())
            {
            /* 如果用户输入了文字, 那么就取消文字的斜体 */
            _SetIsItalic(false);
            setStyleSheet("color:rgb(10, 149, 216)");
            }
        else
            {
            _SetIsItalic(true);
            setStyleSheet("color:darkGray");
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

