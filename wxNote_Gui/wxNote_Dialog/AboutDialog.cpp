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

#include "wxNote_Gui/wxNote_Dialog/AboutDialog.h"

#include "wxNote_Global.h"

#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QIcon>
#include <QTextBrowser>
#include <QDate>
#include <QTime>
#include <QFile>

//.._AboutwxNote类实现

    /* 构造函数实现 */
    _AboutwxNote::_AboutwxNote(QWidget *_Parent)
        : QDialog(_Parent, Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
        {
        m_TabWidgetOnDialog = new QTabWidget;

        _CreateNormalPage();
        _CreateVersionPage();
        _CreateLicensePage();
        _Create2UserPage();

        m_OKButton = new QPushButton(tr("好的"));
        connect(m_OKButton, SIGNAL(clicked()), this, SLOT(close()));

        QHBoxLayout* _BottomLayout = new QHBoxLayout;
        _BottomLayout->addStretch();
        _BottomLayout->addWidget(m_OKButton);
        _BottomLayout->addStretch();

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addWidget(m_TabWidgetOnDialog);
        _MainLayout->addLayout(_BottomLayout);

        setLayout(_MainLayout);
        setFixedSize(sizeHint().width(), sizeHint().height() + 17);
        setWindowTitle(tr("关于..."));
        setFont(wxNote::g_StandardFont);
        setWindowIcon(QIcon(":/wxNote_Icons/wxNoteicon.png"));
        }

    /////////////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreateNormalPage()函数实现 */
    void _AboutwxNote::_CreateNormalPage()
        {
        /* "一般"页 */
        m_LogoLabel = new QLabel("<img src=\":/wxNote_Icons/logoOnAbout.png\">");

        m_NormalTextBrowser = new QTextBrowser;
        m_NormalTextBrowser->setHtml(
                    QString("<B><h3>wxNote</B></h3>"
                            "<p>Version 0.5 (%1)"
                            "<p>wxNote是由一个高中生发起的开源项目，意为Windows and X Note，"
                            "该项目致力于打造一款强大的，开放的，自由的个人笔记软件，以将您的个人数据管理的井井有条。"
                            "<p>我们是一个开源社区，我们热爱开源，崇尚自由，我们坚信Linus Torvalds的一句话："
                            "<p><h4><I>\"Software is like sex: it's better when it's free.\"</h4></I>"
                            "<p>听起来很Cool？"
                            "<a href=\"http://wikipedia.org/\">一起来参与！</a>")
                            .arg(__DATE__));

        QHBoxLayout* _LogoLayout = new QHBoxLayout;
        _LogoLayout->addStretch();
        _LogoLayout->addWidget(m_LogoLabel);
        _LogoLayout->addStretch();

        QVBoxLayout* _NormalPageLayout = new QVBoxLayout;
        _NormalPageLayout->addLayout(_LogoLayout);
        _NormalPageLayout->addWidget(m_NormalTextBrowser);

        m_NormalPage = new QWidget;
        m_NormalPage->setLayout(_NormalPageLayout);

        m_TabWidgetOnDialog->addTab(m_NormalPage, wxNote::g_FairName);
        }

    /* _CreateVersionPage()函数实现 */
    void _AboutwxNote::_CreateVersionPage()
        {
        m_VersionPageBrowser = new QTextBrowser;
        m_VersionPageBrowser->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_VersionPageBrowser->setHtml(
                    QString("<B>wxNote &copy; 2013 郭桐\"Tong G.\" 保留所有权利.</B>"
                            "<p>================================================="
                            "<p>wxNote是一款可以存储并管理您各种笔记的软件，笔记包括一段文字，一张照片，一份Word或PDF文档，一份电子表格，"
                            "亦或是一份幻灯片等，并且wxNote还会对它们进行妥善分类管理，使您的私人数据井井有条，同时您也可以通过E-Mail和您的朋友分享您的笔记。"
                            "<p>wxNote使用C++/Qt编写，这使得它可以在任何流行的平台上流畅运行。"
                            "<p>此版本的的wxNote编写者和编译者是："
                            "<P>郭桐\"Tong G.\"(%1 %2)."
                            "<P>编译器：gcc version 4.7.1 (GCC)."
                            "<p>Qt version: Qt 5.1.0 Beta")
                            .arg(__DATE__).arg(__TIME__));

        QVBoxLayout* _VersionPageLayout = new QVBoxLayout;
        _VersionPageLayout->addWidget(m_VersionPageBrowser);

        m_VersionPage = new QWidget;
        m_VersionPage->setLayout(_VersionPageLayout);

        m_TabWidgetOnDialog->addTab(m_VersionPage, tr("版本"));
        }

    /* _CreateLicensePage()函数实现 */
    void _AboutwxNote::_CreateLicensePage()
        {
        m_LicenseTextBrowser = new QTextBrowser;

        m_LicenseTextBrowser->setHtml(
                    QString("<B>wxNote &copy; 2013 郭桐\"Tong G.\" 保留所有权利.</B>"
                            "<p>================================================="
                            "<p>安装和使用wxNote标志着接受以下这些条款及许可条件。 "
                            "<p>wxNote是自由软件，供私人非商业或教育用途使用，包括非营利组织（如学校，大学，公共机关，警察，消防队和医院）。 "
                            "<p>对于商业使用和部署，必须进行登记。这是对未来版本开发的帮助。 "
                            "<p>您被授予使用权和对该软件不限数量的拷贝权，并可以根据自己的需要修改本软件的代码或重新分发。 "
                            "<p>本软件以“原样”提供。 "
                            "<p>没有任何形式的保修。 "
                            "<P>作者对于使用或滥用本软件造成的数据丢失，损坏，利润损失或任何其他种类的损失不承担责任。 "
                            "<P>对任何建议，反馈和意见表示欢迎，同时也欢迎自己修改软件源代码并再分发。 "));

        QVBoxLayout* _LicensePageLayout = new QVBoxLayout;
        _LicensePageLayout->addWidget(m_LicenseTextBrowser);

        m_LicensePage = new QWidget;
        m_LicensePage->setLayout(_LicensePageLayout);

        m_TabWidgetOnDialog->addTab(m_LicensePage, tr("许可协议"));
        }

    /* _Create2UserPage()函数实现 */
    void _AboutwxNote::_Create2UserPage()
        {
        m_2UserTextBrowser = new QTextBrowser;
        m_2UserTextBrowser->setHtml(
                    QString("<B>wxNote &copy; 2013 郭桐\"Tong G.\" 保留所有权利.</B>"
                            "<p>================================================="
                            "<p>作为wxNote项目唯一的代码编写者，我真的非常希望给用户提供像"
                            "Evernote®（印象笔记®）那样强大的云端同步服务，但是我还只是一名在"
                            "读的高中生，无力承担购买、租用和维护服务器的各种花销。"
                            "<p>正如您所看到的，wxNote是开源软件，当我敲下wxNote的第一行代码时，就想把它"
                            "打造成一款远离商业的软件，这代表从一开始，我就从没想过追求商业利益。"
                            "<p>商业本无罪，但是首先我个人不想让用户付费使用wxNote或付费购买wxNote的增值服务"
                            "，因为这样有悖hacker精神，其次是我认为软件界面应该是一片充满禅意的净土，可以不华丽"
                            "，但决不应该存在任何形式的蹩脚的广告。"
                            "<p>这样做的好处就是能够给用户提供一个相对舒适、自由的使用环境，但代价就是有一些需"
                            "要大量金钱才能实现的功能wxNote并没有做到。"
                            "<p>我深知不能提供云端同步会使wxNote损失很多潜在用户，"
                            "但是现在这项功能对我一个人来说的确是力不从心。"
                            "<p>如果您对wxNote项目感兴趣，并想与我一起打造一款完美的软件，可以选择捐款的方式资助这个项目，"
                            "帮助这个项目解决一些迫在眉睫的不足。我向您保证捐款的合理用途以及会公开所有捐款的流向，并且欢迎任何"
                            "人的监督。也欢迎任何职业的人们（当然必须是合法的O(∩_∩)O）"
                            "以自己的方式支持wxNote或加入wxNote的团队。方式包括：翻译、法律支持、美工"
                            "、代码编写、推广、网站维护等。只要热爱开源，崇尚自由，wxNote欢迎"
                            "任何形式的资助，也欢迎任何人的加入。"
                            "<p>wxNote还有很多不足，但我相信，只要每个热爱开源的人都能够为这个项目"
                            "做出力所能及的贡献，wxNote一定会变得越来越强大。"
                            "<p>我们期待那一天的到来。"
                            "<p>-- 郭桐“Tong G.”   2013年5月19日于家中卧室</B>"));

        QVBoxLayout* _2UserPageLayout = new QVBoxLayout;
        _2UserPageLayout->addWidget(m_2UserTextBrowser);

        m_2UserPage = new QWidget;
        m_2UserPage->setLayout(_2UserPageLayout);

        m_TabWidgetOnDialog->addTab(m_2UserPage, tr("致用户"));
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

