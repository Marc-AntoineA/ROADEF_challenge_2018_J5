#include "checker.h"
/*
int checker()
{
    testPath = "";
    string batchPath, solutionPath, defectsPath, optParamsPath;
    int not_exit = 1, choice = 0;
    cout << endl << "\t------------------- Optimization Checker -------------------" << endl << endl;
    log_file << endl << "\t------------------- Optimization Checker -------------------" << endl;
    cout << "Looking in folder "<< PATH_TO_INSTANCES << endl;

    cout << "\tPlease enter your USED BATCH file Index: ";
    cin >> file_idx;
    batchPath =     PATH_TO_INSTANCES + file_idx + "_batch.csv";
    solutionPath =  PATH_TO_INSTANCES + file_idx + "_solution.csv";
    defectsPath =   PATH_TO_INSTANCES + file_idx + "_defects.csv";
    optParamsPath = PATH_TO_INSTANCES + string("global_param.csv");

    /// If csv files exist and files index is equal.
    string log_name = "logs/" + file_idx + "_log.txt";
    log_file.open(log_name.c_str());
    cout << endl << "\t\t----------- Start files parsing -----------" << endl;
    log_file << endl << "\t\t----------- Start files parsing -----------" << endl;
    parseOptimizationParams(optParamsPath);
    parseBatch(batchPath);
    parseSolution(solutionPath);
    parseDefects(defectsPath);
    /// If a needed file do not exist.
    if (!opened_file)
        return EXIT_FAILURE;

    cout <<     "\t\t----------- End of files parsing ----------" << endl;
    log_file << "\t\t----------- End of files parsing ----------" << endl;
    // Auto verify constraints and generate log file.
    verifyItemProduction();
    verifyDefects();
    verifyIdt_Sequence();
    verifyDimensions();
    displayPlatesAreaUsage();
    violatedConstraints();
    active_log = 0;
    log_file << endl    << "\t------------------------------------------------------------" << endl;
    if (constraint_error != 0)
        log_file        <<   "                       INVALID SOLUTION                     " << endl;
    else
        log_file        <<  "               SOLUTION VALIDATED SUCCESSFULLY               " << endl;
    log_file            << "\t------------------------------------------------------------" << endl;
    log_file.close ();
    statisticsLog ();
    cout << endl << endl << "\t\t------ Start constraints verification -----" << endl ;
    while (not_exit)
    {
        cout << endl << "\tPress to verify constraint: " << endl;
        cout << "\t1 - Items production" << endl;
        cout << "\t2 - Defects superposing" << endl;
        cout << "\t3 - Identity and Sequence" << endl;
        cout << "\t4 - Items dimensions" << endl;
        cout << "\t5 - All constraints (1-4)" << endl;
        cout << "\t6 - Display plates wasted & used surface (%)" << endl;
        cout << "\t0 - Exit" << endl;
        cout << "\tPressed key: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            verifyItemProduction ();
            break;
        case 2:
            verifyDefects ();
            break;
        case 3:
            verifyIdt_Sequence ();
            break;
        case 4:
            verifyDimensions ();
            break;
        case 6:
            displayPlatesAreaUsage ();
            break;
        case 5:
            verifyItemProduction ();
            verifyDefects ();
            verifyIdt_Sequence ();
            verifyDimensions ();
            violatedConstraints ();
            cout << endl    << "\t------------------------------------------------------------" << endl;
            if (constraint_error != 0)
                cout        <<   "                       INVALID SOLUTION                     " << endl;
            else
                cout        <<  "               SOLUTION VALIDATED SUCCESSFULLY               " << endl;
            cout            << "\t------------------------------------------------------------" << endl;
            break;
        case 0:
            not_exit = 0;
            break;
        default:
            cout << "\t\tInvalid key" << endl;
            break;
        }
    }
    cout << "\t\t------ End of constraints verification ----" << endl ;
    cout << endl << "\t------------------------------------------------------------" << endl;
    return EXIT_SUCCESS;
}

/* ==================================================================================== */
/*                       Constraint verification functions                              */
/* ==================================================================================== */

bool verifyItemProduction(const vector<GlassStack>& stacks, const vector<GlassNode>& solution){
    if (active_log)
        log_file << "\n\t--- Item production constraint verification ---" << endl;
    else
        cout << endl << "\t--- Item production constraint verification ---" << endl;
    int i, j, nbr = 0;

    // Loop on batch parsed items.
    for(int k = 0; k < stacks.size(); k++){
        for (i = 0; i < stacks[k].Getitem_nbr(); i++){
            GlassItem item = stacks[k].Getitem(i);
            nbr = 0;
            // For each batch item loop on solution parsed nodes.
            for (j = 0; j < useful_node; j++){
                // Test if batch item id is identical to solution node type.
                if (item.Getitem_id() == solution[j].Gettype())
                    nbr++;
            }
            // If batch item is found more than one time, the item is declared duplicated.
            if (nbr > 1){
                if (active_log){
                    log_file << "\tERROR -- Item " << item.Getitem_id() << ": is duplicated" << endl;
                    constraint_error |= ITEM_PRODUCTION_ERROR_MASK;
                }else{
                    cout << "\tERROR -- Item " << item.Getitem_id() << ": is duplicated" << endl;
                }
            }
        
            // If batch item is not found at all, the item is declared as not produced.
            if (nbr == 0){
                if (active_log){
                    log_file << "\tERROR -- Item " << item.Getitem_id() << ": is not produced" << endl;
                    constraint_error |= ITEM_PRODUCTION_ERROR_MASK;
                }else{
                    cout << "\tERROR -- Item " << item.Getitem_id() << ": is not produced" << endl;
                }
            }
        }

    }

    if (active_log){
        if (nbr == 1)
            log_file << "\tItem production constraint verified successfully" << endl;
        else
            log_file << "\tERROR -- Item production constraint violated" << endl;
    }else{
        if (nbr == 1)
            cout << "\tItem production constraint verified successfully" << endl;
        else
            cout << "\tERROR -- Item production constraint violated" << endl;
    }

    if (active_log)
        log_file <<     "\t--- End of Item production constraint verification ---" << endl;
    else
        cout <<         "\t--- End of Item production constraint verification ---" << endl;
    
    return (nbr == 1);
}



bool verifyDefects(const vector<GlassPlate>& plates, const vector<GlassNode>& solution){
    int error = 0;
    if (active_log)
        log_file << endl << "\t--- Superposing with defects constraint verification ---" << endl;
    else
        cout << endl << "\t--- Superposing with defects constraint verification ---" << endl;
    int i, j, plate_id;
    int defect_width, defect_height, defect_x, defect_y; // Defect Coord.
    int item_width, item_height, item_x, item_y; // Item Coord.
    // Loop on solution parsed nodes.
    cout << solution.size() << endl;
    for (i = 0; i < solution.size(); i++){

        // Get node coordinates.
        item_x = solution[i].Getpos_x();
        item_y = solution[i].Getpos_y();
        item_width = solution[i].Getwidth();
        item_height = solution[i].Getheight();
        plate_id = solution[i].Getplate_id();
        
        // Loop on associated plate defects.
        for (j = 0; j < plates[plate_id].Getdefect_nbr(); j++){
            // Get defect coordinates.
            defect_x = plates[solution[i].Getplate_id()].Getdefect(j).Getpos_x();
            defect_y = plates[solution[i].Getplate_id()].Getdefect(j).Getpos_y();
            defect_width = plates[solution[i].Getplate_id()].Getdefect(j).Getwidth();
            defect_height = plates[solution[i].Getplate_id()].Getdefect(j).Getheight();
            // Test if one or more defect ends are inside the solution useful node area.
            if ((((item_x <= defect_x) 
                        && (defect_x < item_x + item_width)) 
                    || ((item_x < defect_x + defect_width)
                        && (defect_x + defect_width < item_x + item_width)))
                && // horizontal ends
                  (((item_y <= defect_y) && (defect_y < item_y + item_height)) 
                  || ((item_y < defect_y + defect_height) 
                  && (defect_y + defect_height < item_y + item_height)))){ // vertical ends
                error = 1;
                if (active_log)
                {
                    log_file << "\tERROR -- Node " << solution[i].Getnode_id() << ": (type: item, item_id: " << solution [i].Gettype() << ", x: " << item_x << ", y: " << item_y << ", width: " << item_width << ", height: " << item_height
                         << ") superposes defect " << plates[solution[i].Getplate_id()].Getdefect(j).Getdefect_id() << ": (x: " << defect_x << ", y: " << defect_y << ", width: " << defect_width << ", height: " << defect_height << ")" << endl;
                    constraint_error |= DEFECTS_SUPERPOSING_ERROR_MASK;
                }
                else
                {
                    cout << "\tERROR -- Node " << solution[i].Getnode_id() << ": (type: item, item_id: " << solution [i].Gettype() << ", x: " << item_x << ", y: " << item_y << ", width: " << item_width << ", height: " << item_height
                         << ") superposes defect " << plates[solution[i].Getplate_id()].Getdefect(j).Getdefect_id() << ": (x: " << defect_x << ", y: " << defect_y << ", width: " << defect_width << ", height: " << defect_height << ")" << endl;
                }
            }
        }
    }
    if (active_log)
    {
        if (error == 0)
            log_file << "\tSuperposing with defects constraint verified successfully" << endl;
        log_file << "\t--- End of Superposing with defects constraint verification ---" << endl;
    }
    else
    {
        if (error == 0)
            cout << "\tSuperposing with defects constraint verified successfully" << endl;
        cout     << "\t--- End of Superposing with defects constraint verification ---" << endl;
    }

    return (error == 0);
}



bool verifyDimensions(const vector<GlassPlate>& plates, const vector<GlassNode>& solution){

    if (active_log)
        log_file << endl << "\t--- Dimensions constraint verification ---" << endl;
    else
        cout << endl << "\t--- Dimensions constraint verification ---" << endl;
    
    int i, j, height, width, x, y, cut, c_nbr;
    int type;
    //
    success = 1;
    // Loop on plates list.
    for (i = 0; i < PLATES_NBR_LIMIT; i++){
        // Get plate successors number.
        c_nbr = plates[i].Getsuccessor_nbr();
        for (j = 0; j < c_nbr; j++){
            // Get successor coordinates and type.
            height = plates[i].Getsuccessor(j).Getheight();
            width = plates[i].Getsuccessor(j).Getwidth();
            x = plates[i].Getsuccessor(j).Getpos_x();
            y = plates[i].Getsuccessor(j).Getpos_y();
            type = plates[i].Getsuccessor(j).Gettype();
            cut = plates[i].Getsuccessor(j).Getcut();
            // If node cut stage do not equal parent.cut + 1.
            if (cut != plates[i].Getcut()+1)
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.cut != plate.cut + 1 (plate.node_id: " << plates[i].Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.cut != plate.cut + 1 (plate.node_id: " << plates[i].Getnode_id() << ")" << endl;
                success = 0;
            }
            // If branch node type.
            if (type == -2)
            {
                // verifies width is not < min1Cut optimization parameter.
                if (width < min1Cut)
                {
                    if (active_log)
                        log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.width less than " << min1Cut << endl;
                    else
                        cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.width less than " << min1Cut << endl;
                    success = 0;
                }
                // verifies width is not > max1Cut optimization parameter.
                if (width > max1Cut)
                {
                    if (active_log)
                        log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.width greater than " << max1Cut << "mm" << endl;
                    else
                        cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.width greater than " << max1Cut << "mm" << endl;
                    success = 0;
                }
            }
            // verifies node.y parameter is equal or not to plate's y position.
            if (y != plates[i].Getpos_y())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": node.y is not equal to its plate " << plates[i].Getnode_id() << " y: " << plates[i].Getpos_y() << endl;
                else
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": node.y is not equal to its plate " << plates[i].Getnode_id() << " y: " << plates[i].Getpos_y() << endl;
                success = 0;
            }
            // verifies node.height parameter is equal or not to plate's height.
            if (height != plates[i].Getheight())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": node.height is not equal to its plate " << plates[i].Getnode_id() << " height: " << plates[i].Getheight() << endl;
                else
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": node.height is not equal to its plate " << plates[i].Getnode_id() << " height: " << plates[i].Getheight() << endl;
                success = 0;
            }
            // verifies if first node of plates[i] starts at coordinate node.x = 0.
            if ((j == 0) && (x != 0))
            {
                if (active_log)
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != plate.x (plate.node_id: " << plates[i].Getnode_id() << ", x: 0)" << endl;
                else
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != plate.x (plate.node_id: " << plates[i].Getnode_id() << ", x: 0)" << endl;
                success = 0;
            }
            // verifies if nodes from the same cut stage have node[i].x == node[i-1].x + node[i-1].width.
            if ( (j > 0) && (x != (plates[i].Getsuccessor(j-1).Getpos_x() + plates[i].Getsuccessor(j-1).Getwidth())))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != node[" << plates[i].Getsuccessor(j-1).Getnode_id() << "].x + node[" << plates[i].Getsuccessor(j-1).Getnode_id() << "].width" << endl;
                else
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != node[" << plates[i].Getsuccessor(j-1).Getnode_id() << "].x + node[" << plates[i].Getsuccessor(j-1).Getnode_id() << "].width" << endl;
                success = 0;
            }
            // verifies if node.width + node.x overflow plate's width.
            if (x+width > plates[i].Getwidth())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x + node.width > plate.width (plate.node_id: " << plates[i].Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x + node.width > plate.width (plate.node_id: " << plates[i].Getnode_id() << ")" << endl;
                success = 0;
            }
            // if node is last plate's successor, verify if node.width + node.x is less than plate's width.
            if ((j == c_nbr-1) && (x+width < plates[i].Getwidth()))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": is the last of plate " << plates[i].Getplate_id() << " successors and node.x + node.width < plate.width (plate.node_id: " << plates[i].Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": is the last of plate " << plates[i].Getplate_id() << " successors and node.x + node.width < plate.width (plate.node_id: " << plates[i].Getnode_id() << ")" << endl;
                success = 0;
            }
            // If node has waste type.
            if (type == -1)
            {
                // verifies if node.height is not inferior to waste_min optimization parameter prefixed value.
                if (height < waste_min)
                {
                    if (active_log)
                        log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.height less than " << waste_min << "mm" << endl;
                    else
                        cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.height less than " << waste_min << "mm" << endl;
                    success = 0;
                }
                // verifies if node.width is not inferior to waste_min optimization parameter prefixed value.
                if (width < waste_min)
                {
                    if (active_log)
                        log_file << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.width less than " << waste_min << "mm" << endl;
                    else
                        cout << "\tERROR -- Node " << plates[i].Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.width less than " << waste_min << "mm" << endl;
                    success = 0;
                }
            }
            // If node has residual type, verifies if node.width and node.height are not inferior to waste_min optimization parameter prefixed value.
            if ((i == PLATES_NBR_LIMIT-1) && (residual_node > 0))
            {
                if (plates[i].Getresidual().Getwidth() < waste_min)
                {
                    if (active_log)
                        log_file << "\tERROR -- Node " << plates[i].Getresidual().Getnode_id() << ": (type: residual, width: " << plates[i].Getresidual().Getwidth() << ", height: " << plates[i].Getresidual().Getheight() << ") has node.width less than " << waste_min << "mm" << endl;
                    else
                        cout << "\tERROR -- Node " << plates[i].Getresidual().Getnode_id() << ": (type: residual, width: " << plates[i].Getresidual().Getwidth() << ", height: " << plates[i].Getresidual().Getheight() << ") has node.width less than " << waste_min << "mm" << endl;
                    success = 0;
                }
            }
            // If node has branch type, look for its successors dimensions.
            if (type == -2)
                checkSuccDimensions (plates[i].Getsuccessor(j));
        }
    }
    if (success)
    {
        if (active_log)
        {
            log_file << "\tDimension constraint verified successfully"<< endl;
        }
        else
        {
            cout << "\tDimension constraint verified successfully"<< endl;
        }
    }
    else
        constraint_error |= DIMENSIONS_ERROR_MASK;
    if (active_log)
        log_file <<         "\t--- End of Dimension constraint verification ---" << endl;
    else
        cout <<         "\t--- End of Dimension constraint verification ---" << endl;

    return success;
}


void checkSuccDimensions(GlassNode parent){

    int j, height, width, x, y, cut, c_nbr;
    int type;
    // Get node successors number (children).
    c_nbr = parent.Getsuccessor_nbr();
    // Loop on all node successors
    for (j = 0; j < c_nbr; j++)
    {
        // Get successor coordinates.
        height = parent.Getsuccessor(j).Getheight();
        width = parent.Getsuccessor(j).Getwidth();
        x = parent.Getsuccessor(j).Getpos_x();
        y = parent.Getsuccessor(j).Getpos_y();
        cut = parent.Getsuccessor(j).Getcut();
        type = parent.Getsuccessor(j).Gettype();
        // If node cut is not equal to parent.cut + 1.
        if (cut != parent.Getcut()+1)
        {
            if (active_log)
                log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.cut != parent.cut + 1 (parent.node_id: " << parent.Getnode_id() << ")" << endl;
            else
                cout <<     "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.cut != parent.cut + 1 (parent.node_id: " << parent.Getnode_id() << ")" << endl;
            success = 0;
        }
        // If node has waste type and cut stage equals 2.
        if (type == -1)
        {
            // verifies if node.height is not inferior to waste_min optimization parameter prefixed value.
            if (height < waste_min)
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.height less than " << waste_min << "mm" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.height less than " << waste_min << "mm" << endl;
                success = 0;
            }
            // verifies if node.width is not inferior to waste_min optimization parameter prefixed value.
            if (width < waste_min)
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.width less than " << waste_min << "mm" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: waste, width: " << width << ", height: " << height << ") has node.width less than " << waste_min << "mm" << endl;
                success = 0;
            }
        }
        // verifies if node cut stage equals 2 and node.height is inferior to min2Cut optimization parameter prefixed value.
        if ((cut == 2) && (type == -2) && (height < min2Cut))
        {
            if (active_log)
                log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.height less than " << min2Cut << "mm" << endl;
            else
                cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (type: branch, width: " << width << ", height: " << height << ") has node.height less than " << min2Cut << "mm" << endl;
            success = 0;
        }
        // if odd cut stage.
        if (cut%2)
        {
            // verifies if node.y is different than it's parent.pos_y value.
            if (y != parent.Getpos_y())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.y is not equal to its parent node " << parent.Getnode_id() << " y: " << parent.Getpos_y() << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.y is not equal to its parent node " << parent.Getnode_id() << " y: " << parent.Getpos_y() << endl;
                success = 0;
            }
            // verifies if node.height is different than it's parent.height value.
            if (height != parent.Getheight())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.height is not equal to its parent node " << parent.Getnode_id() << " height: " << parent.Getheight() << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.height is not equal to its parent node " << parent.Getnode_id() << " height: " << parent.Getheight() << endl;
                success = 0;
            }
            // verifies if node[j] is the first successor and node[j].x is different than parent.x.
            if ((j == 0) && (x != parent.Getpos_x()))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != parent.x / parent is Node "<< parent.Getnode_id() << "( x: " << parent.Getpos_x() << ", y: " << parent.Getpos_y() << " width: " << parent.Getwidth() << ", height: "<< parent.Getheight() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != parent.x / parent is Node "<< parent.Getnode_id() << "( x: " << parent.Getpos_x() << ", y: " << parent.Getpos_y() << " width: " << parent.Getwidth() << ", height: "<< parent.Getheight() << ")" << endl;
                success = 0;
            }
            // verifies if node[j] is not the first successor and node[j].x is different than node[j-1].x + node[j-1].width.
            if ( (j > 0) && (x != (parent.Getsuccessor(j-1).Getpos_x() + parent.Getsuccessor(j-1).Getwidth())))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != node[" << parent.Getsuccessor(j-1).Getnode_id() << "].x + node[" << parent.Getsuccessor(j-1).Getnode_id() << "].width" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.x != node[" << parent.Getsuccessor(j-1).Getnode_id() << "].x + node[" << parent.Getsuccessor(j-1).Getnode_id() << "].width" << endl;
                success = 0;
            }
            // verifies if node.x + node.width do not overlap parent node area.
            if (x+width > parent.Getwidth()+parent.Getpos_x())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.x + node.width > parent.x + parent.width (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.x + node.width > parent.x + parent.width (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                success = 0;
            }
            // verifies if node[j] is not the last successor and node[j].x + node[j].width less than parent.x + parent.width.
            if ((j == c_nbr-1) && (x+width < parent.Getwidth()+parent.Getpos_x()))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": is the last of node " << parent.Getnode_id() << " successors and node.x + node.width < parent.x + parent.width (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": is the last of node " << parent.Getnode_id() << " successors and node.x + node.width < parent.x + parent.width (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                success = 0;
            }
        }
        else // if even cut stage.
        {
            // if node.x is different from parent.x.
            if (x != parent.Getpos_x())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": x: " << parent.Getsuccessor(j).Getpos_x() << " is not equal to its parent node " << parent.Getnode_id() << " x: " << parent.Getpos_x() << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": x: " << parent.Getsuccessor(j).Getpos_x() << " is not equal to its parent node " << parent.Getnode_id() << " x: " << parent.Getpos_x() << endl;
                success = 0;
            }
            // if node.width is different from parent.width.
            if (width != parent.Getwidth())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.width is not equal to its parent node " << parent.Getnode_id() << " width: " << parent.Getwidth() << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.width is not equal to its parent node " << parent.Getnode_id() << " width: " << parent.Getwidth() << endl;
                success = 0;
            }
            // if node[j] is first parent successor and node.y is different from parent.y.
            if ((j == 0) && (y != parent.Getpos_y()))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.y != parent.y / parent is Node "<< parent.Getnode_id() << "( x: " << parent.Getpos_x() << ", y: " << parent.Getpos_y() << " width: " << parent.Getwidth() << ", height: "<< parent.Getheight() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.y != parent.y / parent is Node "<< parent.Getnode_id() << "( x: " << parent.Getpos_x() << ", y: " << parent.Getpos_y() << " width: " << parent.Getwidth() << ", height: "<< parent.Getheight() << ")" << endl;
                success = 0;
            }
            // if node[j] is not the first parent successor and node.y is different from node[j-1].y + node[j-1].height.
            if ((j > 0) && (y != (parent.Getsuccessor(j-1).Getpos_y() + parent.Getsuccessor(j-1).Getheight())))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.y != node[" << parent.Getsuccessor(j-1).Getnode_id() << "].y + node[" << parent.Getsuccessor(j-1).Getnode_id() << "].height" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": (x: " << x << ", y: " << y << ", width: " << width << ", height: " << height << ") has node.y != node[" << parent.Getsuccessor(j-1).Getnode_id() << "].y + node[" << parent.Getsuccessor(j-1).Getnode_id() << "].height" << endl;
                success = 0;
            }
            // if node.y + node.height overlap parent area.
            if (height+y > parent.Getheight()+parent.Getpos_y())
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.y + node.height > parent.y + parent.height (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": node.y + node.height > parent.y + parent.height (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                success = 0;
            }
            // if node[j] is last successor ans node.y + node.height is inferior to parent.y + parent.height .
            if ((j == c_nbr-1) && (y+height < parent.Getheight() + parent.Getpos_y()))
            {
                if (active_log)
                    log_file << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": is the last of node " << parent.Getnode_id() << " successors and node.y + node.height < parent.y + parent.height (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                else
                    cout << "\tERROR -- Node " << parent.Getsuccessor(j).Getnode_id() << ": is the last of node " << parent.Getnode_id() << " successors and node.y + node.height < parent.y + parent.height (parent.node_id: " << parent.Getnode_id() << ")" << endl;
                success = 0;
            }
        }
        if (type == -2)
            checkSuccDimensions (parent.Getsuccessor(j));
    }
    if (!success)
    {
        constraint_error |= DIMENSIONS_ERROR_MASK;
    }
}



bool verifyIdt_Sequence(vector<GlassStack>& stacks, const vector<GlassNode>& solution){
    if (active_log)
        log_file << endl << "\t--- Sequence & Identity constraints verification ---" << endl;

    else
        cout << endl << "\t--- Sequence & Identity constraints verification ---" << endl;

    int i, j, sol_width, sol_height, batch_width, batch_height, fnd_stck = 0, idtty_error = 0;
    int stack_cur_idx;
    // Verify that no Item production errors occured.
    if ((constraint_error & ITEM_PRODUCTION_ERROR_MASK) >> ITEM_PRODUCTION_ERROR_OFFSET){
        if (active_log)
            log_file << "\tERROR -- Unable to verify Sequence & Identity constraints due to Item Production ERROR" << endl;
        
        else
            cout << "\tERROR -- Unable to verify Sequence & Identity constraints due to Item Production ERROR" << endl;
    
    }else{
        for(j = 0; j < stack_nbr; j++){
            stacks[j].Reset(); // set stacks_idx to 0
        }

        // Loop on all solution nodes.
        for (i = 0; i < solution.size(); i++){
            fnd_stck = 0, idtty_error = 0;
            // Loop on all batch stacks.
            for (j = 0; j < stack_nbr; j++)
            {
                stack_cur_idx = stacks[j].Getcur_item_idx();
                // If Getcur_item_idx() call returned 0xffff it means that all items of stack[i] are already verified and no items left.
                if (stack_cur_idx == 0xffff)
                    break;
                // If solution item type equals to a authorized stack item id.
                if (solution[i].Gettype() == stacks[j].Getitem(stack_cur_idx).Getitem_id())
                {
                    fnd_stck++;
                    sol_width = solution[i].Getwidth();
                    sol_height = solution[i].Getheight();
                    batch_width = stacks[j].Getitem(stacks[j].Getcur_item_idx()).Getitem_w();
                    batch_height = stacks[j].Getitem(stacks[j].Getcur_item_idx()).Getitem_h();
                    stacks[j].Pop();
                    if (!(((sol_height == batch_height) && (sol_width == batch_width)) || ((sol_height == batch_width) && (sol_width == batch_height))))
                    {
                        if (active_log)
                            log_file << "\tERROR -- Identity constraint is not respected " << endl << "\t\tItem " << solution[i].Gettype() << " ( width: " << sol_width << ", height: " << sol_height << ")  parameters are not identical to its batch version (width: " << batch_width << ", height: " << batch_height << ")." << endl;
                        else
                            cout << "\tERROR -- Identity constraint is not respected " << endl << "\t\tItem " << solution[i].Gettype() << " ( width: " << sol_width << ", height: " << sol_height << ")  parameters are not identical to its batch version (width: " << batch_width << ", height: " << batch_height << ")." << endl;
                        idtty_error ++;
                        constraint_error |= IDENTITY_ERROR_MASK;
                    }
                }
            }
            // If solution item is not a authorized item (not found).
            if(fnd_stck == 0){
                if (active_log)
                {
                    log_file << "\tERROR -- Sequence constraint is not respected " << endl << "\t\tItem " << solution[i].Gettype() << " is not allowed" << endl;
                    constraint_error |= SEQUENCE_ERROR_MASK;
                }
                else
                    cout << "\tERROR -- Sequence constraint is not respected " << endl << "\t\tItem " << solution[i].Gettype() << " is not allowed" << endl;
                break;
            }
        }
        // Clear stacks current item index to allow a second run of the test.
        for (j = 0; j < stack_nbr; j++)
        {
            stacks[j].Setcur_item_idx(0);
        }
        // if batch items sequence is respected.
        if (fnd_stck == 1)
        {
            if (active_log)
                log_file << "\tSequence Constraint verified successfully" << endl;
            else
                cout << "\tSequence Constraint verified successfully" << endl;
        }
        // if solution useful nodes are identical to batch items.
        if (idtty_error == 0)
        {
            if (active_log)
                log_file << "\tIdentity Constraint verified successfully" << endl;
            else
                cout << "\tIdentity Constraint verified successfully" << endl;
        }
    
        if (active_log)
            log_file << "\t--- End of Sequence & Identity constraints verification ---" << endl;
        else
            cout << "\t--- End of Sequence & Identity constraints verification ---" << endl;

        return (idtty_error == 0);
    }
}
