#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    syntax_dialog.current_palette_path = "Default";
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);

    window_rect = QRect(QGuiApplication::primaryScreen()->availableGeometry());
    resize(window_rect.width() / 2, window_rect.height() / 2);

    initNewWindowStyle();
    recalcSizes();

    title_name->show();
    menu_bar_left->show();
    menu_bar_right->show();
    main_editor_widget->show();
    shadow_overlay->show();

    setMouseTracking(true);

    exit_button = new QPushButton(0, this);
    exit_button->setObjectName("Exit");
    exit_button->move(6, 6);
    exit_button->setFixedSize(button_height, button_height);
    exit_button->setFlat(true);
    exit_button->setAttribute(Qt::WA_TranslucentBackground);

    minimize_button = new QPushButton(0, this);
    minimize_button->setObjectName("Min");
    minimize_button->move(24, 6);
    minimize_button->setFixedSize(button_height, button_height);
    minimize_button->setFlat(true);
    minimize_button->setAttribute(Qt::WA_TranslucentBackground);

    maximize_button = new QPushButton(0, this);
    maximize_button->setObjectName("Max");
    maximize_button->move(42, 6);
    maximize_button->setFixedSize(button_height, button_height);
    maximize_button->setFlat(true);
    maximize_button->setAttribute(Qt::WA_TranslucentBackground);

    connect(exit_button, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(minimize_button, SIGNAL(clicked()), this, SLOT(minimize()));
    connect(maximize_button, SIGNAL(clicked()), this, SLOT(maximize()));
}

void MainWindow::initNewWindowStyle()
{
    fullscreen = false;

    setStyleSheet("QMainWindow {} #Exit {border-radius: 6px; background-color: #f44040;} #Exit:hover {background-color: #f48080;} #Min {border-radius: 6px; background-color: #e6ac0c;} #Min:hover {background-color: #e6ca7c;} #Max {border-radius: 6px; background-color: #0ddf34;} #Max:hover {background-color: #71dc85;}");

    menu_bar_left = new QMenuBar(this);
    menu_bar_right = new QMenuBar(this);

    menu_bar_left->setStyleSheet("QMenuBar {background-color: #202020; color: #ffffff;} QMenuBar::item:selected {background-color: #303030} QMenuBar::item:pressed {background-color: #303030} QMenu::item:disabled {color: #999999}");
    menu_bar_right->setStyleSheet("QMenuBar {background-color: #202020; color: #ffffff;} QMenuBar::item:selected {background-color: #303030} QMenuBar::item:pressed {background-color: #303030} QMenu::item:disabled {color: #999999}");

    file      = new QMenu("&File", menu_bar_left);
    edit      = new QMenu("&Edit", menu_bar_left);
    format    = new QMenu("&Format", menu_bar_left);
    view      = new QMenu("&View", menu_bar_left);
    about     = new QMenu("&About", menu_bar_right);

    edit->setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");
    file->setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");
    format->setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");
    view->setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");
    about->setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");

    file_new                    = file->addAction("&New",                           this, SLOT(fileNew()),                  Qt::CTRL    | Qt::Key_N                               );            // Done
    file_open                   = file->addAction("&Open",                          this, SLOT(fileOpen()),                 Qt::CTRL    | Qt::Key_O                               );            // Done
    file_save                   = file->addAction("&Save",                          this, SLOT(fileSave()),                 Qt::CTRL    | Qt::Key_S                               );            // Done
    file_save_as                = file->addAction("&Save As",                       this, SLOT(fileSaveAs()),               Qt::SHIFT   | Qt::CTRL      | Qt::Key_S               );            // Done
    file_exit                   = file->addAction("&Exit",                          this, SLOT(fileExit()),                 Qt::CTRL    | Qt::Key_E                               );            // Done

    edit_undo                   = edit->addAction("&Undo",                          this, SLOT(editUndo()),                 Qt::CTRL    | Qt::Key_Z                               );            // Done 50 %
    edit_redo                   = edit->addAction("&Redo",                          this, SLOT(editRedo()),                 Qt::CTRL    | Qt::Key_Y                               );            // Done 50 %
    edit_copy                   = edit->addAction("&Copy",                          this, SLOT(editCopy()),                 Qt::CTRL    | Qt::Key_C                               );            // Done
    edit_cut                    = edit->addAction("&Cut",                           this, SLOT(editCut()),                  Qt::CTRL    | Qt::Key_X                               );            // Done
    edit_paste                  = edit->addAction("&Paste",                         this, SLOT(editPaste()),                Qt::CTRL    | Qt::Key_V                               );            // Done
    edit_find                   = edit->addAction("&Find",                          this, SLOT(editFind()),                 Qt::CTRL    | Qt::Key_F                               );            // Done
    edit_find_and_replace       = edit->addAction("&Find And Replace",              this, SLOT(editFindAndReplace()),       Qt::SHIFT   | Qt::CTRL      | Qt::Key_F               );            // Done
    edit_select_all             = edit->addAction("&Select All",                    this, SLOT(editSelectAll()),            Qt::CTRL    | Qt::Key_A                               );            // Done

    format_newline              = format->addAction("&Toggle Newline",              this, SLOT(formatNewLine()),            Qt::ALT     | Qt::CTRL      | Qt::Key_N               );            // Done
    format_select_font          = format->addAction("&Select Font",                 this, SLOT(formatSelectFont()),         Qt::ALT     | Qt::CTRL      | Qt::Key_F               );            // Done

    view_background_color       = view->addAction("&Background Color",              this, SLOT(viewBackgroundColor()),      Qt::ALT     | Qt::CTRL      | Qt::Key_B               );            // Done
    view_selection_color        = view->addAction("&Selection Color",               this, SLOT(viewSelectionColor()),       Qt::ALT     | Qt::CTRL      | Qt::Key_S               );            // Done
    view_line_highlight_color   = view->addAction("&Line Highlight",                this, SLOT(viewLineHighlightColor()),   Qt::ALT     | Qt::CTRL      | Qt::Key_L               );            // Todo
    view_show_line_number       = view->addAction("&Toggle Line Number",            this, SLOT(viewShowLineNumber()),       Qt::SHIFT   | Qt::CTRL      | Qt::Key_L               );            // Done
    view_show_toolbar           = view->addAction("&Toggle Toolbar",                this, SLOT(viewShowToolBar()),          Qt::ALT     | Qt::CTRL      | Qt::Key_T               );            // Done
    view_show_state_bar         = view->addAction("&Toggle Statusbar",              this, SLOT(viewShowStateBar()),         Qt::ALT     | Qt::CTRL      | Qt::Key_P               );            // Done
    view_show_syntax_highlight  = view->addAction("&Toggle Highlight",              this, SLOT(viewShowSyntaxHighlight()),  Qt::ALT     | Qt::CTRL      | Qt::Key_H               );            // Done
    view_select_syntax          = view->addAction("&Select Synatax",                this, SLOT(viewSelectSyntax()),         Qt::SHIFT   | Qt::CTRL      | Qt::Key_H               );            // Done
    view_select_style           = view->addMenu("&Syntax ..."                                                                                                                     );

    view_select_style_save      = view_select_style->addAction("&Create New",       this, SLOT(viewSelectStyleSave()),      Qt::ALT     | Qt::SHIFT     | Qt::CTRL   | Qt::Key_S  );            // Dane
    view_select_style_load      = view_select_style->addAction("&Load",             this, SLOT(viewSelectStyleLoad()),      Qt::ALT     | Qt::SHIFT     | Qt::CTRL   | Qt::Key_L  );            // Done
    view_select_style_default   = view_select_style->addAction("&Default",          this, SLOT(viewSelectStyleDefault()),   Qt::ALT     | Qt::SHIFT     | Qt::CTRL   | Qt::Key_D  );            // Done
    view_select_style_avaliable = view_select_style->addMenu("&Recent"                                                                                                            );

    about_show                  = about->addAction("&About",                        this, SLOT(aboutShow()),                Qt::ALT     | Qt::SHIFT     | Qt::CTRL   | Qt::Key_A  );            // ToDo

    recalcAlaviableStyles();

    format_newline->setCheckable(true);
    format_newline->setChecked(true);

    view_show_line_number->setCheckable(true);
    view_show_line_number->setChecked(true);

    view_show_toolbar->setCheckable(true);
    view_show_toolbar->setChecked(true);

    view_show_state_bar->setCheckable(true);
    view_show_state_bar->setChecked(true);

    view_show_syntax_highlight->setCheckable(true);
    view_show_syntax_highlight->setChecked(true);

    menu_bar_left->addMenu(file);
    menu_bar_left->addMenu(edit);
    menu_bar_left->addMenu(format);
    menu_bar_left->addMenu(view);
    menu_bar_right->addMenu(about);

    main_menu_layout = new QHBoxLayout(this);

    main_menu_layout->addWidget(menu_bar_left);
    main_menu_layout->addWidget(menu_bar_right);

    main_menu_layout->setAlignment(menu_bar_left, Qt::AlignLeft);
    main_menu_layout->setAlignment(menu_bar_right, Qt::AlignRight);

    title_name = new QLabel("Filename", this);
    title_name->setObjectName("Filename");
    title_name->setText("CHOODITOR");
    title_name->setAlignment(Qt::AlignCenter);
    title_name->setFont(main_ui_font);
    title_name->setStyleSheet("#Filename {color:" + main_tx_color.name() + "}");

    FileInfo temp_file;

    main_editor_widget = new EditorWidget(temp_file, this);
    main_editor_widget->recolorWidget(main_ed_color, main_et_color);
    connect(main_editor_widget, SIGNAL(changed(uint8_t)), this, SLOT(fileChange(uint8_t)));

    status_bar_widget = new QLabel(this);
    status_bar_widget->setAttribute(Qt::WA_TransparentForMouseEvents);
    shadow_overlay = new QWidget(this);

    updateFooter();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFooter()));
    timer->start(100);

    scroll_area = new QScrollArea(this);

    loadSettings();

    if (files.size() != 0 )
    {
        temp_file = files[opened_file_index];
    }
    else
    {
        QDate temp_date = QDate::currentDate();
        QTime temp_time = QTime::currentTime();
        temp_file = {"untitled",temp_date.toString("dd-MM-yyy") + + "-" + temp_time.toString("hh-mm-ss-zzz") + ".cpp", QDir::currentPath() + "/tmp/", 1, 0};
        files.push_back(temp_file);
        opened_file_index = files.size() - 1;
    }

    q_tool_bar = new QToolBar(this);
    q_tool_bar->setMovable(false);
    q_tool_bar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setAttribute(Qt::WA_AlwaysShowToolTips, true);

    q_tool_bar->addAction(QIcon(":/ico/image/new-file"),   "&New",   this, SLOT(fileNew()));
    q_tool_bar->addAction(QIcon(":/ico/image/new-file"),   "&Open",  this, SLOT(fileOpen()));
    q_tool_bar->addAction(QIcon(":/ico/image/save-file"),  "&Save",  this, SLOT(fileSave()));
    q_tool_bar->addAction(QIcon(":/ico/image/undo-file"),  "&Undo",  this, SLOT(editUndo()));
    q_tool_bar->addAction(QIcon(":/ico/image/redo-file"),  "&Redo",  this, SLOT(editRedo()));
    q_tool_bar->addAction(QIcon(":/ico/image/copy-file"),  "&Copy",  this, SLOT(editCopy()));
    q_tool_bar->addAction(QIcon(":/ico/image/cut-file"),   "&Cut",   this, SLOT(editCut()));
    q_tool_bar->addAction(QIcon(":/ico/image/paste-file"), "&Paste", this, SLOT(editPaste()));

    q_tool_bar->actions()[0]->setToolTip("Create New File");
    q_tool_bar->actions()[1]->setToolTip("Open Existing File");
    q_tool_bar->actions()[2]->setToolTip("Save File");
    q_tool_bar->actions()[3]->setToolTip("Undo Last Action");
    q_tool_bar->actions()[4]->setToolTip("Redo Last Action");
    q_tool_bar->actions()[5]->setToolTip("Copy Selected");
    q_tool_bar->actions()[6]->setToolTip("Cut Selected");
    q_tool_bar->actions()[7]->setToolTip("Paste Selected");

    QMenu *temp_menu = new QMenu();
    temp_menu->addAction(QIcon(":/ico/image/find-file"),             "&Find",             this, SLOT(editFind()));
    temp_menu->addAction(QIcon(":/ico/image/find-and-replace-file"), "&Find And Replace", this, SLOT(editFindAndReplace()));

    QToolButton* tool_button = new QToolButton();
    tool_button->setIcon(QIcon(":/ico/image/find-file"));
    tool_button->setMenu(temp_menu);
    tool_button->setPopupMode(QToolButton::InstantPopup);

    QWidgetAction* tool_button_action = new QWidgetAction(this);
    tool_button_action->setDefaultWidget(tool_button);

    q_tool_bar->addAction(tool_button_action);

    q_tool_bar->setStyleSheet("QToolBar QToolButton QIcon {color: #ffffff;} QToolBar QToolButton {background-color: #202020; color: #ffffff; border: none;} QToolBar {background-color: #202020; color: #ffffff; border: none;} QToolBar QToolButton:hover {background-color: #303030} QToolBar QToolButton:pressed {background-color: #303030}");
    temp_menu->setStyleSheet("QMenu {background-color: #303030; color: #ffffff;} QMenu::item:selected {background-color: #404040} QMenu::item:disabled {color: #999999}");

    repaintListing();
    repaint();
}

void MainWindow::openRecentFile(long long index)
{
    viewSelectStyleLoad(syntax_dialog.recent_files.at(index).absoluteFilePath());
}

void MainWindow::updateFooter()
{
    QString action_type;
    switch (footer_event.action)
    {
        case NONE:
        {
            action_type = "Idle action";
        }
        break;

        case NEW_FILE_CREATED:
        {
            action_type = "New File Created";
        }
        break;

        case FILE_OPENED:
        {
            action_type = "File Opened";
        }
        break;

        case FILE_SAVED:
        {
            action_type = "File Saved As";
        }
        break;

        case FILE_SAVED_AS:
        {
            action_type = "File Saved";
        }
        break;

        case FILE_CLOSED:
        {
            action_type = "File Closed";
        }
        break;

        case EDIT_UNDO:
        {
            action_type = "Undo Performed";
        }
        break;

        case EDIT_REDO:
        {
            action_type = "Redo Performed";
        }
        break;

        case EDIT_COPY:
        {
            action_type = "Copy Performed";
        }
        break;

        case EDIT_CUT:
        {
            action_type = "Cut Performed";
        }
        break;

        case EDIT_PASTE:
        {
            action_type = "Paste Performed";
        }
        break;

        case EDIT_FIND:
        {
            action_type = "Find Performed";
        }
        break;

        case EDIT_REPLACE:
        {
            action_type = "Replace Performed";
        }
        break;

        case EDIT_SELECT_ALL:
        {
            action_type = "Select All Performed";
        }
        break;

        case FORMAT_LINE_SWITCH:
        {
            action_type = "Format Toggled";
        }
        break;

        case VIEW_LINE_NUMBER:
        {
            action_type = "Line Number Toggled";
        }
        break;

        case VIEW_TOOL_BAR:
        {
            action_type = "Toolbar Toggled";
        }
        break;

        case VIEW_SX_HIGHLIGHT:
        {
            action_type = "Highliht Toggled";
        }
        break;

        case VIEW_STATUSBAR:
        {
            action_type = "Statusbar Toggled";
        }
        break;

        case FORMAT_CHANGE_FONT:
        {
            action_type = "Font Changed";
        }
        break;

        case VIEW_CHANGED_COLOR:
        {
            action_type = "Color Changed [Background]";
        }
        break;

        case VIEW_SLC_COLOR:
        {
            action_type = "Color Changed [Selection]";
        }
        break;

        case VIEW_SYNTAX_SLC:
        {
            action_type = "Syntax Standard Changed";
        }
        break;

        case WINDOW_MAXIMIZED:
        {
            action_type = "Window Maximized";
        }
        break;

        case WINDOW_MINIMIZED:
        {
            action_type = "Window Minimized";
        }
        break;

        case WINDOW_MOVED:
        {
            action_type = "Window Moved";
        }
        break;

        case WINDOW_RESIZED:
        {
            action_type = "Window Resized";
        }
        break;

        case VIEW_SYNTAX_CRT:
        {
            action_type = "New Syntax Highlight Style Created";
        }
        break;

        case VIEW_SYNTAX_LRT:
        {
            action_type = "Syntax Highlight Style Loaded";
        }
        break;

        case VIEW_SYNTAX_DRT:
        {
            action_type = "Syntax Highlight Style Reset";
        }
        break;

        case ABOUT_OPENED:
        {
            action_type = "About page opened";
        }
        break;

        case KEY_PRESSED:
        {
            action_type = "Key pressed";
        }
        break;

        default:
        {
            action_type = "Undetermined Action";
        }
        break;
    }
    status_bar_widget->setText("Line: " + QString::number(footer_event.line) + " Symbol: " + QString::number(footer_event.symbol) + " | Time: " + QTime::currentTime().toString("hh:mm:ss") + " Last Action: " + action_type + " | Lines:  " + QString::number(footer_event.lines) + " Words:  " + QString::number(footer_event.words) + " Symbols:  " + QString::number(footer_event.symbols) + " KB: " +  QString::number(footer_event.kb_size));
}

void MainWindow::repaintListing()
{
    QWidget *listion_widget = new QWidget(this);
    QHBoxLayout *file_representation_box = new QHBoxLayout(listion_widget);

    scroll_area->setStyleSheet(QString("QScrollArea {background-color: transparent; border: none;}"));
    listion_widget->setStyleSheet(QString("QWidget {background-color: transparent;}"));

    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    scroll_area->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");

    file_representation_box->setContentsMargins(0, 0, 0, 0);
    file_representation_box->setSpacing(0);

    for (const FileInfo &temp_file : files)
    {
        FileRepresentation* file_representation = new FileRepresentation(temp_file.name, temp_file.updated, main_lt_color, file_representation_box->widget());
        file_representation->main_bg_color = main_fr_color;
        file_representation_box->addWidget(file_representation);
        file_representation_box->setAlignment(file_representation, Qt::AlignBottom | Qt::AlignLeft);
        file_representation->show();
        file_representation->setVisible(true);
    }
    file_representation_box->addStretch(0);
    listion_widget->setLayout(file_representation_box);
    scroll_area->setWidget(listion_widget);
    scroll_area->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    scroll_area->installEventFilter(this);

    shadow_overlay->setStyleSheet("* {background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 " + main_bg_color.name() + ", stop:0.1 transparent, stop:0.9 transparent, stop:1 " + main_bg_color.name() + ");}");
    shadow_overlay->setAttribute(Qt::WA_TransparentForMouseEvents);

    status_bar_widget->setStyleSheet("padding-left: 5px; padding-right: 5px; color: " + main_editor_widget->main_tx_color.name());
}

void MainWindow::recalcSizes()
{
    int font_width = title_name->fontMetrics().boundingRect(title_name->text()).width();

    rounded_window_path = new QPainterPath();
    rounded_window_path->setFillRule(Qt::WindingFill);

    if (!fullscreen)
    {
        fullscreen_offset = 10;
        rounded_window_path->addRoundedRect(this->rect().x(), this->rect().y(), this->rect().width(), this->rect().height(), window_border_radius, window_border_radius);

        top_middle_resize       = QRect(colision_offset, 0, this->rect().width() - (colision_offset * 2), colision_offset);
        right_middle_resize     = QRect(this->rect().width() - colision_offset, colision_offset, colision_offset, this->rect().height() - (colision_offset * 2));
        bottom_middle_resize    = QRect(colision_offset, this->rect().height() - colision_offset, this->rect().width() - (colision_offset * 2), colision_offset);
        left_middle_resize      = QRect(0, colision_offset, colision_offset, this->rect().height() - (colision_offset * 2));
        top_left_resize         = QRect(0, 0, colision_offset, colision_offset);
        top_right_resize        = QRect(this->rect().width() - colision_offset, 0, colision_offset, colision_offset);
        bottom_right_resize     = QRect(this->rect().width() - colision_offset, this->rect().height() - colision_offset, colision_offset, colision_offset);
        bottom_left_resize      = QRect(0, this->rect().height() - colision_offset, colision_offset, colision_offset);
    }
    else
    {
        fullscreen_offset = 0;
        rounded_window_path->addRect(this->rect());
    }

    header_rect = QRect(0, 0, this->width(), header_height);
    menubar_rect = QRect(0, header_rect.y() + header_rect.height(), this->width(), toolbar_height);

    if (tool_bar_visible)
    {
        toolbar_rect = QRect(0, menubar_rect.y() + menubar_rect.height(), this->width(), toolbar_height);
    }
    else
    {
        toolbar_rect = QRect(0, menubar_rect.y() + menubar_rect.height(), this->width(), 0);
    }

    listing_rect = QRect(colision_offset, toolbar_rect.y() + toolbar_rect.height(), this->width() - (colision_offset * 2), listing_height);

    if (status_bar_visible)
    {
        footer_rect = QRect(0, this->height() - footer_height, this->width(), footer_height);
    }
    else
    {
        footer_rect = QRect(0, 0, 0, 0);
    }

    editor_rect = QRect(colision_offset, listing_rect.y() + listing_rect.height(), this->width() - (colision_offset * 2), height() - header_rect.height() - toolbar_rect.height() - listing_rect.height() - footer_rect.height() - menubar_rect.height());

    status_bar_widget->setGeometry(footer_rect);

    main_menu_layout->setGeometry(menubar_rect);
    q_tool_bar->setGeometry(toolbar_rect);
    menu_bar_left->setGeometry(menubar_rect.x(), menubar_rect.y(), menubar_rect.width() - menu_bar_right->width(), menubar_rect.height());
    title_name->setGeometry(QRect((header_rect.width() - font_width) / 2 - 5, header_rect.y(), font_width + 10, header_rect.height()));
    main_editor_widget->setGeometry(editor_rect);
    main_editor_widget->resizeWidget();

    scroll_area->setGeometry(listing_rect.x() + colision_offset, listing_rect.y(), listing_rect.width() - (colision_offset * 2), listing_rect.height());
    shadow_overlay->setGeometry(listing_rect.x() + colision_offset, listing_rect.y(), listing_rect.width() - (colision_offset * 2), listing_rect.height());

    main_editor_widget->find_and_replace_widget->recalcSize();

    repaint();
}

void MainWindow::recalcAlaviableStyles()
{
    for (long long index = 0; index < syntax_dialog.recent_files.length(); ++index)
    {
        view_select_style_avaliable->addAction("&" + syntax_dialog.recent_files.at(index).fileName(), this, [=]() {this->openRecentFile(index);});
    }
}

size_t MainWindow::borderColisionType(QPoint point)
{
    if (!fullscreen)
    {
        if (top_middle_resize.contains(point))
        {
            return TOP_MIDDLE_PRESS;
        }
        if (bottom_middle_resize.contains(point))
        {
            return BOTTOM_MIDDLE_PRESS;
        }
        if (left_middle_resize.contains(point))
        {
            return LEFT_MIDDLE_PRESS;
        }
        if (right_middle_resize.contains(point))
        {
            return RIGHT_MIDDLE_PRESS;
        }
        if (top_left_resize.contains(point))
        {
            return TOP_LEFT_PRESS;
        }
        if (top_right_resize.contains(point))
        {
            return TOP_RIGHT_PRESS;
        }
        if (bottom_right_resize.contains(point))
        {
            return BOTTOM_RIGHT_PRESS;
        }

        if (bottom_left_resize.contains(point))
        {
            return BOTTOM_LEFT_PRESS;
        }
        if (header_rect.contains(point))
        {
            return HEADER_PRESS;
        }
        if (footer_rect.contains(point))
        {
            return FOOTER_PRESS;
        }
        if (listing_rect.contains(point))
        {
            return LISTING_PRESS;
        }
        return OUTSIDE_PRESS;
    }
    return OUTSIDE_PRESS;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setClipping(true);
    painter.setClipPath(*rounded_window_path);

    painter.setBrush(main_bg_color);
    painter.drawRect(listing_rect.x() - colision_offset, listing_rect.y(), listing_rect.width() + (colision_offset * 2), listing_rect.height());
    painter.drawRect(toolbar_rect);
    painter.drawRect(editor_rect.x() - colision_offset, editor_rect.y(), editor_rect.width() + (colision_offset * 2), editor_rect.height());

    painter.setBrush(main_hl_color);
    painter.drawRect(header_rect);
    painter.drawRect(footer_rect);

    QMainWindow::paintEvent(event);
}

void MainWindow::maximize()
{
    footer_event.action = WINDOW_MAXIMIZED;
    if (fullscreen)
    {
        exit_button->move(6, 6);
        minimize_button->move(24, 6);
        maximize_button->move(42, 6);
        showNormal();
    }
    else
    {
        exit_button->move(12, 10);
        minimize_button->move(30, 10);
        maximize_button->move(48, 10);
        this->setWindowState(Qt::WindowFullScreen);
    }
    fullscreen = !fullscreen;
    recalcSizes();
    repaint();
}

void MainWindow::minimize()
{
    footer_event.action = WINDOW_MINIMIZED;
    this->setWindowState(Qt::WindowMinimized);
    recalcSizes();
    repaint();
}

void MainWindow::fileNew()
{
    footer_event.action = NEW_FILE_CREATED;
    if (opened_file_index == std::numeric_limits<std::size_t>::max())
    {
        file_save_as->setDisabled(false);
        file_save->setDisabled(false);
        file_exit->setDisabled(false);
        main_editor_widget->setDisabled(false);
    }
    qDebug() << "fileNew connected";
    QDate temp_date = QDate::currentDate();
    QTime temp_time = QTime::currentTime();
    files.push_back({"untitled", temp_date.toString("dd-MM-yyy") + "-" + temp_time.toString("hh-mm-ss-zzz") + ".cpp", QDir::currentPath() + "/tmp/", 1, 0});
    opened_file_index = files.size() - 1;
    repaintListing();
    main_editor_widget->loadFile(files[files.size() - 1]);
}

void MainWindow::fileOpen()
{
    footer_event.action = FILE_OPENED;
    if (opened_file_index == std::numeric_limits<std::size_t>::max())
    {
        file_save_as->setDisabled(false);
        file_save->setDisabled(false);
        file_exit->setDisabled(false);
        main_editor_widget->setDisabled(false);
    }
    QFileDialog dialog(this);
    FileInfo temp;
    dialog.setFileMode(QFileDialog::AnyFile);

    temp.path = dialog.getOpenFileName(this, "Select a file...");
    temp.real_name = temp.path.mid(temp.path.lastIndexOf("/") + 1),
    temp.name = temp.real_name.left(temp.real_name.lastIndexOf("."));
    temp.path = temp.path.left(temp.path.lastIndexOf("/")) + "/";
    if (temp.path != "/")
    {
        files.push_back({temp.name, temp.real_name, temp.path, 0, 1});
        opened_file_index = files.size() - 1;
        main_editor_widget->loadFile(files[opened_file_index]);
        repaintListing();
        FileRepresentation *temp_fr = (FileRepresentation*)scroll_area->findChild<QWidget*>()->findChild<QHBoxLayout*>()->itemAt(opened_file_index)->widget();
        temp_fr->updateInfo(files[opened_file_index].name, files[opened_file_index].updated);
    }
}

void MainWindow::fileSave()
{
    footer_event.action = FILE_SAVED;
    files[opened_file_index].updated = false;
    main_editor_widget->updateFileInfo(files[opened_file_index]);
    main_editor_widget->saveFile();
    repaintListing();
    FileRepresentation *temp = (FileRepresentation*)scroll_area->findChild<QWidget*>()->findChild<QHBoxLayout*>()->itemAt(opened_file_index)->widget();
    temp->updateInfo(files[opened_file_index].name, files[opened_file_index].updated);
}

void MainWindow::fileChange(uint8_t latest_activity)
{
    qDebug() << latest_activity;
    if (latest_activity != NONE)
    {
        footer_event.action = latest_activity;
    }

    if (latest_activity == EDIT_REDO || latest_activity == EDIT_CUT || latest_activity == EDIT_REPLACE || latest_activity == KEY_PRESSED)
    {
        files[opened_file_index].updated = true;
    }
    main_editor_widget->updateFileInfo(files[opened_file_index]);
    repaintListing();
    FileRepresentation *temp = (FileRepresentation*)scroll_area->findChild<QWidget*>()->findChild<QHBoxLayout*>()->itemAt(opened_file_index)->widget();
    temp->updateInfo(files[opened_file_index].name, files[opened_file_index].updated);
    footer_event.line = main_editor_widget->text_editor_window->textCursor().blockNumber() + 1;
    footer_event.symbol = main_editor_widget->text_editor_window->textCursor().positionInBlock() + 1;
    footer_event.lines = main_editor_widget->text_editor_window->document()->lineCount();
    footer_event.symbols = main_editor_widget->text_editor_window->document()->characterCount();
    footer_event.words = main_editor_widget->text_editor_window->document()->toPlainText().split(QRegularExpression("(\\s|\\n|\\r)+"), Qt::SkipEmptyParts).count();
    footer_event.kb_size = QByteArray(main_editor_widget->text_editor_window->document()->toRawText().toUtf8()).size() / 1024.0f;
}

void MainWindow::fileSaveAs()
{
    footer_event.action = FILE_SAVED_AS;
    files[opened_file_index].updated = true;
    files[opened_file_index].path = QFileDialog::getSaveFileName(this, tr("Save File As ..."));
    if (files[opened_file_index].path != "")
    {
        files[opened_file_index].real_name = files[opened_file_index].path.mid(files[opened_file_index].path.lastIndexOf("/") + 1),
        files[opened_file_index].name = files[opened_file_index].real_name.left(files[opened_file_index].real_name.lastIndexOf("."));
        files[opened_file_index].path = files[opened_file_index].path.left(files[opened_file_index].path.lastIndexOf("/")) + "/";
        main_editor_widget->updateFileInfo(files[opened_file_index]);
        main_editor_widget->saveFile();
        repaintListing();
        FileRepresentation *temp = (FileRepresentation*)scroll_area->findChild<QWidget*>()->findChild<QHBoxLayout*>()->itemAt(opened_file_index)->widget();
        temp->updateInfo(files[opened_file_index].name, files[opened_file_index].updated);
    }
}

void MainWindow::fileExit()
{
    footer_event.action = FILE_CLOSED;
    files.erase(files.begin() + opened_file_index);
    FileRepresentation *temp = (FileRepresentation*)scroll_area->findChild<QWidget*>()->findChild<QHBoxLayout*>()->itemAt(opened_file_index)->widget();
    temp->close();
    qDebug() << files.size();
    if (files.size() > 0)
    {
        qDebug() << "test";
        if(opened_file_index > 0)
        {
            opened_file_index--;
        }
        else
        {
            opened_file_index++;
        }
         main_editor_widget->updateFileInfo(files[opened_file_index]);
    }
    else
    {
        file_save_as->setDisabled(true);
        file_save->setDisabled(true);
        file_exit->setDisabled(true);
        opened_file_index = std::numeric_limits<std::size_t>::max();
        main_editor_widget->setDisabled(true);
    }
    repaintListing();
}

void MainWindow::saveSettings()
{
    QSettings *settings = new QSettings(QDir::currentPath() + config_path + "settings.ini", QSettings::IniFormat);
    settings->setValue("temp_file_path", files[opened_file_index].path + files[opened_file_index].real_name);
    settings->setValue("temp_file_path_name", files[opened_file_index].name);
    settings->setValue("current_palette_path", syntax_dialog.current_palette_path);
    settings->setValue("fullscreen", fullscreen);
    settings->setValue("left_button_pressed", left_button_pressed);
    settings->setValue("tool_bar_visible", tool_bar_visible);
    settings->setValue("status_bar_visible", status_bar_visible);
    settings->setValue("newline_instead_wrap", main_editor_widget->newline_instead_wrap);
    settings->setValue("line_number_visible", main_editor_widget->line_number_visible);
    settings->setValue("find_visible", main_editor_widget->find_visible);
    settings->setValue("window_border_radius", window_border_radius);
    settings->setValue("fullscreen_offset", fullscreen_offset);
    settings->setValue("header_height", header_height);
    settings->setValue("toolbar_height", toolbar_height);
    settings->setValue("listing_height", listing_height);
    settings->setValue("footer_height", footer_height);
    settings->setValue("button_height", button_height);
    settings->setValue("colision_offset", colision_offset);
    settings->setValue("press_type", press_type);
    settings->setValue("syntax_standart", main_editor_widget->syntax_standart);
    settings->setValue("main_pos", this->pos());
    settings->setValue("main_size", this->size());
    settings->setValue("main_bg_color", main_editor_widget->main_bg_color.name());
    settings->setValue("main_ui_color", main_editor_widget->main_ui_color.name());
    settings->setValue("main_hl_color", main_editor_widget->main_hl_color.name());
    settings->setValue("main_tx_color", main_editor_widget->main_tx_color.name());
    settings->setValue("main_ed_color", main_editor_widget->main_ed_color.name());
    settings->setValue("main_et_color", main_editor_widget->main_et_color.name());
    settings->setValue("main_eh_color", main_editor_widget->main_eh_color.name());
    settings->setValue("main_tc_color", main_editor_widget->main_tc_color.name());
    settings->setValue("main_tt_color", main_editor_widget->main_tt_color.name());
    settings->setValue("line_highlight_color", main_editor_widget->line_highlight_color.name());
    settings->setValue("main_bg_color", main_bg_color.name());
    settings->setValue("main_ui_color", main_ui_color.name());
    settings->setValue("main_hl_color", main_hl_color.name());
    settings->setValue("main_tx_color", main_tx_color.name());
    settings->setValue("main_fr_color", main_fr_color.name());
    settings->setValue("main_ed_color", main_ed_color.name());
    settings->setValue("main_et_color", main_et_color.name());
    settings->setValue("main_lt_color", main_lt_color.name());
    settings->setValue("main_ui_font_family", main_ui_font.family());
    settings->setValue("main_ui_font_size", main_ui_font.pointSize());
    settings->setValue("main_ui_font_weight", main_ui_font.weight());
    settings->setValue("main_ed_font_family", main_ed_font.family());
    settings->setValue("main_ed_font_size", main_ed_font.pointSize());
    settings->setValue("main_ed_font_weight", main_ed_font.weight());
    settings->setValue("editor_font_family", main_editor_widget->editor_font.family());
    settings->setValue("editor_font_size", main_editor_widget->editor_font.pointSize());
    settings->setValue("editor_font_weight", main_editor_widget->editor_font.weight());

    for (long long index = 0; index < 10; ++index)
    {
        if ((long long)syntax_dialog.recent_files.size() > index)
        {
            qDebug() << syntax_dialog.recent_files.size() << " - " << index ;
            settings->setValue("recent_file_" + QString::number(index), syntax_dialog.recent_files[index].absoluteFilePath());
        }
        else
        {
            settings->setValue("recent_file_" + QString::number(index), "//NONE");
        }
    }
}

void MainWindow::loadSettings()
{
    QString temp_file_path, temp_file_path_name;
    QSettings *settings = new QSettings(QDir::currentPath() + config_path + "settings.ini", QSettings::IniFormat);
    if(settings->contains("temp_file_path"))
    {
        temp_file_path = settings->value("temp_file_path").toString();
    }
    if(settings->contains("temp_file_path_name"))
    {
        temp_file_path_name = settings->value("temp_file_path_name").toString();
    }
    if(settings->contains("current_palette_path"))
    {
        viewSelectStyleLoad(settings->value("current_palette_path").toString());
    }
    if(settings->contains("fullscreen"))
    {
        fullscreen = settings->value("fullscreen").toBool();
    }
    if(settings->contains("left_button_pressed"))
    {
        left_button_pressed = settings->value("left_button_pressed").toBool();
    }
    if(settings->contains("tool_bar_visible"))
    {
        tool_bar_visible = settings->value("tool_bar_visible").toBool();
    }
    if(settings->contains("newline_instead_wrap"))
    {
        main_editor_widget->newline_instead_wrap = settings->value("newline_instead_wrap").toBool();

        if (!main_editor_widget->newline_instead_wrap)
        {
            main_editor_widget->text_editor_window->setLineWrapMode(QTextEdit::WidgetWidth);
        }
        else
        {
            main_editor_widget->text_editor_window->setLineWrapMode(QTextEdit::NoWrap);
        }
    }
    if(settings->contains("line_number_visible"))
    {
        main_editor_widget->line_number_visible = settings->value("line_number_visible").toBool();
    }
    if(settings->contains("find_visible"))
    {
        main_editor_widget->find_visible = settings->value("find_visible").toBool();
    }
    if(settings->contains("replace_mode"))
    {
        main_editor_widget->replace_mode = settings->value("replace_mode").toBool();
    }
    if(settings->contains("window_border_radius"))
    {
        window_border_radius = settings->value("window_border_radius").toUInt();
    }
    if(settings->contains("fullscreen_offset"))
    {
        fullscreen_offset = settings->value("fullscreen_offset").toUInt();
    }
    if(settings->contains("header_height"))
    {
        header_height = settings->value("header_height").toUInt();
    }
    if(settings->contains("toolbar_height"))
    {
        toolbar_height = settings->value("toolbar_height").toUInt();
    }
    if(settings->contains("listing_height"))
    {
        listing_height = settings->value("listing_height").toUInt();
    }
    if(settings->contains("footer_height"))
    {
        footer_height = settings->value("footer_height").toUInt();
    }
    if(settings->contains("button_height"))
    {
        button_height = settings->value("button_height").toUInt();
    }
    if(settings->contains("colision_offset"))
    {
        colision_offset = settings->value("colision_offset").toUInt();
    }
    if(settings->contains("press_type"))
    {
        press_type = settings->value("press_type").toUInt();
    }
    if(settings->contains("syntax_standart"))
    {
        main_editor_widget->syntax_standart = settings->value("syntax_standart").toInt();
    }
    if(settings->contains("main_pos"))
    {
        this->move(settings->value("main_pos").toPoint());
    }
    if(settings->contains("main_size"))
    {
        this->resize(settings->value("main_size").toSize());
    }
    if(settings->contains("main_bg_color"))
    {
         main_editor_widget->main_bg_color = QColor(settings->value("main_bg_color").toString());
    }
    if(settings->contains("main_ui_color"))
    {
        main_editor_widget->main_ui_color = QColor(settings->value("main_ui_color").toString());
    }
    if(settings->contains("main_hl_color"))
    {
        main_editor_widget->main_hl_color = QColor(settings->value("main_hl_color").toString());
    }
    if(settings->contains("main_tx_color"))
    {
        main_editor_widget->main_tx_color = QColor(settings->value("main_tx_color").toString());
    }
    if(settings->contains("main_ed_color"))
    {
        main_editor_widget->main_ed_color = QColor(settings->value("main_ed_color").toString());
    }
    if(settings->contains("main_et_color"))
    {
        main_editor_widget->main_et_color = QColor(settings->value("main_et_color").toString());
    }
    if(settings->contains("main_eh_color"))
    {
        main_editor_widget->main_eh_color = QColor(settings->value("main_eh_color").toString());
    }
    if(settings->contains("main_tc_color"))
    {
        main_editor_widget->main_tc_color = QColor(settings->value("main_tc_color").toString());
    }
    if(settings->contains("main_tt_color"))
    {
        main_editor_widget->main_tt_color = QColor(settings->value("main_tt_color").toString());
    }
    if(settings->contains("line_highlight_color"))
    {
        main_editor_widget->line_highlight_color = QColor(settings->value("line_highlight_color").toString());
    }
    if(settings->contains("main_bg_color"))
    {
        main_bg_color = QColor(settings->value("main_bg_color").toString());
    }
    if(settings->contains("main_ui_color"))
    {
        main_ui_color = QColor(settings->value("main_ui_color").toString());
    }
    if(settings->contains("main_hl_color"))
    {
        main_hl_color = QColor(settings->value("main_hl_color").toString());
    }
    if(settings->contains("main_tx_color"))
    {
        main_tx_color = QColor(settings->value("main_tx_color").toString());
    }
    if(settings->contains("main_fr_color"))
    {
        main_fr_color = QColor(settings->value("main_fr_color").toString());
    }
    if(settings->contains("main_ed_color"))
    {
        main_ed_color = QColor(settings->value("main_ed_color").toString());
    }
    if(settings->contains("main_et_color"))
    {
        main_et_color = QColor(settings->value("main_et_color").toString());
    }
    if(settings->contains("main_lt_color"))
    {
        main_lt_color = QColor(settings->value("main_lt_color").toString());
    }
    if(settings->contains("main_ui_font_family") && settings->contains("main_ui_font_size") && settings->contains("main_ui_font_weight"))
    {
        main_ui_font = QFont(settings->value("main_ui_font_family").toString(), settings->value("main_ui_font_size").toInt(), settings->value("main_ui_font_weight").toInt());
    }
    if(settings->contains("main_ed_font_family") && settings->contains("main_ed_font_size") && settings->contains("main_ed_font_weight"))
    {
        main_ed_font = QFont(settings->value("main_ed_font_family").toString(), settings->value("main_ed_font_size").toInt(), settings->value("main_ed_font_weight").toInt());
    }
    if(settings->contains("editor_font_family") && settings->contains("editor_font_size") && settings->contains("editor_font_weight"))
    {
        main_editor_widget->editor_font = QFont(settings->value("editor_font_family").toString(), settings->value("editor_font_size").toInt(), settings->value("editor_font_weight").toInt());
        main_editor_widget->text_editor_window->setFont(main_editor_widget->editor_font);
    }
    QString temp_recent_path;
    for (long long index = 0; index < 10; ++index)
    {
        if(settings->contains("recent_file_" + QString::number(index)))
        {
            temp_recent_path = settings->value("recent_file_" + QString::number(index)).toString();
        }
        else
        {
            break;
        }
        if (temp_recent_path != "//NONE")
        {
            QFile temp_recent_file(temp_recent_path);
            if (temp_recent_file.exists())
            {
                syntax_dialog.recent_files.push_front(QFileInfo(temp_recent_file));
            }
        }
        else
        {
            break;
        }
    }

    FileInfo temp;

    temp.path = temp_file_path;
    temp.real_name = temp.path.mid(temp.path.lastIndexOf("/") + 1),
    temp.name = temp_file_path_name;
    temp.path = temp.path.left(temp.path.lastIndexOf("/")) + "/";

    if (temp.path != "/")
    {
        files.push_back({temp.name, temp.real_name, temp.path, 0, 1});
        opened_file_index = files.size() - 1;
        main_editor_widget->loadFile(files[opened_file_index]);
        repaintListing();
        FileRepresentation *temp_fr = (FileRepresentation*)scroll_area->findChild<QWidget*>()->findChild<QHBoxLayout*>()->itemAt(opened_file_index)->widget();
        temp_fr->updateInfo(files[opened_file_index].name, files[opened_file_index].updated);
    }

    repaintListing();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    last_mouse_pos = event->pos();
    switch (event->button())
    {
        case Qt::LeftButton:
        {
            left_button_pressed = true; 
        }
        break;

        default:
        {
            left_button_pressed = false;
        }
        break;
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::editUndo()
{
    main_editor_widget->makeUndo();
}

void MainWindow::editRedo()
{
    main_editor_widget->makeRedo();
}

void MainWindow::editCopy()
{
    main_editor_widget->makeCopy();
}

void MainWindow::editCut()
{
    main_editor_widget->makeCut();
}

void MainWindow::editPaste()
{
    main_editor_widget->makePaste();
}

void MainWindow::editFind()
{
    main_editor_widget->makeFind();
}

void MainWindow::editFindAndReplace()
{
    main_editor_widget->makeFindAndReplace();
}

void MainWindow::editSelectAll()
{
    main_editor_widget->makeSelectAll();
}

void MainWindow::formatNewLine()
{
    main_editor_widget->toggleNewLineMode();
}

void MainWindow::formatSelectFont()
{
    QFont temp_font = QFontDialog::getFont(0, main_ed_font);
    main_ed_font = temp_font;
    main_editor_widget->changeFont(temp_font);
    repaintListing();
}

void MainWindow::viewBackgroundColor()
{
    QColor temp_color = QColorDialog::getColor(main_editor_widget->main_ed_color);
    main_fr_color = main_editor_widget->colorChange(temp_color);
    repaintListing();
}

void MainWindow::viewSelectionColor()
{
    QColor temp_color = QColorDialog::getColor(main_editor_widget->main_tc_color);
    main_editor_widget->selectionColorChange(temp_color);
    repaintListing();
}

void MainWindow::viewLineHighlightColor()
{
    main_editor_widget->line_highlight_color = QColorDialog::getColor(main_editor_widget->line_highlight_color);
    main_editor_widget->highlightCurrentLine();
    repaintListing();
}

void MainWindow::viewShowStateBar()
{
    footer_event.action = VIEW_STATUSBAR;
    status_bar_visible = !status_bar_visible;
    recalcSizes();
}

void MainWindow::viewShowLineNumber()
{
    main_editor_widget->toggleLineNumber();
}

void MainWindow::viewShowToolBar()
{
    footer_event.action = VIEW_TOOL_BAR;
    tool_bar_visible = !tool_bar_visible;
    recalcSizes();
}

void MainWindow::viewShowSyntaxHighlight()
{
    main_editor_widget->toggleHighlight();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    left_button_pressed = false;
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::aboutShow()
{
    if (about_window.isVisible())
    {
        about_window.activateWindow();
    }
    else
    {
        about_window.show();
    }
}

void MainWindow::viewSelectSyntax()
{
    footer_event.action = VIEW_SYNTAX_SLC;
    int temp_syntax_type = syntax_dialog.getSyntaxType(main_editor_widget->syntax_standart);
    main_editor_widget->syntax_standart = temp_syntax_type;
    main_editor_widget->highlighter->initializePalette(temp_syntax_type);
    main_editor_widget->initHighlight(main_editor_widget->highlighter->document());
    repaintListing();
}

void MainWindow::viewSelectStyleSave()
{
    footer_event.action = VIEW_SYNTAX_CRT;
    highlightPalette new_palette = syntax_dialog.getSyntax(main_editor_widget->default_highlight_palette, main_editor_widget->main_ed_color);
    main_editor_widget->default_highlight_palette = new_palette;
    main_editor_widget->initHighlight(main_editor_widget->highlighter->document());

    recalcAlaviableStyles();
}

void MainWindow::viewSelectStyleLoad(QString path)
{
    footer_event.action = VIEW_SYNTAX_LRT;

    if (path == "")
    {
        path = QFileDialog::getOpenFileName(0, "Load Highlight Theme", QDir::currentPath(), "Highlight style (*.cfg)");
    }

    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        highlightPalette new_palette = main_editor_widget->default_highlight_palette;

        QDataStream in(&file);
        QColor temp[13];

        in >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5] >> temp[6] >> temp[7] >> temp[8] >> temp[9] >> temp[10] >> temp[11] >> temp[12];

        if (syntax_dialog.recent_files.length() > 10)
        {
            syntax_dialog.recent_files.pop_back();
        }
        syntax_dialog.recent_files.push_front(QFileInfo(file));
        syntax_dialog.current_palette_path = QFileInfo(file).absoluteFilePath();

        file.close();

        new_palette.keyword_format.setForeground(temp[0]);
        new_palette.data_type_format.setForeground(temp[1]);
        new_palette.single_line_comment_format.setForeground(temp[2]);
        new_palette.multi_line_comment_format.setForeground(temp[3]);
        new_palette.preprocessor_format.setForeground(temp[4]);
        new_palette.quotation_format.setForeground(temp[5]);
        new_palette.function_format.setForeground(temp[6]);
        new_palette.numbers_format.setForeground(temp[7]);
        new_palette.preprocessor_numbers_format.setForeground(temp[8]);
        new_palette.special_symbols_format.setForeground(temp[9]);
        new_palette.class_format.setForeground(temp[10]);
        new_palette.variable_format.setForeground(temp[11]);
        new_palette.font_format.setForeground(temp[12]);

        main_editor_widget->default_highlight_palette = new_palette;
        main_editor_widget->initHighlight(main_editor_widget->highlighter->document());
    }
    else
    {
        qDebug() << "Unable to open file " << path;
    }

    recalcAlaviableStyles();
}

void MainWindow::viewSelectStyleDefault()
{
    footer_event.action = VIEW_SYNTAX_DRT;
    SyntaxSelectWidget syntax_dialog;
    highlightPalette new_palette = main_editor_widget->default_highlight_palette;

    new_palette.keyword_format.setForeground(main_editor_widget->keyword_highlight_color);
    new_palette.data_type_format.setForeground(main_editor_widget->data_type_highlight_color);
    new_palette.single_line_comment_format.setForeground(main_editor_widget->single_line_comment_highlight_color);
    new_palette.multi_line_comment_format.setForeground(main_editor_widget->multi_line_comment_highlight_color);
    new_palette.preprocessor_format.setForeground(main_editor_widget->preprocessor_highlight_color);
    new_palette.quotation_format.setForeground(main_editor_widget->quotation_highlight_color);
    new_palette.function_format.setForeground(main_editor_widget->function_highlight_color);
    new_palette.numbers_format.setForeground(main_editor_widget->numbers_highlight_color);
    new_palette.preprocessor_numbers_format.setForeground(main_editor_widget->preprocessor_numbers_highlight_color);
    new_palette.special_symbols_format.setForeground(main_editor_widget->special_symbol_highlight_color);
    new_palette.class_format.setForeground(main_editor_widget->class_highlight_color);
    new_palette.variable_format.setForeground(main_editor_widget->variable_highlight_color);
    new_palette.font_format.setForeground(main_editor_widget->main_et_color);

    main_editor_widget->default_highlight_palette = new_palette;
    main_editor_widget->initHighlight(main_editor_widget->highlighter->document());

    syntax_dialog.current_palette_path = "Default";
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (!left_button_pressed)
    {
        press_type = borderColisionType(event->pos());
    }

    switch (press_type)
    {
        case TOP_LEFT_PRESS:
        {
            setCursor(Qt::SizeFDiagCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                QPoint temp = {pos().x(), pos().y()};
                if (size().width() - (QCursor::pos().x() - pos().x()) > 480)
                {
                    temp.setX(QCursor::pos().x());
                }
                if (size().height() - (QCursor::pos().y() - pos().y()) > 240)
                {
                    temp.setY(QCursor::pos().y());
                }
                this->setGeometry(temp.x(), temp.y(), size().width() - (temp.x() - pos().x()), size().height() - (temp.y() - pos().y()));
                event->accept();
            }
        }
        break;

        case TOP_RIGHT_PRESS:
        {
            setCursor(Qt::SizeBDiagCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                QPoint temp = {size().width(), pos().y()};
                if (event->pos().x() > 480)
                {
                    temp.setX(event->pos().x());
                }
                if (size().height() - (QCursor::pos().y() - pos().y()) > 240)
                {
                    temp.setY(QCursor::pos().y());
                }
                this->setGeometry(pos().x(), temp.y(), temp.x(), size().height() - (temp.y() - pos().y()));
                event->accept();
            }
        }
        break;

        case BOTTOM_RIGHT_PRESS:
        {
            setCursor(Qt::SizeFDiagCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                QPoint temp = {size().width(), size().height()};
                if (event->pos().x() > 480)
                {
                    temp.setX(event->pos().x());
                }
                if (event->pos().y() > 240)
                {
                    temp.setY(event->pos().y());
                }

                this->resize(temp.x(), temp.y());
                event->accept();
            }
        }
        break;

        case BOTTOM_LEFT_PRESS:
        {
            setCursor(Qt::SizeBDiagCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                QPoint temp = {pos().x(), size().height()};
                if (size().width() - (QCursor::pos().x() - pos().x()) > 480)
                {
                    temp.setX(QCursor::pos().x());
                }
                if (event->pos().y() > 240)
                {
                    temp.setY(event->pos().y());
                }
                this->setGeometry(temp.x(), pos().y(), size().width() - (temp.x() - pos().x()), temp.y());
                event->accept();
            }
        }
        break;

        case TOP_MIDDLE_PRESS:
        {
            setCursor(Qt::SizeVerCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                int temp = pos().y();
                if (size().height() - (QCursor::pos().y() - pos().y()) > 240)
                {
                    temp = QCursor::pos().y();
                }
                this->setGeometry(pos().x(), temp, size().width(), size().height() - (temp - pos().y()));
                event->accept();
            }
        }
        break;

        case RIGHT_MIDDLE_PRESS:
        {
            setCursor(Qt::SizeHorCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                int temp = size().width();
                if (event->pos().x() > 480)
                {
                    temp = event->pos().x();
                }
                this->resize(temp, size().height());
                event->accept();
            }
        }
        break;

        case BOTTOM_MIDDLE_PRESS:
        {
            setCursor(Qt::SizeVerCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                int temp = size().height();
                if (event->pos().y() > 240)
                {
                    temp = event->pos().y();
                }
                this->resize(size().width(), temp);
                event->accept();
            }
        }
        break;

        case LEFT_MIDDLE_PRESS:
        {
            setCursor(Qt::SizeHorCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_RESIZED;
                int temp = pos().x();
                if (size().width() - (QCursor::pos().x() - pos().x()) > 480)
                {
                    temp = QCursor::pos().x();
                }
                this->setGeometry(temp, pos().y(), size().width() - (temp - pos().x()), size().height());
                event->accept();
            }
        }
        break;

        case HEADER_PRESS:
        {
            setCursor(Qt::ArrowCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_MOVED;
                QPoint shift = event->pos() - last_mouse_pos;
                this->move(this->pos() + shift);
                event->accept();
            }
        }
        break;

        case FOOTER_PRESS:
        {
            setCursor(Qt::ArrowCursor);
            if (left_button_pressed)
            {
                footer_event.action = WINDOW_MOVED;
                QPoint shift = event->pos() - last_mouse_pos;
                this->move(this->pos() + shift);
                event->accept();
            }
        }
        break;

        case LISTING_PRESS:
        {
            setCursor(Qt::ArrowCursor);
        }
        break;

        default:
        {
            setCursor(Qt::ArrowCursor);
        }
        break;
    }

    repaint();
    recalcSizes();
    QMainWindow::mouseMoveEvent(event);
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    switch (event->type())
    {
        case QEvent::Wheel:
        {
            QWheelEvent* we = static_cast<QWheelEvent*>(event);

            if (object == scroll_area)
            {
                QWheelEvent d(we->position(), we->globalPosition(), we->pixelDelta(), QPoint(we->angleDelta().y(), we->angleDelta().x()), we->button(), we->modifiers(), we->phase(), we->inverted(), we->source());
                if (scrolled_index >= 3)
                {

                }
                else if(scrolled_index > 0)
                {

                }

                object->removeEventFilter(this);
                QApplication::sendEvent(scroll_area->horizontalScrollBar(), &d);
                object->installEventFilter(this);
                return true;
            }
            else
            {
            }
        }
        break;

        default:
        {

        }
        break;
    }
    return QObject::eventFilter(object, event);
}

void MainWindow::closeWindow()
{
    saveSettings();
    about_window.close();
    this->close();
}

MainWindow::~MainWindow()
{
}
