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

#include "wxNote_Gui/wxNote_Window/TextEditorWindow.h"
#include "wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h"
#include "wxNote_Item/NoteListItem.h"

#include "wxNote_Global.h"

#include <QTextEdit>
#include <QDir>
#include <QErrorMessage>
#include <QListWidgetItem>

namespace wxNote
    {
    QSettings g_Settings("Tong G.", "wxNote");

    QString g_LocalFilePath;

    QStringList g_UserNoteBookNameList;

    /* 用于存储新建的编辑窗口的指针 */
    QList<_TextEditorWindow *> g_AllTextEditorList;

    /* 用于_FinalTabWindow中的shadow编辑窗口的指针 */
    QList<_TextEditorWindow *> g_ShadowTextEditorList;

    QList<QAction *> g_NoteBookNameActionList;

    /* "笔记本列表" */
    QString g_NoteBooksListName(QObject::tr("笔记本列表"));
    QString g_NoteBooksName(QObject::tr("笔记本"));
    QString g_AllNotesName(QObject::tr("全部笔记"));
    QString g_LabelName(QObject::tr("标签"));
    QString g_PropertiesName(QObject::tr("属性"));
    QString g_TrashName(QObject::tr("废纸篓"));

    /* "分类" */
    QString g_CategoriesListName(QObject::tr("分类"));
    QString g_RatingListName(QObject::tr("评分"));
    QString g_ExcellentName(QObject::tr("极佳"));
    QString g_GoodName(QObject::tr("良好"));
    QString g_AverageName(QObject::tr("普通"));
    QString g_FairName(QObject::tr("一般"));
    QString g_PoorName(QObject::tr("差"));
    QString g_ColorLabelListName(QObject::tr("颜色标签"));
    QString g_ImportantName(QObject::tr("重要"));
    QString g_WorkName(QObject::tr("工作"));
    QString g_PersonalName(QObject::tr("个人"));
    QString g_ToDoName(QObject::tr("待办"));
    QString g_LaterName(QObject::tr("稍后"));

    QString g_NonTitleNoteName(QObject::tr("无标题笔记"));

    QString g_MatchTextMark_Prefix(
                "<span style=\"background-color:rgb(253, 254, 46);\">");

    QString g_MatchTextMark_Suffix("</span>");

    QString g_SearchLineEditStyleSheet_NotFound(
                        "background-color:rgb(215, 168, 189)");

    QString g_SearchLineEditStyleSheet_Found(
                        "background-color:rgb(255, 255, 255)");

    QString g_TableHTML_Prefix("<table border=\"1\" style=\" margin-top:0px; "
                               "margin-bottom:0px; margin-left:0px; margin-right:0px;"
                               "\"width=\"%1%\" cellspacing=\"0\" cellpadding=\"2\">");

    QString g_TableHTML_Content("<td style=\" vertical-align:top;\">"
                                "<style=\" margin-top:0px; margin-bottom:0px;"
                                "margin-left:0px; margin-right:0px; "
                                "-qt-block-indent:0; text-indent:0px;\">"
                                "<span style=><br /></span></td>");

    QString g_UnorderedListHTML_Prefix("<ul style=\"margin-top: 5px; "
                                       "margin-bottom: 0px; margin-left: 0px;"
                                       " margin-right: 0px; -qt-list-indent: 1;"
                                       "\">");

    QString g_UnorderedListHTML_Content("<li style=\" font-family:'Courier New';"
                                        " font-size:medium; font-weight:400; "
                                        "color:#000000;\" style=\" margin-top:12px; "
                                        "margin-bottom:px; margin-left:0px; "
                                        "margin-right:0px; -qt-block-indent:0; "
                                        "text-indent:0px;\"><span style=\" font-size:medium;\">"
                                        "<br /></span></li>");

    QString g_OrderedListHTML_Prefix("<ol style=\"margin-top: 0px; margin-bottom: 0px;"
                                     " margin-left: 0px; margin-right: 0px; "
                                     "-qt-list-indent: 1;\">");

    QString g_OrderedListHTML_Content("<li style= font-family:'Courier New';"
                                      "font-size:medium; font-weight:400; color:#000000;\" "
                                      "style=\" margin-top:12px; margin-bottom:0px; "
                                      "margin-left:0px; margin-right:0px; "
                                      "-qt-block-indent:0; text-indent:0px;\">"
                                      "<span style=\" font-size:medium;\">"
                                      "<br /></span></li>");

    QString g_NonCategories_Mini("<img src=\":/wxNote_Icons/nonCategories-mini.png\">");
    QString g_Important_Mini("<img src=\":/wxNote_Icons/important-mini.png\">");
    QString g_Work_Mini("<img src=\":/wxNote_Icons/work-mini.png\">");
    QString g_Personal_Mini("<img src=\":/wxNote_Icons/personal-mini.png\">");
    QString g_ToDO_Mini("<img src=\":/wxNote_Icons/toDo-mini.png\">");
    QString g_Later_Mini("<img src=\":/wxNote_Icons/later-mini.png\">");

    QString g_Excellent_Mini("<img src=\":/wxNote_Icons/excellent-mini.png\">");
    QString g_Good_Mini("<img src=\":/wxNote_Icons/good-mini.png\">");
    QString g_Average_Mini("<img src=\":/wxNote_Icons/average-mini.png\">");
    QString g_Fair_Mini("<img src=\":/wxNote_Icons/fair-mini.png\">");
    QString g_Poor_Mini("<img src=\":/wxNote_Icons/poor-mini.png\">");

    QString g_Locking_Mini("<img src=\":/wxNote_Icons/locking-mini.png\">");

    QString g_CheckBoxHTML("<img src=\"file:///C:/Users/ADMINI~1/AppData/Local/Temp/enhtmlclip/entodo_unchecked.png\" />");

    QString g_NoteNameSplitSymbol(".~_");

    QFont g_StandardFont("微软雅黑", 9, 9);

    /* _PointerChecker()函数实现
     * 用于检测程序中的致命空指针 */
    void _PointerChecker(const void* _SomePtr)
        {
        if (!_SomePtr)
            {
            QErrorMessage _ErrorDialog;
            _ErrorDialog.resize(QSize(300, 210));
            _ErrorDialog.showMessage(
                        QObject::tr("<B>Error:</B>"
                                    "<p>Pointer error."
                                    "<p>Adress:  %1\n"
                                    "<p><font color=red><B>程序出现致命错误，必须重新启动。</B>"
                                    "<p>报告此Bug：wxNote-Developer@outlook.com")
                                    .arg("0x" + QString::number((long)_SomePtr, 16)));
            _ErrorDialog.exec();

            exit(EXIT_FAILURE);
            }
        }

    /* _InitializeGlobalFilePath()函数实现 */
    void _InitializeGlobalFilePath()
        {
        wxNote::g_Settings.beginGroup("TextEditor");

            QString _LocalFilePath =
                    wxNote::g_Settings.value("LocalFilePath").toString();

            wxNote::g_LocalFilePath = _LocalFilePath.isEmpty() ? QDir::homePath() + "/wxNote_USER"
                                                               : _LocalFilePath;
            QDir _Dir(wxNote::g_LocalFilePath);

            if (!_Dir.exists())
                _Dir.mkpath(wxNote::g_LocalFilePath);

        wxNote::g_Settings.endGroup();
        }

    /* _InitializeNoteBooks()函数实现 */
    void _InitializeNoteBooks()
        {
        wxNote::g_Settings.beginGroup("MainWindow");

            QStringList _UserNoteBookNames =
                    wxNote::g_Settings.value("UserNoteBookNames").toStringList();

            for (const QString& _Elem : _UserNoteBookNames)
                {
                QString _Path(wxNote::g_LocalFilePath + '/' + _Elem);
                QDir _Dir(_Path);

                if (!_Dir.exists())
                    _Dir.mkpath(_Path);
                }

        wxNote::g_Settings.endGroup();
        }

    /* _GetEWFromGlobalList_BySpecifiedItem()函数实现
     * 接受给定笔记项的指针以获取与之对应的编辑窗口的指针 */
    _TextEditorWindow*
        _GetEWFromGlobalList_BySpecifiedItem(QListWidgetItem *_CurrentNoteItem)
        {
        _NoteListItem* __CurrentNoteItem =
                    dynamic_cast<_NoteListItem *>(_CurrentNoteItem);

        _TextEditorWindow* __CurrentEditorWindow = __CurrentNoteItem->_GetBindTextEW();

        return __CurrentEditorWindow ? __CurrentEditorWindow : nullptr;
        }


    /* _ExtractFirstImagePath()函数实现 */
    QString _ExtractFirstImagePath(const QTextEdit *_NoteEditor)
        {
        QString _ContentOfTextEditor_HTML = _NoteEditor->toHtml();

        if (_ContentOfTextEditor_HTML.contains("img src=\"file:///"))
            {
            int _AtIndex
                    = _ContentOfTextEditor_HTML.indexOf("img src=\"file:///");
            int _FirstQuotationIndex
                    = _ContentOfTextEditor_HTML.indexOf('\"', _AtIndex);
            int _LastQuotationIndex
                    = _ContentOfTextEditor_HTML.indexOf('\"', _FirstQuotationIndex + 1);;

            /* 从HTML代码中提取第一个image的路径, 作为当前笔记项的图标 */
            QString _IconPath
                    = _ContentOfTextEditor_HTML.mid(_FirstQuotationIndex,
                                                    _LastQuotationIndex - _FirstQuotationIndex).mid(9);
            return _IconPath;
            }

        return QString();
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

