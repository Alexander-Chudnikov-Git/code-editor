#include "highlighter.h"


Highlighter::Highlighter(int syntax_type, highlightPalette hp, QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    text_doc = new QTextDocument(parent);
    changeHighlightPalette(hp);
    initializePalette(syntax_type);
}

void Highlighter::initializePalette(int syntax_type)
{
    HighlightingRule rule;
    highlighting_rules = new QVector<HighlightingRule>;

    std::vector<QString> keyword_patterns = {QStringLiteral("\\bdebug\\b")};
    std::vector<QString> preprocessor_patterns = {QStringLiteral("\\bdebug\\b")};
    std::vector<QString> datatype_pattern = {QStringLiteral("\\bdebug\\b")};
    std::vector<QString> preprocessor_numbers_patterns = {QStringLiteral("\\bdebug\\b")};
    std::vector<QString> special_symbols_patterns = {QStringLiteral("\\bdebug\\b")};
    std::vector<QString> numbers_patterns = {QStringLiteral("\\bdebug\\b")};
    std::vector<QString> variables_pattens = {QStringLiteral("\\bdebug\\b")};

    switch (syntax_type)
    {
        case C_STD_2011:
        {
            keyword_patterns = {
                QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bconst\\b"),
                QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdefault\\b"), QStringLiteral("\\bdo\\b"),
                QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bextern\\b"),
                QStringLiteral("\\binline\\b"), QStringLiteral("\\bregister\\b"), QStringLiteral("\\brestrict\\b"),
                QStringLiteral("\\breturn\\b"), QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"),
                QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"), QStringLiteral("\\btypedef\\b"),
                QStringLiteral("\\bunion\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bfor\\b"),
                QStringLiteral("\\bgoto\\b"), QStringLiteral("\\bif\\b"), QStringLiteral("\\b_Alignas\\b"),
                QStringLiteral("\\b_Alignof\\b"), QStringLiteral("\\b_Atomic\\b"), QStringLiteral("\\b_Generic\\b"),
                QStringLiteral("\\b_Imaginary\\b"), QStringLiteral("\\b_Noreturn\\b"), QStringLiteral("\\b_Static_assert\\b"),
                QStringLiteral("\\b_Thread_local\\b"), QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bfalse\\b"),
                QStringLiteral("\\btrue\\b")
            };

            // TODO: Fix #ifned/#else preprocessor directices
            preprocessor_patterns = {
                QStringLiteral("^[\\s]*#"), QStringLiteral("^[\\s]*#\\binclude\\b"), QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:n|)def|)\\b"), QStringLiteral("^[\\s]*#\\bendif\\b"),
                QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b[\\s]+[^\\s#]+(?:[\\s]+[^#\\n]+|)"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:|n)def|)\\b[\\s]+[^\\n#]+|^[\\s]"), QStringLiteral("^[\\s]*#\\belse(?:if\\b[\\s]+[^#\\n]+|\\b)"),
                QStringLiteral("^[\\s]*#\\bline\\b[\\s]+\\d+(?:[\\s]+\"[^\\n\\s#]+\"|)"), QStringLiteral("^[\\s]*#\\b(?:error|pragma)\\b[\\s]+[^#\\n]+")
            };

            datatype_pattern = {
                QStringLiteral("\\b(?:u|\\s*)int(?:|(?:8|16|32|64)_t)\\b"), QStringLiteral("\\bchar(?:|(?:8|16|32)_t)\\b"), QStringLiteral("\\blong\\b"),
                QStringLiteral("\\bshort\\b"), QStringLiteral("\\bbool\\b"), QStringLiteral("\\b(?:|un)signed\\b"), QStringLiteral("\\bfloat\\b"),
                QStringLiteral("\\bdouble\\b"), QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\b_Bool\\b"), QStringLiteral("\\b_Complex\\b")
            };

            preprocessor_numbers_patterns = {
                QStringLiteral("(?:L|u|U|u8)(\".*\")"), QStringLiteral("([\\d\\.]+)(?:U|u|L|l|UL|Ul|uL|ul|LU|Lu|lU|lu|ll|LL|ULL|Ull|uLL|ull|LLU|LLu|llU|llu|F|f|L|l)")
            };

            special_symbols_patterns = {
                QStringLiteral("(?:\\+|\\-|&|\\||\\*|~|!|\\/|%|=|<|>|\\^|:|\\?|\\.\\.\\.)")
            };

            numbers_patterns = {
                QStringLiteral("\\b0(?:x|X)[0-9a-fA-F]*\\b"), QStringLiteral("\\b[0-9]*(?:(?:e|E)(?:\\+|\\-|)|)[0-9]*\\b")
            };
        }
        break;

        case C_STD_2018:
        {
            keyword_patterns = {
                QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bconst\\b"),
                QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdefault\\b"), QStringLiteral("\\bdo\\b"),
                QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bextern\\b"),
                QStringLiteral("\\binline\\b"), QStringLiteral("\\bregister\\b"), QStringLiteral("\\brestrict\\b"),
                QStringLiteral("\\breturn\\b"), QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"),
                QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"), QStringLiteral("\\btypedef\\b"),
                QStringLiteral("\\bunion\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bfor\\b"),
                QStringLiteral("\\bgoto\\b"),  QStringLiteral("\\bif\\b"),  QStringLiteral("\\bwhile\\b"),
                QStringLiteral("\\b_Alignas\\b"), QStringLiteral("\\b_Alignof\\b"), QStringLiteral("\\b_Atomic\\b"),
                QStringLiteral("\\b_Bool\\b"), QStringLiteral("\\b_Complex\\b"), QStringLiteral("\\b_Generic\\b"),
                QStringLiteral("\\b_Imaginary\\b"), QStringLiteral("\\b_Noreturn\\b"), QStringLiteral("\\b_Static_assert\\b"),
                QStringLiteral("\\b_Thread_local\\b"), QStringLiteral("\\bfalse\\b"), QStringLiteral("\\btrue\\b")
            };

            preprocessor_patterns = {
                QStringLiteral("^[\\s]*#"), QStringLiteral("^[\\s]*#\\binclude\\b"), QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:n|)def|)\\b"), QStringLiteral("^[\\s]*#\\bendif\\b"),
                QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b[\\s]+[^\\s#]+(?:[\\s]+[^#\\n]+|)"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:|n)def|)\\b[\\s]+[^\\n#]+|^[\\s]"), QStringLiteral("^[\\s]*#\\belse(?:if\\b[\\s]+[^#\\n]+|\\b)"),
                QStringLiteral("^[\\s]*#\\bline\\b[\\s]+\\d+(?:[\\s]+\"[^\\n\\s#]+\"|)"), QStringLiteral("^[\\s]*#\\b(?:error|pragma)\\b[\\s]+[^#\\n]+")
            };

            datatype_pattern = {
                QStringLiteral("\\b(?:u|\\s*)int(?:|(?:8|16|32|64)_t)\\b"), QStringLiteral("\\bchar(?:|(?:8|16|32)_t)\\b"),
                QStringLiteral("\\blong\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bbool\\b"),
                QStringLiteral("\\b(?:|un)signed\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
                QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\bsize_t\\b"), QStringLiteral("\\b_Bool\\b"),
                QStringLiteral("\\b_Complex\\b")
            };

            preprocessor_numbers_patterns = {
                QStringLiteral("(?:L|u|U|u8)(\".*\")"), QStringLiteral("([\\d\\.]+)(?:U|u|L|l|UL|Ul|uL|ul|LU|Lu|lU|lu|ll|LL|ULL|Ull|uLL|ull|LLU|LLu|llU|llu|F|f|L|l)")
            };

            special_symbols_patterns = {
                QStringLiteral("(?:\\+|\\-|&|\\||\\*|~|!|\\/|%|=|<|>|\\^|:|\\?|\\.\\.\\.)")
            };

            numbers_patterns = {
                QStringLiteral("\\b0(?:x|X)[0-9a-fA-F]*\\b"), QStringLiteral("\\b[0-9]*(?:(?:e|E)(?:\\+|\\-|)|)[0-9]*\\b")
            };

            variables_pattens = {
                QStringLiteral("\\b(?<=struct)\\s+([_A-Za-z][_A-Za-z0-9]*\\b)")
            };
        }
        break;

        case CPP_STD_2014:
        {
            keyword_patterns = {
                QStringLiteral("\\balignas\\b"), QStringLiteral("\\balignof\\b"), QStringLiteral("\\basm\\b"),
                QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bcatch\\b"),
                QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconcept\\b"), QStringLiteral("\\bconstexpr\\b"),
                QStringLiteral("\\bconst_cast\\b"), QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdecltype\\b"),
                QStringLiteral("\\bdefault\\b"), QStringLiteral("\\bdelete\\b"), QStringLiteral("\\bdo\\b"),
                QStringLiteral("\\bdynamic_cast\\b"), QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"),
                QStringLiteral("\\bexplicit\\b"), QStringLiteral("\\bexport\\b"), QStringLiteral("\\bextern\\b"),
                QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfor\\b"),  QStringLiteral("\\bfriend\\b"),
                QStringLiteral("\\bgoto\\b"), QStringLiteral("\\bif\\b"), QStringLiteral("\\binline\\b"),
                QStringLiteral("\\bmutable\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\bnew\\b"),
                QStringLiteral("\\bnoexcept\\b"), QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\boperator\\b"),
                QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
                QStringLiteral("\\bregister\\b"), QStringLiteral("\\breinterpret_cast\\b"), QStringLiteral("\\breturn\\b"),
                QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic_assert\\b"),
                QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"),
                QStringLiteral("\\btemplate\\b"), QStringLiteral("\\bthis\\b"), QStringLiteral("\\bthread_local\\b"),
                QStringLiteral("\\bthrow\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\btry\\b"),
                QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypeid\\b"), QStringLiteral("\\btypename\\b"),
                QStringLiteral("\\bunion\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bvirtual\\b"),
                QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bwhile\\b")
            };

            preprocessor_patterns = {
                QStringLiteral("^[\\s]*#"), QStringLiteral("^[\\s]*#\\binclude\\b"), QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:n|)def|)\\b"), QStringLiteral("^[\\s]*#\\bendif\\b"),
                QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b[\\s]+[^\\s#]+(?:[\\s]+[^#\\n]+|)"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:|n)def|)\\b[\\s]+[^\\n#]+|^[\\s]"), QStringLiteral("^[\\s]*#\\belse(?:if\\b[\\s]+[^#\\n]+|\\b)"),
                QStringLiteral("^[\\s]*#\\bline\\b[\\s]+\\d+(?:[\\s]+\"[^\\n\\s#]+\"|)"), QStringLiteral("^[\\s]*#\\b(?:error|pragma)\\b[\\s]+[^#\\n]+")
            };

            datatype_pattern = {
                QStringLiteral("\\b(?:u|\\s*)int(?:|(?:8|16|32|64)_t)\\b"), QStringLiteral("\\bchar(?:|(?:8|16|32)_t)\\b"),
                QStringLiteral("\\blong\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bbool\\b"),
                QStringLiteral("\\b(?:|un)signed\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
                QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\bsize_t\\b"), QStringLiteral("\\b_Bool\\b"),
                QStringLiteral("\\b_Complex\\b"), QStringLiteral("\\bauto\\b"), QStringLiteral("\\bvoid\\b")
            };

            special_symbols_patterns = {
                QStringLiteral("(?:\\+|\\-|&|\\||\\*|~|!|\\/|%|=|<|>|\\^|:|\\?|\\.\\.\\.)"), QStringLiteral("\\band\\b"), QStringLiteral("\\band_eq\\b"),
                QStringLiteral("\\bbitand\\b"), QStringLiteral("\\bbitor\\b"), QStringLiteral("\\bcompl\\b"), QStringLiteral("\\bnot\\b"),
                QStringLiteral("\\bnot_eq\\b"), QStringLiteral("\\bor\\b"), QStringLiteral("\\bor_eq\\b"), QStringLiteral("\\bxor\\b"),
                QStringLiteral("\\bxor_eq\\b")
            };


            preprocessor_numbers_patterns = {
                QStringLiteral("(?:L|u|U|u8)(\".*\")"), QStringLiteral("([\\d\\.]+)(?:U|u|L|l|UL|Ul|uL|ul|LU|Lu|lU|lu|ll|LL|ULL|Ull|uLL|ull|LLU|LLu|llU|llu|F|f|L|l)")
            };

            numbers_patterns = {
                QStringLiteral("\\b0(?:x|X)[0-9a-fA-F]*\\b"), QStringLiteral("\\b[0-9]*(?:(?:e|E)(?:\\+|\\-|)|)[0-9]*\\b"), QStringLiteral("\\b0(?:b|B)[01\\']*\\b")
            };

            variables_pattens = {
                QStringLiteral("\\b(?<=class|struct)\\s+([_A-Za-z][_A-Za-z0-9]*\\b)")
            };
        }
        break;

        case CPP_STD_2017:
        {
            keyword_patterns = {
                QStringLiteral("\\balignas\\b"), QStringLiteral("\\balignof\\b"), QStringLiteral("\\basm\\b"),
                QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bcatch\\b"),
                QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconcept\\b"), QStringLiteral("\\bconstexpr\\b"),
                QStringLiteral("\\bconst_cast\\b"), QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdecltype\\b"),
                QStringLiteral("\\bdefault\\b"), QStringLiteral("\\bdelete\\b"), QStringLiteral("\\bdo\\b"),
                QStringLiteral("\\bdynamic_cast\\b"), QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"),
                QStringLiteral("\\bexplicit\\b"), QStringLiteral("\\bexport\\b"), QStringLiteral("\\bextern\\b"),
                QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfor\\b"),  QStringLiteral("\\bfriend\\b"),
                QStringLiteral("\\bgoto\\b"), QStringLiteral("\\bif\\b"), QStringLiteral("\\binline\\b"),
                QStringLiteral("\\bmutable\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\bnew\\b"),
                QStringLiteral("\\bnoexcept\\b"), QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\boperator\\b"),
                QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
                QStringLiteral("\\bregister\\b"), QStringLiteral("\\breinterpret_cast\\b"), QStringLiteral("\\breturn\\b"),
                QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic_assert\\b"),
                QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"),
                QStringLiteral("\\btemplate\\b"), QStringLiteral("\\bthis\\b"), QStringLiteral("\\bthread_local\\b"),
                QStringLiteral("\\bthrow\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\btry\\b"),
                QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypeid\\b"), QStringLiteral("\\btypename\\b"),
                QStringLiteral("\\bunion\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bvirtual\\b"),
                QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bwhile\\b")
            };

            preprocessor_patterns = {
                QStringLiteral("^[\\s]*#"), QStringLiteral("^[\\s]*#\\binclude\\b"), QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:n|)def|)\\b"), QStringLiteral("^[\\s]*#\\bendif\\b"),
                QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b[\\s]+[^\\s#]+(?:[\\s]+[^#\\n]+|)"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:|n)def|)\\b[\\s]+[^\\n#]+|^[\\s]"), QStringLiteral("^[\\s]*#\\belse(?:if\\b[\\s]+[^#\\n]+|\\b)"),
                QStringLiteral("^[\\s]*#\\bline\\b[\\s]+\\d+(?:[\\s]+\"[^\\n\\s#]+\"|)"), QStringLiteral("^[\\s]*#\\b(?:error|pragma)\\b[\\s]+[^#\\n]+")
            };

            datatype_pattern = {
                QStringLiteral("\\b(?:u|\\s*)int(?:|(?:8|16|32|64)_t)\\b"), QStringLiteral("\\bchar(?:|(?:8|16|32)_t)\\b"),
                QStringLiteral("\\blong\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bbool\\b"),
                QStringLiteral("\\b(?:|un)signed\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
                QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\bsize_t\\b"), QStringLiteral("\\b_Bool\\b"),
                QStringLiteral("\\b_Complex\\b"), QStringLiteral("\\bauto\\b"), QStringLiteral("\\bvoid\\b")
            };

            special_symbols_patterns = {
                QStringLiteral("(?:\\+|\\-|&|\\||\\*|~|!|\\/|%|=|<|>|\\^|:|\\?|\\.\\.\\.)"), QStringLiteral("\\band\\b"), QStringLiteral("\\band_eq\\b"),
                QStringLiteral("\\bbitand\\b"), QStringLiteral("\\bbitor\\b"), QStringLiteral("\\bcompl\\b"), QStringLiteral("\\bnot\\b"),
                QStringLiteral("\\bnot_eq\\b"), QStringLiteral("\\bor\\b"), QStringLiteral("\\bor_eq\\b"), QStringLiteral("\\bxor\\b"),
                QStringLiteral("\\bxor_eq\\b")
            };

            preprocessor_numbers_patterns = {
                QStringLiteral("(?:L|u|U|u8|R|u8R|uR|UR|LR)(\".*\")"), QStringLiteral("([\\d\\.]+)(?:U|u|L|l|UL|Ul|uL|ul|LU|Lu|lU|lu|ll|LL|ULL|Ull|uLL|ull|LLU|LLu|llU|llu|F|f|L|l)")
            };

            numbers_patterns = {
                QStringLiteral("\\b0(?:x|X)[0-9a-fA-F]*\\b"), QStringLiteral("\\b[0-9]*(?:(?:e|E)(?:\\+|\\-|)|)[0-9]*\\b"), QStringLiteral("\\b0(?:b|B)[01\\']*\\b")
            };

            variables_pattens = {
                QStringLiteral("\\b(?<=class|struct)\\s+([_A-Za-z][_A-Za-z0-9]*\\b)")
            };
        }
        break;

        case CPP_STD_2020:
        {
            keyword_patterns = {
                QStringLiteral("\\balignas\\b"), QStringLiteral("\\balignof\\b"), QStringLiteral("\\basm\\b"),
                QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bcatch\\b"),
                QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconcept\\b"), QStringLiteral("\\bconsteval\\b"),
                QStringLiteral("\\bconstexpr\\b"), QStringLiteral("\\bconstinit\\b"), QStringLiteral("\\bconst_cast\\b"),
                QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bco_await\\b"), QStringLiteral("\\bco_return\\b"),
                QStringLiteral("\\bco_yield\\b"), QStringLiteral("\\bdecltype\\b"), QStringLiteral("\\bdefault\\b"),
                QStringLiteral("\\bdelete\\b"), QStringLiteral("\\bdo\\b"), QStringLiteral("\\bdynamic_cast\\b"),
                QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
                QStringLiteral("\\bexport\\b"), QStringLiteral("\\bextern\\b"), QStringLiteral("\\bfalse\\b"),
                QStringLiteral("\\bfor\\b"),  QStringLiteral("\\bfriend\\b"), QStringLiteral("\\bgoto\\b"),
                QStringLiteral("\\bif\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bmutable\\b"),
                QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\bnew\\b"), QStringLiteral("\\bnoexcept\\b"),
                QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bprivate\\b"),
                QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"), QStringLiteral("\\bregister\\b"),
                QStringLiteral("\\breinterpret_cast\\b"), QStringLiteral("\\brequires\\b"), QStringLiteral("\\breturn\\b"),
                QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic_assert\\b"),
                QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"),
                QStringLiteral("\\btemplate\\b"), QStringLiteral("\\bthis\\b"), QStringLiteral("\\bthread_local\\b"),
                QStringLiteral("\\bthrow\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\btry\\b"),
                QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypeid\\b"), QStringLiteral("\\btypename\\b"),
                QStringLiteral("\\bunion\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bvirtual\\b"),
                QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bwhile\\b")
            };

            preprocessor_patterns = {
                QStringLiteral("^[\\s]*#"), QStringLiteral("^[\\s]*#\\binclude\\b"), QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:n|)def|)\\b"), QStringLiteral("^[\\s]*#\\bendif\\b"),
                QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b[\\s]+[^\\s#]+(?:[\\s]+[^#\\n]+|)"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:|n)def|)\\b[\\s]+[^\\n#]+|^[\\s]"), QStringLiteral("^[\\s]*#\\belse(?:if\\b[\\s]+[^#\\n]+|\\b)"),
                QStringLiteral("^[\\s]*#\\bline\\b[\\s]+\\d+(?:[\\s]+\"[^\\n\\s#]+\"|)"), QStringLiteral("^[\\s]*#\\b(?:error|pragma)\\b[\\s]+[^#\\n]+")
            };

            datatype_pattern = {
                QStringLiteral("\\b(?:u|\\s*)int(?:|(?:8|16|32|64)_t)\\b"), QStringLiteral("\\bchar(?:|(?:8|16|32)_t)\\b"),
                QStringLiteral("\\blong\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bbool\\b"),
                QStringLiteral("\\b(?:|un)signed\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
                QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\bsize_t\\b"), QStringLiteral("\\b_Bool\\b"),
                QStringLiteral("\\b_Complex\\b"), QStringLiteral("\\bauto\\b"), QStringLiteral("\\bvoid\\b")
            };

            special_symbols_patterns = {
                QStringLiteral("(?:\\+|\\-|&|\\||\\*|~|!|\\/|%|=|<|>|\\^|:|\\?|\\.\\.\\.)"), QStringLiteral("\\band\\b"), QStringLiteral("\\band_eq\\b"),
                QStringLiteral("\\bbitand\\b"), QStringLiteral("\\bbitor\\b"), QStringLiteral("\\bcompl\\b"), QStringLiteral("\\bnot\\b"),
                QStringLiteral("\\bnot_eq\\b"), QStringLiteral("\\bor\\b"), QStringLiteral("\\bor_eq\\b"), QStringLiteral("\\bxor\\b"),
                QStringLiteral("\\bxor_eq\\b")
            };


            preprocessor_numbers_patterns = {
                QStringLiteral("(?:L|u|U|u8|R|u8R|uR|UR|LR)(\".*\")"), QStringLiteral("([\\d\\.]+)(?:U|u|L|l|UL|Ul|uL|ul|LU|Lu|lU|lu|ll|LL|ULL|Ull|uLL|ull|LLU|LLu|llU|llu|F|f|L|l)")
            };

            numbers_patterns = {
                QStringLiteral("\\b0(?:x|X)[0-9a-fA-F]*\\b"), QStringLiteral("\\b[0-9]*(?:(?:e|E)(?:\\+|\\-|)|)[0-9]*\\b"), QStringLiteral("\\b0(?:b|B)[01\\']*\\b")
            };

            variables_pattens = {
                QStringLiteral("\\b(?<=class|struct)\\s+([_A-Za-z][_A-Za-z0-9]*\\b)")
            };
        }
        break;

        case TEST:
        {
            keyword_patterns = {
                QStringLiteral("\\balignas\\b"), QStringLiteral("\\balignof\\b"), QStringLiteral("\\basm\\b"),
                QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bcatch\\b"),
                QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconcept\\b"), QStringLiteral("\\bconsteval\\b"),
                QStringLiteral("\\bconstexpr\\b"), QStringLiteral("\\bconstinit\\b"), QStringLiteral("\\bconst_cast\\b"),
                QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bco_await\\b"), QStringLiteral("\\bco_return\\b"),
                QStringLiteral("\\bco_yield\\b"), QStringLiteral("\\bdecltype\\b"), QStringLiteral("\\bdefault\\b"),
                QStringLiteral("\\bdelete\\b"), QStringLiteral("\\bdo\\b"), QStringLiteral("\\bdynamic_cast\\b"),
                QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
                QStringLiteral("\\bexport\\b"), QStringLiteral("\\bextern\\b"), QStringLiteral("\\bfalse\\b"),
                QStringLiteral("\\bfor\\b"),  QStringLiteral("\\bfriend\\b"), QStringLiteral("\\bgoto\\b"),
                QStringLiteral("\\bif\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bmutable\\b"),
                QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\bnew\\b"), QStringLiteral("\\bnoexcept\\b"),
                QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bprivate\\b"),
                QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"), QStringLiteral("\\bregister\\b"),
                QStringLiteral("\\breinterpret_cast\\b"), QStringLiteral("\\brequires\\b"), QStringLiteral("\\breturn\\b"),
                QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic_assert\\b"),
                QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"),
                QStringLiteral("\\btemplate\\b"), QStringLiteral("\\bthis\\b"), QStringLiteral("\\bthread_local\\b"),
                QStringLiteral("\\bthrow\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\btry\\b"),
                QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypeid\\b"), QStringLiteral("\\btypename\\b"),
                QStringLiteral("\\bunion\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bvirtual\\b"),
                QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bwhile\\b")
            };

            preprocessor_patterns = {
                QStringLiteral("^[\\s]*#"), QStringLiteral("^[\\s]*#\\binclude\\b"), QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:n|)def|)\\b"), QStringLiteral("^[\\s]*#\\bendif\\b"),
                QStringLiteral("^[\\s]*#\\b(?:define|undef)\\b[\\s]+[^\\s#]+(?:[\\s]+[^#\\n]+|)"),
                QStringLiteral("^[\\s]*#\\bif(?:(?:|n)def|)\\b[\\s]+[^\\n#]+|^[\\s]"), QStringLiteral("^[\\s]*#\\belse(?:if\\b[\\s]+[^#\\n]+|\\b)"),
                QStringLiteral("^[\\s]*#\\bline\\b[\\s]+\\d+(?:[\\s]+\"[^\\n\\s#]+\"|)"), QStringLiteral("^[\\s]*#\\b(?:error|pragma)\\b[\\s]+[^#\\n]+")
            };

            datatype_pattern = {
                QStringLiteral("\\b(?:u|\\s*)int(?:|(?:8|16|32|64)_t)\\b"), QStringLiteral("\\bchar(?:|(?:8|16|32)_t)\\b"),
                QStringLiteral("\\blong\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bbool\\b"),
                QStringLiteral("\\b(?:|un)signed\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
                QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\bsize_t\\b"), QStringLiteral("\\b_Bool\\b"),
                QStringLiteral("\\b_Complex\\b"), QStringLiteral("\\bauto\\b"), QStringLiteral("\\bvoid\\b")
            };

            special_symbols_patterns = {
                QStringLiteral("(?:\\+|\\-|&|\\||\\*|~|!|\\/|%|=|<|>|\\^|:|\\?|\\.\\.\\.)"), QStringLiteral("\\band\\b"), QStringLiteral("\\band_eq\\b"),
                QStringLiteral("\\bbitand\\b"), QStringLiteral("\\bbitor\\b"), QStringLiteral("\\bcompl\\b"), QStringLiteral("\\bnot\\b"),
                QStringLiteral("\\bnot_eq\\b"), QStringLiteral("\\bor\\b"), QStringLiteral("\\bor_eq\\b"), QStringLiteral("\\bxor\\b"),
                QStringLiteral("\\bxor_eq\\b")
            };

            preprocessor_numbers_patterns = {
                QStringLiteral("(?:L|u|U|u8|R|u8R|uR|UR|LR)(\".*\")"), QStringLiteral("([\\d\\.]+)(?:U|u|L|l|UL|Ul|uL|ul|LU|Lu|lU|lu|ll|LL|ULL|Ull|uLL|ull|LLU|LLu|llU|llu|F|f|L|l)")
            };

            numbers_patterns = {
                QStringLiteral("\\b0(?:x|X)[0-9a-fA-F]*\\b"), QStringLiteral("\\b[0-9]*(?:(?:e|E)(?:\\+|\\-|)|)[0-9]*\\b"), QStringLiteral("\\b0(?:b|B)[01\\']*\\b")
            };
            variables_pattens = {
                QStringLiteral("\\b(?<=class|struct)\\s+([_A-Za-z][_A-Za-z0-9]*\\b)")
            };
        }
        break;
    }
    for (const QString &pattern : keyword_patterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.keyword_format;
        highlighting_rules->append(rule);
    }

    for (const QString &pattern : preprocessor_patterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.preprocessor_format;
        highlighting_rules->append(rule);
    }

    for (const QString &pattern : datatype_pattern)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.data_type_format;
        highlighting_rules->append(rule);
    }

    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = highlight_palette.function_format;
    highlighting_rules->append(rule);

    if (syntax_type >= CPP_STD_2014)
    {
        rule.pattern = QRegularExpression(QStringLiteral("\\b(?:class|struct)\\b[\\s]+"));
        rule.format = highlight_palette.class_format;
        highlighting_rules->append(rule);
    }

    for (const QString &pattern : special_symbols_patterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.special_symbols_format;
        highlighting_rules->append(rule);
    }

    for (const QString &pattern : numbers_patterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.numbers_format;
        highlighting_rules->append(rule);
    }

    for (const QString &pattern : variables_pattens)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.variable_format;
        highlighting_rules->append(rule);
    }

    for (const QString &pattern : preprocessor_numbers_patterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = highlight_palette.preprocessor_numbers_format;
        highlighting_rules->append(rule);
    }

    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = highlight_palette.single_line_comment_format;
    highlighting_rules->append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("(?><.*?>)*(([\"'])(?:|(?:.*?)(?:[^\\\\]|[^\\\\]?(?:\\\\{2})+))\\2)?"));
    rule.format = highlight_palette.quotation_format;
    highlighting_rules->append(rule);

    comment_start_expression = QRegularExpression(QStringLiteral("/\\*"));
    comment_end_expression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::rehighlightBlock(const QString &text)
{
    highlightBlock(text);
}

void Highlighter::highlightBlock(const QString &text)
{
    if (!highlight_visible)
    {
        setFormat(0, text.length(), highlight_palette.font_format);
        return;
    }
    for (const HighlightingRule &rule : qAsConst(*highlighting_rules))
    {
        QRegularExpressionMatchIterator match_iterator = rule.pattern.globalMatch(text);
        while (match_iterator.hasNext())
        {
            QRegularExpressionMatch match = match_iterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = text.indexOf(comment_start_expression);
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = comment_end_expression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, highlight_palette.multi_line_comment_format);
        startIndex = text.indexOf(comment_start_expression, startIndex + commentLength);
    }
}

void Highlighter::changeHighlightPalette(highlightPalette new_highlight_palette)
{
    highlight_palette = new_highlight_palette;
}
