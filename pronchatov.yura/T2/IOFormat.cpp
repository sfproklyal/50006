#include "IOFormat.h"

#include <cctype>
#include <limits>
#include <string>

namespace
{
    bool readOneSpace(std::istream& in)
    {
        char c = '\0';
        in.get(c);

        if (!in || c != ' ')
        {
            in.setstate(std::ios::failbit);
            return false;
        }

        if (in.peek() == ' ')
        {
            in.setstate(std::ios::failbit);
            return false;
        }

        return true;
    }

    bool isDigit(char c)
    {
        return std::isdigit(static_cast<unsigned char>(c)) != 0;
    }

    bool isCorrectDblSci(const std::string& token)
    {
        if (token.empty())
        {
            return false;
        }

        std::size_t ePos = token.find('e');
        if (ePos == std::string::npos)
        {
            ePos = token.find('E');
        }

        std::size_t dotPos = token.find('.');

        if (ePos == std::string::npos || dotPos == std::string::npos)
        {
            return false;
        }

        if (dotPos > ePos)
        {
            return false;
        }

        std::size_t mantissaStart = 0;

        if (token[mantissaStart] == '+' || token[mantissaStart] == '-')
        {
            ++mantissaStart;
        }

        if (mantissaStart >= dotPos)
        {
            return false;
        }

        for (std::size_t i = mantissaStart; i < dotPos; ++i)
        {
            if (!isDigit(token[i]))
            {
                return false;
            }
        }

        if (dotPos + 1 >= ePos)
        {
            return false;
        }

        for (std::size_t i = dotPos + 1; i < ePos; ++i)
        {
            if (!isDigit(token[i]))
            {
                return false;
            }
        }

        std::size_t expStart = ePos + 1;

        if (expStart < token.size() && (token[expStart] == '+' || token[expStart] == '-'))
        {
            ++expStart;
        }

        if (expStart >= token.size())
        {
            return false;
        }

        for (std::size_t i = expStart; i < token.size(); ++i)
        {
            if (!isDigit(token[i]))
            {
                return false;
            }
        }

        return true;
    }
}

std::istream& operator>>(std::istream& in, DelimeterIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    char c = '\0';
    in.get(c);

    if (!in || c != dest.exp)
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, LabelIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    dest.ref.clear();
    while (in && in.peek() != ' ')
    {
        char c = static_cast<char>(in.get());
        if (!std::isalnum(static_cast<unsigned char>(c)))
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        dest.ref += c;
    }

    if (dest.ref.empty() || in.peek() != ' ')
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    if (!readOneSpace(in))
    {
        return in;
    }

    in >> DelimeterIO{ '"' };
    if (!in)
    {
        return in;
    }

    std::getline(in, dest.ref, '"');

    if (!in)
    {
        return in;
    }

    if (in.peek() != ':')
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, UllBinIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    if (!readOneSpace(in))
    {
        return in;
    }

    char zero = '\0';
    char prefix = '\0';

    in.get(zero);
    in.get(prefix);

    if (!in || zero != '0' || (prefix != 'b' && prefix != 'B'))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    unsigned long long value = 0;
    bool hasDigits = false;

    while (in.peek() == '0' || in.peek() == '1')
    {
        char digitChar = static_cast<char>(in.get());
        unsigned long long digit = static_cast<unsigned long long>(digitChar - '0');

        if (value > (std::numeric_limits<unsigned long long>::max() - digit) / 2)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        value = value * 2 + digit;
        hasDigits = true;
    }

    if (!hasDigits)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (in.peek() != ':')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref = value;

    return in;
}

std::istream& operator>>(std::istream& in, DblSciIO&& dest)
{
    std::istream::sentry sentry(in, true);
    if (!sentry)
    {
        return in;
    }

    if (!readOneSpace(in))
    {
        return in;
    }

    std::string token;

    while (in && in.peek() != ':')
    {
        token += static_cast<char>(in.get());
    }

    if (!in || token.empty())
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!isCorrectDblSci(token))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    try
    {
        std::size_t processed = 0;
        dest.ref = std::stod(token, &processed);
        if (processed != token.size())
        {
            in.setstate(std::ios::failbit);
        }
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

iofguard::iofguard(std::basic_ios<char>& s):
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{}

iofguard::~iofguard()
{
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}
