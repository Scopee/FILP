//
// Created by Степан on 16/05/2020.
//

#ifndef BUDDY_ALLOC_FILE_H
#define BUDDY_ALLOC_FILE_H

#include <iostream>
#include "windows.h"

using namespace std;

struct File {
private:
    HANDLE file;
    char *filename;
    char *text = nullptr;

    DWORD getSize() {
        LARGE_INTEGER size;

        bool ok = GetFileSizeEx(file, &size);

        if (ok) {
            return size.guadPart;
        }

        exit(-1);
    }

public:
    DWORD Size;

    File(char *filename) {
        this->filename = filename;
        file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (file == INVALIDE_HANDLE_VALUE) {
            cout << "Can't open file";
            exit(-1);
        }

        Size = getSize();
    }

    ~File() {
        Close();
    }

    void Close() {
        CloseHandler(file);
    }

    char *getText() {
        if (text != nullptr) {
            return text;
        }

        text = new char[Size + 1];
        DWORD buff;

        ReadFile(file, text, Size, &buff, NULL);

        text[buff] = '\x00';

        return text;
    }
};

#endif //BUDDY_ALLOC_FILE_H
