#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include "aallocator.h"
#include "file.h"
#include <ctime>
#include "windows.h"

using namespace std;

struct Cmp {
    bool operator()(char *c1, char *c2) const {
        while (isalpha(*c1)) {
            char lc1 = tolower(*c1);
            char lc2 = tolower(*c2);
            if (lc1 < lc2)
                return true;
            if (lc1 > lc2)
                return false;

            c1++, c2++;
        }
        return isalpha(*c2);
    }
};

map<char *, size_t, Cmp, aallocator<char *>> parse(char *text) {
    auto count = map<char *, size_t, Cmp, aallocator<char *>>();
    bool begin = true;

    while (*text != '\x00') {
        if (isalpha(*text)) {
            if (begin) {
                count[text] += 1;
                begin = false;
            }
        } else {
            begin = true;
        }

        ++text;
    }
    return count;
}

map<string, size_t> parsedef(char *text) {
    map<string, size_t> count = map<string, size_t>();
    string cur;
    while (*text != '\x00') {
        if (isalpha(*text)) {
            cur += tolower(*text);
        } else {
            if (!cur.empty()) {
                count[cur] += 1;
                cur = "";
            }
        }
        text++;
    }
    return count;
}

void start(char *text, DWORD size) {
    int startTime = clock();
    auto firstMap = parse(text);
    int time1 = clock() - startTime;

    startTime = clock();
    auto secondMap = parsedef(text);
    int time2 = clock() - startTime;

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
    auto file = File(L"..\\test.txt");
    auto text = file.getText();
    start(text, file.size);
    file.close();
}
