// ALGO ROADEF

void cut(vector<GlassStack>& stacks, vector<GlassNode>){
    
    Cut max_cut;
    double max_score;    
    
    for(int s = 0; s < stacks.size(); s++){
        if(!stacks[s].Depleted()){
            current_item = stacks[i].Top();

        }
    }
    for item in available(items){
        for position in positions(item){
            for orientation in orientations(item){
                for cut in available_cuts(item, position, orientation){
                    s = compute_score(cut);
                    if(s > max_score){
                        max_score = s;
                        max_cut = cut;
                    }
                }
            }
        }
    }
    
    cut(cut);


}
