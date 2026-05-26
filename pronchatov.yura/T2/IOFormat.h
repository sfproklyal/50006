#ifndef IOFORMAT_H
#define IOFORMAT_H

#include <iostream>
#include <string>

struct DelimeterIO
{
    char exp;
};

struct LabelIO
{
    std::string& ref;
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
std::istream& operator>>(std::istream& in, LabelIO&& dest);
std::istream& operator>>(std::istream& in, DblSciIO&& dest);
std::istream& operator>>(std::istream& in, UllBinIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);

class iofguard
{
public:
    iofguard(std::basic_ios<char>& s);
    ~iofguard();

private:
    std::basic_ios<char>& s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

#endif
