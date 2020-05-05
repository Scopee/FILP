#include <fstream>
#include "windows.h"

using namespace std;

struct File {
private:
    HANDLE h;
    char *filename;
    char *text = nullptr;

    DWORD getSize() {
        LARGE_INTEGER size;
        bool ok = GetFileSizeEx(h, &size);

        if (ok)
            return size.QuadPart;

        exit(-1);
    }

public:
    DWORD size;

    File(LPCWSTR filename) {
        this->filename = filename;
        h = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (h == INVALID_HANDLE_VALUE) {
            cout << "Can not open file";
            exit(-1);
        }

        size = getSize();
    }

    void close(){
        CloseHandle(h);
    }

    ~File() {
        CloseHandle(h);
    }

    char *getText() {
        if (text != nullptr)
            return text;
        text = new char[size + 1];
        DWORD buf;

        ReadFile(h, text, size, &buf, NULL);

        text[buf] = '\x00';
        return text;
    }
};
