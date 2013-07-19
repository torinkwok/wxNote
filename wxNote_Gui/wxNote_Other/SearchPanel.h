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
#include <QWidget>

class _NestingButton;

class QHBoxLayout;
class QLineEdit;

class _SearchPanel : public QWidget
    {
    Q_OBJECT

public:
    _SearchPanel(QWidget* _Parent = nullptr);

    QLineEdit* _GetSearchLineEdit() const
            { return m_SearchLineEdit; }

signals:
    void _BeginFilterSignal();
    void _StopFilterSignal();

protected slots:
    void _SearchLineEditBeginSearchSlot(const QString& _Text);
    void _ControlToolButtonClicked();

protected:
    enum { _SearchLineEditMimimumHeight = 23 };
    enum { _SearchPanelMinimumHeight = 41 };

    inline void _SetSearchLineIsItalic(bool _IsItaic);

    QString m_FontFamily;

    _NestingButton* m_ControlToolButton;

    QLineEdit*     m_SearchLineEdit;
    QHBoxLayout*   m_SearchHorizontalLayout;
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
