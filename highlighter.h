#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>

#define C_STD_2011      0
#define C_STD_2018      1
#define CPP_STD_2014    2
#define CPP_STD_2017    3
#define CPP_STD_2020    4
#define TEST            5


struct highlightPalette
{
    QTextCharFormat keyword_format;
    QTextCharFormat data_type_format;
    QTextCharFormat single_line_comment_format;
    QTextCharFormat multi_line_comment_format;
    QTextCharFormat preprocessor_format;
    QTextCharFormat quotation_format;
    QTextCharFormat function_format;
    QTextCharFormat numbers_format;
    QTextCharFormat preprocessor_numbers_format;
    QTextCharFormat special_symbols_format;
    QTextCharFormat class_format;
    QTextCharFormat variable_format;
    QTextCharFormat font_format;
};

struct HighlightingRule
{
    QRegularExpression pattern;
    QTextCharFormat format;
};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(int, highlightPalette, QTextDocument *parent = nullptr);
    void changeHighlightPalette(highlightPalette);
    void initializePalette(int);
    void rehighlightBlock(const QString &text);

protected:
    void highlightBlock(const QString &text) override;

private:
    QTextDocument *text_doc;

    QVector<HighlightingRule>* highlighting_rules;

    QRegularExpression comment_start_expression;
    QRegularExpression comment_end_expression;

public:
    bool highlight_visible = 1;

    highlightPalette highlight_palette;
};
#endif // HIGHLIGHTER_H
