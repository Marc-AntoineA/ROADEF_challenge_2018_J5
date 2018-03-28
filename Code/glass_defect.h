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
        GlassDefect(unsigned int id, unsigned int pid, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        virtual ~GlassDefect();
        unsigned int Getdefect_id() const { return defect_id; }
        unsigned int Getplate_id() const { return plate_id; }
        unsigned int Getpos_x() const { return pos_x; }
        unsigned int Getpos_xSup() const { return pos_x + width; } // x max (attention, non inclus)
        unsigned int Getpos_y() const { return pos_y; }
        unsigned int Getpos_ySup() const { return pos_y + height; }
        unsigned int Getwidth() const { return width; }
        unsigned int Getheight() const { return height; }

        void Setdefect_id(unsigned int val) { defect_id = val; }
        void Setplate_id(unsigned int val) { plate_id = val; }
        void Setpos_x(unsigned int val) { pos_x = val; }
        void Setpos_y(unsigned int val) { pos_y = val; }
        void Setwidth(unsigned int val) { width = val; }
        void Setheight(unsigned int val) { height = val; }
    protected:
    private:
        unsigned int defect_id; // Defect's Id.
        unsigned int plate_id; // Defect's plate Id
        unsigned int pos_x; // x position.
        unsigned int pos_y; // y position.
        unsigned int width; // Defect's width.
        unsigned int height; // Defect's height.
};

std::ostream& operator<<(std::ostream& os, const GlassDefect& defect);