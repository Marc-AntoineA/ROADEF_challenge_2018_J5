#include "glassInstance.h"

#include "glassConstants.h"

#include <iostream>
#include <fstream>
#include <string>

GlassInstance::GlassInstance(std::string instancePath): instancePath(instancePath) {
    if (VERBOSE) std::cout << "Chargement de l'instance : " << instancePath << " en cours..." << std::endl;
    buildPlates();
    buildItems();
    computeItemsArea();
    if (VERBOSE) std::cout << "\t... Chargement de l'instance effectué." << std::endl;
}

void GlassInstance::buildItems() {
    if (VERBOSE) std::cout << "Chargement des items en cours..." << std::endl;
    std::string filename = instancePath + "_batch.csv";
    std::ifstream batchFile(filename.c_str());
    items.clear();
    if (!batchFile.is_open()) return;
    std::string line;
    std::getline(batchFile, line); // skip header
    while (std::getline(batchFile, line))
        items.push_back(GlassItem(line));
    batchFile.close();
    if (VERBOSE) std::cout << "\t... Chargement des items effectué." << std::endl;
}

void GlassInstance::computeItemsArea() {
    itemsArea = 0;
    for (const GlassItem& item: items)
        itemsArea += item.getArea();
}

void GlassInstance::initPlates() {
    plates.resize(NB_PLATES);
    for (unsigned int plateId = 0; plateId < plates.size(); plateId++) 
        plates[plateId].setPlateId(plateId);
}

void GlassInstance::buildPlates() {
    if (VERBOSE) std::cout << "Chargement des plaques (et des défauts) en cours..." << std::endl;
    initPlates();
    std::string filename = instancePath + "_defects.csv";
    std::ifstream defectsFile (filename.c_str());
    if (!defectsFile.is_open()) return;
    std::string line;
    std::getline(defectsFile, line); // skip header
    while (getline(defectsFile, line)){
        GlassDefect defect(line);
        plates[defect.getPlateId()].addDefect(defect);
    }
    defectsFile.close();
    for (unsigned int plateIndex = 0; plateIndex < NB_PLATES; plateIndex++){
        plates[plateIndex].sortDefects();
    }
    if (VERBOSE) std::cout << "\t... Chargement des plaques effectué" << std::endl;
}