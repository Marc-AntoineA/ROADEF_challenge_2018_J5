#include "../GlassCutter/glassCutter.h"
#include "../GlassData/glassConstants.h"

#include <limits>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <sstream>

GlassCutter::GlassCutter(GlassInstance* instance, std::vector<unsigned int>& sequence)
    :instance(instance), sequence(sequence), scoredTree(new ScoredLocationTree()) {
    currentBinId = 0;
    nbRollbacks = 0;
    nbAttempts = 0;
    nbInfeasible = 0;
    currentSequenceIndex = 0;
    xLimit = NB_PLATES * WIDTH_PLATES;
    currentMaxX = xLimit;
    buildStacks();
    buildMonsters();
    buildNodes();
    buildLocations();
    buildFirstIndexOnEachPlate();
    resetErrorsStatistics();
}

void GlassCutter::reset() {
    currentBinId = 0;
    xLimit = NB_PLATES * WIDTH_PLATES;
    currentSequenceIndex = 0;
    for (GlassNode& node: nodes) 
        node.reset();
    for (RedMonster& monster: monsters)
        monster.reset();
    for (GlassStack& stack: stacks)
        stack.reset();
    for (std::vector<GlassLocation>& locationsTmp: locations) 
        locationsTmp.clear();
}

void GlassCutter::revertPlatesUntilSequenceIndex(unsigned int sequenceIndex) {
    currentBinId++;
    while (currentSequenceIndex >= sequenceIndex) {
        for (const GlassLocation& location: locations[currentBinId]){
            stacks[location.getStackId()].push();
            currentSequenceIndex--;
        }
        locations[currentBinId].clear();
        monsters[currentBinId].reset();
        nodes[currentBinId].reset();
        if (currentBinId > 0)
            currentBinId--;   
        else
            break;
    }
}

double GlassCutter::getCurrentBigNodeSurfaceOccupation() {
    if (currentNode()->getNbItems() == 0) return 0;
    std::vector<GlassNode>& sons = currentNode()->getSons();
    if (sons.back().getNbItems() == 0) {
        return sons[sons.size() - 2].getSurfaceOccupation();
    }
    return sons.back().getSurfaceOccupation();
}

bool GlassCutter::cut(unsigned int depth, unsigned int endSequenceIndex){
    //displayStacks();
    scoredTree->reset();
    unsigned int nbScore = 0;
    unsigned int nbNot = 0;

    if (VERBOSE) std::cout << "InitWithSequence en cours..." << std::endl;
    while (currentSequenceIndex < sequence.size()) {
        unsigned int stackId = sequence[currentSequenceIndex];
        unsigned int itemIndex = stacks[stackId].top();
        buildLazyDeepScoreTree(currentSequenceIndex, depth, scoredTree);
        if(computeBestLocationAndApplyIfNecessary()) {
            currentSequenceIndex++;
        } else {
            incrBinId();
            if (currentSequenceIndex > endSequenceIndex && isCurrentBinUnmodified(currentSequenceIndex)) {
                currentMaxX = xLimit;
                //std::cout << currentMaxX << std::endl;
                return true;
            }
            if (currentBinId * WIDTH_PLATES > xLimit) { currentMaxX = currentBinId*WIDTH_PLATES + getXMax(); return false;}
        }
    }
    assert(currentSequenceIndex == sequence.size());
    currentMaxX = currentBinId * WIDTH_PLATES + getXMax();
    /*if (xLimit > 218207 && currentMaxX == 218207) {
        displayLocations();
        assert(false);
    }*/
    xLimit = std::min(currentMaxX, xLimit);
    return true;
}

double GlassCutter::quickEvaluateLocation(double lazy) {
    double xMax = lazy ? getLazyXMax() : getXMax();
    //double monsterArea = currentMonster()->computeArea();

    return 1 - xMax/WIDTH_PLATES;
}

double GlassCutter::buildLazyDeepScoreTree(unsigned int sequenceIndex, unsigned int depth, ScoredLocationTree* tree) {
    //std::cout << "buildLazyDeepScoreTree " << sequenceIndex << " " << depth << std::endl;
    //tree->reset();// TODO remove ça...
    tree->depth = depth;
    double currentScore = 0;
    //std::cout << sequenceIndex << " >= " << sequence.size() << std::endl;
    if (depth == 0 || sequenceIndex >= sequence.size()) {
        tree->scoredLocation.score = quickEvaluateLocation(LAZY);
        return tree->scoredLocation.score;
    }

    if (isLessGood()) {
        tree->scoredLocation.score = -1000000;
        return tree->scoredLocation.score;
    }

    if (!tree->sons.empty()) {
        for (unsigned int index = 0; index < tree->sons.size(); index++) {
            ScoredLocationTree* son = tree->sons[index];
            if (!lazyAttempt(son->scoredLocation.location)) { revertSameBin(); continue; }
            currentScore = std::max(currentScore, 1. + buildLazyDeepScoreTree(sequenceIndex + 1, depth - 1, son));
            revertSameBin();
        }
        tree->scoredLocation.score = currentScore;
        return tree->scoredLocation.score;
    }

    unsigned int itemIndex = stacks[sequence[sequenceIndex]].top();
    const std::vector<GlassLocation>& currentLocations = currentMonster()->getLocationsForItemIndex(itemIndex);
    /*if (itemIndex == 18) {
        for (const GlassLocation& location: currentLocations) {
            std::cout << location << std::endl;
        }
        std::cout << "==========" << std::endl;
    }*/
    if (currentLocations.size() == 0) return 0;

    for (const GlassLocation& location: currentLocations) {
        if (!lazyAttempt(location));
        tree->sons.push_back(new ScoredLocationTree(location));
        ScoredLocationTree* son = tree->sons.back();
        currentScore = std::max(currentScore, 1. + buildLazyDeepScoreTree(sequenceIndex + 1, depth - 1, son));
        revertSameBin();
    }
    
    tree->scoredLocation.score = currentScore;
    return tree->scoredLocation.score;
}

unsigned int GlassCutter::compute1CutFeasibleForMinWaste(unsigned int x) const {
    unsigned int cutX = x;
    const GlassLocation& mainLocation = locations[currentBinId].back();
    /*if (mainLocation.getX() == 2413 && mainLocation.getY() == 0 && mainLocation.getItemIndex() == 15) 
        std::cout << "  " << locations[currentBinId].back() << std::endl;*/
    for (const GlassLocation& location: locations[currentBinId]) {
        //std::cout << location << std::endl;
        if (location.getXW() == x) continue;
        if (location.getXW() + MIN_WASTE_AREA < x) continue;
        if (x < location.getXW()) continue;
        //std::cout << location << std::endl;
        cutX = std::max(cutX, 1 + location.getXW() + MIN_WASTE_AREA);
        if (location.getX() >= x) return x;
    }
    if (cutX > x) cutX = std::max(cutX, x + MIN_WASTE_AREA);
    return cutX;
}

unsigned int GlassCutter::convertBinSequenceToMainSequence(unsigned int binSequence) const {
    unsigned int mainSequence = 0;
    for (unsigned int binId = 0; binId < currentBinId; binId++) {
        mainSequence += locations[binId].size();
    }
    mainSequence += binSequence;
    return mainSequence;
}

GlassCutter::ScoredLocation GlassCutter::treeScore(ScoredLocationTree* tree) {
    tree->sort();
    double bestScore = quickEvaluateLocation(LAZY); // TODO potentiel bug ici avec les attempt

    unsigned int bestLocationIndex = tree->sons.size();
    for (unsigned int index = 0; index < tree->sons.size(); index++) {
        ScoredLocationTree* son = tree->sons[index];
        ScoredLocation& scoredLocation = son->scoredLocation;
        //std::cout << scoredLocation.location << std::endl;
        if(bestScore >= 1 + scoredLocation.score) break;
        GlassLocation& location = scoredLocation.location;
        switch (scoredLocation.feasible) {
        case NOT_FEASIBLE:
            continue;

        case NOT_TESTED:
            if (!fullAttempt(location)) {
                revertSameBin();
                //std::cout << "failed" << location << std::endl;
                scoredLocation.feasible = NOT_FEASIBLE;
                unsigned int cutX = compute1CutFeasibleForMinWaste(location.getX());
                if (cutX == location.getX()) continue;
                assert(cutX >= location.getX());
                assert(cutX >= location.getX() + MIN_WASTE_AREA);  
                location.setX(cutX);
                if (!currentPlate()->rectangleIsFreeOutOfDefects(location.getX(), location.getY(), location.getXW(), location.getYH())) {
                    scoredLocation.feasible = NOT_FEASIBLE;
                    continue;
                }
                son->reset();
                if (!fullAttempt(location)) {
                    revertSameBin();
                    //std::cout << "still not feasible" << location << std::endl;
                    scoredLocation.feasible = NOT_FEASIBLE;
                    continue;
                }
                //std::cout << "builLazyDeepScoreTree" << std::endl;
                buildLazyDeepScoreTree(convertBinSequenceToMainSequence(location.getLocationSequence()), son->depth, son); // depth??
                //son->display();
            }
            break;

        case FEASIBLE:
            if (!lazyAttempt(location)) {
                assert(false);
                revertSameBin();
                scoredLocation.feasible = NOT_FEASIBLE;
                continue;
            }
            break;
        }
        scoredLocation.feasible = FEASIBLE;
        double currentScore = 1 + treeScore(son).score;
        //std::cout << scoredLocation.location << " : " << currentScore << std::endl;
        if (currentScore >= bestScore) {
            bestLocationIndex = index;
            bestScore = currentScore;
        }
        revertSameBin();
    } 
    //std::cout << bestScore << std::endl;
    //std::cout << bestLocationIndex << std::endl;
    if (bestLocationIndex < tree->sons.size() && bestScore > 0) {
        //std::cout << bestScore << " vs " << bestLocationIndex << " < " << tree.sons.size() << std::endl;
        return ScoredLocation(tree->sons[bestLocationIndex]->scoredLocation.location, bestScore);
    }

    return ScoredLocation(GlassLocation(), bestScore);
}

bool GlassCutter::computeBestLocationAndApplyIfNecessary() {
    //std::cout << "computing best location " << std::endl;
    //scoredTree.display();
    ScoredLocation best = treeScore(scoredTree);

    if (best.location.getInstance() == NULL) return false;
    if (best.score <= 0) return false;

    if (!fullAttempt(best.location)) assert(false);
    /*std::cout << "fullAttempt(GlassLocation(" <<  best.location.getItemIndex()<< ", " << currentBinId << ", ";
    std::cout << best.location.getX() << ", " << best.location.getY() << ", " << (best.location.getRotated() ? "true, this))" : "false, this));") << std::endl;*/
    for (unsigned int sonIndex = 0; sonIndex < scoredTree->sons.size(); sonIndex++) {
        const GlassLocation& currentLocation = scoredTree->sons[sonIndex]->scoredLocation.location;
        if (currentLocation == best.location) { 
            ScoredLocationTree* newScoredTree = scoredTree->sons[sonIndex];
            scoredTree->sons.erase(scoredTree->sons.begin() + sonIndex);
            delete scoredTree;
            scoredTree = newScoredTree;
            return true;
        }
    }
    assert(false);
}

unsigned int GlassCutter::getCurrentScore() {
    return currentMaxX*HEIGHT_PLATES - instance->getItemsArea();
}

void GlassCutter::buildFirstIndexOnEachPlate() {
    firstIndexInEachPlate.resize(NB_PLATES);
    for (unsigned int index = 0; index < NB_PLATES; index++) {
        firstIndexInEachPlate[index] = -1;
    }
}

void GlassCutter::buildStacks() {
    stacks.clear();
    for (const GlassItem& item: instance->getItems()) {
        unsigned int stackId = item.getStackId();
        if (stacks.size() <= stackId) {
            for (unsigned int k = stacks.size(); k < stackId + 1; k++)
                stacks.emplace_back(GlassStack(instance, k));
        }
        stacks[stackId].insert(item);
    }
    if (VERBOSE) displayStacks();
}

void GlassCutter::buildNodes() {
    nodes.clear();
    nodes.resize(NB_PLATES);
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
        nodes[plateIndex].setPlateIndex(plateIndex);
        nodes[plateIndex].setInstance(instance);
        nodes[plateIndex].setCutter(this);
    }
}

void GlassCutter::buildMonsters() {
    monsters.clear();
    monsters.resize(NB_PLATES);
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++) {
        monsters[plateIndex].setPlateIndex(plateIndex);
        monsters[plateIndex].setInstance(instance);
    }
}

void GlassCutter::buildLocations() {
    locations.clear();
    locations.resize(NB_PLATES);
}

bool GlassCutter::isCurrentBinUnmodified(unsigned int newBinItemIndex) const {
    int previousBinItemIndex = firstIndexInEachPlate[currentBinId];
    if (previousBinItemIndex < 0) return false;
    /*if (((unsigned int)previousBinItemIndex) == newBinItemIndex)
        std::cout << currentBinId << " -- " << newBinItemIndex << " < " << sequence.size() << std::endl;*/
    return ((unsigned int)previousBinItemIndex) == newBinItemIndex;
}

void GlassCutter::incrBinId() {
    currentBinId++;
    if (currentBinId >= NB_PLATES) throw std::runtime_error("Incr bin impossible");
    scoredTree->reset();
    if(VERBOSE) std::cout << "Incr bin id to " << currentBinId << std::endl;
}

void GlassCutter::decrBinId() {
    if (currentBinId == 0) return;
    currentMonster()->reset();
    currentLocations()->clear();
    currentBinId--;
    //std::cout << "decrBinId" << std::endl;
}

bool GlassCutter::checkTreeFeasibilityAndBuildCurrentNode() {
    const GlassLocation& location = currentLocations()->back();
    /*if (location.getX() == 1828 && location.getY() == 2256 && location.getXW() == 4126 && location.getYH() == 2684)
            displayLocations();*/
    try {
        currentNode()->checkNodeAndReturnNbItemsCuts(0, currentLocations()->size());
        return true;
    } catch (const std::runtime_error& e) {
        addErrorStatistic(e.what());
        /*std::cout << "---|||---|||---" << std::endl;
        std::cout << e.what() << std::endl;
        displayLocations();*/
        nbInfeasible++;
        return false;
    }
}

bool GlassCutter::lazyAttempt(const GlassLocation& location) {
    return attempt(location, true);
}

bool GlassCutter::fullAttempt(const GlassLocation& location) {
    return attempt(location, false);
}

// TODO build pour guillotine cut?
bool GlassCutter::attempt(const GlassLocation& location, bool fast) {
    //  std::cout << "attempt location: " << location << std::endl;
    setBinId(location.getBinId());
    nbAttempts++;
    stacks[location.getStackId()].pop();
    currentMonster()->incrRedMonster(location);
    currentLocations()->push_back(location);
    if (fast) return true;
    if (!checkTreeFeasibilityAndBuildCurrentNode()) {
        //std::cout << (*currentMonster()) << std::endl;
        return false;
    }
    return true;
}

void GlassCutter::revertSameBin() {
    unsigned int stackId = currentLocations()->back().getStackId();
    if (VERBOSE) std::cout << "revert for location " << currentLocations()->back() << std::endl;
    stacks[stackId].push();
    currentLocations()->pop_back();
    currentMonster()->revert();
}

void GlassCutter::revert() {
    nbRollbacks++;
    while (currentLocations()->empty()) {
        if (currentBinId == 0) throw std::runtime_error("Rollback impossible (not enough items).");
        decrBinId();
    }
    revertSameBin();
    if (currentLocations()->empty()) {
        decrBinId();    
    }
}

void GlassCutter::saveBest(std::string name) {
    if (VERBOSE) std::cout << "Sauvegarde de l'instance " << name << std::endl;
    std::ofstream outputFile;
    outputFile.open(name.c_str());
    outputFile << "PLATE ID;NODE ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT" << std::endl;

    unsigned int lastBinId = 0;
    for (unsigned int binId=0; binId < locations.size(); binId++){
        if (locations[binId].empty()) break;
        lastBinId++;
    }
    int maxSonId = -1;
    bool notLazy = false;
    for (unsigned int binId=0; binId < lastBinId; binId++){
        nodes[binId].reset();
        nodes[binId].buildNodeAndReturnNbItemsCuts(0, locations[binId].size(), notLazy);
        maxSonId = nodes[binId].saveNode(outputFile, maxSonId + 1, -1, binId == lastBinId - 1);
    }

    std::cout << "Sauvegarde effectuée avec succès " << std::endl;
}

void GlassCutter::addErrorStatistic(std::string errorMessage) {
    if (errorMessage == "Trimming failed (more than 1 item)" 
        || errorMessage == "Trimming failed (more than 1 cut)"){
        nbTrimmingFailed++;
        return;
    }

    if (errorMessage == "Tree too depth") {
        nbTreeTooDepth++;
        return;
    }
    
    if (errorMessage == "Waste too small") {
        nbWasteTooSmall++;
        /*std::cout << "======" << std::endl;
        displayLocations();*/
        return;
    }

    if (errorMessage == "Trimming failed (prechecked)") {
        nbTrimmingPreChecked++;
        return;
    }

    //8std::cout << errorMessage << std::endl;
}

void GlassCutter::resetErrorsStatistics() {
    nbTrimmingFailed = 0;
    nbTreeTooDepth = 0;
    nbWasteTooSmall = 0;
    nbTrimmingPreChecked = 0;
}

void GlassCutter::displayStatistics() const {
    std::cout << "Attempts: " << nbAttempts;
    std::cout << "\tNb infeasible: " << nbInfeasible;
    std::cout << "\tNb Rollbacks: " << nbRollbacks;
    std::cout << std::endl;
}

double GlassCutter::getSurfacePlateOccupation(unsigned int plateIndex) {
    return nodes[plateIndex].getSurfaceOccupation();
}

unsigned int GlassCutter::getXMax() {
    if (currentMonster()->getXMax() > currentNode()->getXMax()) { // TODO warning, c'est bien là pour une raison
        std::cout << (*currentMonster()) << std::endl;
        currentNode()->displayNode();
         for (const GlassLocation& location: locations[7]) 
            std::cout << location << std::endl;
    }
    assert(currentMonster()->getXMax() <= currentNode()->getXMax());
    return currentNode()->getXMax();
}

unsigned int GlassCutter::getLazyXMax() {
    return currentMonster()->getXMax();
}

void GlassCutter::displayStacks() const {
    for (const GlassStack& stack: stacks) 
        std::cout << stack;
}

void GlassCutter::displayLocations() const {
    std::cout << "==================" << std::endl;
    for (const std::vector<GlassLocation> locationsIt: locations) {
        for (const GlassLocation& location: locationsIt) 
            std::cout << location << std::endl;
    }
    std::cout << "==================" << std::endl;
}

void GlassCutter::displayErrorStatistics() const {
    std::cout << "Waste too small: " << nbWasteTooSmall << std::endl;
    std::cout << "Tree too depth: " << nbTreeTooDepth << std::endl;
    std::cout << "NbTrimmingFailed: " << nbTrimmingFailed << std::endl;
    std::cout << "NbTrimmingPreChecked: " << nbTrimmingPreChecked << std::endl;
}

bool GlassCutter::isLessGood() {
    return currentBinId * WIDTH_PLATES + getLazyXMax() > xLimit; 
}

void GlassCutter::ScoredLocationTree::sort() {
    std::sort(sons.begin(), sons.end(), compareTrees);
}

void GlassCutter::ScoredLocationTree::display() const {
    display("");
}

void GlassCutter::ScoredLocationTree::display(std::string prefix) const {
    std::cout << prefix;
    if (scoredLocation.location.getInstance() == NULL)
        std::cout << "(null)";
    else 
        std::cout << scoredLocation.location;

    std::cout << " | " << scoredLocation.score << " " << scoredLocation.feasible<< std::endl;

    for (ScoredLocationTree* son: sons) {
        son->display(prefix + " ");
    }
}

void GlassCutter::ScoredLocationTree::reset() {
    for (ScoredLocationTree* son: sons) {
        delete son;
    }
    sons.clear();
}

void GlassCutter::setBinId(unsigned int binId) {
    currentBinId = binId;
}

GlassCutter::ScoredLocationTree::~ScoredLocationTree() {

    for (ScoredLocationTree* son: sons) {
        delete son; 
    }
}

GlassCutter::~GlassCutter() {
    delete scoredTree;
}

// TODO on peut ignorer les premières plaques dans la boucle
void GlassCutter::commitFirstIndexInEachPlate() {
    unsigned int nbItemsFromPlate0 = 0;
    for (unsigned int index = 0; index < currentBinId; index++) {
        //std::cout << nbItemsFromPlate0 << std::endl;
        firstIndexInEachPlate[index] = nbItemsFromPlate0;
        nbItemsFromPlate0 += locations[index].size();
        //std::cout << nbItemsFromPlate0 <<  " VS " << firstIndexInEachPlate[index] << std::endl;
    }

    if (currentLocations()->empty()) { // On a zappé
        assert(firstIndexInEachPlate[currentBinId] == nbItemsFromPlate0);
    
    } else { // On a pas zappé
        firstIndexInEachPlate[currentBinId] = nbItemsFromPlate0;
        for (unsigned int index = currentBinId + 1; index < NB_PLATES; index++) {
            firstIndexInEachPlate[index] = -1;
        }
    }
}