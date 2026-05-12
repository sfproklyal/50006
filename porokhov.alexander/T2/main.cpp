#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <cctype>
#include <sstream>

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

class iofmtguard
{
public:
    explicit iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        precision_(s.precision()),
        fill_(s.fill()),
        fmt_(s.flags())
    {
    }

    ~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    std::streampos start_pos = in.tellg();
    
    char ch;
    if (!(in >> ch) || ch != '(')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    DataStruct tmp{};
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_key3 = false;

    while (true)
    {
        in >> std::ws;
        
        std::streampos label_pos = in.tellg();
        
        std::string label;
    bool label_read = false;
    while (in && std::isprint(in.peek()) && in.peek() != ' ' && in.peek() != ':')
    {
        char c;
        in.get(c);
        label.push_back(c);
    }
    label_read = !label.empty();
    
    if (!label_read)
    {
        in.clear();
        in.seekg(start_pos);
        in.setstate(std::ios::failbit);
        return in;
    }

        if (label == ":)")
        {
            break;
        }
        else if (label == ":key1")
        {
            double val = 0.0;
            char suffix = ' ';
            if (in >> val >> suffix && 
                std::tolower(static_cast<unsigned char>(suffix)) == 'd')
            {
                tmp.key1 = val;
                has_key1 = true;
            }
            else
            {
                in.clear();
                in.seekg(start_pos);
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else if (label == ":key2")
        {
            unsigned long long val = 0;
            char u = ' ', l1 = ' ', l2 = ' ';
            if (in >> val >> u >> l1 >> l2 &&
                std::tolower(static_cast<unsigned char>(u)) == 'u' &&
                std::tolower(static_cast<unsigned char>(l1)) == 'l' &&
                std::tolower(static_cast<unsigned char>(l2)) == 'l')
            {
                tmp.key2 = val;
                has_key2 = true;
            }
            else
            {
                in.clear();
                in.seekg(start_pos);
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else if (label == ":key3")
        {
            char quote;
            if (in >> quote && quote == '"')
            {
                std::string val;
                while (in && in.peek() != '"')
                {
                    char c;
                    in.get(c);
                    val.push_back(c);
                }
                if (in >> quote && quote == '"')
                {
                    tmp.key3 = val;
                    has_key3 = true;
                }
                else
                {
                    in.clear();
                    in.seekg(start_pos);
                    in.setstate(std::ios::failbit);
                    return in;
                }
            }
            else
            {
                in.clear();
                in.seekg(start_pos);
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else
        {
            in.clear();
            in.seekg(start_pos);
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    if (has_key1 && has_key2 && has_key3)
    {
        dest = tmp;
        return in;
    }
    else
    {
        in.clear();
        in.seekg(start_pos);
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    iofmtguard fmtguard(out);
    out << "(:key1 ";
    out << std::fixed;
    out << std::setprecision(1);
    out << src.key1;
    out << "d:key2 ";
    out << src.key2;
    out << "ull:key3 \"";
    out << src.key3;
    out << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2)
    {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}

int main()
{
    std::vector<DataStruct> vec;
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(vec)
    );

    std::sort(vec.begin(), vec.end(), compareDataStruct);

    std::copy(
        vec.begin(),
        vec.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
