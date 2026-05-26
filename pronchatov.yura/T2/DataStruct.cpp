#include "DataStruct.h"
#include "IOFormat.h"

#include <iomanip>
#include <sstream>
#include <string>

namespace
{
    std::string formatDblSci(double value)
    {
        std::ostringstream out;
        iofguard guard(out);

        out << std::scientific << std::setprecision(1) << value;
        std::string result = out.str();
        std::size_t expPos = result.find('e');
        if (expPos == std::string::npos)
        {
            return result;
        }

        std::size_t signPos = expPos + 1;
        std::size_t firstDigit = signPos + 1;
        while (firstDigit + 1 < result.size() && result[firstDigit] == '0')
        {
            result.erase(firstDigit, 1);
        }

        return result;
    }

    std::string formatUllBin(unsigned long long value)
    {
        std::string result;

        do
        {
            result.insert(result.begin(), static_cast<char>('0' + value % 2));
            value /= 2;
        }
        while (value != 0);

        return "0b" + result;
    }

    bool readField(std::istream& in, DataStruct& data, bool& hasKey1, bool& hasKey2, bool& hasKey3)
    {
        std::string label;
        in >> LabelIO{ label };

        if (label == "key1" && !hasKey1)
        {
            in >> DblSciIO{ data.key1 };
            hasKey1 = true;
        }
        else if (label == "key2" && !hasKey2)
        {
            in >> UllBinIO{ data.key2 };
            hasKey2 = true;
        }
        else if (label == "key3" && !hasKey3)
        {
            in >> StringIO{ data.key3 };
            hasKey3 = true;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }

        in >> DelimeterIO{ ':' };

        return static_cast<bool>(in);
    }
}

bool DataStructComparator::operator()(const DataStruct& left, const DataStruct& right) const
{
    if (left.key1 != right.key1)
    {
        return left.key1 < right.key1;
    }

    if (left.key2 != right.key2)
    {
        return left.key2 < right.key2;
    }

    return left.key3.length() < right.key3.length();
}

std::istream& operator>>(std::istream& in, DataStruct& data)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    DataStruct input;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    in >> DelimeterIO{ '(' } >> DelimeterIO{ ':' };
    for (std::size_t i = 0; i < 3 && in; ++i)
    {
        readField(in, input, hasKey1, hasKey2, hasKey3);
    }
    in >> DelimeterIO{ ')' };

    if (in && hasKey1 && hasKey2 && hasKey3)
    {
        data = input;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    out << "(:key1 " << formatDblSci(data.key1)
        << ":key2 " << formatUllBin(data.key2)
        << ":key3 \"" << data.key3 << "\":)";

    return out;
}
