#ifndef IOFORMAT_H
#define IOFORMAT_H

#include <ios>
#include <string>

struct DelimeterIO
{
    char exp;
};

struct DblSciIO
{
    double& ref;
};

struct UllBinIO
{
    unsigned long long& ref;
};

struct StringIO
{
    std::string& ref;
};

std::istream& operator>>(std::istream& in, DelimeterIO&& dest);
std::istream& operator>>(std::istream& in, DblSciIO&& dest);
std::istream& operator>>(std::istream& in, UllBinIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);

std::string formatDblSci(double value);
std::string formatUllBin(unsigned long long value);

class iofguard
{
public:
    explicit iofguard(std::basic_ios<char>& s);
    ~iofguard();

private:
    std::basic_ios<char>& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

#endif
