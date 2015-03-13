#include "BraceCounter.h"

namespace Twofold {
namespace intern {
namespace Javascript {
namespace {

#define ROUND_OPEN '('
#define ROUND_CLOSE ')'
#define INDEX_OPEN '['
#define INDEX_CLOSE ']'
#define CURLY_OPEN '{'
#define CURLY_CLOSE '}'
#define DOUBLE_QUOTE '"'
#define SINGLE_QUOTE '\''
#define BACK_SLASH '\\'

inline bool isCurlyOrQuote(QChar chr)
{
    switch (chr.unicode())
    {
    case CURLY_OPEN:
    case CURLY_CLOSE:
    case DOUBLE_QUOTE:
    case SINGLE_QUOTE:
        return true;
    default:
        return false;
    }
}

inline bool isBraceOrQuote(QChar chr)
{
    switch (chr.unicode())
    {
    case ROUND_OPEN:
    case ROUND_CLOSE:
    case INDEX_OPEN:
    case INDEX_CLOSE:
    case CURLY_OPEN:
    case CURLY_CLOSE:
    case DOUBLE_QUOTE:
    case SINGLE_QUOTE:
        return true;
    default:
        return false;
    }
}

QString::const_iterator findQuoteEnd(QString::const_iterator it, const QString::const_iterator end)
{
    QChar quote = *it;
    it++;
    while (it != end) {
        if (*it == quote) return it; // found closing quote
        if (*it == BACK_SLASH) {
            it++;
            if (it == end) break; // invalid (TODO: continue next line)
        }
        it++;
    }
    return it; // invalid
}

} // namespace

int
BraceCounter::countExpressionDepth(It it, const It end, int depth)
{
    while (it != end) {
        auto event = std::find_if(it, end, isBraceOrQuote);
        if (event == end) break;
        switch (event->unicode())
        {
        case ROUND_CLOSE:
        case INDEX_CLOSE:
        case CURLY_CLOSE:
            if (0 == depth) return -1; // more cloing than opening
            depth--;
            break;
        case ROUND_OPEN:
        case INDEX_OPEN:
        case CURLY_OPEN:
            depth++;
            break;
        default: // quote
            event = findQuoteEnd(event, end);
            if (event == end) return depth + 1; // invalid
        }
        it = event + 1;
    }
    return depth;
}

BraceCounter::It
BraceCounter::findExpressionEnd(It it, It end)
{
    int depth = 0;
    while (it != end) {
        auto event = std::find_if(it, end, isCurlyOrQuote);
        if (event == end) return event; // invalid
        switch (event->unicode())
        {
        case CURLY_CLOSE:
            if (0 == depth) return event; // found end
            depth--;
            break;
        case CURLY_OPEN:
            depth++;
            break;
        default: // quote
            event = findQuoteEnd(event, end);
            if (event == end) return event; // invalid
        }
        it = event + 1;
    }
    return it;
}

} // namespace Javascript
} // namespace intern
} // namespace Twofold
