#include <iostream>


template<class T>
class aallocator {
public:
    typedef T* pointer;
    typedef T value_type;
    typedef size_t size_type;

    aallocator() {
        ptr = malloc(1024 * 1024);
        last = 0;
    }

    pointer allocate(size_t s) {
        pointer r = ((pointer) ptr) + last;
        last += s;
        return r;
    }

    void deallocate(pointer p, size_t s){
        last -= s;
    }

    ~aallocator(){
        free(ptr);
    }

private:
    void *ptr;
    size_t last;

};