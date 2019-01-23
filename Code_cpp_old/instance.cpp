#include "instance.h"

#include <iostream>
#include <fstream>


Instance::Instance(std::string instance_name): m_instance_name(instance_name){
    std::cout << "Chargement de l'instance : " << instance_name << " - en cours" << std::endl;
    m_instance_name = instance_name;
    area = 0;
    build_items();
    build_plates();    
    std::cout << "Chargement effectué " << std::endl;
}

void Instance::build_items(){
    std::cout << "\tChargement des items " << std::endl;
    std::ifstream batch_file (PATH + m_instance_name + "_batch.csv");
    area = 0;
    items.clear();
    if (batch_file.is_open()) {
        std::string line;
        getline(batch_file, line);
        while (getline(batch_file, line)) {
            GlassItem item(line);
            items.push_back(item);
            area += item.get_area();
        }        
        batch_file.close();
    } else {
        std::cerr << "Le fichier de batch n'a pas pu être trouvé " << std::endl;
    }
}

void Instance::build_plates(){
    std:: cout << "\tChargement des plaques (défauts) " << std::endl;

    std::ifstream defects_file (PATH + m_instance_name + "_defects.csv");
    glass_plates = std::vector<GlassPlate>();
    glass_plates.resize(nbPlates);

    for (int i = 0; i < nbPlates; ++i){
        glass_plates[i].set_plate_id(i);
    }

    if (defects_file.is_open()) {
        std::string line;
        getline(defects_file, line);
        while (getline(defects_file, line)) {
            GlassDefect defect(line);
            glass_plates[defect.get_plate_id()].add_defect(defect); 
        }
        defects_file.close();
    }
}

