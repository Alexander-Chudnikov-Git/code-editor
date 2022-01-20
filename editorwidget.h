#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include "highlighter.h"

#include <QAbstractTextDocumentLayout>
#include <QGuiApplication>
#include <QFontMetrics>
#include <QPainterPath>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QPaintEvent>
#include <QScrollBar>
#include <QTextBlock>
#include <QClipboard>
#include <QTextEdit>
#include <QLineEdit>
#include <QPainter>
#include <QPalette>
#include <QWidget>
#include <QRectF>
#include <QMenu>
#include <QFile>
#include <QDir>

#define NONE                0
#define NEW_FILE_CREATED    1
#define FILE_OPENED         2
#define FILE_SAVED          3
#define FILE_SAVED_AS       4
#define FILE_CLOSED         5
#define EDIT_UNDO           6
#define EDIT_REDO           7
#define EDIT_COPY           8
#define EDIT_CUT            9
#define EDIT_PASTE          10
#define EDIT_FIND           11
#define EDIT_REPLACE        12
#define EDIT_SELECT_ALL     13
#define FORMAT_LINE_SWITCH  14
#define VIEW_LINE_NUMBER    15
#define VIEW_TOOL_BAR       16
#define VIEW_SX_HIGHLIGHT   17
#define VIEW_STATUSBAR      18
#define FORMAT_CHANGE_FONT  19
#define VIEW_CHANGED_COLOR  20
#define VIEW_SLC_COLOR      21
#define VIEW_SYNTAX_SLC     22
#define VIEW_SYNTAX_CRT     23
#define VIEW_SYNTAX_LRT     24
#define VIEW_SYNTAX_DRT     25
#define ABOUT_OPENED        26
#define KEY_PRESSED         50
#define WINDOW_MAXIMIZED    100
#define WINDOW_MINIMIZED    101
#define WINDOW_MOVED        103
#define WINDOW_RESIZED      104



struct FileInfo
{
    QString         name;
    QString         real_name;
    QString         path;
    bool            updated;
    bool            exist;
};

struct SearchDocument
{
    QRegularExpressionMatchIterator     *match_iterator = nullptr;
    int                                 current_line_number = 0;
    QString                             last_regex = "";

};

class LineNumberWidget;
class FindWidget;

class EditorWidget : public QWidget
{
    Q_OBJECT

public:
    EditorWidget(FileInfo file, QWidget *parent = nullptr);

    void initEditor();
    void initHighlight(QTextDocument* input = nullptr);

    void makeUndo();
    void makeRedo();
    void makeCopy();
    void makeCut();
    void makePaste();
    void makeFind();
    void makeFindAndReplace();
    void makeSelectAll();
    void hideFindAndReplace();
    void changeFont(QFont);
    void selectionColorChange(QColor);

    bool searchString(QString regex, bool mode, bool second = false);

    void toggleNewLineMode();
    void toggleLineNumber();
    void toggleHighlight();

    void saveFile();
    void resizeWidget();
    void loadFile(FileInfo file);
    void updateFileInfo(FileInfo file);
    void highlightCurrentLine();

    void recolorWidget(QColor background, QColor text);
    void mouseMoveEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent *event);
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    int getFirstVisibleBlockId();

    bool eventFilter(QObject*, QEvent*);

    QColor colorChange(QColor);

private slots:
    void recalcLineWidth();

    void updateLineNumberWidgetWidth(int new_block_count);
    void updateLineNumberWidget(QRectF /*rect_f*/);
    void updateLineNumberWidget(int /*slider_pos*/);
    void updateLineNumberWidget();

    void showContextMenu(const QPoint &pos);

    void contextUndo();
    void contextRedo();
    void contextSelect();
    void contextSelectLine();
    void contextCopy();
    void contextCut();
    void contextPaste();
    void contextDelete();
    void contextSelectAll();

signals:
    void changed(uint8_t latest_activity);

private:
    QWidget                     *line_number_widget;

    SearchDocument              search_document;

    bool                        line_number_enabled                     = true;

    QColor                      background_current;
    QColor                      font_current;

    QFontMetrics                *font_metrics;

    FileInfo                    *file_opened;

public:
    QTextEdit                   *text_editor_window;
    Highlighter                 *highlighter;

    size_t                      line_number_width                       = 12;
    size_t                      find_height                             = 30;
    size_t                      tab_width                               = 4;
    FindWidget                  *find_and_replace_widget;

    QColor                      main_bg_color                           = QColor("#101010");
    QColor                      main_ui_color                           = QColor("#202020");
    QColor                      main_hl_color                           = QColor("#303030");
    QColor                      main_tx_color                           = QColor("#909090");
    QColor                      main_ed_color                           = QColor("#30353b");
    QColor                      main_et_color                           = QColor("#ededed");
    QColor                      main_eh_color                           = QColor("#484f58");
    QColor                      main_tc_color                           = QColor("#ffffff");
    QColor                      main_tt_color                           = QColor("#000000");
    QColor                      line_highlight_color                    = QColor("#484f58");

    QColor                      keyword_highlight_color                 = QColor("#d7db57");
    QColor                      data_type_highlight_color               = QColor("#d879e2");
    QColor                      single_line_comment_highlight_color     = QColor("#646464");
    QColor                      multi_line_comment_highlight_color      = QColor("#646464");
    QColor                      preprocessor_highlight_color            = QColor("#d143e0");
    QColor                      quotation_highlight_color               = QColor("#e7a631");
    QColor                      function_highlight_color                = QColor("#09a2ed");
    QColor                      numbers_highlight_color                 = QColor("#edb009");
    QColor                      preprocessor_numbers_highlight_color    = QColor("#e7b546");
    QColor                      special_symbol_highlight_color          = QColor("#e06c22");
    QColor                      class_highlight_color                   = QColor("#22e096");
    QColor                      variable_highlight_color                = QColor("#cce096");

    int                         syntax_standart                         = C_STD_2011;

    QFont                       editor_font                             = QFont("Courier", 10, QFont::Thin);

    highlightPalette            default_highlight_palette;

    bool                        newline_instead_wrap                    = 0;
    bool                        line_number_visible                     = 1;
    bool                        find_visible                            = 0;
    bool                        replace_mode                            = 0;

};

class LineNumberWidget : public QWidget
{
    Q_OBJECT

public:
    LineNumberWidget(EditorWidget *editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    EditorWidget            *code_editor;

};

class FindWidget : public QWidget
{
    Q_OBJECT

public:
    FindWidget(bool mode, EditorWidget *editor);

    void recolorWidget();
    void changeMode(bool mode);
    void recalcSize();

    QSize sizeHint() const;

private slots:
    void findNext();
    void findPrevious();
    void replaceCurrent();
    void replaceAll();
    void exitFindAndReplace();

private:
    EditorWidget            *code_editor;

    QGridLayout             *search_and_replace_layout;

    QHBoxLayout             *search_layout;
    QHBoxLayout             *replace_layout;

    QWidget                 *search_widget;
    QWidget                 *replace_widget;

    QLineEdit               *search_box;
    QLineEdit               *replace_box;

    QPushButton             *find_previous;                 // Implement buttons
    QPushButton             *find_next;
    QPushButton             *replace_current;
    QPushButton             *replace_all;
    QPushButton             *exit_find_and_replace;

public:
    bool                    replace_mode;

};


#endif // EDITORWIDGET_H
