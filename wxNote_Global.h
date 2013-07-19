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
#include <QTextStream>
#include <QSettings>
#include <QList>

class _TextEditorWindow;

class QListWidgetItem;
class QTextEdit;
class QAction;
class QFont;

#ifndef _DEBUG
    #define _DEBUG
#endif

#ifdef _DEBUG
    static QTextStream cout(stdout);
    static QTextStream cerr(stderr);
#endif

#define __PTR_INVALID_ERROR_OUTPUT__(_Ptr)                    \
                                                              \
        cerr << "Nullptr!" << endl                            \
             << "This pointer = " << _Ptr << endl             \
             << "File: " << __FILE__ << endl                  \
             << "Function: " << __FUNCTION__ << "()" << endl  \
             << "Line: " << __LINE__ << endl;

namespace wxNote
    {
    typedef enum { _Normal, _Shadow } _ParentWindowType;

    typedef enum
        { _NonCategories, _Important,
          _Work, _Personal, _ToDo, _Later } _NoteCategories;

    typedef enum
        { _NonRating, _Poor, _Fair, _Average, _Good, _Excellent } _NoteRating;

    /* 用于存储新建的编辑窗口的指针, 在wxNote_Global.cpp中定义 */
    extern QList<_TextEditorWindow *> g_AllTextEditorList;

    /* 用于_FinalTabWindow中的shadow编辑窗口的指针 */
    extern QList<_TextEditorWindow *> g_ShadowTextEditorList;

    extern QList<QAction *> g_NoteBookNameActionList;

    /* "笔记本列表" */
    extern QString g_NoteBooksListName;
    extern QString g_NoteBooksName;
    extern QString g_AllNotesName;
    extern QString g_LabelName;
    extern QString g_PropertiesName;
    extern QString g_TrashName;

    /* "分类"列表 */
    extern QString g_CategoriesListName;
    extern QString g_RatingListName;
    extern QString g_ExcellentName;
    extern QString g_GoodName;
    extern QString g_AverageName;
    extern QString g_FairName;
    extern QString g_PoorName;
    extern QString g_ColorLabelListName;
    extern QString g_ImportantName;
    extern QString g_WorkName;
    extern QString g_PersonalName;
    extern QString g_ToDoName;
    extern QString g_LaterName;

    extern QString g_NonTitleNoteName;

    extern QString g_MatchTextMark_Prefix;
    extern QString g_MatchTextMark_Suffix;

    extern QString g_SearchLineEditStyleSheet_NotFound;
    extern QString g_SearchLineEditStyleSheet_Found;

    extern QString g_TableHTML_Prefix;
    extern QString g_TableHTML_Content;

    extern QString g_UnorderedListHTML_Prefix;
    extern QString g_UnorderedListHTML_Content;
    extern QString g_OrderedListHTML_Prefix;
    extern QString g_OrderedListHTML_Content;

    extern QString g_NonCategories_Mini;
    extern QString g_Important_Mini;
    extern QString g_Work_Mini;
    extern QString g_Personal_Mini;
    extern QString g_ToDO_Mini;
    extern QString g_Later_Mini;

    extern QString g_Excellent_Mini;
    extern QString g_Good_Mini;
    extern QString g_Average_Mini;
    extern QString g_Fair_Mini;
    extern QString g_Poor_Mini;

    extern QString g_Locking_Mini;

    extern QString g_CheckBoxHTML;

    extern QFont g_StandardFont;

    extern QSettings g_Settings;

    /* 用于检测程序中的致命空指针 */
    inline void _PointerChecker(const void* _SomePtr);

    /* 接受给定笔记项的指针以获取与之对应的编辑窗口的指针 */
    _TextEditorWindow* _GetEWFromGlobalList_BySpecifiedItem(
                            QListWidgetItem* _CurrentNoteItem);

    /* 从给定的编辑器的文本中提取出第一个image路径 */
    QString _ExtractFirstImagePath(const QTextEdit* _NoteEditor);
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
