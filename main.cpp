#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <functional>
#include <fstream>
#include <set>
#include <cassert>

#include <chrono>
#include <iomanip>
using namespace std;

class LogDuration {
public:
    // заменим имя типа std::chrono::steady_clock
    // с помощью using для удобства
    using Clock = std::chrono::steady_clock;
    std::string text;
    LogDuration() {
    }
    LogDuration(std::string t) {
        text=t;
    }

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        std::cerr <<text<< ": "s  <<duration_cast<microseconds>(dur).count() << " µs"s << std::endl;
    }

private:
    const Clock::time_point start_time_ = Clock::now();
};


class Domain {
    
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
public:
    Domain (string_view dom): domain_str_({dom.rbegin(), dom.rend()}){}

    bool operator==(const Domain& rhs) const {
        return domain_str_ == rhs.domain_str_;
    }
    bool operator!=(const Domain& rhs) const { 
        return !(*this == rhs);
    }
    
    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain (const Domain& rhs) const {
        
        if (domain_str_.size() < rhs.domain_str_.size()) {return false;}
        
        if (domain_str_.substr(0, rhs.domain_str_.size() + 1) == (rhs.domain_str_ + ".")) {
            return true;
        }  
        return false;
    }

    string* GetValue () {
        return &domain_str_;
    }
    const string* GetValue () const {
        const string* temp = &domain_str_;
        return temp;
    }
    bool operator< (const Domain& rhs) const {
        return std::lexicographical_compare(domain_str_.begin(), domain_str_.end(),
                                           rhs.domain_str_.begin(), rhs.domain_str_.end());
        
    }

   

private:
    string domain_str_;
    

};

template <typename Type>
std::ostream& operator<< (std::ostream &os, const vector <Type>& container)
{
    for (const auto& item: container) {
        string_view temp = *(item.GetValue());
        string temp2 {temp.rbegin(), temp.rend()};
        os << temp2 << endl;
    }
    
    return os;
}

class DomainChecker {
    public:
        // конструктор должен принимать список запрещённых доменов через пару итераторов
        template <typename Iterator>
        DomainChecker (const Iterator begin, const Iterator end) { // : forb_dom_(begin, end)
            //LogDuration test_time12 ("DomainChecker "s);
            set<Domain> s ( begin, end );
            forb_dom_.assign( s.begin(), s.end() );

           //  std::sort (forb_dom_.begin(), forb_dom_.end());
            bool exit = false;
            size_t beg = 0;
            size_t en = 0;

            do {
                //LogDuration test_time3e232 ("unique do-while "s);
                beg = forb_dom_.size();
                

                auto last2 = std::unique(forb_dom_.begin(), forb_dom_.end(), //execution::par,
                                            [&](const auto& a, const auto& b){

                        //if (a == b) {return true;}
                        if (b.IsSubdomain(a) ) {return true;} 
                        
                        return false;
                        
                });
                forb_dom_.erase(last2, forb_dom_.end());

                en = forb_dom_.size();
                if (beg == en) {
                    exit = true;
                }
            } while (!exit);

            // ofstream out_file_f_dom ("forbid_domens.txt");
            // out_file_f_dom << forb_dom_ << endl;

           
        }
        
        // разработайте метод IsForbidden, возвращающий true, если домен запрещён
        bool IsForbidden (const Domain& domain) const {
            if (forb_dom_.empty()) {return false;}
            auto upper = std::upper_bound(forb_dom_.begin(), forb_dom_.end(), domain);
              
                //cout <<"upper-1 "<< *((*(upper-1)).GetValue()) << endl;
                //cout <<"upper "<< *((*(upper)).GetValue()) << endl;

            if (*(std::prev(upper, 1)) == domain || domain.IsSubdomain(*(std::prev(upper, 1)))) {
                //cout <<"upper-1 v2"<< *((*(upper-1)).GetValue()) << endl;
                //cout <<"upper "<< *((*(upper)).GetValue()) << endl;
                return true;   
            }

            return false;
        }
        
    private:    
       vector <Domain> forb_dom_={};
};



template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num = 0;
    //input >> num;
    std::istringstream(line) >> num;

    return num;
}

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Func>
std::vector<Domain> ReadDomains (istream& input, Func func) {
    std::vector<Domain> result;
    auto number = func;
    for (auto i = 0; std::less{}(i, number); ++i) {
        string temp_str;
        getline(input, temp_str);
        //Domain temp(temp_str);
        result.emplace_back(temp_str);
    }
    return result;
}

void Test () {
    LogDuration test_time("Time1 "s);
    istringstream instr ("4\ngdz.ru\nmaps.me\nm.gdz.ru\ncom\n7\ngdz.ru\ngdz.com\nm.maps.me\nalg.m.gdz.ru\nmaps.com\nmaps.ru\ngdz.ua");
    ostringstream outstr;
    const std::vector<Domain> forbidden_domains = ReadDomains(instr, ReadNumberOnLine<size_t>(instr));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    const std::vector<Domain> test_domains = ReadDomains(instr, ReadNumberOnLine<size_t>(instr));
    for (const Domain& domain : test_domains) {
        outstr << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
    string test =outstr.str();
    assert (outstr.str() == "Bad\nBad\nBad\nBad\nBad\nGood\nGood\n");
}

void Test2 (){
    LogDuration test_time2("Time2 "s);
    ifstream in_file ("in_test_8.txt");
    //ifstream in_file2 ("input2.txt");
    ofstream out_file ("out2.txt");
    const std::vector<Domain> forbidden_domains = ReadDomains(in_file, ReadNumberOnLine<size_t>(in_file));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    const std::vector<Domain> test_domains = ReadDomains(in_file, ReadNumberOnLine<size_t>(in_file));
    for (const Domain& domain : test_domains) {
        out_file << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

void Test3 (){
    //LogDuration test_time3("Time3 "s); 
    ifstream in_file ("input_hard.txt");
    //ifstream in_file2 ("input2.txt");
    ofstream out_file ("out3.txt");
    const std::vector<Domain> forbidden_domains = ReadDomains(in_file, ReadNumberOnLine<size_t>(in_file));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    const std::vector<Domain> test_domains = ReadDomains(in_file, ReadNumberOnLine<size_t>(in_file));
    for (const Domain& domain : test_domains) {
        out_file << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

void Test4 (){
    //LogDuration test_time4("Time4 "s); 
    ifstream in_file ("in_test_10.txt");
    //ifstream in_file2 ("input2.txt");
    ofstream out_file ("out4.txt");
    const std::vector<Domain> forbidden_domains = ReadDomains(in_file, ReadNumberOnLine<size_t>(in_file));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    const std::vector<Domain> test_domains = ReadDomains(in_file, ReadNumberOnLine<size_t>(in_file));
    for (const Domain& domain : test_domains) {
        out_file << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

void Test5 () {
    LogDuration test_time("Time5 "s);
    istringstream instr ("1\nopqj.wbo\n1\ntpq.a.hve.t.b.m.h.dgg.opqj.wbo");
    ostringstream outstr;
    const std::vector<Domain> forbidden_domains = ReadDomains(instr, ReadNumberOnLine<size_t>(instr));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    const std::vector<Domain> test_domains = ReadDomains(instr, ReadNumberOnLine<size_t>(instr));
    for (const Domain& domain : test_domains) {
        outstr << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
    string test =outstr.str();
    assert (outstr.str() == "Bad\n");
}

int main() {
        // Test();
        // //  Test2();
        //  Test3();
        // {
        //     LogDuration test_time6("Time_Test3_10_times "s);
        //     for (int i = 0; i < 10; ++i) {
        //         Test3();
        //     } 
        // }
        // {
        //     LogDuration test_time6("Time6 "s); 
        //     for (int i = 0; i < 10; ++i) {
        //          Test4();
        //     }   
        // }  
        //Test5();
    const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    DomainChecker checker(forbidden_domains.cbegin(), forbidden_domains.cend());
    //auto test = ReadNumberOnLine<size_t>(cin);
    const std::vector<Domain> test_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}