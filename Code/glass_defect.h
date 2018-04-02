#pragma once

/////GlassDefect/////

/* This class is created
 * to model a defect instance with its all attributes
**/

#include <iostream>

class GlassDefect
{
    public:
        GlassDefect();
        GlassDefect(int id, int pid, int x, int y, int w, int h);
        virtual ~GlassDefect();
        int Getdefect_id() const { return defect_id; }
        int Getplate_id() const { return plate_id; }
        int Getpos_x() const { return pos_x; }
        int Getpos_xSup() const { return pos_x + width; } // x max (attention, non inclus)
        int Getpos_y() const { return pos_y; }
        int Getpos_ySup() const { return pos_y + height; }
        int Getwidth() const { return width; }
        int Getheight() const { return height; }

        void Setdefect_id(int val) { defect_id = val; }
        void Setplate_id(int val) { plate_id = val; }
        void Setpos_x(int val) { pos_x = val; }
        void Setpos_y(int val) { pos_y = val; }
        void Setwidth(int val) { width = val; }
        void Setheight(int val) { height = val; }
    protected:
    private:
        int defect_id; // Defect's Id.
        int plate_id; // Defect's plate Id
        int pos_x; // x position.
        int pos_y; // y position.
        int width; // Defect's width.
        int height; // Defect's height.
};

std::ostream& operator<<(std::ostream& os, const GlassDefect& defect);