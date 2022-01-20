#ifndef COLORSWITCHBUTTON_H
#define COLORSWITCHBUTTON_H

#include <QColorDialog>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>

class ColorSwitchButton : public QWidget
{
    Q_OBJECT
public:
    ColorSwitchButton(QColor, QColor, QString, QWidget* = nullptr);

private slots:
    void buttonPressHandle();

private:
    QPushButton *main_button;

public:
    QColor      main_color;
    QColor      main_bg_color;

};

#endif // COLORSWITCHBUTTON_H
