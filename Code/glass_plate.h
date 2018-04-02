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
        int Getplate_id() const { return plate_id; }
        int Getnode_id() const { return node_id; }
        int Getpos_x() const { return pos_x; }
        int Getpos_y() const { return pos_y; }
        int Getwidth() const { return width; }
        int Getheight() const { return height; }
        int Gettype() const { return type; }
        int Getcut() const { return cut; }
        int Getnodes_nbr() const { return nodes_nbr; }
        int Getwaste() const { return waste; }
        int Getuseful() const { return useful; }
        GlassNode Getsuccessor(int index) const { return successors[index]; }
        GlassDefect Getdefect(int index) const { return defects[index]; }
        std::vector<GlassDefect>* Getdefects() { return &defects; }
        int Getsuccessor_nbr() const { return successor_nbr; }
        int Getdefect_nbr() const { return defect_nbr; }
        GlassNode Getresidual() const { return residual; }

        void Setplate_id(int val) { plate_id = val; }
        void Setnode_id(int val) { node_id = val; }
        void Setpos_x(int val) { pos_x = val; }
        void Setpos_y(int val) { pos_y = val; }
        void Setwidth(int w) { width = w; }
        void Setheight(int h) { height = h; }
        void Settype(int val) { type = val; }
        void Setcut(int val) { cut = val; }
        void Setnodes_nbr(int val) { nodes_nbr = val; }
        void Setwaste(int val) { waste = val; }
        void Setuseful(int val) { useful = val; }
        void Setsuccessor(GlassNode c) { successors[successor_nbr] = c; successor_nbr++; }
        void Setdefect(GlassDefect c) { defects.push_back(c); defect_nbr++; }
        void Setresidual(GlassNode r) { residual = r; }

    public:
        std::vector<GlassNode> successors; // Successors list.
        GlassNode residual; // Residual node (only if last used plate).
        std::vector<GlassDefect> defects; // Defects list of plate instance.
    protected:
    private:
        int plate_id; // Plate Id.
        int node_id; // Node Id.
        int pos_x; // x position.
        int pos_y; // y position.
        int width; // Plate's width.
        int height; // Plate's height.
        int type;   /* node type
                     * > 0 glass piece index in batch file
                     * = -1 wasted glass
                     * = -2 branch
                     * = -3 residual
                     * Note: since this is plate class, plate's instance type should be -2 (branch).
                     */
        int cut; // Cut stage (Note: since this is plate class, plate's instance cut level should be 0-cut).
        int nodes_nbr; // Number of all nodes on a plate instance.
        int waste; // Sum of Wasted area.
        int useful; // Sum of Used area.
        int successor_nbr; // Plate's successor nodes number (children).
        int defect_nbr; // Defects number.
};

std::ostream& operator<<(std::ostream& os, const GlassPlate& plate);