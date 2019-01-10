#pragma once

#include <string>

class Heuristic;

enum MOVE_STATISTIC {
    IMPROVE,
    ACCEPTED,
    REFUSED
};

class GlassMove {
    public:
    GlassMove(Heuristic* heuristic): heuristic(heuristic) {}
    GlassMove() {}

    virtual void commit() {}
    virtual void revert() {}
    virtual bool attempt() {return true;}
    void addStat(MOVE_STATISTIC statistic);
    void addTested();
    void displayStatistics();

    protected:
    Heuristic* heuristic;
    std::string name;

    private:
    unsigned int nbImproved;
    unsigned int nbAccepted;
    unsigned int nbRefused;
    unsigned int nbTested;
};