#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutwindow.h"
#include "editorwidget.h"
#include "filerepresentation.h"
#include "syntaxselectwidget.h"

#include <QRegularExpression>
#include <QWidgetAction>
#include <QApplication>
#include <QPainterPath>
#include <QColorDialog>
#include <QWidgetList>
#include <QMainWindow>
#include <QScrollArea>
#include <QPushButton>
#include <QFontDialog>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QToolButton>
#include <QFileDialog>
#include <QByteArray>
#include <QSettings>
#include <QPainter>
#include <QMenuBar>
#include <QToolBar>
#include <QToolTip>
#include <QScreen>
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <QMenu>
#include <QDir>

#define OUTSIDE_PRESS       -1
#define IDLE_PRESS          0
#define TOP_LEFT_PRESS      1
#define TOP_RIGHT_PRESS     2
#define BOTTOM_RIGHT_PRESS  3
#define BOTTOM_LEFT_PRESS   4
#define TOP_MIDDLE_PRESS    5
#define RIGHT_MIDDLE_PRESS  6
#define BOTTOM_MIDDLE_PRESS 7
#define LEFT_MIDDLE_PRESS   8
#define HEADER_PRESS        9
#define FOOTER_PRESS        10
#define LISTING_PRESS       11



//              [settings.ini]
// QString  palette               [path/Default]
// bool     fullscreen            [true/false]
// bool     left_button_pressed   [true/false]
// bool     tool_bar_visible      [true/false]
// bool     status_bar_visible    [true/false]
// bool     newline_instead_wrap  [true/false]
// bool     line_number_visible   [true/false]
// bool     find_visible          [true/false]
// bool     replace_mode          [true/false]
// size_t   window_border_radius  [size_t]
// size_t   fullscreen_offset     [size_t]
// size_t   header_height         [size_t]
// size_t   toolbar_height        [size_t]
// size_t   listing_height        [size_t]
// size_t   footer_height         [size_t]
// size_t   button_height         [size_t]
// size_t   colision_offset       [size_t]
// size_t   press_type            [size_t]
// int      syntax_standart       [int]
//


struct last_event
{
    uint64_t line = 0;
    uint64_t symbol = 0;
    uint8_t action = 0;
    uint64_t lines = 0;
    uint64_t words = 0;
    uint64_t symbols = 0;
    double kb_size = 0;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initNewWindowStyle();
    void recalcSizes();
    void recalcAlaviableStyles();
    void repaintListing();

    size_t borderColisionType(QPoint);                                                                  // -1 outside    0 idle     1 left-top     2 right-top    3 right-bottom    4 left-bottom     5 top-middle    6 right-middle     7 bottom-middle    8 left-middle    9 header    10 footer

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    bool eventFilter(QObject* object, QEvent* event);

    void saveSettings();
    void loadSettings();

private slots:
    void minimize();
    void maximize();

    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExit();

    void fileChange(uint8_t latest_activity);

    void editUndo();
    void editRedo();
    void editCopy();
    void editCut();
    void editPaste();
    void editFind();
    void editFindAndReplace();
    void editSelectAll();

    void formatNewLine();
    void formatSelectFont();

    void viewBackgroundColor();
    void viewSelectionColor();
    void viewLineHighlightColor();
    void viewShowLineNumber();
    void viewShowToolBar();
    void viewShowSyntaxHighlight();
    void viewShowStateBar();
    void viewSelectSyntax();

    void viewSelectStyleSave();
    void viewSelectStyleLoad(QString = "");
    void viewSelectStyleDefault();

    void openRecentFile(long long index);

    void aboutShow();

    void updateFooter();

    void closeWindow();

public:

private:
    // Variables used for main window
    size_t                          window_border_radius    = 10;
    size_t                          fullscreen_offset       = 0;
    size_t                          header_height           = 30;
    size_t                          toolbar_height          = 21;
    size_t                          listing_height          = 30;
    size_t                          footer_height           = 20;
    size_t                          button_height           = 12;
    size_t                          colision_offset         = 5;
    size_t                          press_type              = 0;

    // Flags
    bool                            fullscreen;
    bool                            left_button_pressed;
    bool                            tool_bar_visible        = 1;
    bool                            status_bar_visible      = 1;

    // Colision Rects
    QRect                           top_middle_resize;
    QRect                           right_middle_resize;
    QRect                           bottom_middle_resize;
    QRect                           left_middle_resize;
    QRect                           top_left_resize;
    QRect                           top_right_resize;
    QRect                           bottom_right_resize;
    QRect                           bottom_left_resize;

    // Blocks used for drawing ui elements
    QRect                           header_rect;                                                                // Header
    QRect                           toolbar_rect;                                                               // Toolbar (menu and stuff)
    QRect                           menubar_rect;                                                               // Toolbar (menu and stuff)
    QRect                           listing_rect;                                                               // Listing of files, opened earlier
    QRect                           editor_rect;                                                                // Main editor field
    QRect                           footer_rect;                                                                // Footer (some statistics)

    // Blocks used for alignment
    QRect                           window_rect;

    // Colors, used by programm
    QColor                          main_bg_color           = QColor("#101010");
    QColor                          main_ui_color           = QColor("#202020");
    QColor                          main_hl_color           = QColor("#303030");
    QColor                          main_tx_color           = QColor("#909090");
    QColor                          main_fr_color           = QColor("#21262c");
    QColor                          main_ed_color           = QColor("#30353b");
    QColor                          main_et_color           = QColor("#ededed");
    QColor                          main_lt_color           = QColor("#49515a");

    // Main fonts
    QFont                           main_ui_font            = QFont("Helvetica", 10, QFont::Bold);
    QFont                           main_ed_font            = QFont("Times", 10, QFont::Thin);

    // Lables for ui elements
    QLabel                          *title_name;
    QLabel                          *status_bar_widget;

    // Menu and submenu
    QHBoxLayout                     *main_menu_layout;

    QMenuBar                        *menu_bar_left;
    QMenuBar                        *menu_bar_right;

    QMenu                           *file;

    QAction                         *file_new;
    QAction                         *file_open;
    QAction                         *file_save;
    QAction                         *file_save_as;
    QAction                         *file_exit;

    QMenu                           *edit;

    QAction                         *edit_undo;
    QAction                         *edit_redo;
    QAction                         *edit_copy;
    QAction                         *edit_cut;
    QAction                         *edit_paste;
    QAction                         *edit_find;
    QAction                         *edit_find_and_replace;
    QAction                         *edit_select_all;

    QMenu                           *format;

    QAction                         *format_newline;
    QAction                         *format_select_font;

    QMenu                           *view;

    QAction                         *view_background_color;
    QAction                         *view_selection_color;
    QAction                         *view_line_highlight_color;
    QAction                         *view_show_line_number;
    QAction                         *view_show_toolbar;
    QAction                         *view_show_state_bar;
    QAction                         *view_show_syntax_highlight;
    QAction                         *view_select_syntax;
    QMenu                           *view_select_style;

    QAction                         *view_select_style_save;
    QAction                         *view_select_style_load;
    QAction                         *view_select_style_default;
    QMenu                           *view_select_style_avaliable;

    QMenu                           *about;
    QAction                         *about_show;

    // Ui buttons
    QPushButton                     *exit_button;
    QPushButton                     *minimize_button;
    QPushButton                     *maximize_button;

    // Painter path for rounded window
    QPainterPath                    *rounded_window_path;

    // Mouse last position
    QPoint                          last_mouse_pos;

    // Editor
    EditorWidget                    *main_editor_widget;

    // Loaded files
    QVector<FileInfo>               files;
    size_t                          opened_file_index;
    size_t                          scrolled_index;
    QScrollArea                     *scroll_area;
    QWidget                         *shadow_overlay;

    last_event                      footer_event;
    SyntaxSelectWidget              syntax_dialog;

    QString                         config_path = "/tmp/settings/";

    AboutWindow                     about_window;

    QToolBar                        *q_tool_bar;

};
#endif // MAINWINDOW_H
