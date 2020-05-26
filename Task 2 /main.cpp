#include <iostream>
#include <ctime>
#include <algorithm>
#include <map>
#include <vector>
#include "buddy_alloc.h"
#include "file.h"


using namespace std;

struct Comparator {
    bool operator()(char *a, char *b) const {
        while (isalpha(*a)) {
            char la = tolower(*a);
            char lb = tolower(*b);
            if (la < lb)
                return true;
            if (la > lb)
                return false;

            a++;
            b++;
        }
        return isalpha(*b);
    }
};

map<char *, size_t, Comparator, BuddyAlloc<char *>> parseWithBuddy(char *text) {
    auto counter = map<char *, size_t, Comparator, BuddyAlloc<char *>>();
    bool begin = true;

    while (*text != '\x00') {
        if (isalpha(*text)) {
            if (begin) {
                counter[text] += 1;
                begin = false;
            }
        } else {
            begin = true;
        }

        ++text;

        return counter;
    }
}

map<string, size_t> parseWithSystem(char *text) {
    auto counter = map<string, size_t>();
    string cur;

    while (*text != '\x00') {
        if (isalpha(*text)) {
            cur += tolower(*text);
        } else {
            if (!cur.empty()) {
                counter[cur] += 1;
                cur = "";
            }
        }

        text++;
    }

    return counter;

}

template<class T>
bool cmp(pair<T, size_t> first, pair<T, size_t> second) {
    return first.second > second.second;
}

void run(char *text, DWORD size) {
    int start = clock();
    auto firstMap = parseWithBuddy(text);
    int time1 = clock() - start;

    start = clock();
    auto secondMap = parseWithSystem(text);
    int time2 = clock() - start;

    vector<pair<char *, size_t>> sortedFirst;
    for (auto &it : firstMap) {
        sortedFirst.push_back(it);
    }

    sort(sortedFirst.begin(), sortedFirst.end(), cmp<char *>);

    vector<pair<string, size_t>> sortedSecond;
    for (auto &it : secondMap) {
        sortedSecond.push_back(it);
    }

    sort(sortedSecond.begin(), sortedSecond.end(), cmp<string>);

    cout << "Размер файла: " << size << "B\n";
    cout << "Мой аллокатор\n";
    cout << "Время: " << time1 << "мс\n";
    cout << "Частота: \n";
    auto it1 = firstMap.begin();
    for (; it1 != firstMap.end(); ++it1) {
        cout << "\t";
        char *letter = it1->first;
        while (isalpha(*letter))
            cout << *(letter++);
        cout << " : " << it1->second << "\n";
    }
    cout << "Встроенный аллокатор\n";
    cout << "Время: " << time2 << "мс\n";
    cout << "Частота: \n";
    auto it2 = secondMap.begin();
    for (; it2 != secondMap.end(); ++it2) {
        cout << "\t";
        cout << it2->first << " : " << it2->second << "\n";
    }

}

int main() {
    auto file = File((char *) "../text.txt");
    auto text = file.getText();
    run(text, file.Size);
    return 0;
}
