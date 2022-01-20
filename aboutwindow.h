#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QGuiApplication>
#include <QFontDatabase>
#include <QPainterPath>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QDialog>
#include <QLabel>

const QString build_date_time =  QStringLiteral(__DATE__ " " __TIME__);
const QString build_version =  QStringLiteral(QT_VERSION_STR);

class AboutWindow: public QDialog
{
    Q_OBJECT
public:
    AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

    void aboutInit();

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    QFont        *main_font;
    QFont        *sub_font;
    QRect        window_rect;
    QPoint       last_mouse_pos;
    QImage       *avatar;
    QLabel       *name;
    QLabel       *site;
    QLabel       *email;
    QLabel       *hwid;
    QLabel       *build_date;
    QLabel       *qt_build_version;
    QLabel       *qt_runtime_version;
    QPushButton  *exit_button;
    QPainterPath *rounded_window_path;

    bool         left_button_pressed = false;
};

#endif // ABOUTWINDOW_H
