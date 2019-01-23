#include "glass_node.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <cassert>

#include "instance.h"

GlassNode::GlassNode(GlassPlate* plate): m_plate(plate) {
    m_x = 0;
    m_y = 0;
    m_w = widthPlates;
    m_h = heightPlates;
    m_depth = 0;
    m_type = BRANCH;
    sons = std::vector<GlassNode>();
}

void GlassNode::set_plate(GlassPlate* plate) {
   m_plate = plate;
}

int GlassNode::check_node(std::vector<Location>::iterator first, std::vector<Location>::iterator last){
    sons.clear();
    if (m_w < 20 || m_h < 20) {
        //std::cout << *this << std::endl;
        throw std::runtime_error("Waste too small");
    }

    //std::cout << first->get_item()->get_id() << " <> " << (last - 1)->get_item()->get_id() << std::endl;
    // WASTE area
    if (first  == last) {
        m_type = WASTE;
        return 0;
    }

    // An item area
    if (first + 1 == last){
        //std::cout << m_x << ", " << m_y << ", " << ", " << m_w  + m_x << ", " << m_h + m_y << ", " << m_type << std::endl;
        if (m_x == first->get_x() && m_y == first->get_y()
            && m_w == first->get_w() && m_h == first->get_h()){
                m_type = first->get_item()->get_id();
                return 1;
            }
    }

    if (m_depth > 3){
        throw std::runtime_error("Tree too depth");
    }
    // TODO : intégrer ça à l'objet ?
    int idx = 0;
    std::vector<GlassCut> cuts_available;
    for (auto location_it = first; location_it != last; location_it++){
        if (m_depth % 2 == 0) {
            cuts_available.push_back(GlassCut(location_it->get_x(), idx, true, true));
            cuts_available.push_back(GlassCut(location_it->get_xw(), idx, false, true));
        } else {
            cuts_available.push_back(GlassCut(location_it->get_y(), idx, true, false));
            cuts_available.push_back(GlassCut(location_it->get_yh(), idx, false, false));
        }
        idx++;
    }

    std::sort(cuts_available.rbegin(), cuts_available.rend());
    unsigned int opened = 0;
    int nb_items = std::distance(first, last); // Nombre d'items vus
    int max_item = nb_items; // Plus grande valeur d'un item vu.

    int x_prev = m_x + m_w; // Pour vérifier les contraintes de 1-cut et de 2-cuts
    int nb_prev = nb_items; // idem
    std::vector<RealCut> real_cuts;
    for (GlassCut cut: cuts_available){
        if (cut.get_begin()){
            opened--;
        }

        //std::cout << opened << " " << nb_items << " " << nb_prev << " " << max_item << "--" << is_free_of_defects(cut) << std::endl;
        if (cut.get_x() - x_prev >= 20 // Toutes les découpes, waste ou non doivent être suffisamment larges
            && opened == 0 // interdiction de couper des items
            && max_item == nb_items
            && is_free_of_defects(cut)) { // l'ordre des "locations" doit être respecté.
            // 1-cut
            //std::cout << "OK" << std::endl;
            if (m_depth == 0) {
                // TODO large ou strict ?
                //std::cout << x_prev << " &&_" << cut.get_x() << std::endl;
                if (nb_prev - nb_items > 0 // Ce n'est pas une découpe 'waste'
                    && x_prev - cut.get_x() > minXX
                    && x_prev - cut.get_x() < maxXX ) {

                    real_cuts.push_back(RealCut(cut.get_x(), nb_prev - nb_items));
                    break;
                    x_prev = cut.get_x();
                    nb_prev = nb_items;
                // Si zone sans items (=> waste)
                } else if (nb_items == nb_prev){
                    //std::cout << "ooook" << std::endl;
                    real_cuts.push_back(RealCut(cut.get_x(), nb_prev - nb_items));
                    x_prev = cut.get_x();
                }
            } else if (m_depth == 1) { // 2-cut
                if (nb_prev - nb_items > 0 && x_prev - cut.get_x() > minYY) {
                    real_cuts.push_back(RealCut(cut.get_x(), nb_prev - nb_items));
                    break;
                } else if (nb_items == nb_prev){
                    real_cuts.push_back(RealCut(cut.get_x(), 0));
                    x_prev = cut.get_x();
                }
            } else if (m_depth == 3) { // 3-cut, contrôle du trimming
                if (nb_prev - nb_items > 1 || real_cuts.size() >= 1){
                    throw std::runtime_error("Trimming failed (check node)");
                }else {
                    real_cuts.push_back(RealCut(cut.get_x(), nb_prev - nb_items));
                    x_prev = cut.get_x();
                }
            } else { // Les autres cuts à la con
                real_cuts.push_back(RealCut(cut.get_x(), nb_prev - nb_items));
                if (nb_items != nb_prev) break;
                x_prev = cut.get_x();
            }
        }
        if (!cut.get_begin()){
            //std::cout << cut.get_item() << std::endl;
            opened++;
            max_item = std::min(max_item, (int) cut.get_item());
            //std::cout << max_item << std::endl;
            nb_items--;
        }
    }
    //std::cout << "--->" << real_cuts.size() << std::endl;
    /*for (RealCut cut: real_cuts){
        std::cout << "\t" << cut.x << " - " << cut.nb_items << std::endl;
    }*/

    int nb_cuts_items = 0;
    if (m_depth % 2 == 0){
        unsigned int x_prev = m_x + m_w;
        unsigned int nb_prev = std::distance(first, last);
        for (RealCut cut: real_cuts) {
            if (cut.x != x_prev){
                //std::cout << nb_prev << std::endl;
                //std::cout << cut.x << " -O- " << x_prev << " & " << cut.nb_items << std::endl;
                GlassNode glassNode(cut.x, m_y, x_prev - cut.x, m_h, BRANCH, m_depth + 1, m_plate);
                nb_cuts_items += glassNode.check_node(first + nb_prev - cut.nb_items, first + nb_prev);
                sons.push_back(glassNode);
            }
            x_prev = cut.x;
        }
        if (x_prev != 0){
            GlassNode glassNode(0, m_y, x_prev, m_h, BRANCH, m_depth + 1, m_plate);
            nb_cuts_items += glassNode.check_node(first, first);
            sons.push_back(glassNode);
        }
    } else {
        unsigned int x_prev = m_y + m_h;
        unsigned int nb_prev = std::distance(first, last);
        for (RealCut cut: real_cuts) {
            if (cut.x != x_prev){
                GlassNode glassNode(m_x, cut.x, m_w, x_prev - cut.x, BRANCH, m_depth + 1, m_plate);
                nb_cuts_items += glassNode.check_node(first + nb_prev - cut.nb_items, first + nb_prev);
                sons.push_back(glassNode);
            }
            x_prev = cut.x;
        }
        if (x_prev != 0){
            GlassNode glassNode(m_x, 0, m_w, x_prev, BRANCH, m_depth + 1, m_plate);
            nb_cuts_items += glassNode.check_node(first, first);
            sons.push_back(glassNode);
        }
    }

    if(nb_prev - nb_items != nb_cuts_items) throw std::runtime_error("Unfeasible cut");
    return nb_cuts_items;
}

int GlassNode::build_node(std::vector<Location>::iterator first,
    std::vector<Location>::iterator last) {
    sons.clear();
    if (m_w < 20 || m_h < 20) {
        //std::cout << *this << std::endl;
        throw std::runtime_error("Waste too small");
    }
    //std::cout << first->get_item()->get_id() << " <> " << (last - 1)->get_item()->get_id() << std::endl;
    // WASTE area
    if (first  == last) {
        m_type = WASTE;
        return 0;
    }

    // An item area
    if (first + 1 == last){
        //std::cout << m_x << ", " << m_y << ", " << ", " << m_w  + m_x << ", " << m_h + m_y << ", " << m_type << std::endl;
        if (m_x == first->get_x() && m_y == first->get_y()
            && m_w == first->get_w() && m_h == first->get_h()){
                m_type = first->get_item()->get_id();
                return 1;
            }
    }

    if (m_depth > 3){
        throw std::runtime_error("Tree too depth");
    }

    // TODO : intégrer ça à l'objet ?
    int idx = 0;
    std::vector<GlassCut> cuts_available;
    for (auto location_it = first; location_it != last; location_it++){

        if (m_depth % 2 == 0) {
            cuts_available.push_back(GlassCut(location_it->get_x(), idx, true, true));
            cuts_available.push_back(GlassCut(location_it->get_xw(), idx, false, true));
        } else {
            cuts_available.push_back(GlassCut(location_it->get_y(), idx, true, false));
            cuts_available.push_back(GlassCut(location_it->get_yh(), idx, false, false));
        }
        idx++;
    }

    std::sort(cuts_available.begin(), cuts_available.end());
    /*for (GlassCut cut: cuts_available){
        std::cout << cut.get_x() << " & ";
    }
    std::cout << std::endl;*/

    // Parcours de tous les cuts de gauche à droite ou de haut en bas
    // pour savoir lesquels sont faisables et lesquels ne le sont pas
    unsigned int opened = 0; // Nombre d'items 'ouverts', c'est à dire sur lesquels on ne peut pas couper
    // Les coupes ne peuvent pas couper des séquences d'items
    int max_item = -1; // Plus grande valeur d'un item vu.
    int nb_items = 0; // Nombre d'items vus

    int x_prev = m_x; // Pour vérifier les contraintes de 1-cut et de 2-cuts
    int nb_prev = 0; // idem
    std::vector<RealCut> real_cuts;
    for (GlassCut cut: cuts_available){
        if (!cut.get_begin()){
            opened--;
        }
        //std::cout << opened << " " << nb_items << " " << nb_prev << " " << max_item << "--" << is_free_of_defects(cut) << std::endl;
        if (cut.get_x() - x_prev >= 20 // Toutes les découpes, waste ou non doivent être suffisamment large
            && opened == 0 // interdiction de couper des items
            && max_item + 1 == nb_items
            && is_free_of_defects(cut)) { // l'ordre des "locations" doit être respecté.
            // 1-cut
            if (m_depth == 0) {
                // TODO large ou strict ?
                if (nb_items - nb_prev > 0 // Ce n'est pas une découpe 'waste'
                    && cut.get_x() - x_prev > minXX
                    && cut.get_x() - x_prev < maxXX ) {
                    real_cuts.push_back(RealCut(cut.get_x(), nb_items));
                    x_prev = cut.get_x();
                    nb_prev = nb_items;
                // Si zone sans items (=> waste)
                } else if (nb_items == nb_prev){
                    real_cuts.push_back(RealCut(cut.get_x(), nb_items));
                    x_prev = cut.get_x();
                }
            } else if (m_depth == 1) { // 2-cut
                if (nb_items - nb_prev > 0 && cut.get_x() - x_prev > minYY) {
                    real_cuts.push_back(RealCut(cut.get_x(), nb_items));
                    x_prev = cut.get_x();
                    nb_prev = nb_items;
                } else if (nb_items == nb_prev){
                    real_cuts.push_back(RealCut(cut.get_x(), nb_items));
                    x_prev = cut.get_x();
                }
            } else if (m_depth == 3) { // 3-cut, contrôle du trimming
                if (nb_items - nb_prev > 1){
                    throw std::runtime_error("trimming failed (nb_items)");
                } else if (real_cuts.size() >= 1){
                    throw std::runtime_error("Trimming failed (nb_cuts)");
                }else {
                    real_cuts.push_back(RealCut(cut.get_x(), nb_items));
                    x_prev = cut.get_x();
                    nb_prev = nb_items;
                }
            } else { // Les autres cuts à la con
                real_cuts.push_back(RealCut(cut.get_x(), nb_items));
                x_prev = cut.get_x();
                nb_prev = nb_items;
            }
        }
        if (cut.get_begin()){
            opened++;
            
            max_item = std::max(max_item, (int) cut.get_item());
            nb_items++;
            // TODO potentiel bug, il y a une ligne en plus à cet endroit là  dans le code Python
        }
    }
    /*std::cout << "--->" << std::endl;
    for (RealCut cut: real_cuts){
        std::cout << "\t" << cut.x << " - " << cut.nb_items << std::endl;
    }*/
    int nb_cuts_items = 0;
    if (m_depth % 2 == 0){
        unsigned int x_prev = m_x;
        unsigned int nb_prev = 0;
        for (RealCut cut: real_cuts) {
            if (cut.x != x_prev){
                GlassNode glassNode(x_prev, m_y, cut.x - x_prev, m_h, BRANCH, m_depth + 1, m_plate);
                nb_cuts_items += glassNode.build_node(first + nb_prev, first + cut.nb_items);
                sons.push_back(glassNode);
            }
            x_prev = cut.x;
            nb_prev = cut.nb_items;
        }
        if (x_prev != m_x + m_w){
            GlassNode glassNode(x_prev, m_y, m_x + m_w - x_prev, m_h, BRANCH, m_depth + 1, m_plate);
            nb_cuts_items += glassNode.build_node(first, first);
            sons.push_back(glassNode);
        }
    } else {
        unsigned int x_prev = m_y;
        unsigned int nb_prev = 0;
        for (RealCut cut: real_cuts) {
            if (cut.x != x_prev){
                GlassNode glassNode(m_x, x_prev, m_w, cut.x - x_prev, BRANCH, m_depth + 1, m_plate);
                nb_cuts_items += glassNode.build_node(first + nb_prev, first + cut.nb_items);
                sons.push_back(glassNode);
            }
            x_prev = cut.x;
            nb_prev = cut.nb_items;
        }
        if (x_prev != m_y + m_h){
            GlassNode glassNode(m_x, x_prev, m_w, m_y + m_h - x_prev, BRANCH, m_depth + 1, m_plate);
            nb_cuts_items += glassNode.build_node(first, first);
            sons.push_back(glassNode);
        }
    }
    if (std::distance(first, last) != nb_cuts_items) throw std::runtime_error("Unfeasible cut");
    return nb_cuts_items;
}

bool GlassNode::is_free_of_defects(const GlassCut& cut) {
    if (cut.get_vertical())
        return m_plate->is_free_out_of_defects(cut.get_x(), m_y, m_h, cut.get_vertical());
    else
        return m_plate->is_free_out_of_defects(cut.get_x(), m_x, m_w, cut.get_vertical());
}

int GlassNode::save_node(std::ofstream& output_file, int node_id, int parent_id, int bin_id, bool last) {
    //std::cout << bin_id << ";" << m_x << ";" << m_y << ";" << m_w << ";" << m_h << ";";
    //std::cout << m_type << ";" << m_depth << std::endl;
    output_file << bin_id << ";" << node_id << ";";
    output_file << m_x << ";" << m_y << ";" << m_w << ";" << m_h << ";";
    output_file << m_type << ";" << m_depth << ";";
    if (parent_id < 0)
        output_file << "" << std::endl;
    else
        output_file << parent_id << std::endl;

    if (last)
        sons.back().set_type(RESIDUAL);
    int max_son_id = node_id;
    for (GlassNode son: sons) {
        max_son_id = son.save_node(output_file, max_son_id + 1, node_id, bin_id, false);
    }
    return max_son_id;
}

std::ostream& operator<<(std::ostream& os, GlassNode& node) {
    os << "(" << node.get_x() << ", " << node.get_y() << ", " << node.get_xw() << ", " << node.get_yh() << ")";
    os << " -t " << node.get_type() << " -d " << node.get_depth();
    if (node.get_sons()->empty()){
        os << ";"<< std::endl;
        return os;
    }

    os << " : " << std::endl;
    for (auto it = node.get_sons()->begin(); it != node.get_sons()->end(); it++){
        os << "\t" << node << std::endl;
    }
    return os;
}
