#include "aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent)
{
    aboutInit();
}

AboutWindow::~AboutWindow()
{

}

void AboutWindow::aboutInit()
{
    int id = QFontDatabase::addApplicationFont(tr(":/ttf/font/HAR.ttf"));
    QString har_font = QFontDatabase::applicationFontFamilies(id).at(0);
    main_font = new QFont(har_font, 25, 100);
    sub_font = new QFont(har_font, 12, 100);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);

    window_rect = QRect(QGuiApplication::primaryScreen()->availableGeometry());
    resize(QPixmap(":/img/image/clipping_pixmap.png").size());

    setMask((new QPixmap(":/img/image/clipping_pixmap.png"))->mask());

    avatar = new QImage(":/img/image/avatar.png");

    rounded_window_path = new QPainterPath();
    rounded_window_path->setFillRule(Qt::WindingFill);

    rounded_window_path->addRoundedRect(this->rect().x(), this->rect().y(), this->rect().width(), this->rect().height(), 10, 10);

    name = new QLabel("Alexander Chudnikov", this);
    name->setAttribute(Qt::WA_TransparentForMouseEvents);
    name->setFont(*main_font);
    name->setStyleSheet("color: white");
    name->move(0, 220);
    name->resize(this->size().width(), name->size().height() + 20);
    name->setAlignment(Qt::AlignCenter);

    build_date = new QLabel(" Build date: " + build_date_time, this);
    build_date->setAttribute(Qt::WA_TransparentForMouseEvents);
    build_date->setFont(*sub_font);
    build_date->setStyleSheet("color: white");
    build_date->move(50, name->y() + name->height());
    build_date->resize(this->size().width() - 50, build_date->size().height());
    build_date->setAlignment(Qt::AlignLeft);

    qt_build_version = new QLabel(" Build: version: " + build_version, this);
    qt_build_version->setAttribute(Qt::WA_TransparentForMouseEvents);
    qt_build_version->setFont(*sub_font);
    qt_build_version->setStyleSheet("color: white");
    qt_build_version->move(50, build_date->y() + build_date->height());
    qt_build_version->resize(this->size().width() - 50, qt_build_version->height());
    qt_build_version->setAlignment(Qt::AlignLeft);

    qt_runtime_version = new QLabel(tr(" QT version: ") + tr(qVersion()), this);
    qt_runtime_version->setAttribute(Qt::WA_TransparentForMouseEvents);
    qt_runtime_version->setFont(*sub_font);
    qt_runtime_version->setStyleSheet("color: white");
    qt_runtime_version->move(50, qt_build_version->y() + qt_build_version->height());
    qt_runtime_version->resize(this->size().width() - 50, qt_runtime_version->size().height());
    qt_runtime_version->setAlignment(Qt::AlignLeft);

    site = new QLabel(tr(" Site: https://choodick.com"), this);
    site->setAttribute(Qt::WA_TransparentForMouseEvents);
    site->setFont(*sub_font);
    site->setStyleSheet("color: white");
    site->move(50, qt_runtime_version->y() + qt_runtime_version->height() + 20);
    site->resize(this->size().width() - 50, site->size().height());
    site->setAlignment(Qt::AlignLeft);

    email = new QLabel(tr(" Email: admin@choodick.com"), this);
    email->setAttribute(Qt::WA_TransparentForMouseEvents);
    email->setFont(*sub_font);
    email->setStyleSheet("color: white");
    email->move(50, site->y() + site->height());
    email->resize(this->size().width() - 50, email->size().height());
    email->setAlignment(Qt::AlignLeft);

    hwid = new QLabel(QString(QString(" System: ") + QString(QSysInfo::productType()) + "_" + QString(QSysInfo::productVersion()) + "_" + QString(QSysInfo::currentCpuArchitecture())), this);
    hwid->setAttribute(Qt::WA_TransparentForMouseEvents);
    hwid->setFont(*sub_font);
    hwid->setStyleSheet("color: white");
    hwid->move(50, email->y() + email->height());
    hwid->resize(this->size().width() - 50, hwid->size().height());
    hwid->setAlignment(Qt::AlignLeft);

    exit_button = new QPushButton(this);
    exit_button->setGeometry(((this->rect().width() / 2) - (100)), email->y() + email->height() + 50, 200, 80);
    exit_button->setFont(*main_font);
    exit_button->setText("EXIT");
    exit_button->setStyleSheet("QPushButton {border-image:url(:/img/image/button.png); color: black; padding-top: 10px;} QPushButton::hover {border-image:url(:/img/image/button_hover.png); color: white;}");

    connect(exit_button, SIGNAL(clicked()), this, SLOT(close()));
}

void AboutWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setClipping(true);
    painter.setClipPath(*rounded_window_path);

    painter.setBrush(QColor(13, 13, 13));
    painter.drawRect(this->rect());

    painter.drawImage(QRect(((this->rect().width() / 2) - (avatar->width() / 4)), 50, (avatar->width() / 2), (avatar->height() / 2)), *avatar);
    QDialog::paintEvent(event);
}

void AboutWindow::mouseReleaseEvent(QMouseEvent* event)
{
    left_button_pressed = false;
    QDialog::mouseReleaseEvent(event);
}

void AboutWindow::mousePressEvent(QMouseEvent* event)
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
    QDialog::mousePressEvent(event);
}


void AboutWindow::mouseMoveEvent(QMouseEvent* event)
{
    setCursor(Qt::ArrowCursor);
    if (left_button_pressed)
    {
        QPoint shift = event->pos() - last_mouse_pos;
        this->move(this->pos() + shift);
        event->accept();
    }

    repaint();
    QDialog::mouseMoveEvent(event);
}

