#include <iostream>

using namespace std;

template<typename T>
class SmartPointer {
private:
    T *value;
    unsigned int *counter;

public:
    SmartPointer() {
        counter = nullptr;
        value = nullptr;
    }

    void Set(T *new_value) {
        Release();
        value = new_value;
        counter = new unsigned int(1);
    }

    T Get() {
        return *value;
    }

    void Release() {
        if (!value)
            return;


        (*counter)--;
        if (*counter == 0) {
            delete value;
            delete counter;
        }

        counter = nullptr;
        value = nullptr;

    }

    ~SmartPointer() {
        if (value)
            Release();
    }

    T &operator*() {
        return *value;
    }

    T *operator->() {
        return value;
    }

    SmartPointer &operator=(const SmartPointer &other) {
        if (value)
            Release();
        if (this != &other) {
            value = other.value;
            counter = other.counter;
            (*counter)++;
        }

        return *this;
    }
};

int main() {
    SmartPointer<int> pointer1 = SmartPointer<int>();
    SmartPointer<int> pointer2 = SmartPointer<int>();
    pointer1.Set(new int(1));
    pointer2.Set(new int(2));
    cout << "Pointer 1 value: " << pointer1.Get() << " | Pointer 2 value: " << pointer2.Get() << endl;
    pointer1 = pointer2;
    cout << "Pointer 1 value: " << pointer1.Get() << " | Pointer 2 value: " << pointer2.Get() << endl;
    pointer1.Release();
    pointer2.Release();
}