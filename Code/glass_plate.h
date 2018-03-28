#pragma once

#include "glass_node.h"
#include "glass_defect.h"

#include <vector>
#include <iostream>
/////GlassPlate/////

/* This class is created
 * to model a solution used plate instance with its all attributes
**/

class GlassPlate
{
    public:
        GlassPlate();
        virtual ~GlassPlate();
        unsigned int Getplate_id() const { return plate_id; }
        unsigned int Getnode_id() const { return node_id; }
        unsigned int Getpos_x() const { return pos_x; }
        unsigned int Getpos_y() const { return pos_y; }
        unsigned int Getwidth() const { return width; }
        unsigned int Getheight() const { return height; }
        int Gettype() const { return type; }
        unsigned int Getcut() const { return cut; }
        unsigned int Getnodes_nbr() const { return nodes_nbr; }
        unsigned int Getwaste() const { return waste; }
        unsigned int Getuseful() const { return useful; }
        GlassNode Getsuccessor(unsigned int index) const { return successors[index]; }
        GlassDefect Getdefect(unsigned int index) const { return defects[index]; }
        unsigned int Getsuccessor_nbr() const { return successor_nbr; }
        unsigned int Getdefect_nbr() const { return defect_nbr; }
        GlassNode Getresidual() const { return residual; }

        void Setplate_id(unsigned int val) { plate_id = val; }
        void Setnode_id(unsigned int val) { node_id = val; }
        void Setpos_x(unsigned int val) { pos_x = val; }
        void Setpos_y(unsigned int val) { pos_y = val; }
        void Setwidth(unsigned int w) { width = w; }
        void Setheight(unsigned int h) { height = h; }
        void Settype(int val) { type = val; }
        void Setcut(unsigned int val) { cut = val; }
        void Setnodes_nbr(unsigned int val) { nodes_nbr = val; }
        void Setwaste(unsigned int val) { waste = val; }
        void Setuseful(unsigned int val) { useful = val; }
        void Setsuccessor(GlassNode c) { successors[successor_nbr] = c; successor_nbr++; }
        void Setdefect(GlassDefect c) { defects.push_back(c); defect_nbr++; }
        void Setresidual(GlassNode r) { residual = r; }

    public:
        std::vector<GlassNode> successors; // Successors list.
        GlassNode residual; // Residual node (only if last used plate).
        std::vector<GlassDefect> defects; // Defects list of plate instance.
    protected:
    private:
        unsigned int plate_id; // Plate Id.
        unsigned int node_id; // Node Id.
        unsigned int pos_x; // x position.
        unsigned int pos_y; // y position.
        unsigned int width; // Plate's width.
        unsigned int height; // Plate's height.
        int type;   /* node type
                     * > 0 glass piece index in batch file
                     * = -1 wasted glass
                     * = -2 branch
                     * = -3 residual
                     * Note: since this is plate class, plate's instance type should be -2 (branch).
                     */
        unsigned int cut; // Cut stage (Note: since this is plate class, plate's instance cut level should be 0-cut).
        unsigned int nodes_nbr; // Number of all nodes on a plate instance.
        unsigned int waste; // Sum of Wasted area.
        unsigned int useful; // Sum of Used area.
        unsigned int successor_nbr; // Plate's successor nodes number (children).
        unsigned int defect_nbr; // Defects number.
};

std::ostream& operator<<(std::ostream& os, const GlassPlate& plate);