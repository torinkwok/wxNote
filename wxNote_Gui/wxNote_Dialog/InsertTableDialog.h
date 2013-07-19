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

class QLabel;
class QSpinBox;
class QDialogButtonBox;
class QGroupBox;
class QComboBox;
class QGridLayout;

class _InsertTableDialog : public QDialog
    {
    Q_OBJECT

public:
    _InsertTableDialog(QWidget* _Parent = nullptr);

    int _GetRowCnt() const
            { return m_RowCnt; }
    int _GetColumnCnt() const
            { return m_ColumnCnt; }
    int _GetTableWidth() const
            { return m_TableWidth; }

protected slots:
    void _RowCntSpinBoxValChanged(int _NewVal);
    void _ColumnCntSpinBoxValChanged(int _NewVal);
    void _TableWidthSpinBoxValChanged(int _NewVal);

    void _SetOKButtonEnabled();

protected:
    void _CreateWidgetsInGroupBox();
    void _CreateSizeGroupBox();

    QLabel*   m_RowCountLabel;
    QLabel*   m_ColumnCountLabel;
    QLabel*   m_TableWidthLabel;

    QSpinBox* m_RowCountSpinBox;
    QSpinBox* m_ColumnCountSpinBox;
    QSpinBox* m_TableWidthSpinBox;

    QComboBox* m_SymbolComboBox;

    QGroupBox*   m_TableSizeGroupBox;
    QGridLayout* m_MainGridLayout;

    QDialogButtonBox* m_ButtonBox;

    int m_RowCnt;
    int m_ColumnCnt;
    int m_TableWidth;
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
