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
    virtual int attempt() { return NOTHING; }
    void addStat(MOVE_STATISTIC statistic);
    void addTested();
    void displayStatistics();
    const std::string& getName() { return name; }

    protected:
    Heuristic* heuristic;
    std::string name;

    private:
    unsigned int nbImproved;
    unsigned int nbAccepted;
    unsigned int nbRefused;
    unsigned int nbTested;
};