#pragma once

#include "global_var.h"
#include <iostream>

/////GlassNode/////

/* This class is created
 * to model a solution node instance with its all attributes
**/
class GlassNode
{
    public:
      GlassNode();
      GlassNode(int plate, int node, int x,
                int y, int w, int h,
                int t, int c, int p, int *c_array, int n);
      virtual ~GlassNode();
      int Getplate_id() const { return plate_id; }
      int Getnode_id() const { return node_id; }
      int Getpos_x() const { return pos_x; }
      int Getpos_y() const { return pos_y; }
      int Getwidth() const { return width; }
      int Getheight() const { return height; }
      int Gettype() const { return type; }
      int Getcut() const { return cut; }
      int Getparent() const { return parent; }
      int Getsuccessor_nbr() const { return successor_nbr; }
      GlassNode Getsuccessor(int idx) const { return successor[idx]; }

      void Setplate_id(int id) { plate_id = id; }
      void Setnode_id(int id) { node_id = id; }
      void Setpos_x(int x) { pos_x = x; }
      void Setpos_y(int y) { pos_y = y; }
      void Setwidth(int w) { width = w; }
      void Setheight(int h) { height = h; }
      void Settype(int t) { type = t; }
      void Setcut(int c) { cut = c; }
      void Setparent(int p) { parent = p; }
      void Setchild_nbr(int nbr) { successor_nbr = nbr; }
      bool Setsuccessor(GlassNode c) { if(successor_nbr >= 2 && cut > 2) { return 1; } successor[successor_nbr] = c; successor_nbr++; return 0; }

      GlassNode * successor; // Successor nodes list.

      string toString() const;

    private:
      int plate_id; // Plate Id.
      int node_id; // Node Id.
      int pos_x; // x position.
      int pos_y; // y position.
      int width; // Plate's width.
      int height; // Plate's height.
      int type;   /* node type
                   * > 0 glass piece index in batch file.
                   * = -1 wasted glass.
                   * = -2 branch.
                   * = -3 residual.
                   * = -4 undefined
                   */
      int cut; // cut level(there are 1, 2, 3 and 4-cut level, could not be 0-cut because GlassNode class instance can't be an entire plate).
      int parent; // parent node Id.
      int successor_nbr; // number of children for this node instance.
};

// TODO
std::ostream& operator<<(std::ostream& os, const GlassNode& node);

