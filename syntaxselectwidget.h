#ifndef SYNTAXSELECTWIDGET_H
#define SYNTAXSELECTWIDGET_H

#include "highlighter.h"
#include "colorswitchbutton.h"

#include <QSyntaxHighlighter>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QDialog>
#include <QString>
#include <QFile>
#include <QDir>



class SyntaxSelectWidget : public QWidget
{
    Q_OBJECT
public:
    SyntaxSelectWidget(QWidget *parent = nullptr);
    ~SyntaxSelectWidget();

    int getSyntaxType(int);
    highlightPalette getSyntax(highlightPalette, QColor);

private:
    int seleced_syntax_type;

private slots:
    void changeType(int type);

public:
    QVector<QFileInfo> recent_files;

    QString            current_palette_path;

};

#endif // SYNTAXSELECTWIDGET_H
