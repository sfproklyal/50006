#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "DataStruct.h"
#include "Comparator.h"

class DataStructIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = DataStruct;
    using difference_type = std::ptrdiff_t;
    using pointer = const DataStruct*;
    using reference = const DataStruct&;

    DataStructIterator() : is_(nullptr), isValid_(false) {}

    explicit DataStructIterator(std::istream& is) : is_(&is), isValid_(false) {
        readNext();
    }

    reference operator*() const { return current_; }
    pointer operator->() const { return &current_; }

    DataStructIterator& operator++() {
        readNext();
        return *this;
    }

    DataStructIterator operator++(int) {
        DataStructIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const DataStructIterator& other) const {
        if (!isValid_ && !other.isValid_) {
            return true;
        }
        if (is_ != other.is_) {
            return false;
        }
        return isValid_ == other.isValid_;
    }

    bool operator!=(const DataStructIterator& other) const {
        return !(*this == other);
    }

private:
    void readNext() {
        if (!is_) {
            isValid_ = false;
            return;
        }

        while (true) {
            if (is_->eof()) {
                isValid_ = false;
                return;
            }

            DataStruct temp{};
            *is_ >> temp;

            if (!is_->fail()) {
                current_ = temp;
                isValid_ = true;
                return;
            }

            is_->clear();
            
            int ch = is_->peek();
            while (ch != '(' && ch != std::char_traits<char>::eof()) {
                is_->get();
                ch = is_->peek();
            }

            if (ch == std::char_traits<char>::eof()) {
                isValid_ = false;
                return;
            }
        }
    }

    std::istream* is_;
    DataStruct current_;
    bool isValid_;
};

int main() {
    std::vector<DataStruct> data{};

    DataStructIterator beginIter(std::cin);
    DataStructIterator endIter;

    std::copy(beginIter, endIter, std::back_inserter(data));

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
