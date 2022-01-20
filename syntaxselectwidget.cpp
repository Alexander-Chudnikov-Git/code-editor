#include "syntaxselectwidget.h"

SyntaxSelectWidget::SyntaxSelectWidget(QWidget *parent) : QWidget(parent)
{

}

int SyntaxSelectWidget::getSyntaxType(int syntax_type)
{
    seleced_syntax_type = syntax_type;
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Syntax Standard Selector"));

    QComboBox *combo_box = new QComboBox(&dlg);
    combo_box->addItem("C Standard 2011", QVariant(C_STD_2011));
    combo_box->addItem("C Standard 2018", QVariant(C_STD_2018));
    combo_box->addItem("C++ Standard 2014", QVariant(CPP_STD_2014));
    combo_box->addItem("C++ Standard 2017", QVariant(CPP_STD_2017));
    combo_box->addItem("C++ Standard 2020", QVariant(CPP_STD_2020));
    combo_box->setCurrentIndex(syntax_type);

    if (syntax_type == TEST)
    {
        combo_box->addItem("DEBUG", QVariant(TEST));
        combo_box->currentData(TEST);
    }

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btn_box->resize(combo_box->size());
    btn_box->setCenterButtons(true);

    connect(btn_box,   &QDialogButtonBox::accepted,     &dlg, &QDialog::accept);
    connect(btn_box,   &QDialogButtonBox::rejected,     &dlg, &QDialog::reject);
    connect(combo_box, &QComboBox::currentIndexChanged, this, &SyntaxSelectWidget::changeType);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(combo_box);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    if(dlg.exec() == QDialog::Accepted)
    {
        syntax_type = seleced_syntax_type;
    }
    return syntax_type;
}

void SyntaxSelectWidget::changeType(int type)
{
    qDebug() << type;
    seleced_syntax_type = type;
}

highlightPalette SyntaxSelectWidget::getSyntax(highlightPalette palette, QColor bg_color)
{
    highlightPalette new_palette;
    QLineEdit *line_edit = new QLineEdit(this) ;
    QDialog dlg(this);
    QGridLayout *layout = new QGridLayout(this);

    line_edit->setPlaceholderText(tr("Enter palette name"));
    dlg.setWindowTitle(tr("Syntax Palette Selector"));
    layout->setSpacing(10);

    QVector<ColorSwitchButton*> buttons;
    ColorSwitchButton *buffer_button = new ColorSwitchButton(palette.keyword_format.foreground().color(), bg_color, "KEYWORD COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.data_type_format.foreground().color(), bg_color, "DATATYPE COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.single_line_comment_format.foreground().color(), bg_color, "COMMENT // COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.multi_line_comment_format.foreground().color(), bg_color, "COMMENT /**/ COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.preprocessor_format.foreground().color(), bg_color, "PREPROCESSOR DIRECTIVES COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.quotation_format.foreground().color(), bg_color, "QUOTES COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.function_format.foreground().color(), bg_color, "FUNCTIONS COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.numbers_format.foreground().color(), bg_color, "NUMBERS COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.preprocessor_numbers_format.foreground().color(), bg_color, "PREPROCESSOR NUMBERS COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.special_symbols_format.foreground().color(), bg_color, "SPECIAL SYMBOL COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.class_format.foreground().color(), bg_color, "CLASSES COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.variable_format.foreground().color(), bg_color, "VARIABLES COLOR", this);
    buttons.push_back(buffer_button);
    buffer_button = new ColorSwitchButton(palette.font_format.foreground().color(), bg_color, "FONT COLOR", this);
    buttons.push_back(buffer_button);

    size_t grid_index = 0;

    layout->addWidget(line_edit, grid_index, 0, 2, 4);

    grid_index+= 2;

    for (QWidget *button : buttons)
    {
        layout->addWidget(button, grid_index, 0, 2, 4);
        ++grid_index;
    }

    ++grid_index;

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btn_box->setCenterButtons(true);

    connect(btn_box,   &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box,   &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    layout->setAlignment(Qt::AlignCenter);

    layout->addWidget(btn_box, grid_index, 0, 1, 4);

    dlg.setLayout(layout);

    if(dlg.exec() == QDialog::Accepted)
    {
        QString temp = line_edit->text();
        QString path = (QDir::currentPath() + "/tmp/cls/");
        new_palette.keyword_format.setForeground(buttons[0]->main_color);
        new_palette.data_type_format.setForeground(buttons[1]->main_color);
        new_palette.single_line_comment_format.setForeground(buttons[2]->main_color);
        new_palette.multi_line_comment_format.setForeground(buttons[3]->main_color);
        new_palette.preprocessor_format.setForeground(buttons[4]->main_color);
        new_palette.quotation_format.setForeground(buttons[5]->main_color);
        new_palette.function_format.setForeground(buttons[6]->main_color);
        new_palette.numbers_format.setForeground(buttons[7]->main_color);
        new_palette.preprocessor_numbers_format.setForeground(buttons[8]->main_color);
        new_palette.special_symbols_format.setForeground(buttons[9]->main_color);
        new_palette.class_format.setForeground(buttons[10]->main_color);
        new_palette.variable_format.setForeground(buttons[11]->main_color);
        new_palette.font_format.setForeground(buttons[12]->main_color);


        if (!QDir(path).exists())
        {
            QDir().mkdir(path);
        }

        if (temp.length() != 0)
        {
            path += (temp + ".cfg");
        }
        else
        {
            QDate temp_date = QDate::currentDate();
            QTime temp_time = QTime::currentTime();
            path += (temp_date.toString("dd-MM-yyy") + + "-" + temp_time.toString("hh-mm-ss-zzz") + ".cfg");
        }

        QFile file_r(path);

        if (!file_r.exists())
        {
            if (file_r.open(QIODevice::WriteOnly))
            {
                QDataStream out(&file_r);
                out << new_palette.keyword_format.foreground().color();
                out << new_palette.data_type_format.foreground().color();
                out << new_palette.single_line_comment_format.foreground().color();
                out << new_palette.multi_line_comment_format.foreground().color();
                out << new_palette.preprocessor_format.foreground().color();
                out << new_palette.quotation_format.foreground().color();
                out << new_palette.function_format.foreground().color();
                out << new_palette.numbers_format.foreground().color();
                out << new_palette.preprocessor_numbers_format.foreground().color();
                out << new_palette.special_symbols_format.foreground().color();
                out << new_palette.class_format.foreground().color();
                out << new_palette.variable_format.foreground().color();
                out << new_palette.font_format.foreground().color();

                if (recent_files.length() > 10)
                {
                    recent_files.pop_back();
                }
                recent_files.push_front(QFileInfo(file_r));

                current_palette_path = QFileInfo(file_r).absoluteFilePath();

                file_r.close();
            }
            else
            {
                qDebug() << "File cannot be opened" << path;
            }
        }
        else
        {
            qDebug() << "File already exist" << path;
        }


        palette = new_palette;
    }
    return palette;
}

SyntaxSelectWidget::~SyntaxSelectWidget()
{

}


