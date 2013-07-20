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

#include "wxNote_Gui/wxNote_Dialog/OptionsDialog.h"
#include "wxNote_Gui/wxNote_Other/FontComboBox.h"
#include "wxNote_Gui/wxNote_Other/FontSizeComboBox.h"

#include "wxNote_Global.h"

#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QDialogButtonBox>

//.._OptionsDialog类实现

    /* 构造函数实现 */
    _OptionsDialog::_OptionsDialog(QWidget *_Parent)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint),
          mb_IsChanged(false)
        {
        m_TabWidgetOnDialog = new QTabWidget;

        _CreateCommonOptionPage();
        _CreateNoteOptionsPage();

        m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                | QDialogButtonBox::Cancel);
        m_ButtonBox->button(QDialogButtonBox::Ok)->setText("好的");
        m_ButtonBox->button(QDialogButtonBox::Cancel)->setText("离开");

        connect(m_ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(m_ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_TabWidgetOnDialog);
        _MainLayout->addWidget(m_ButtonBox);

        setLayout(_MainLayout);
        setFont(wxNote::g_StandardFont);
        setFixedSize(QSize(430, 380));
        setWindowTitle(tr("选项"));
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateCommonOptionPage()函数实现 */
    void _OptionsDialog::_CreateCommonOptionPage()
        {
        using namespace wxNote;

        g_Settings.beginGroup("TextEditor");

            QString _LocalFilePath = g_Settings.value("LocalFilePath").toString();

        g_Settings.endGroup();

        m_LocalFilePathLineEdit = new QLineEdit;
        m_LocalFilePathLineEdit->setReadOnly(true);
        m_LocalFilePathLineEdit->setText(_LocalFilePath.isEmpty() ? QDir::homePath() + tr("/wxNote_USER")
                                                                  : _LocalFilePath);

        m_ChangePathPushButton = new QPushButton(tr("更改(&A)..."));
        connect(m_ChangePathPushButton, SIGNAL(clicked()),
                this, SLOT(_ChangeLocalFilePathSlot()));

        m_EditLocalFilePathLayout = new QHBoxLayout;
        m_EditLocalFilePathLayout->addWidget(m_LocalFilePathLineEdit);
        m_EditLocalFilePathLayout->addWidget(m_ChangePathPushButton);

        m_LocalFileLayout = new QVBoxLayout;
        m_LocalFileLayout->addLayout(m_EditLocalFilePathLayout);
        m_LocalFileLayout->addStretch();

        m_LocalFileGroupBox = new QGroupBox(tr("wxNote 本地文件"));
        m_LocalFileGroupBox->setLayout(m_LocalFileLayout);

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_LocalFileGroupBox);

        m_CommonOptionPage = new QWidget;
        m_CommonOptionPage->setLayout(_MainLayout);

        m_TabWidgetOnDialog->addTab(m_CommonOptionPage, tr("通用"));

        connect(m_LocalFilePathLineEdit, SIGNAL(textChanged(QString)),
                this, SLOT(_SetCurrentSettingsChanged()));
        }

    /* _CNoreateteOptionsPage()函数实现 */
    void _OptionsDialog::_CreateNoteOptionsPage()
        {
        using namespace wxNote;

        m_NoteEditorFontLabel = new QLabel(tr("笔记编辑器字体(&N)："));

        /////////////////////////////////////////////////////////////////////
        /// 创建字体和字体大小选择框, 并读取用户设置
        ///
        g_Settings.beginGroup("TextEditor");
            QString _DefaultFontFamily
                    = g_Settings.value("DefaultFont", QString("微软雅黑")).toString();

            int _DefaultFontSize = g_Settings.value("DefaultFontSize", 8).toInt();
        g_Settings.endGroup();

        m_FontComboBox = new _FontComboBox;

        m_FontSizeComboBox = new _FontSizeComboBox;
        m_FontSizeComboBox->setEditable(true);
        m_FontSizeComboBox->setCurrentText(QString::number(_DefaultFontSize));
        m_NoteEditorFontLabel->setBuddy(m_FontComboBox);

        for (int _Index = _FontMinSize; _Index <= _FontMaxSize; _Index++)
                m_FontSizeComboBox->addItem(QString::number(_Index), _Index);

        m_FontComboBox->setCurrentFont(QFont(_DefaultFontFamily));
        m_FontSizeComboBox->setCurrentText(QString::number(_DefaultFontSize));

        /////////////////////////////////////////////////////////////////////

        /* "字体设置"部分 */
        m_ComboBoxLayout = new QHBoxLayout;
        m_ComboBoxLayout->addWidget(m_FontComboBox);
        m_ComboBoxLayout->addWidget(m_FontSizeComboBox);
        m_ComboBoxLayout->addStretch();

        m_FontLayout = new QVBoxLayout;
        m_FontLayout->addWidget(m_NoteEditorFontLabel);
        m_FontLayout->addLayout(m_ComboBoxLayout);
        m_FontLayout->addStretch();

        g_Settings.beginGroup("TextEditor");
            bool _IsChecked = g_Settings.value("AcceptRichText", true).toBool();
        g_Settings.endGroup();

        m_AcceptRichTextCheckBox = new QCheckBox(tr("使用HTML存储笔记(&H)"));
        m_AcceptRichTextCheckBox->setChecked(_IsChecked);

        m_FontGroupBox = new QGroupBox;
        m_FontGroupBox->setLayout(m_FontLayout);

        /* 复选部分 */
        m_AcceptGroupBox = new QGroupBox;

        m_AcceptLayout = new QVBoxLayout;
        m_AcceptLayout->addWidget(m_AcceptRichTextCheckBox);
        m_AcceptLayout->addStretch();

        m_AcceptGroupBox->setLayout(m_AcceptLayout);

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_FontGroupBox);
        _MainLayout->addWidget(m_AcceptGroupBox);

        m_NoteOptionPage = new QWidget;
        m_NoteOptionPage->setLayout(_MainLayout);

        m_TabWidgetOnDialog->addTab(m_NoteOptionPage, tr("笔记"));

        connect(m_AcceptRichTextCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(_SetCurrentSettingsChanged()));
        }

    /* _SetCurrentSettingsChanged()槽实现 */
    void _OptionsDialog::_SetCurrentSettingsChanged()
        {
        mb_IsChanged = true;
        }

    /* _ChangeLocalFilePathSlot() */
    void _OptionsDialog::_ChangeLocalFilePathSlot()
        {
        QStringList _DirSplit = wxNote::g_LocalFilePath.split('/');
        _DirSplit.pop_back();

        /* 取当前"本地文件夹"所在文件夹的path */
        QString _Dir = (_DirSplit.count() > 1) ? _DirSplit.join('/')
                                               : _DirSplit.at(0) + '/';

        QString _NewPath =
                QFileDialog::getExistingDirectory(this
                                                  , tr("浏览文件夹")
                                                  , _Dir);
        if (!_NewPath.isEmpty())
            {
            m_LocalFilePathLineEdit->setText(_NewPath);

            wxNote::g_LocalFilePath = _NewPath;
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
