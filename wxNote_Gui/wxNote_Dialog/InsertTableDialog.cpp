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

#include "wxNote_Gui/wxNote_Dialog/InsertTableDialog.h"

#include "wxNote_Global.h"

#include <QLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>

//.._InserTableDialog类实现

    /* 构造函数实现 */
    _InsertTableDialog::_InsertTableDialog(QWidget *_Parent)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint),
          m_RowCnt(2), m_ColumnCnt(2), m_TableWidth(100)
        {
        _CreateWidgetsInGroupBox();
        _CreateSizeGroupBox();

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_TableSizeGroupBox);
        _MainLayout->addStretch();
        _MainLayout->addWidget(m_ButtonBox);

        setLayout(_MainLayout);
        setFont(wxNote::g_StandardFont);
        setFixedSize(sizeHint().width() + 50, sizeHint().height() + 10);
        setWindowTitle("插入表格");
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateWidgetsInGroupBox()函数实现 */
    void _InsertTableDialog::_CreateWidgetsInGroupBox()
        {
        m_RowCountLabel = new QLabel(tr("行数(&R)："));
        m_ColumnCountLabel = new QLabel(tr("列数(&C)："));
        m_TableWidthLabel = new QLabel(tr("表格宽度(&W)："));

        m_RowCountSpinBox = new QSpinBox;
        m_RowCountSpinBox->setValue(2);
        m_RowCountSpinBox->setRange(0, 15);

        m_ColumnCountSpinBox = new QSpinBox;
        m_ColumnCountSpinBox->setValue(2);
        m_ColumnCountSpinBox->setRange(0, 15);

        m_TableWidthSpinBox = new QSpinBox;
        m_TableWidthSpinBox->setValue(100);
        m_TableWidthSpinBox->setRange(0, 100);

        m_RowCountLabel->setBuddy(m_RowCountSpinBox);
        m_ColumnCountLabel->setBuddy(m_ColumnCountSpinBox);
        m_TableWidthLabel->setBuddy(m_TableWidthSpinBox);

        m_SymbolComboBox = new QComboBox;
        m_SymbolComboBox->addItem("%");
        m_SymbolComboBox->addItem(tr("像素"));
        m_SymbolComboBox->setCurrentIndex(0);

        m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);
        QPushButton* _OKButton =
                m_ButtonBox->button(QDialogButtonBox::Ok);
        _OKButton->setText(tr("好的"));

        QPushButton* _CancelButton =
                m_ButtonBox->button(QDialogButtonBox::Cancel);
        _CancelButton->setText(tr("取消"));

        connect(m_RowCountSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(_SetOKButtonEnabled()));
        connect(m_ColumnCountSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(_SetOKButtonEnabled()));
        connect(m_TableWidthSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(_SetOKButtonEnabled()));

        connect(m_RowCountSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(_RowCntSpinBoxValChanged(int)));
        connect(m_ColumnCountSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(_ColumnCntSpinBoxValChanged(int)));
        connect(m_TableWidthSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(_TableWidthSpinBoxValChanged(int)));

        connect(m_ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

        }

    /* _CreateSizeGroupBox()函数实现 */
    void _InsertTableDialog::_CreateSizeGroupBox()
        {
        m_TableSizeGroupBox = new QGroupBox(tr("表格大小"));

        m_MainGridLayout = new QGridLayout;
        m_MainGridLayout->addWidget(m_RowCountLabel, 0, 0);
        m_MainGridLayout->addWidget(m_RowCountSpinBox, 0, 1);
        m_MainGridLayout->addWidget(m_ColumnCountLabel, 1, 0);
        m_MainGridLayout->addWidget(m_ColumnCountSpinBox, 1, 1);
        m_MainGridLayout->addWidget(m_TableWidthLabel, 2, 0);
        m_MainGridLayout->addWidget(m_TableWidthSpinBox, 2, 1);
        m_MainGridLayout->addWidget(m_SymbolComboBox, 2, 2);

        m_TableSizeGroupBox->setLayout(m_MainGridLayout);
        }

    /* _RowCntSpinBoxValChanged()槽实现 */
    void _InsertTableDialog::_RowCntSpinBoxValChanged(int _NewVal)
        {
        m_RowCnt = _NewVal;
        }

    /* _ColumnCntSpinBoxValChanged()槽实现 */
    void _InsertTableDialog::_ColumnCntSpinBoxValChanged(int _NewVal)
        {
        m_ColumnCnt = _NewVal;
        }

    /* _TableWidthSpinBoxValChanged()槽实现 */
    void _InsertTableDialog::_TableWidthSpinBoxValChanged(int _NewVal)
        {
        m_TableWidth = _NewVal;
        }

    /* _SetOKButtonEnabled(int)槽实现 */
    void _InsertTableDialog::_SetOKButtonEnabled()
        {
        QPushButton* _OKButton = m_ButtonBox->button(QDialogButtonBox::Ok);

        _OKButton->setEnabled(m_RowCountSpinBox->value()
                                && m_ColumnCountSpinBox->value()
                                && m_TableWidthSpinBox->value());
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
