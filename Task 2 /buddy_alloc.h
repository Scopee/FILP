
#ifndef BUDDY_ALLOC_BUDDY_ALLOC_H
#define BUDDY_ALLOC_BUDDY_ALLOC_H

#include <iostream>

using namespace std;

struct Block {
    size_t level;
    size_t num;
    Block *next{};
    Block *prev{};

    Block(size_t level, size_t number) {
        this->level = level;
        this->num = number;
    }

    size_t getSize() {
        return 1 << level;
    }

    size_t getOffset() {
        return num * getSize();
    }
};

template<class T>
struct BuddyAlloc {
    typedef T value_type;
    typedef T *pointer;
    typedef size_t size_type;

    BuddyAlloc() {
        init(20);
    }

    explicit BuddyAlloc(size_t maxDepth) {
        init(maxDepth);
    }

    pointer allocate(size_t size) {
        size *= typeSize;
        auto level = getLevel(size);
        auto block = getBlock(level);
        if (block == nullptr) {
            if (nextAlloc == nullptr) {
                nextAlloc = new BuddyAlloc(maxDepth);
            }
            return nextAlloc->allocate(size);
        }

        void *res = ptr + block->getOffset();
        return reinterpret_cast<pointer> (res);
    }

    void deallocate(pointer p, size_t size) {
        size *= typeSize;
        auto level = getLevel(size);
        auto offset = getOffset(p);
        auto number = offset / (1 << level);
        auto block = Block(level, number);
        block.next = block[level];
        blocks[level] = &block;
    }

    ~BuddyAlloc() {
        delete nextAlloc;
        free(ptr);
        delete blocks;
    }

private:
    size_t baseSize;
    size_t maxDepth;
    size_t typeSize;
    void *ptr;

    Block **blocks;
    BuddyAlloc *nextAlloc;

    size_t getLevel(size_t size) {
        size_t level = 0;
        while (size > 0) {
            size /= 2;
            level++;
        }
        return level;
    }

    void init(size_t maxDepth) {
        this->maxDepth = maxDepth;
        baseSize = 1 << maxDepth;
        ptr = malloc(baseSize);

        auto p1 = (pointer) ptr;
        auto p2 = p1 + 1;

        typeSize = (size_t) p2 - (size_t) p1;
        blocks = new Block *[maxDepth];
        for (size_t i = 0; i < maxDepth; i++) {
            blocks[i] = nullptr;
        }

        blocks[maxDepth - 1] = new Block(20, 0);
        nextAlloc = nullptr;
    }

    Block *takeBlock(size_t level) {
        if (level == maxDepth)
            return nullptr;

        if (blocks[level] != nullptr) {
            Block *current = blocks[level];
            blocks[level] = current->next;
            return current;
        }

        Block *block = takeBlock(level - 1);
        auto *res = new Block(level, block->num * 2);
        block = new Block(level, block->num * 2 + 1);
        blocks[level] = block;

        return res;
    }

    size_t getOffset(pointer pptr) {
        return (size_t) pptr - (size_t) ptr;
    }
};


#endif //BUDDY_ALLOC_BUDDY_ALLOC_H
