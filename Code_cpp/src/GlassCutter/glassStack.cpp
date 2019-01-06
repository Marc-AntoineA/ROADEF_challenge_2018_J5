#include "glassStack.h"

#include <algorithm>
#include <string>

// est-ce qu'on insère forcément à la fin
void GlassStack::insert(const GlassItem& item) {
    items.push_back(item.getId());    
    for (unsigned int k = items.size() - 1; k > 0; k--) {
        unsigned int previousItemPosition = instance->getItem(items[k]).getStackSequence();
        unsigned int itemPosition = instance->getItem(items[k - 1]).getStackSequence();
        if (itemPosition >= previousItemPosition) 
            std::iter_swap(items.begin() + k, items.begin() + k - 1);
    }
}

unsigned int GlassStack::top() {
    if (isEmpty())
        throw std::runtime_error("top impossible for stack# " + std::to_string(stackId) + " (empty).");
    return items[currentIndex];
}

void GlassStack::push() {
    if (currentIndex == 0) 
        throw std::runtime_error("push impossible for stack#" + std::to_string(stackId));
    currentIndex--;
}

unsigned int GlassStack::pop() {
    unsigned int item = top();
    currentIndex++;
    return item;
}

bool GlassStack::isEmpty() {
    return currentIndex == items.size();
}

void GlassStack::reset() {
    currentIndex = 0;
}

std::ostream& operator<<(std::ostream& os, GlassStack stack) {
    os << "GlassStack #" << stack.getStackId();
    if (!stack.isEmpty())
        os << " -Top: item# " << stack.top();
    else    
        os << " -Top: empty stack";
    os << " -items: [";
    for (unsigned int itemIndex: stack.getItems()) {
        os << itemIndex << ", ";
    }
    os << "]" << std::endl;
    return os;
}