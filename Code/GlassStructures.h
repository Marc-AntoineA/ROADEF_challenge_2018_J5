#ifndef GLASSSTRUCTURES_H
#define GLASSSTRUCTURES_H

/////GlassDefect/////

/* This class is created
 * to model a defect instance with its all attributes
**/
class GlassDefect
{
    public:
        GlassDefect();
        GlassDefect(unsigned int id, unsigned int pid, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        virtual ~GlassDefect();
        unsigned int Getdefect_id() { return defect_id; }
        unsigned int Getplate_id() { return plate_id; }
        unsigned int Getpos_x() { return pos_x; }
        unsigned int Getpos_y() { return pos_y; }
        unsigned int Getwidth() { return width; }
        unsigned int Getheight() { return height; }

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

/////GlassItem/////

/* This class is created
 * to model a batch item instance with its all attributes
**/
class GlassItem
{
    public:
        GlassItem();
        virtual ~GlassItem();
        int Getitem_id() { return item_id; }
        unsigned int Getitem_w() { return item_w; }
        unsigned int Getitem_h() { return item_h; }
        unsigned int Getitem_stack() { return item_stack; }
        unsigned int Getitem_seq() { return item_seq; }

        void Setitem_id(int val) { item_id = val; }
        void Setitem_w(unsigned int val) { item_w = val; }
        void Setitem_h(unsigned int val) { item_h = val; }
        void Setitem_stack(unsigned int val) { item_stack = val; }
        void Setitem_seq(unsigned int val) { item_seq = val; }

    protected:
    private:
        int item_id; // Item's Id.
        unsigned int item_w; // Item's width.
        unsigned int item_h; // Item's height.
        unsigned int item_stack; // Item's stack Id
        unsigned int item_seq; // Item's stack sequence.
};

/////GlassNode/////

/* This class is created
 * to model a solution node instance with its all attributes
**/
class GlassNode
{
    public:
      GlassNode ();
      GlassNode(unsigned int plate, unsigned int node, unsigned int x,
                unsigned int y, unsigned int w, unsigned int h,
                int t, unsigned int c, unsigned int p, unsigned int *c_array, unsigned int n);
      virtual ~GlassNode();
      unsigned int Getplate_id() { return plate_id; }
      unsigned int Getnode_id() { return node_id; }
      unsigned int Getpos_x() { return pos_x; }
      unsigned int Getpos_y() { return pos_y; }
      unsigned int Getwidth() { return width; }
      unsigned int Getheight() { return height; }
      int Gettype() { return type; }
      unsigned int Getcut() { return cut; }
      unsigned int Getparent() { return parent; }
      unsigned int Getsuccessor_nbr() { return successor_nbr; }
      GlassNode Getsuccessor (unsigned int idx) { return successor[idx]; }

      void Setplate_id (unsigned int id) { plate_id = id; }
      void Setnode_id (unsigned int id) { node_id = id; }
      void Setpos_x (unsigned int x) { pos_x = x; }
      void Setpos_y (unsigned int y) { pos_y = y; }
      void Setwidth (unsigned int w) { width = w; }
      void Setheight (unsigned int h) { height = h; }
      void Settype (int t) { type = t; }
      void Setcut (unsigned int c) { cut = c; }
      void Setparent (unsigned int p) { parent = p; }
      void Setchild_nbr (unsigned int nbr) { successor_nbr = nbr; }
      unsigned int Setsuccessor (GlassNode c) { if (successor_nbr >= 2 && cut > 2) { return 1; } successor[successor_nbr] = c; successor_nbr++; return 0; }

    public:
      GlassNode * successor; // Successor nodes list.
    protected:
    private:
      unsigned int plate_id; // Plate Id.
      unsigned int node_id; // Node Id.
      unsigned int pos_x; // x position.
      unsigned int pos_y; // y position.
      unsigned int width; // Plate's width.
      unsigned int height; // Plate's height.
      int type;   /* node type
                   * > 0 glass piece index in batch file.
                   * = -1 wasted glass.
                   * = -2 branch.
                   * = -3 residual.
                   */
      unsigned int cut; // cut level (there are 1, 2, 3 and 4-cut level, could not be 0-cut because GlassNode class instance can't be an entire plate).
      unsigned int parent; // parent node Id.
      unsigned int successor_nbr; // number of children for this node instance.
};

/////GlassPlate/////

/* This class is created
 * to model a solution used plate instance with its all attributes
**/
class GlassPlate
{
    public:
        GlassPlate();
        virtual ~GlassPlate();
        unsigned int Getplate_id() { return plate_id; }
        unsigned int Getnode_id() { return node_id; }
        unsigned int Getpos_x() { return pos_x; }
        unsigned int Getpos_y() { return pos_y; }
        unsigned int Getwidth() { return width; }
        unsigned int Getheight() { return height; }
        int Gettype() { return type; }
        unsigned int Getcut() { return cut; }
        unsigned int Getnodes_nbr () { return nodes_nbr; }
        unsigned int Getwaste() { return waste; }
        unsigned int Getuseful() { return useful; }
        GlassNode Getsuccessor (unsigned int index) { return successor[index]; }
        GlassDefect Getdefect (unsigned int index) { return defect[index]; }
        unsigned int Getsuccessor_nbr () { return successor_nbr; }
        unsigned int Getdefect_nbr () { return defect_nbr; }
        GlassNode Getresidual () { return residual; }

        void Setplate_id(unsigned int val) { plate_id = val; }
        void Setnode_id(unsigned int val) { node_id = val; }
        void Setpos_x(unsigned int val) { pos_x = val; }
        void Setpos_y(unsigned int val) { pos_y = val; }
        void Setwidth (unsigned int w) { width = w; }
        void Setheight (unsigned int h) { height = h; }
        void Settype(int val) { type = val; }
        void Setcut(unsigned int val) { cut = val; }
        void Setnodes_nbr(unsigned int val) { nodes_nbr = val; }
        void Setwaste(unsigned int val) { waste = val; }
        void Setuseful(unsigned int val) { useful = val; }
        void Setsuccessor (GlassNode c) { successor[successor_nbr] = c; successor_nbr++; }
        void Setdefect (GlassDefect c) { defect[defect_nbr] = c; defect_nbr++; }
        void Setresidual (GlassNode r) { residual = r; }
    public:
        GlassNode *successor; // Successors list.
        GlassNode residual; // Residual node (only if last used plate).
        GlassDefect *defect; // Defects list of plate instance.
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

/////GlassStack/////

/* This class is created
 * to model a batch stack instance with its all attributes
**/
class GlassStack
{
    public:
        GlassStack();
        virtual ~GlassStack();
        unsigned int Getstack_id() { return stack_id; }
        unsigned int Getitem_nbr() { return item_nbr; }
        unsigned int Getcur_item_idx() { return cur_item_idx; }
        GlassItem Getitem (unsigned int idx) { return item[idx]; }

        void Setstack_id(unsigned int val) { stack_id = val; }
        void Setitem_nbr(unsigned int val) { item_nbr = val; }
        void Setcur_item_idx(unsigned int val) { cur_item_idx = val; }
        void Setitem (GlassItem i) { item[idx] = i; idx++; }

        void AllocateItems () { item = new GlassItem[item_nbr]; }
        void Increaseitem_nbr () { item_nbr++; }
        void Increasecur_item_idx () { if (cur_item_idx >= item_nbr) {cur_item_idx = 0xffff;} else { cur_item_idx++; } }

    public:
        GlassItem *item; // Items list.
    protected:
    private:
        unsigned int stack_id; // Stack Id.
        unsigned int item_nbr;  // Stack's associated items number.
        unsigned int cur_item_idx; // Current items index of stack instance.
        unsigned int idx; // Used to add item on top of items list of stack instance.
};

#endif // GLASSSTRUCTURES_H
