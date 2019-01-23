#pragma once

#include <string>

#define NOTHING -1

class Heuristic;

enum MOVE_STATISTIC {
    IMPROVE,
    ACCEPTED,
    REFUSED
};

class GlassMove {
    public:
    GlassMove(Heuristic* heuristic);
    
    virtual void commit() {}
    virtual void revert() {}
    virtual void attempt(unsigned int& beginIndex, unsigned int& endIndex) = 0;
    void addStat(MOVE_STATISTIC statistic);
    void addTested();
    void displayStatistics();
    const std::string& getName() { return name; }

    protected:
    unsigned int getSequenceSize() const;
    Heuristic* heuristic;
    std::string name;

    private:
    unsigned int nbImproved;
    unsigned int nbAccepted;
    unsigned int nbRefused;
    unsigned int nbTested;
};