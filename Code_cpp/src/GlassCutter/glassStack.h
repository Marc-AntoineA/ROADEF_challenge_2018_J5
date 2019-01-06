#pragma once

#include "../GlassData/glassInstance.h"

class GlassStack {
    public:
    GlassStack(GlassInstance* instance, unsigned int stackId)
        :instance(instance), currentIndex(0), stackId(stackId){}

    void insert(const GlassItem& item);
    void push();
    void reset();
    unsigned int pop();
    unsigned int top();
    bool isEmpty();
    unsigned int getStackId() { return stackId; }
    const std::vector<unsigned int>& getItems() { return items; }

    private:
    GlassInstance* instance;
    std::vector<unsigned int> items;
    unsigned int currentIndex;
    unsigned int stackId;
};

std::ostream& operator<<(std::ostream& os, GlassStack glassStack);