#include "editorwidget.h"

EditorWidget::EditorWidget(FileInfo file, QWidget *parent) : QWidget(parent)
{
    file_opened = &file;
    initEditor();
    initHighlight();

    loadFile(file);

    connect(text_editor_window->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberWidgetWidth(int)));
    connect(text_editor_window->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberWidget(int)));
    connect(text_editor_window, SIGNAL(textChanged()), this, SLOT(updateLineNumberWidget()));
    connect(text_editor_window, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberWidget()));

    recalcLineWidth();
}

void EditorWidget::initEditor()
{
    editor_font.setPointSize(10);

    text_editor_window = new QTextEdit(this);
    line_number_widget = new LineNumberWidget(this);
    find_and_replace_widget = new FindWidget(0, this);

    text_editor_window->setAcceptRichText(false);
    text_editor_window->installEventFilter(this);

    QPalette temp_palette = text_editor_window->palette();
    temp_palette.setColor(QPalette::Highlight, main_tc_color);
    temp_palette.setColor(QPalette::HighlightedText, main_tt_color);
    text_editor_window->setPalette(temp_palette);

    QFontMetrics metrics(editor_font);
    text_editor_window->setTabStopDistance(tab_width * metrics.averageCharWidth());

    text_editor_window->setFont(editor_font);

    QTextCharFormat temp_format;

    temp_format.setForeground(keyword_highlight_color);
    default_highlight_palette.keyword_format = temp_format;
    temp_format.setForeground(data_type_highlight_color);
    default_highlight_palette.data_type_format = temp_format;
    temp_format.setForeground(single_line_comment_highlight_color);
    default_highlight_palette.single_line_comment_format = temp_format;
    temp_format.setForeground(multi_line_comment_highlight_color);
    default_highlight_palette.multi_line_comment_format = temp_format;
    temp_format.setForeground(preprocessor_highlight_color);
    default_highlight_palette.preprocessor_format = temp_format;
    temp_format.setForeground(quotation_highlight_color);
    default_highlight_palette.quotation_format = temp_format;
    temp_format.setForeground(function_highlight_color);
    default_highlight_palette.function_format = temp_format;
    temp_format.setForeground(numbers_highlight_color);
    default_highlight_palette.numbers_format = temp_format;
    temp_format.setForeground(preprocessor_numbers_highlight_color);
    default_highlight_palette.preprocessor_numbers_format = temp_format;
    temp_format.setForeground(special_symbol_highlight_color);
    default_highlight_palette.special_symbols_format = temp_format;
    temp_format.setForeground(class_highlight_color);
    default_highlight_palette.class_format = temp_format;
    temp_format.setForeground(variable_highlight_color);
    default_highlight_palette.variable_format = temp_format;
    temp_format.setForeground(main_et_color);
    default_highlight_palette.font_format = temp_format;

    text_editor_window->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(text_editor_window, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
}

void EditorWidget::showContextMenu(const QPoint &pos)
{
   QMenu context_menu(tr("Context menu"), text_editor_window);

   QAction action_undo(       "Undo",        text_editor_window);
   QAction action_redo(       "Redo",        text_editor_window);
   QAction action_select(     "Select",      text_editor_window);
   QAction action_select_line("Select Line", text_editor_window);
   QAction action_copy(       "Copy",        text_editor_window);
   QAction action_cut(        "Cut",         text_editor_window);
   QAction action_paste(      "Paste",       text_editor_window);
   QAction action_delete(     "Delete",      text_editor_window);
   QAction action_select_all( "Select All",  text_editor_window);

   connect(&action_undo,        SIGNAL(triggered()), this, SLOT(contextUndo()));
   connect(&action_redo,        SIGNAL(triggered()), this, SLOT(contextRedo()));
   connect(&action_select,      SIGNAL(triggered()), this, SLOT(contextSelect()));
   connect(&action_select_line, SIGNAL(triggered()), this, SLOT(contextSelectLine()));
   connect(&action_copy,        SIGNAL(triggered()), this, SLOT(contextCopy()));
   connect(&action_cut,         SIGNAL(triggered()), this, SLOT(contextCut()));
   connect(&action_paste,       SIGNAL(triggered()), this, SLOT(contextPaste()));
   connect(&action_delete,      SIGNAL(triggered()), this, SLOT(contextDelete()));
   connect(&action_select_all,  SIGNAL(triggered()), this, SLOT(contextSelectAll()));

   QTextCursor cursor = text_editor_window->textCursor();
   QClipboard *clipboard = QGuiApplication::clipboard();

   if (cursor.hasSelection())
   {
        action_select.setDisabled(true);
        action_select_line.setDisabled(true);
        action_select_all.setDisabled(true);
   }
   else
   {
        if (clipboard->text() == "")
        {
            action_paste.setDisabled(true);
        }
        action_delete.setDisabled(true);
        action_copy.setDisabled(true);
        action_cut.setDisabled(true);
   }

   context_menu.addAction(&action_undo);
   context_menu.addAction(&action_redo);
   context_menu.addSeparator();
   context_menu.addAction(&action_select);
   context_menu.addAction(&action_select_line);
   context_menu.addAction(&action_select_all);
   context_menu.addSeparator();
   context_menu.addAction(&action_delete);
   context_menu.addSeparator();
   context_menu.addAction(&action_cut);
   context_menu.addAction(&action_paste);

   context_menu.setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");

   context_menu.exec(mapToGlobal(pos));
}

void EditorWidget::contextUndo()
{
    makeUndo();
}

void EditorWidget::contextRedo()
{
    makeRedo();
}

void EditorWidget::contextSelect()
{
    QTextCursor cursor = text_editor_window->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    text_editor_window->setTextCursor(cursor);
}

void EditorWidget::contextSelectLine()
{
    QTextCursor cursor = text_editor_window->textCursor();
    cursor.select(QTextCursor::LineUnderCursor);
    text_editor_window->setTextCursor(cursor);
}

void EditorWidget::contextCopy()
{
    makeCopy();
}

void EditorWidget::contextCut()
{
    makeCut();
}

void EditorWidget::contextPaste()
{
    makePaste();
}

void EditorWidget::contextDelete()
{
    QTextCursor cursor = text_editor_window->textCursor();
    cursor.removeSelectedText();
}

void EditorWidget::contextSelectAll()
{
    makeSelectAll();
}

void EditorWidget::initHighlight(QTextDocument* input)
{
    if (input == nullptr)
    {
         highlighter = new Highlighter(syntax_standart, default_highlight_palette, text_editor_window->document());
    }
    else
    {
         highlighter = new Highlighter(syntax_standart, default_highlight_palette, input);
    }
}

void EditorWidget::resizeWidget()
{
    int max_height_offset;
    if (find_visible)
    {
        max_height_offset = find_height * (replace_mode + 1);
    }
    else
    {
        max_height_offset = 0;
    }

    if (line_number_visible)
    {
        text_editor_window->setGeometry(line_number_width, rect().y(), rect().width() - line_number_width, rect().height() - max_height_offset);
        line_number_widget->setGeometry(rect().x(), rect().y(), line_number_width, rect().height() - max_height_offset);
    }
    else
    {
        text_editor_window->setGeometry(0, rect().y(), rect().width(), rect().height() - max_height_offset);
        line_number_widget->setGeometry(rect().x(), rect().y(), 0, rect().height() - max_height_offset);
    }
    find_and_replace_widget->setGeometry(0, rect().height() - max_height_offset, rect().width(), max_height_offset);
    update();
}

void EditorWidget::recalcLineWidth()
{
    int digits = 1;
    int max = qMax(1, text_editor_window->document()->blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    font_metrics = new QFontMetrics(text_editor_window->font());
    size_t space = 10 + font_metrics->averageCharWidth() * digits;

    if (line_number_visible)
    {
        if (line_number_width != space)
        {
            line_number_width = space;
        }
    }
    else
    {
        space = 0;
    }
    resizeWidget();
}

void EditorWidget::makeUndo()
{
    emit changed(EDIT_UNDO);
    text_editor_window->undo();
}

void EditorWidget::makeRedo()
{
    emit changed(EDIT_REDO);
    text_editor_window->redo();
}

void EditorWidget::makeCopy()
{
    emit changed(EDIT_COPY);
    text_editor_window->copy();
}

void EditorWidget::makeCut()
{
    emit changed(EDIT_CUT);
    text_editor_window->cut();
}

void EditorWidget::makePaste()
{
    emit changed(EDIT_PASTE);
    text_editor_window->paste();
}

void EditorWidget::makeFind()
{
    find_visible = true;
    replace_mode = false;
    resizeWidget();
    find_and_replace_widget->changeMode(false);
}

void EditorWidget::makeFindAndReplace()
{
    find_visible = true;
    replace_mode = true;
    resizeWidget();
    find_and_replace_widget->changeMode(true);
}

void EditorWidget::hideFindAndReplace()
{
    find_visible = false;
    replace_mode = false;
    resizeWidget();
    find_and_replace_widget->changeMode(false);
}

void EditorWidget::toggleLineNumber()
{
    emit changed(VIEW_LINE_NUMBER);
    line_number_visible = !line_number_visible;
    resizeWidget();
    recolorWidget(background_current, font_current);
}

void EditorWidget::toggleHighlight()
{
    emit changed(VIEW_SX_HIGHLIGHT);
    bool temp = !highlighter->highlight_visible;
    initHighlight(highlighter->document());
    highlighter->highlight_visible = temp;
}

void EditorWidget::toggleNewLineMode()
{
    emit changed(VIEW_LINE_NUMBER);
    if (newline_instead_wrap)
    {
        text_editor_window->setLineWrapMode(QTextEdit::WidgetWidth);
    }
    else
    {
        text_editor_window->setLineWrapMode(QTextEdit::NoWrap);
    }
    newline_instead_wrap = !newline_instead_wrap;
}

void EditorWidget::makeSelectAll()
{
    emit changed(EDIT_SELECT_ALL);
    text_editor_window->selectAll();
}

void EditorWidget::updateLineNumberWidgetWidth(int /* newBlockCount */)
{
    recalcLineWidth();
}

void EditorWidget::updateLineNumberWidget(QRectF /*rect_f*/)
{
    EditorWidget::updateLineNumberWidget();
}

void EditorWidget::updateLineNumberWidget(int /*slider_pos*/)
{
    EditorWidget::updateLineNumberWidget();
}

void EditorWidget::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     QTextEdit::ExtraSelection selection;

     selection.format.setBackground(line_highlight_color);
     selection.format.setProperty(QTextFormat::FullWidthSelection, true);
     selection.cursor = text_editor_window->textCursor();
     selection.cursor.clearSelection();
     extraSelections.append(selection);

     text_editor_window->setExtraSelections(extraSelections);
 }

void EditorWidget::updateLineNumberWidget()
{
    highlightCurrentLine();
    file_opened->updated = true;
    emit changed(NONE);
    text_editor_window->verticalScrollBar()->setSliderPosition(text_editor_window->verticalScrollBar()->sliderPosition());

    QRect rect =  this->contentsRect();
    line_number_widget->update(0, rect.y(), line_number_widget->width(), rect.height());
    updateLineNumberWidgetWidth(0);

    int dy = text_editor_window->verticalScrollBar()->sliderPosition();
    if (dy > -1)
    {
        line_number_widget->scroll(0, dy);
    }

    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || text_editor_window->textCursor().block().blockNumber() == first_block_id - 1)
    {
        if (line_number_visible)
        {
            text_editor_window->verticalScrollBar()->setSliderPosition(dy - line_number_width);
        }
        else
        {
            text_editor_window->verticalScrollBar()->setSliderPosition(dy);
        }
    }
    resizeWidget();
}

void EditorWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QRect cr = rect();
    if (line_number_visible)
    {
        line_number_widget->setGeometry(QRect(cr.x(), cr.y(), line_number_width, cr.height()));
    }
    else
    {
        line_number_widget->setGeometry(QRect(cr.x(), cr.y(), 0, cr.height()));
    }
}

void EditorWidget::recolorWidget(QColor background, QColor text)
{
    background_current = background;
    font_current = text;
    if (line_number_visible)
    {
        text_editor_window->setStyleSheet(QString(
                                              "QTextEdit {"
                                              "    background-color: " + background.name() + ";"
                                              "    color: " + text.name() + ";"
                                              "    border-style: outset;"
                                              "    border-width: 0px;"
                                              "    border-top-right-radius: 10px;"
                                              "    border-top-left-radius: 0px;"
                                              "    padding: 5px 5px 5px 5px;"
                                              "}"
                                              ));
        line_number_widget->setStyleSheet(QString(
                                               "QTextEdit {"
                                               "    background-color: red;"
                                               "    color: red;"
                                               "    border-style: outset;"
                                               "    border-width: 10px;"
                                               "    border-top-left-radius: 10px;"
                                               "}"
                                               ));
    }
    else
    {
        text_editor_window->setStyleSheet(QString(
                                              "QTextEdit {"
                                              "    background-color: " + background.name() + ";"
                                              "    color: " + text.name() + ";"
                                              "    border-style: outset;"
                                              "    border-width: 0px;"
                                              "    border-top-right-radius: 10px;"
                                              "    border-top-left-radius: 10px;"
                                              "    padding: 5px 5px 5px 5px;"
                                              "}"
                                              ));
        line_number_widget->setStyleSheet(QString(
                                               "QTextEdit {"
                                               "    background-color: red;"
                                               "    color: red;"
                                               "    border-style: outset;"
                                               "    border-width: 0px;"
                                               "    border-top-left-radius: 0px;"
                                               "}"
                                               ));
    }

    text_editor_window->verticalScrollBar()->setStyleSheet(QString(
                                                               "QScrollBar {"
                                                               "    background: " + background.name() + ";"
                                                               "    width: 8px;"
                                                               "    margin: 0px 0px 0px 0px;"
                                                               "}"
                                                               "QScrollBar::handle {"
                                                               "    background: " + background.name() + ";"
                                                               "    border-radius: 4px;"
                                                               "    background-color: #838383;"
                                                               "    min-height: 16px;"
                                                               "}"
                                                               "QScrollBar::sub-line, QScrollBar::add-line {"
                                                               "    background: none;"
                                                               "}"
                                                               "QScrollBar::add-page, QScrollBar::sub-page {"
                                                               "    background: none;"
                                                               "}"
                                                               ));
    text_editor_window->horizontalScrollBar()->setStyleSheet(QString(
                                                               "QScrollBar {"
                                                               "    background: " + background.name() + ";"
                                                               "    height: 8px;"
                                                               "    margin: 0px 0px 0px 0px;"
                                                               "}"
                                                               "QScrollBar::handle {"
                                                               "    background: " + background.name() + ";"
                                                               "    border-radius: 4px;"
                                                               "    background-color: #838383;"
                                                               "    min-width: 16px;"
                                                               "}"
                                                               "QScrollBar::sub-line, QScrollBar::add-line {"
                                                               "   background: none;"
                                                               "}"
                                                               "QScrollBar::add-page, QScrollBar::sub-page {"
                                                               "    background: none;"
                                                               "}"
                                                               ));
    QPalette temp_palette = text_editor_window->palette();
    temp_palette.setColor(QPalette::Highlight, main_tc_color);
    temp_palette.setColor(QPalette::HighlightedText, main_tt_color);
    text_editor_window->setPalette(temp_palette);
}

bool EditorWidget::eventFilter(QObject* object, QEvent* event)
{
    switch (event->type())
    {
        case QEvent::KeyPress:
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->matches(QKeySequence::Copy))
            {
                emit changed(EDIT_COPY);
                makeCopy();
                return true;
            }
            if (ke->matches(QKeySequence::Paste))
            {
                emit changed(EDIT_PASTE);
                makePaste();
                return true;
            }
            if (ke->matches(QKeySequence::Undo))
            {
                emit changed(EDIT_UNDO);
                makeUndo();
                return true;
            }
            if (ke->matches(QKeySequence::Redo))
            {
                emit changed(EDIT_REDO);
                makeRedo();
                return true;
            }
            if (ke->matches(QKeySequence::SelectAll))
            {
                emit changed(EDIT_SELECT_ALL);
                makeSelectAll();
                return true;
            }
            if (ke->matches(QKeySequence::Cut))
            {
                emit changed(EDIT_CUT);
                makeCut();
                return true;
            }
            emit changed(KEY_PRESSED);
        }
        break;

        default:
        {

        }
        break;
    }
    return QObject::eventFilter(object, event);
}

void EditorWidget::loadFile(FileInfo file)
{
    if (!QDir(file.path).exists())
    {
        QDir().mkdir(file.path);
    }
    QFile file_r(file.path + file.real_name);
    if (!file_r.exists())
    {
        qDebug() << "No such file" << file.path + file.real_name ;
        file_r.open(QIODevice::WriteOnly);
        file_r.close();
    }
    if (file_r.open(QIODevice::ReadOnly))
    {
        QString temp(file_r.readAll());
        text_editor_window->setPlainText(temp);
        file_r.close();
    }
    else
    {
        qDebug() << "File cannot be opened" << file.path + file.real_name ;
    }
}

void EditorWidget::updateFileInfo(FileInfo file)
{
    file_opened = new FileInfo(file);
}

void EditorWidget::saveFile()
{
    QFile file_r(file_opened->path + file_opened->real_name);
    if(file_r.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file_r);
        out << text_editor_window->toPlainText().toUtf8();
        file_r.close();
    }
    else
    {
        qDebug() << "File cannot be created" << file_opened->path + file_opened->real_name ;
    }
}

void EditorWidget::changeFont(QFont font)
{
    editor_font = font;
    text_editor_window->textCursor().charFormat().setFont(editor_font);
    text_editor_window->setFont(editor_font);
    updateLineNumberWidget();
    emit changed(FORMAT_CHANGE_FONT);
}

QColor EditorWidget::colorChange(QColor color)
{
    main_ed_color = color;
    emit changed(VIEW_CHANGED_COLOR);
    recolorWidget(color, font_current);
    main_eh_color = QColor(((color.red() + 25) > 255) ? (255) : (color.red() + 25), ((color.green() + 25) > 255) ? (255) : (color.green() + 25), ((color.blue() + 25) > 255) ? (255) : (color.blue() + 25));
    find_and_replace_widget->recolorWidget();
    return (QColor(((color.red() - 25) < 0) ? (0) : (color.red() - 25), ((color.green() - 25) < 0) ? (0) : (color.green() - 25), ((color.blue() - 25) < 0) ? (0) : (color.blue() - 25)));
}

void EditorWidget::selectionColorChange(QColor color)
{
    main_tc_color = color;
    main_tt_color = QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
    QPalette temp_palette = text_editor_window->palette();
    temp_palette.setColor(QPalette::Highlight, main_tc_color);
    temp_palette.setColor(QPalette::HighlightedText, main_tt_color);
    text_editor_window->setPalette(temp_palette);
    emit changed(VIEW_SLC_COLOR);
}

void EditorWidget::mouseMoveEvent(QMouseEvent * event)
{
    setCursor(Qt::ArrowCursor);
    QWidget::mouseMoveEvent(event);
}

int EditorWidget::getFirstVisibleBlockId()
{
    QTextCursor curs = QTextCursor(text_editor_window->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < text_editor_window->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = text_editor_window->viewport()->geometry();
        QRect r2 = text_editor_window->document()->documentLayout()->blockBoundingRect(block).translated(text_editor_window->viewport()->geometry().x(), text_editor_window->viewport()->geometry().y() - (text_editor_window->verticalScrollBar()->sliderPosition())).toRect();

        if (r1.contains(r2, true))
        {
            return i;
        }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

bool EditorWidget::searchString(QString regex, bool mode, bool second)
{
    bool state = true;
    QTextCursor cursor = text_editor_window->textCursor();
    QTextCursor cursorSaved = cursor;
    if (mode)
    {
        if (!text_editor_window->find(QRegularExpression(regex), QTextDocument::FindCaseSensitively))
        {
            state = false;
        }
    }
    else
    {
        if (!text_editor_window->find(QRegularExpression(regex), QTextDocument::FindBackward | QTextDocument::FindCaseSensitively))
        {
            state = false;
        }
    }
    if (!state && !second)
    {
        cursor.movePosition(mode?QTextCursor::Start:QTextCursor::End);
        text_editor_window->setTextCursor(cursor);
        searchString(regex, mode, true);
    }
    else if (!state && second)
    {
        qDebug() << "No more selections found";
        text_editor_window->setTextCursor(cursorSaved);
    }
    text_editor_window->setFocus();
    return state;
}

void EditorWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    text_editor_window->verticalScrollBar()->setSliderPosition(text_editor_window->verticalScrollBar()->sliderPosition());

    QPainter painter(line_number_widget);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(line_number_widget->rect(), 10, 10);
    path.addRect(line_number_widget->rect().width() - 10, line_number_widget->rect().y(), 10, 10);
    path.addRect(line_number_widget->rect().x(), line_number_widget->rect().height() - 10, line_number_widget->rect().width(), 10);
    painter.fillPath(path, main_ed_color);
    int block_number = getFirstVisibleBlockId();

    QTextBlock block = text_editor_window->document()->findBlockByNumber(block_number);
    QTextBlock prev_block = (block_number > 0) ? text_editor_window->document()->findBlockByNumber(block_number-1) : block;
    int translate_y = (block_number > 0) ? -text_editor_window->verticalScrollBar()->sliderPosition() : 0;

    int top = text_editor_window->viewport()->geometry().top();

    int additional_margin;
    if (block_number == 0)
    {
        additional_margin = (int) 5 - text_editor_window->verticalScrollBar()->sliderPosition();
    }
    else
    {
        additional_margin = (int) text_editor_window->document()->documentLayout()->blockBoundingRect(prev_block).translated(0, translate_y).intersected(text_editor_window->viewport()->geometry()).height() + 5;
    }

    top += additional_margin;

    int bottom = top + (int) text_editor_window->document()->documentLayout()->blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(block_number + 1);
            painter.setPen(main_ed_color);
            painter.setPen((text_editor_window->textCursor().blockNumber() == block_number) ? main_et_color : main_tx_color);
            painter.setFont(text_editor_window->font());
            painter.fillRect(QRect(0, (int)top, (int)line_number_widget->width(), (int)font_metrics->height()), (text_editor_window->textCursor().blockNumber() == block_number) ? main_eh_color : main_ed_color);
            painter.drawText(0, top, line_number_widget->width(), font_metrics->height(), Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) text_editor_window->document()->documentLayout()->blockBoundingRect(block).height();
        ++block_number;
    }
}

LineNumberWidget::LineNumberWidget(EditorWidget *editor) : QWidget(editor)
{
    code_editor = editor;
}

QSize LineNumberWidget::sizeHint() const
{
    return QSize(((EditorWidget *)code_editor)->line_number_width, 0);
}

void LineNumberWidget::paintEvent(QPaintEvent *event)
{
    ((EditorWidget *)code_editor)->lineNumberAreaPaintEvent(event);
}

FindWidget::FindWidget(bool mode, EditorWidget *editor) : QWidget(editor)
{
    code_editor = editor;
    replace_mode = mode;

    search_and_replace_layout = new QGridLayout(this);

    search_widget = new QWidget();
    replace_widget = new QWidget();

    search_layout = new QHBoxLayout(search_widget);
    replace_layout = new QHBoxLayout(replace_widget);

    search_box = new QLineEdit("");
    replace_box = new QLineEdit("");

    search_layout->addWidget(search_box, Qt::AlignLeft);
    replace_layout->addWidget(replace_box, Qt::AlignLeft);

    search_and_replace_layout->addWidget(search_widget, 0, 0, Qt::AlignLeft);
    search_and_replace_layout->addWidget(replace_widget, 0, 1, Qt::AlignLeft);


    find_previous = new QPushButton("Previous");                 // Implement buttons
    find_next = new QPushButton("Next");
    replace_current = new QPushButton("Replace");
    replace_all = new QPushButton("Replace All");
    exit_find_and_replace = new QPushButton("Exit");

    connect(find_previous, SIGNAL(clicked()), this, SLOT(findPrevious()));
    connect(find_next, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(replace_current, SIGNAL(clicked()), this, SLOT(replaceCurrent()));
    connect(replace_all, SIGNAL(clicked()), this, SLOT(replaceAll()));
    connect(exit_find_and_replace, SIGNAL(clicked()), this, SLOT(exitFindAndReplace()));

    search_layout->addWidget(find_previous, Qt::AlignRight);
    search_layout->addWidget(find_next, Qt::AlignRight);
    search_layout->addWidget(exit_find_and_replace, Qt::AlignRight);
    replace_layout->addWidget(replace_current, Qt::AlignRight);
    replace_layout->addWidget(replace_all, Qt::AlignRight);

    recalcSize();
    recolorWidget();
}

void FindWidget::findNext()
{
    code_editor->searchString(search_box->text(), 1);
    emit code_editor->changed(EDIT_FIND);
}

void FindWidget::findPrevious()
{
    code_editor->searchString(search_box->text(), 0);
    emit code_editor->changed(EDIT_FIND);
}

void FindWidget::replaceCurrent()
{
    if (code_editor->text_editor_window->textCursor().hasSelection())
    {
        code_editor->text_editor_window->textCursor().removeSelectedText();
        code_editor->text_editor_window->textCursor().insertText(replace_box->text());
    }
    emit code_editor->changed(EDIT_REPLACE);
}

void FindWidget::replaceAll()
{
    code_editor->text_editor_window->setFocus();
    code_editor->text_editor_window->textCursor().movePosition(QTextCursor::Start);
    do
    {
        replaceCurrent();
    }
    while (code_editor->searchString(search_box->text(), 0));
    emit code_editor->changed(EDIT_REPLACE);
}

void FindWidget::exitFindAndReplace()
{
    code_editor->hideFindAndReplace();
    code_editor->resizeWidget();
    code_editor->text_editor_window->setFocus();
}

void FindWidget::recalcSize()
{
    search_and_replace_layout->setGeometry(rect());
    if (replace_mode)
    {
        search_widget->setGeometry(QRect(0, 0, width(), height() / 2));
        replace_widget->setGeometry(QRect(0, height() / 2, width(), height() / 2));

        search_box->setGeometry(QRect(0, 0, width() * 0.8, height() / 2));
        replace_box->setGeometry(QRect(0, 0, width() * 0.8, height() / 2));

        find_previous->setGeometry(QRect(search_box->width(), 0, (width() - search_box->width()) / 3, height() / 2));
        find_next->setGeometry(QRect(search_box->width() + (width() - search_box->width()) / 3, 0, (width() - search_box->width()) / 3, height() / 2));
        exit_find_and_replace->setGeometry(QRect(search_box->width() - 1 + (2 * (width() - search_box->width())) / 3, 0, (width() - find_previous->width() - find_next->width() - search_box->width()) + 1, height() / 2));

        replace_current->setGeometry(QRect(replace_box->width(), 0, (width() - replace_box->width()) / 2, height() / 2));
        replace_all->setGeometry(QRect(replace_box->width() + (width() - replace_box->width()) / 2, 0, (width() - replace_box->width()) / 2, height() / 2));
    }
    else
    {
        search_widget->setGeometry(QRect(0, 0, width(), height()));
        replace_widget->setGeometry(QRect(0, height() / 2, width(), 0));

        search_box->setGeometry(QRect(0, 0, width() * 0.8, height()));
        replace_box->setGeometry(QRect(0, 0, width() * 0.8, 0));

        find_previous->setGeometry(QRect(search_box->width(), 0, (width() - search_box->width()) / 3, height()));
        find_next->setGeometry(QRect(search_box->width() + (width() - search_box->width()) / 3, 0, (width() - search_box->width()) / 3, height()));
        exit_find_and_replace->setGeometry(QRect(search_box->width() - 1 + (2 * (width() - search_box->width())) / 3, 0, (width() - find_previous->width() - find_next->width() - search_box->width()) + 1, height()));
    }

    search_layout->setGeometry(search_widget->rect());
    replace_layout->setGeometry(replace_widget->rect());
}

void FindWidget::changeMode(bool mode)
{
    replace_mode = mode;
    recalcSize();
}

void FindWidget::recolorWidget()
{
    search_widget->setStyleSheet(QString("background-color: " + code_editor->main_ed_color.name() + "; color: " + code_editor->main_et_color.name() + ";"));
    replace_widget->setStyleSheet(QString("background-color: " + code_editor->main_ed_color.name() + "; color: " + code_editor->main_et_color.name() + ";"));
    search_box->setStyleSheet(QString("background-color: " + code_editor->main_eh_color.name() + "; border: 0px solid black; border-bottom-width: 1px; border-top-left-radius: 10px; padding: 5px;"));
    find_previous->setStyleSheet(QString("background-color: " + code_editor->main_eh_color.name() + "; border: 0px solid black; border-bottom-width: 1px; border-left-width: 1px;"));
    find_next->setStyleSheet(QString("background-color: " + code_editor->main_eh_color.name() + ";  border: 0px solid black; border-bottom-width: 1px; border-left-width: 1px;"));
    exit_find_and_replace->setStyleSheet(QString("background-color: " + code_editor->main_eh_color.name() + "; border: 0px solid black; border-left-width: 1px; border-bottom-width: 1px; border-top-right-radius: 10px; padding: 5px;"));
    replace_box->setStyleSheet(QString("QLineEdit {background-color: " + code_editor->main_eh_color.name() + "; border: 1px; padding: 5px;}"));
    replace_current->setStyleSheet(QString("background-color: " + code_editor->main_eh_color.name() + "; border: 0px solid black; border-left-width: 1px;"));
    replace_all->setStyleSheet(QString("background-color: " + code_editor->main_eh_color.name() + "; border: 0px solid black; border-left-width: 1px;"));

}

QSize FindWidget::sizeHint() const
{
    if (replace_mode)
    {
        return QSize(((EditorWidget *)code_editor)->find_height * 2, 0);
    }
    else
    {
        return QSize(((EditorWidget *)code_editor)->find_height, 0);
    }
}
