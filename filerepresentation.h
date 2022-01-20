#ifndef FILEREPRESENTATION_H
#define FILEREPRESENTATION_H

#include <QPainterPath>
#include <QPainter>
#include <QWidget>
#include <QLabel>


class FileRepresentation : public QWidget
{

    Q_OBJECT

public:
    FileRepresentation(QString filename, bool changed, QColor bg_color, QWidget *parent = 0);
    ~FileRepresentation();

    QSize sizeHint() const override;

    void paintEvent(QPaintEvent *event) override;

    void recalcEdges();
    void importSettings(QColor color, QRect window_border_radius);
    void updateInfo(QString filename, bool changed);

public:
    QColor          main_bg_color           = QColor("#303030");

private:
    QLabel          *center_label;
    QFont           *editor_font;
    QPainterPath    *rounded_window_path;

    size_t           window_border_radius   = 10;

    bool            tl_edge_state           = 1;
    bool            tr_edge_state           = 1;
    bool            bl_edge_state           = 0;
    bool            br_edge_state           = 0;

};

#endif // FILEREPRESENTATION_H
