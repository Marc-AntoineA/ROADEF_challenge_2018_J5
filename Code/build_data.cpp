#include "build_data.h"

/* ==================================================================================== */
/*                          Tree structure Build functions                              */
/* ==================================================================================== */
/**
 * This function takes the node array given by the solution parser
 * and build a tree structure of plates and their successors
 * to build the first level of the tree structure.
 * node: input / all solution nodes as a GlassNode array.
**/
void buildDataStructure (GlassNode *node)
{
    unsigned int i, j, index = 0;
    int waste = 0.0, useful = 0.0;
    unsigned int c_nbr; // maintains plate[i] nodes number.
    sol_items = new GlassNode[useful_node];
    // Loop on plates list.
    for (i = 0; i < plates_nbr; i++)
    {
        useful = 0.0;
        waste = 0.0;
        c_nbr = plate[i].Getnodes_nbr ();
        // if first plate, start at index 0 in node array. if not, start read at index = plate[i-1].nodes_nbr.
        index += (i > 0) ? plate[i-1].Getnodes_nbr () : 0;
        // Loop on plate node list (not only its successors).
        for (j = 0; j < c_nbr; j++)
        {
            if (node[index+j].Getparent() == plate[i].Getnode_id())
            {
                switch (node[index+j].Gettype())
                {
                case -3: // residual.
                    if (node[index+j].Getplate_id() == (plates_nbr - 1))
                    {
                        if (j == c_nbr-1)
                        {
                            cout << "\tNode " << node[index+j].Getnode_id() << ": is a residual" << endl;
                            log_file << "\tNode " << node[index+j].Getnode_id() << ": is a residual" << endl;
                            plate[node[index+j].Getplate_id()].Setresidual (node[index+j]);
                        }
                        else
                        {
                            cout << "\tERROR -- Node " << node[index+j].Getnode_id() << ": is a residual (ERROR: Residual item should be the rightmost node of the last plate)" << endl;
                            log_file << "\tERROR -- Node " << node[index+j].Getnode_id() << ": is a residual (ERROR: Residual item should be the rightmost node of the last plate)" << endl;
                            constraint_error |= PARSE_ERROR_MASK;
                        }
                    }
                    else
                    {
                        cout << "\tERROR -- Node " << node[index+j].Getnode_id() << ": is a residual (ERROR: Residual item can be located only on the last plate)" << endl;
                        log_file << "\tERROR -- Node " << node[index+j].Getnode_id() << ": is a residual (ERROR: Residual item can be located only on the last plate)" << endl;
                        constraint_error |= PARSE_ERROR_MASK;
                    }
                    break;
                case -2: // branch.
                    checkSuccessors (node, sol_items, i, node[index+j], index + c_nbr, index+j+1);
                    break;
                case -1: // waste.
                    if ((node[index+j].Getplate_id() == (plates_nbr - 1)) && (j == c_nbr-1))
                    {
                        cout << "\tNode " << node[index+j].Getnode_id() << ": is a WASTE that should be considered as a RESIDUAL" << endl;
                        log_file << "\tNode " << node[index+j].Getnode_id() << ": is a WASTE that should be considered as a RESIDUAL" << endl;
                        residual_node++;
                        waste_node--;
                        plate[node[index+j].Getplate_id()].Setresidual (node[index+j]);
                    }
                    else
                    {
                        waste = (node[index+j].Getwidth() * node[index+j].Getheight());
                        waste += plate[i].Getwaste();
                        plate[i].Setwaste(waste);
                    }
                    break;
                default: // final piece.
                    sol_items[s_idx++] = node [index+j];
                    useful = (node[index+j].Getwidth() * node[index+j].Getheight());
                    useful += plate[i].Getuseful();
                    plate[i].Setuseful(useful);
                    break;
                }
                plate[i].Setsuccessor(node[index+j]);
            }
        }
    }
}


/**
 * This function, for each node of a plate, found its successors
 * recursively to build the rest of the tree.
 * node: input / node array.
 * sol_items: output / will contain solution items (final items)
 * plate_id: input / plate index
 * parent: in-out / the parent node looking for its successors
 * c_end: input / end index in node array
 * index: input / start index in node array
**/
void checkSuccessors (GlassNode *node, GlassNode *sol_items, int plate_id, GlassNode &parent, int c_end, int index)
{
    int i, nbr = 0;
    int waste = 0.0 , useful = 0.0;
    // Compute node successors number.
    for (i = index; i < c_end; i++)
        if (node[i].Getparent() == parent.Getnode_id())
            nbr++;
    // Allocate node successors array.
    parent.successor = new GlassNode[nbr];
    for (i = index; i < c_end; i++)
    {
        if (node[i].Getparent() == parent.Getnode_id())
        {
            // if Setsuccessor function call returned 1.
            if (parent.Setsuccessor(node[i]) == 1)
            {
                cout << "\tERROR -- Node: " << parent.Getnode_id() << " is " << parent.Getcut() << "-cut and has more than 2 successors" << endl;
                log_file << "\tERROR -- Node: " << parent.Getnode_id() << " is " << parent.Getcut() << "-cut and has more than 2 successors" << endl;
                constraint_error |= PARSE_ERROR_MASK;
            }
            switch (node[i].Gettype())
            {
            case -3: // residual.
                cout << "\tERROR -- Node " << node[i].Getnode_id() << ": is a residual (ERROR: residual must be 1-cut node)" << endl;
                log_file << "\tERROR -- Node " << node[i].Getnode_id() << ": is a residual (ERROR: residual must be 1-cut node)" << endl;
                constraint_error |= PARSE_ERROR_MASK;
                break;
            case -2: // branch.
                checkSuccessors (node, sol_items, plate_id, parent.successor[parent.Getsuccessor_nbr()-1], c_end, i+1);
                break;
            case -1: // waste.
                waste = (node[i].Getwidth() * node[i].Getheight());
                waste += plate[plate_id].Getwaste();
                plate[plate_id].Setwaste(waste);
                break;
            default: // final piece.
                sol_items[s_idx++] = node[i];
                useful = (node[i].Getwidth() * node[i].Getheight());
                useful += plate[plate_id].Getuseful();
                plate[plate_id].Setuseful(useful);
                break;
            }
        }
    }
}
