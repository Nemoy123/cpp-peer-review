#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <execution>
//#include <mutex>
#include <sstream>
#include <fstream>
//#include <set>
#include <chrono>
#include <cassert>

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
        std::cerr <<text<< ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const Clock::time_point start_time_ = Clock::now();
};


class Ebook {
    private:
    
    
    std::vector <int> number_pages_; // индекс - id пользователя, int номер страницы, до которой дочитал пользователь id
    std::vector <int> number_users_; //по каждому индексу m будет храниться количество пользователей, дочитавших как минимум до страницы m.
    int users_count = 0;

    public:

    Ebook() {
        number_pages_.assign(100000, 0);
        number_users_.assign(1000, 0);
    };
    void Read (int id, int page) {
        
       
        if (!page) {return;}
        if (!number_pages_[id-1]) {
            ++number_users_[page-1];
            ++users_count;
        }
        else {
           --number_users_[number_pages_[id-1]-1];
           ++number_users_[page-1];
        }
        
        number_pages_[id-1] = std::move (page);
        
    }
    void Cheer(ostream& out, const int id) const {
        
        if (!number_pages_[id-1]) {
            out << 0.0; 
            return;
        }
        else if (users_count == 1) {
            out << 1.0; 
            return;
        }

        int less_all_size_without_null = std::reduce(
                                            number_users_.cbegin(), number_users_.cbegin() + number_pages_[id-1]-1);
        
        double result = static_cast<double> (less_all_size_without_null) / static_cast<double> ((users_count-1));
        
        out << setprecision(6) << result;
    }

    void Run (istream& input, ostream& out) {
        int count = 0;
        input >> count;
        bool check_first_time = false;
        for (int i = 0; i < count; ++i) {

            int id = 0;
            string operation;
            input >> operation >> id;
            
            if (operation == "READ"s) {
                int page = 0;
                input >> page;
                Read(std::move (id), std::move (page));
            }
            else if (operation == "CHEER"s && check_first_time) {
                out << endl;
                Cheer(out, std::move (id));
            }
            else if (operation == "CHEER"s && !check_first_time) {
                Cheer(out, std::move (id));
                check_first_time = true;
            }
            
        }
    }

};



void Test1 () {
    //LogDuration test_time("Test1"s);
    Ebook test_ebook;
    ostringstream out_s;
    std::string str {R"(12 CHEER 5
                        READ 1 10
                        CHEER 1
                        READ 2 5
                        READ 3 7
                        CHEER 2
                        CHEER 3
                        READ 3 10
                        CHEER 3
                        READ 3 11
                        CHEER 3
                        CHEER 1)"};
    istringstream in_s(str);
    cout << endl;
    test_ebook.Run(in_s, out_s);
    assert (out_s.str() == "0\n1\n0\n0.5\n0.5\n1\n0.5");
    
   

}

void Test2 () {
    LogDuration test_time("Test2"s);
    Ebook test_ebook;
    ostringstream out_s;
    std::string str {R"(4 
                        READ 100000 1000
                        READ 1 1
                        CHEER 100000
                        CHEER 1)"};
    istringstream in_s(str);

    cout << endl;
    cout << "test2 start" << endl;
    test_ebook.Run(in_s, out_s);
    cout << out_s.str();
    cout << endl;
    cout << "test2 end" << endl;

}

void Test3 () {
    LogDuration test_time("Test3"s);
    Ebook test_ebook;
    ostringstream out_s;
    std::string str {R"(50
                    READ 4 29
                    READ 1 49
                    CHEER 7
                    READ 5 10
                    CHEER 6
                    CHEER 2
                    READ 2 23
                    READ 5 13
                    READ 2 48
                    CHEER 5
                    READ 4 75
                    CHEER 1
                    CHEER 5
                    CHEER 4
                    CHEER 7
                    READ 8 41
                    READ 3 24
                    READ 4 118
                    READ 2 87
                    CHEER 4
                    CHEER 8
                    READ 5 54
                    CHEER 6
                    CHEER 4
                    CHEER 6
                    READ 5 59
                    CHEER 6
                    CHEER 8
                    READ 8 51
                    READ 3 40
                    READ 7 38
                    READ 6 15
                    CHEER 8
                    CHEER 1
                    CHEER 3
                    CHEER 7
                    CHEER 7
                    READ 8 85
                    READ 1 93
                    CHEER 5
                    READ 2 106
                    READ 3 70
                    CHEER 5
                    CHEER 2
                    READ 4 128
                    READ 3 105
                    CHEER 6
                    READ 1 101
                    READ 5 75
                    CHEER 4)"};
    istringstream in_s(str);

    cout << endl;
    cout << "test3 start" << endl;
    test_ebook.Run(in_s, out_s);
    //Run (test_ebook, in_s, out_s);
    cout << out_s.str();
    cout << endl;
    cout << "test3 end" << endl;

}

void Test4 () {
    LogDuration test_time("Test4"s);
    Ebook test_ebook;
    ifstream in_file ("input_9.txt");
    ofstream out_file ("out.txt");

    cout << endl;
    cout << "test4 start" << endl;
    test_ebook.Run(in_file, out_file);
    cout << endl;
    cout << "test4 end" << endl;

}


int main() {
    // {
    //     LogDuration test_time("Total time"s);
      //   Test1 ();
    //     Test2 ();
    //     Test3 ();
    //     Test4 ();
    // }
    Ebook ebook;
    istream& input = std::cin;
    ostream& out = std::cout;
    ebook.Run(input, out);
    
   

}
