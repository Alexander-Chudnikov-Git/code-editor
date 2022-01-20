#include "colorswitchbutton.h"

ColorSwitchButton::ColorSwitchButton(QColor color, QColor bg_color, QString text, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    main_button = new QPushButton();
    main_button->setText(text);
    main_button->setGeometry(this->pos().x(), this->pos().y(), this->size().width(), this->size().height());

    main_color    = color;
    main_bg_color = bg_color;

    main_button->setStyleSheet("QPushButton {color: " + main_color.name() + "; background-color:" + main_bg_color.name() + "; border: none; padding: 10px}");
    layout->addWidget(main_button);
    connect(main_button, SIGNAL(clicked()), this, SLOT(buttonPressHandle()));
    this->setLayout(layout);
}

void ColorSwitchButton::buttonPressHandle()
{
   main_color = QColorDialog::getColor(main_color);
   main_button->setStyleSheet("QPushButton {color: " + main_color.name() + "; background-color:" + main_bg_color.name() + "; border: none; padding: 10px}");
}
