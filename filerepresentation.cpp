#include "filerepresentation.h"


FileRepresentation::FileRepresentation(QString filename, bool changed, QColor bg_color, QWidget *parent) : QWidget(parent)
{
    setStyleSheet(QString("color: white"));
    main_bg_color = bg_color;
    setAcceptDrops(true);
    editor_font = new QFont();
    editor_font->setFamily("Helvetica [Cronyx]");
    editor_font->setFixedPitch(true);
    editor_font->setPointSize(8);

    QFontMetrics metrics(*editor_font);
    if (filename.length() > 32)
    {
        filename = filename.left(31);
        filename += "…";
    }
    size_t width = 20 + metrics.averageCharWidth() * (filename.length() + (changed == 0 ? 0 : 1));
    resize(width, 25);

    recalcEdges();

    center_label = new QLabel(this);
    center_label->setText((changed == 0 ? "" : "*") + filename);
    center_label->setFont(*editor_font);
    center_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    center_label->setGeometry(QRect((rect().width() - width) / 2 - 5, rect().y(), width + 10, rect().height()));
    center_label->show();

}

void FileRepresentation::updateInfo(QString filename, bool changed)
{
    QFontMetrics metrics(*editor_font);
    size_t width = 20 + metrics.averageCharWidth() * (filename.length() + (changed == 0 ? 0 : 1));
    resize(width, 25);

    recalcEdges();
    center_label->setText((changed == 0 ? "" : "*") + filename);
}

void FileRepresentation::recalcEdges()
{
    rounded_window_path = new QPainterPath();
    rounded_window_path->setFillRule(Qt::WindingFill);
    rounded_window_path->addRoundedRect(rect().x(), rect().y(), rect().width(), rect().height(), window_border_radius, window_border_radius);

    if (!tl_edge_state)
    {
        rounded_window_path->addRect(0, 0, window_border_radius, window_border_radius);
    }
    if (!tr_edge_state)
    {
        rounded_window_path->addRect(rect().width() - window_border_radius, 0, window_border_radius, window_border_radius);
    }
    if (!bl_edge_state)
    {
        rounded_window_path->addRect(0, rect().height() - window_border_radius, window_border_radius, window_border_radius);
    }
    if (!br_edge_state)
    {
        rounded_window_path->addRect(rect().width() - window_border_radius,  rect().height() - window_border_radius, window_border_radius, window_border_radius);
    }
}

void FileRepresentation::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setClipping(true);
    painter.setClipPath(*rounded_window_path);

    painter.setBrush(main_bg_color);
    painter.drawRect(rect());

    QWidget::paintEvent(event);
}

QSize FileRepresentation::sizeHint() const
{
    return this->size();
}

FileRepresentation::~FileRepresentation()
{

}
