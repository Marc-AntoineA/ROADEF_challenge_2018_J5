#include "parser.h"

/* ==================================================================================== */
/*                             CSV Parse files functions                                */
/* ==================================================================================== */
/**
 * This function parse optimization parameters csv file
 * to obtain prefixed value of those parameter.
 * path: input / path to optimization parameters file.
**/
/*
void parseOptimizationParams(string path)
{
    unsigned int i = 0;
    string value, temp, token [OPT_PARAM_RAWS_LENGTH];
    ifstream filess(path.c_str());
    if(!filess.is_open())
    {
        opened_file = false;
        cout << endl << "\t--- Unable to Open Optimization Parameters file / file not found ---" << endl << "\tOptimization Parameters file path: " << path << endl;
        log_file << endl << "\t--- Unable to Open Optimization Parameters file / file not found ---" << endl << "\tOptimization Parameters file path: " << path << endl;
        cout <<     "\t--- Optimization Parameters file not parsed ---" << endl;
        log_file << "\t--- Optimization Parameters file not parsed ---" << endl;
        return;
    }
    cout << endl << "\t--- Parsing Optimization Parameters file ---" << endl << "\tOptimization Parameters file path: " << path << endl;
    log_file << endl << "\t--- Parsing Optimization Parameters file ---" << endl << "\tOptimization Parameters file path: " << path << endl;
    getline(filess, value, '\n'); // skip first line.
    // Get nPlates parameter.
    while(filess.good())
    {
        getline(filess, value, '\n');
        temp = value.substr(0, value.find(';'));
        if(temp.size())
            token[i++] = value.erase(0, temp.size()+1);
    }
    plate_nbr_limit = atoi(token[OPT_PARAM_NPLATES_RAW].c_str());
    plate_w = atoi(token[OPT_PARAM_WIDTHPLATES_RAW].c_str());
    plate_h = atoi(token[OPT_PARAM_HEIGHTPLATES_RAW].c_str());
    min1Cut = atoi(token[OPT_PARAM_MIN1CUT_RAW].c_str());
    min2Cut = atoi(token[OPT_PARAM_MIN2CUT_RAW].c_str());
    max1Cut = atoi(token[OPT_PARAM_MAX1CUT_RAW].c_str());
    waste_min = atoi(token[OPT_PARAM_MINWASTE_RAW].c_str());
    cout << "\tnPlates: " << plate_nbr_limit << endl
         << "\twidthPlates: " << plate_w << endl
         << "\theightPlates: " << plate_h << endl
         << "\tmin1Cut: " << min1Cut << endl
         << "\tmax1Cut: " << max1Cut << endl
         << "\tmin2Cut: " << min2Cut << endl
         << "\tminWaste: " << waste_min << endl;
    log_file << "\tnPlates: " << plate_nbr_limit << endl
         << "\twidthPlates: " << plate_w << endl
         << "\theightPlates: " << plate_h << endl
         << "\tmin1Cut: " << min1Cut << endl
         << "\tmax1Cut: " << max1Cut << endl
         << "\tmin2Cut: " << min2Cut << endl
         << "\tminWaste: " << waste_min << endl;
    cout << "\t--- Optimization Parameters file parsed successfully ---" << endl;
    log_file << "\t--- Optimization Parameters file parsed successfully ---" << endl;
    filess.close();
}

/**
 * This function parse batch csv file to obtain a
 * prefixed production sequence of items and stacks.
 * path: input / path to batch file.
 * log écrits dans log_file(variable globale)(ofstream)
**/


vector<GlassStack> parseBatch(string path){
    unsigned int j, i, k, p = 0, s = 0;
    unsigned int stack_nbr = 0;

    string token[BATCH_COL_LENGTH]; //tableau de longueur 5 pour stocker les valeurs lignes par lignes du fichier de batch
    string temp;
    string value;

    ifstream filess(path.c_str());
    if(!filess.is_open())
    {
        opened_file = false;
        cout << endl << "\t--- Unable to Open Batch file / file not found ---" << endl << "\tBatch file path: " << path << endl;
        log_file << endl << "\t--- Unable to Open Batch file / file not found ---" << endl << "\tBatch file path: " << path << endl;
        cout <<     "\t--- Batch file not parsed ---" << endl;
        log_file << "\t--- Batch file not parsed ---" << endl;
        return vector<GlassStack>(0);
    }
    cout << endl << "\t--- Parsing Batch file ---" << endl << "\tBatch file path: " << path << endl;
    log_file << endl << "\t--- Parsing Batch file ---" << endl << "\tBatch file path: " << path << endl;
    // Skip first line.
    getline(filess, value, '\n');

    unsigned int batch_items = 0;
    while(filess.good())// tant qu'il y a des lignes dans le fichiers (permet de compte le nombre de lignes)
    {
        getline(filess, value, '\n');
        temp = value.substr(0, value.find(';'));
        if(temp.size())
            batch_items++;
    }

    vector<GlassItem> items(batch_items);

    cout << "\tItems: " << batch_items << endl;
    log_file << "\tItems: " << batch_items << endl;
    filess.clear(); // On recommence la lecture à partir de 0
    filess.seekg(0, filess.beg); // beg pour 'beginning'
    getline(filess, value, '\n'); // on zappe la premiere ligne
    
    while(filess.good())
    {
        getline(filess, value, '\n');
        // Loop on value string to split batch column tokens.
        for(i = 0; i < value.size();)
        {
            for(k = i; k < value.size(); k++)
            {
                if(value[k] == ';'){ // si c'est un ; ça veut dire que value[i:k] est un chiffre
                    i = k + 1;
                    s =(s+1)%5; // correspond à l'indice de la colonne
                    break;
                // sinon et si on est à la fin, on ajoute la dernière valeur et c'est fini.
                }else if((k ==(value.size()-1)) &&(s == 4)){
                    token[s].append(value, k, 1);
                    i = k+1;
                    break;
                }else{// on ajoute à token value[k]
                    token[s].append(value, k, 1);
                }
            }
        }

        /* INUTILE (sauf preuve du contraire...)
        if(token[BATCH_ITEM_ID_COL].size() && first == 0)
        {
            first = 1;
            if(atoi(token[BATCH_STACK_COL].c_str()) == 0)
                init = 0;
            else
                init = 0;
        }*/

        // Une fois les valeurs obtenues, on les enregistre dans 'items'
        if(token[BATCH_ITEM_ID_COL].size() > 0)
        {
            // si jamais l'identifiant est sup à n, l'identifiant du plus grand stack
            if(1 + atoi(token[BATCH_STACK_COL].c_str()) > stack_nbr){
                stack_nbr = 1 + atoi(token[BATCH_STACK_COL].c_str());
            }
            createItem(items, token, p++);
        }

        // On réinitialise pour la ligne suivante
        for(k = 0; k < BATCH_COL_LENGTH; k++){
            token[k].clear();
        }
        s = 0;
    }
    
    // Create stacks.
    vector<GlassStack> stacks(stack_nbr);
    cout << "\tStacks: " << stack_nbr << endl;
    log_file << "\tStacks: " << stack_nbr << endl;
    j = 0, k = 0;
  
    // Loop on batch's stacks number.
    for(i = 0; i < stack_nbr; i++)
    {
        stacks[i].Setstack_id(i);
        // compute each stack's items number.
        for(j = k; j < batch_items; j++)
        {
            if(items[j].Getitem_stack() == stacks[i].Getstack_id()){
                stacks[i].Increaseitem_nbr();
            }else{
                k = j;
                break;
            }
        }
        // allocate memory for stacks's items.
        stacks[i].AllocateItems();
    }
    j = 0, k = 0;
    // Loop on batch's stacks and set each stack's items.
    for(i = 0; i < stack_nbr; i++){

        for(j = k; j < batch_items; j++){
            if(items[j].Getitem_stack() == stacks[i].Getstack_id()){
                stacks[i].Setitem(items[j]);
            }else{
                k = j;
                break;
            }
        }
    }
    filess.close();
    cout << "\t--- Batch file parsed successfully ---" << endl;
    log_file << "\t--- Batch file parsed successfully ---" << endl;
    return stacks;
}

/**
 * This function parse solution csv file to obtain
 * the user algorithm result into a node array.
 * path: input / path to solution file.
**//*
void parseSolution(string path)
{
    GlassNode *node;
    string value, temp;
    string token[SOLUTION_COL_LENGTH];
    unsigned int i, k, s = 0;
    int p = 0;
    unsigned int plate_id = 0;
    ifstream filess(path.c_str());
    if(!filess.is_open())
    {
        opened_file = false;
        cout << endl << "\t--- Unable to Open Solution file / file not found ---" << endl << "\tSolution file path: " << path << endl;
        log_file << endl << "\t--- Unable to Open Solution file / file not found ---" << endl << "\tSolution file path: " << path << endl;
        cout <<     "\t--- Solution file not parsed ---" << endl;
        log_file << "\t--- Solution file not parsed ---" << endl;
        return;
    }
    cout << endl << "\t--- Parsing Solution file ---" << endl << "\tSolution file path: " << path << endl;
    log_file << endl << "\t--- Parsing Solution file ---" << endl << "\tSolution file path: " << path << endl;
    // Get plates list.
    getline(filess, value, '\n');
    // Get the first plate ID.
    getline(filess, value, '\n');
    lines_nbr++;
    temp = value.substr(0, value.find(';'));
    plate_id = atoi(temp.c_str());
    plates_list[plates_nbr][0] = plate_id;
    plates_list[plates_nbr][1] = -1;
    plates_nbr +=1;
    while(filess.good())
    {
        getline(filess, value, '\n');
        lines_nbr++;
        temp = value.substr(0, value.find(';'));
        plate_id = atoi(temp.c_str());
        // Loop on solution used plates number.
        for(i = 0; i < plates_nbr; i++){
            // If plate is already counted, increase plate's children number.
            if(plate_id == plates_list[i][0])
            {
                plates_list[i][1]++;
                break;
            }
        }
        // If plate_id is not counted, increase plates number.
        if(i == plates_nbr)
        {
            plates_list[plates_nbr][0] = plate_id;
            plates_nbr +=1;
            plates_list[plates_nbr][1] = 0;
        }
    }
    node_nbr =(lines_nbr - plates_nbr - 1);
    plate = new GlassPlate[plates_nbr];
    node = new GlassNode[node_nbr];
    filess.clear();
    filess.seekg(0, filess.beg);
    // Get rid of fields name line.
    getline(filess, value, '\n');
    while(filess.good())
    {
        getline(filess, value, '\n');
        // Loop on value string to split solution column tokens.
        for(i = 0; i < value.size();)
        {
            for(k = i; k < value.size(); k++)
            {
                if(value[k] == ';')
                {
                    i = k+1;
                    s =(s+1)%9;
                    break;
                }
                else if(k ==(value.size()-1) &&(s == 8))
                {
                    token[s].append(value, k, 1);
                    i = k+1;
                    break;
                }
                else
                {
                    token[s].append(value, k, 1);
                }
            }
        }
        if(token [SOLUTION_PLATE_ID_COL].size() != 0)
        {
            if(token[SOLUTION_PARENT_COL].size() == 0) // Plate.
            {
                if(atoi(token[SOLUTION_PLATE_ID_COL].c_str()) == p)
                {
                    createPlate(token, p++);
                }
                else
                {
                    cout << "\tPlate utilization Sequence Constraint violated ";
                    cout << "\t(ERROR: Plate " << atoi(token[SOLUTION_PLATE_ID_COL].c_str())<< " is used when Plate " << p << " is expected)" << endl;
                    log_file << "\tPlate utilization Sequence Constraint violated ";
                    log_file << "\t(ERROR: Plate " << atoi(token[SOLUTION_PLATE_ID_COL].c_str())<< " is used when Plate " << p << " is expected)" << endl;
                    constraint_error |= PARSE_ERROR_MASK;
                }
            }
            else // Piece.
            {
                //createNode(node, token ,n++);
                max_cut_stage =( max_cut_stage < atoi(token[SOLUTION_CUT_COL].c_str()) ) ? atoi(token[SOLUTION_CUT_COL].c_str()) : max_cut_stage;
                switch(atoi(token[SOLUTION_TYPE_COL].c_str()))
                {
                case -3:
                    residual_node++;
                    break;
                case -2:
                    branch_node++;
                    break;
                case -1:
                    waste_node++;
                    break;
                default:
                    useful_node++;
                    break;
                }
            }
        }
        for(k = 0; k < SOLUTION_COL_LENGTH; k++)
        {
            token[k].clear();
        }
        s = 0;
    }
    cout << "\tPlates: " << plates_nbr ;
    log_file << "\tPlates: " << plates_nbr ;
    if(plates_nbr > plate_nbr_limit)
    {
        cout << "(ERROR: Exceed plates number limit which's " << plate_nbr_limit << ")" << endl;
        log_file << "(ERROR: Exceed plates number limit which's " << plate_nbr_limit << ")" << endl;
        constraint_error |= PARSE_ERROR_MASK;
    }
    else
    {
        cout << endl;
        log_file << endl;
    }
    // build tree structure.
    buildDataStructure(node);
    cout << "\tNodes: " <<(lines_nbr - 1);
    log_file << "\tNodes: " <<(lines_nbr - 1);
    filess.close();
    cout << endl;
    log_file << endl;
    cout << "\tbranches: " << branch_node + plates_nbr << endl << "\tItems: " << useful_node << endl << "\tResiduals: " << residual_node;
    log_file << "\tbranches: " << branch_node << endl << "\tItems: " << useful_node << endl << "\tResiduals: " << residual_node;
    if(residual_node > 1)
    {
        cout << "(ERROR: Solution should have one and only one residual item located on the rightmost node of the last plate)";
        log_file << "(ERROR: Solution should have one and only one residual item located on the rightmost node of the last plate)";
        constraint_error |= PARSE_ERROR_MASK;
    }
    cout << endl << "\tWaste: " << waste_node;
    log_file << endl << "\tWaste: " << waste_node;
    cout << endl << "\tUsed Cut Stages: " << max_cut_stage;
    log_file << endl << "\tUsed Cut Stages: " << max_cut_stage;
    if(max_cut_stage > 4)
    {
        cout << "(ERROR: Solution Algorithm has " << max_cut_stage << " cut stages, while the max is 4 stages)" << endl;
        log_file << "(ERROR: Solution Algorithm has " << max_cut_stage << " cut stages, while the max is 4 stages)" << endl;
        constraint_error |= PARSE_ERROR_MASK;
    }
    cout << endl; // << " waste: " << node_nbr -(useful_node + branch_node) << endl;
    log_file << endl;
    if(constraint_error > 0)
    {
        cout << "\tTree structure built" << endl;
        log_file << "\tTree structure built" << endl;
        cout << "\t--- ERROR -- Solution file Contains an INVALID SOLUTION ---" << endl;
        log_file << "\t--- ERROR -- Solution file Contains an INVALID SOLUTION ---" << endl;
    }
    else
    {
        cout << "\tTree structure built" << endl;
        log_file << "\tTree structure built" << endl;
    }
    cout << "\t--- Solution file parsed successfully ---" << endl;
    log_file << "\t--- Solution file parsed successfully ---" << endl;
}

/**
 * This function parse Defects csv file.
 * path: input / path to Defects file.
**/
vector<GlassPlate> parseDefects(string path)
{
    unsigned int i, k, s = 0;
    unsigned int defects_nbr = 0;
    string token[DEFECTS_COL_LENGTH], temp;
    string value;

    ifstream filess(path.c_str());
    if(!filess.is_open())
    {
        opened_file = false;
        cout << endl << "\t--- Unable to Open Defects file / file not found ---" << endl << "\tDefects file path: " << path << endl;
        log_file << endl << "\t--- Unable to Open Defects file / file not found ---" << endl << "\tDefects file path: " << path << endl;
        cout <<     "\t--- Defects file not parsed ---" << endl;
        log_file << "\t--- Defects file not parsed ---" << endl;
        return vector<GlassPlate>(0);
    }
    cout << endl << "\t--- Parsing Defects file ---" << endl << "\tDefects file path: " << path << endl;
    log_file << endl << "\t--- Parsing Defects file ---" << endl << "\tDefects file path: " << path << endl;

    vector<GlassPlate> plates(PLATES_NBR_LIMIT);
    for(k = 0; k < plates.size(); k++){
        plates[k].Setplate_id(k);
        plates[k].Setwidth(plate_w);
        plates[k].Setheight(plate_h);
    }


    // Skip first line.   
    getline(filess, value, '\n');
    while(filess.good()){ // tant qu'il reste des lignes dans le parser

        getline(filess, value, '\n');
        // Loop on value string to split defects column tokens.
        for(i = 0; i < value.size();){
            for(k = i; k < value.size(); k++){
                
                if(value[k] == ';'){
                    i = k + 1;
                    s =(s+1)%6;
                    break;
                }else if(k ==(value.size()-1) &&(s == 5)){

                    token[s].append(value, k, 1);
                    i = k+1;
                    break;
                }else{
                    token[s].append(value, k, 1);
                }
            }
        }
        if(token[DEFECTS_DEFECT_ID_COL].size() &&(atoi(token[DEFECTS_PLATE_ID_COL].c_str()) < PLATES_NBR_LIMIT)){
            defects_nbr ++;
            // Create defect with parsed parameters.
            GlassDefect d(atoi(token[DEFECTS_DEFECT_ID_COL].c_str()),
                           atoi(token[DEFECTS_PLATE_ID_COL].c_str()),
                           atoi(token[DEFECTS_X_COL].c_str()),
                           atoi(token[DEFECTS_Y_COL].c_str()),
                           atoi(token[DEFECTS_WIDTH_COL].c_str()),
                           atoi(token[DEFECTS_HEIGHT_COL].c_str()));
            // Add defect to respective plate.
            plates[atoi(token[DEFECTS_PLATE_ID_COL].c_str())].Setdefect(d);
        }else{
            break;
        }

        for(k = 0; k < DEFECTS_COL_LENGTH; k++){

            token[k].clear();
        }
        s = 0;
    }
    cout << "\tPlates : " << PLATES_NBR_LIMIT << endl;
    log_file << "\tPlates : " << PLATES_NBR_LIMIT << endl;
    cout <<     "\tDefects: " << defects_nbr << endl;
    log_file << "\tDefects: " << defects_nbr << endl;
    filess.close();
    cout << "\t--- Defects file parsed successfully ---" << endl << endl;
    log_file << "\t--- Defects file parsed successfully ---" << endl << endl;

    return plates;
}
