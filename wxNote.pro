    ############################################################################
    #                                                                          #
    #                               .======.                                   #
    #                               | INRI |                                   #
    #                               |      |                                   #
    #                               |      |                                   #
    #                      .========'      '========.                          #
    #                      |   _      xxxx      _   |                          #
    #                      |  /_;-.__ / _\  _.-;_\  |                          #
    #                      |     `-._`'`_/'`.-'     |                          #
    #                      '========.`\   /`========'                          #
    #                               | |  / |                                   #
    #                               |/-.(  |                                   #
    #                               |\_._\ |                                   #
    #                               | \ \`;|                                   #
    #                               |  > |/|                                   #
    #                               | / // |                                   #
    #                               | |//  |                                   #
    #                               | \(\  |                                   #
    #                               |  ``  |                                   #
    #                               |      |                                   #
    #                               |      |                                   #
    #                               |      |                                   #
    #                               |      |                                   #
    #                   \\    _  _\\| \//  |//_   _ \// _                      #
    #                  ^ `^`^ ^`` `^ ^` ``^^`  `^^` `^ `^                      #
    #                                                                          #
    #                    Copyright © 1997-2013 by Tong G.                      #
    #                          ALL RIGHTS RESERVED.                            #
    #                                                                          #
    ############################################################################

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wxNote
TEMPLATE = app
CONFIG   += C++11

SOURCES += main.cpp                                             \
    wxNote_Gui/wxNote_Window/AbstractMainWindow.cpp             \
    wxNote_Gui/wxNote_Window/TextEditorWindow.cpp               \
    wxNote_Gui/wxNote_Window/MainWindowNormal.cpp               \
    wxNote_Gui/wxNote_Window/MainWindowShadow.cpp               \
    wxNote_Gui/wxNote_Window/FinalTabWindow.cpp                 \
                                                                \
    wxNote_Gui/wxNote_Dialog/AboutDialog.cpp                    \
    wxNote_Gui/wxNote_Dialog/CommonDialog.cpp                   \
    wxNote_Gui/wxNote_Dialog/GetNameForNewNotebook.cpp          \
    wxNote_Gui/wxNote_Dialog/SetPasswordDialog.cpp              \
    wxNote_Gui/wxNote_Dialog/GetPasswordDialog.cpp              \
    wxNote_Gui/wxNote_Dialog/InsertTableDialog.cpp              \
    wxNote_Gui/wxNote_Dialog/OptionsDialog.cpp                  \
                                                                \
    wxNote_Gui/wxNote_Button/EliminateOrRestorePushButton.cpp   \
    wxNote_Gui/wxNote_Button/wxNote_ToolButton.cpp              \
    wxNote_Gui/wxNote_Button/NestingButton.cpp                  \
                                                                \
    wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.cpp                \
    wxNote_Gui/wxNote_Edit/NoteEditor.cpp                       \
                                                                \
    wxNote_Gui/wxNote_List/NoteList.cpp                         \
                                                                \
    wxNote_Gui/wxNote_Tree/NoteBookTree.cpp                     \
    wxNote_Gui/wxNote_Tree/CategoriesTree.cpp                   \
                                                                \
    wxNote_Gui/wxNote_Other/NoteEditorSplitter.cpp              \
    wxNote_Gui/wxNote_Other/SearchPanel.cpp                     \
    wxNote_Gui/wxNote_Other/FontComboBox.cpp                    \
    wxNote_Gui/wxNote_Other/FontSizeComboBox.cpp                \
                                                                \
    wxNote_Item/NoteListItem.cpp                                \
                                                                \
    wxNote_Global.cpp

HEADERS  +=                                                     \
    wxNote_Gui/wxNote_Window/AbstractMainWindow.h               \
    wxNote_Gui/wxNote_Window/TextEditorWindow.h                 \
    wxNote_Gui/wxNote_Window/MainWindowNormal.h                 \
    wxNote_Gui/wxNote_Window/MainWindowShadow.h                 \
    wxNote_Gui/wxNote_Window/FinalTabWindow.h                   \
                                                                \
    wxNote_Gui/wxNote_Dialog/AboutDialog.h                      \
    wxNote_Gui/wxNote_Dialog/CommonDialog.h                     \
    wxNote_Gui/wxNote_Dialog/GetNameForNewNotebook.h            \
    wxNote_Gui/wxNote_Dialog/SetPasswordDialog.h                \
    wxNote_Gui/wxNote_Dialog/GetPasswordDialog.h                \
    wxNote_Gui/wxNote_Dialog/InsertTableDialog.h                \
    wxNote_Gui/wxNote_Dialog/OptionsDialog.h                    \
                                                                \
    wxNote_Gui/wxNote_Button/EliminateOrRestorePushButton.h     \
    wxNote_Gui/wxNote_Button/wxNote_ToolButton.h                \
    wxNote_Gui/wxNote_Button/NestingButton.h                    \
                                                                \
    wxNote_Gui/wxNote_Edit/NoteTitleLineEdit.h                  \
    wxNote_Gui/wxNote_Edit/NoteEditor.h                         \
                                                                \
    wxNote_Gui/wxNote_List/NoteList.h                           \
                                                                \
    wxNote_Gui/wxNote_Tree/NoteBookTree.h                       \
    wxNote_Gui/wxNote_Tree/CategoriesTree.h                     \
                                                                \
    wxNote_Gui/wxNote_Other/NoteEditorSplitter.h                \
    wxNote_Gui/wxNote_Other/SearchPanel.h                       \
    wxNote_Gui/wxNote_Other/FontComboBox.h                      \
    wxNote_Gui/wxNote_Other/FontSizeComboBox.h                  \
                                                                \
    wxNote_Item/NoteListItem.h                                  \
                                                                \
    wxNote_Global.h

RESOURCES += \
    resource.qrc

RC_FILE += "rc.rc"

OTHER_FILES += \
    LICENSE.GPL


    ###########################################################################
    #                                                                         #
    #      _________                                      _______             #
    #     |___   ___|                                   / ______ \            #
    #         | |     _______   _______   _______      | /      |_|           #
    #         | |    ||     || ||     || ||     ||     | |    _ __            #
    #         | |    ||     || ||     || ||     ||     | |   |__  \           #
    #         | |    ||     || ||     || ||     ||     | \_ _ __| |  _        #
    #         |_|    ||_____|| ||     || ||_____||      \________/  |_|       #
    #                                           ||                            #
    #                                    ||_____||                            #
    #                                                                         #
    ###########################################################################
