#include "IOFormat.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
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

    bool readTokenUntilColon(std::istream& in, std::string& token)
    {
        token.clear();

        while (in && in.peek() != std::char_traits<char>::eof() && in.peek() != ':')
        {
            char c = '\0';
            in.get(c);
            token += c;
        }

        if (!in || in.peek() == std::char_traits<char>::eof() || token.empty())
        {
            in.setstate(std::ios::failbit);
            return false;
        }

        return true;
    }

    bool hasWhitespace(const std::string& str)
    {
        for (char c : str)
        {
            if (std::isspace(static_cast<unsigned char>(c)))
            {
                return true;
            }
        }

        return false;
    }

    bool isDblSciToken(const std::string& token)
    {
        if (token.empty() || hasWhitespace(token))
        {
            return false;
        }

        std::size_t pos = 0;

        if (token[pos] == '+' || token[pos] == '-')
        {
            ++pos;
        }

        std::size_t digitsBeforeDot = pos;
        while (pos < token.size() && std::isdigit(static_cast<unsigned char>(token[pos])))
        {
            ++pos;
        }

        if (digitsBeforeDot == pos)
        {
            return false;
        }

        if (pos >= token.size() || token[pos] != '.')
        {
            return false;
        }

        ++pos;

        std::size_t digitsAfterDot = pos;
        while (pos < token.size() && std::isdigit(static_cast<unsigned char>(token[pos])))
        {
            ++pos;
        }

        if (digitsAfterDot == pos)
        {
            return false;
        }

        if (pos >= token.size() || (token[pos] != 'e' && token[pos] != 'E'))
        {
            return false;
        }

        ++pos;

        if (pos < token.size() && (token[pos] == '+' || token[pos] == '-'))
        {
            ++pos;
        }

        std::size_t exponentStart = pos;
        while (pos < token.size() && std::isdigit(static_cast<unsigned char>(token[pos])))
        {
            ++pos;
        }

        return exponentStart != pos && pos == token.size();
    }

    bool parseUllBinToken(const std::string& token, unsigned long long& value)
    {
        if (token.size() < 3)
        {
            return false;
        }

        if (token[0] != '0' || (token[1] != 'b' && token[1] != 'B'))
        {
            return false;
        }

        unsigned long long result = 0;
        const unsigned long long maxValue = std::numeric_limits<unsigned long long>::max();

        for (std::size_t i = 2; i < token.size(); ++i)
        {
            if (token[i] != '0' && token[i] != '1')
            {
                return false;
            }

            unsigned long long bit = static_cast<unsigned long long>(token[i] - '0');

            if (result > (maxValue - bit) / 2)
            {
                return false;
            }

            result = result * 2 + bit;
        }

        value = result;
        return true;
    }
}

std::istream& operator>>(std::istream& in, DelimeterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char c = '\0';
    in >> c;

    if (in && c != dest.exp)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (in.peek() == ' ')
    {
        in.setstate(std::ios::failbit);
    }

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
    if (!readTokenUntilColon(in, token))
    {
        return in;
    }

    if (!isDblSciToken(token))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    try
    {
        std::size_t parsedChars = 0;
        double value = std::stod(token, &parsedChars);

        if (parsedChars != token.size())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.ref = value;
    }
    catch (const std::exception&)
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

    std::string token;
    if (!readTokenUntilColon(in, token))
    {
        return in;
    }

    unsigned long long value = 0;
    if (!parseUllBinToken(token, value))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest.ref = value;
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

    in >> std::quoted(dest.ref);

    if (!in)
    {
        return in;
    }

    if (in.peek() == ' ')
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::string formatDblSci(double value)
{
    std::ostringstream out;
    out << std::scientific << std::setprecision(1) << value;

    std::string result = out.str();

    std::size_t expPos = result.find('e');
    if (expPos == std::string::npos)
    {
        expPos = result.find('E');
    }

    if (expPos == std::string::npos)
    {
        return result;
    }

    std::string mantissa = result.substr(0, expPos);
    std::string exponent = result.substr(expPos + 1);

    char sign = '+';
    std::size_t digitStart = 0;

    if (!exponent.empty() && (exponent[0] == '+' || exponent[0] == '-'))
    {
        sign = exponent[0];
        digitStart = 1;
    }

    std::string digits = exponent.substr(digitStart);

    while (digits.size() > 1 && digits[0] == '0')
    {
        digits.erase(digits.begin());
    }

    if (digits.empty())
    {
        digits = "0";
    }

    return mantissa + "e" + sign + digits;
}

std::string formatUllBin(unsigned long long value)
{
    if (value == 0)
    {
        return "0b0";
    }

    std::string result;

    while (value > 0)
    {
        result += static_cast<char>('0' + value % 2);
        value /= 2;
    }

    std::reverse(result.begin(), result.end());

    return "0b" + result;
}

iofguard::iofguard(std::basic_ios<char>& s):
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{
}

iofguard::~iofguard()
{
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

