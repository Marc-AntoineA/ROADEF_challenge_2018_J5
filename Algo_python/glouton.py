# -*- coding: utf-8 -*-
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import geometry

def heads(list_stacks):
    """ Return all items we can cut """
    return [x[0] for x in list_stacks if len(x) > 0]

def remove_item(item, list_stacks):
    """ Remove item which is a head of a stack """
    for k in range(len(list_stacks)):
        if len(list_stacks[k]) > 0 and item == list_stacks[k][0]:
            list_stacks[k].remove(list_stacks[k][0])

def areaIsFreeOfDefects(x, y, w, h, list_defects):
    for defect_id in list_defects:
        (_, _, xd, yd, wd, hd) = list(defects.loc[defect_id])
        if geometry.intersecte_rectangle(x, y, w, h, xd, yd, wd, hd):
            return False
    return True


def best_position(current_node, current_item, rotateItem=False):
    """
    Input:
    current_node = (alpha_cut, id_bin, x_pos, y_pos, width, height) 
        defects are read in defects 
    current_item = id 
        dimensions are read in batch.
    Output: (x, y, score) = 
    * position of the item in the current_node
    * score to estimate the 'best position'
    Remarks:
    * Constraints verified (soon)
    * The algorithm can rotate the item
    """
    (alpha_cut, id_bin, _, _, XX, YY, W, H, _) = current_node
    [_, h, w, _, _] = list(batch.loc[current_item])
    list_defects = list(defects.loc[defects["PLATE_ID"] == id_bin+95].DEFECT_ID)

    if rotateItem:
        h, w = w, h
    
    if w <= W and h <= H:
        printed = False
        for x in range(XX, XX + W):
            for y in range(YY, YY + H):
                # Verifying constraints
                # Avoiding defects
                if x + w <= XX + W\
                and y + h <= YY + H\
                and areaIsFreeOfDefects(x, y, w, h, list_defects):
                
                    if alpha_cut == 1:
                        minDimensionsAreOk = (x-XX == 0 or x-XX > minXX)\
                                         and (w > minXX)\
                                         and (XX+W-x-w == 0 or XX+W-x-w > minXX)\
                                         and (y-YY == 0 or y-YY > minYY)\
                                         and (h > minYY)\
                                         and (YY+H-y-h == 0 or YY+H-y-h > minYY)
                    elif alpha_cut == 2:
                        minDimensionsAreOk = (y-YY == 0 or y-YY > minYY)\
                                         and (h > minYY)\
                                         and (YY+H-y-h == 0 or YY+H-y-h > minYY)
                    else:
                        minDimensionsAreOk = True
                    
                    minWasteIsOk = (x-XX == 0 or x-XX > minWaste)\
                               and (XX+W-x-w == 0 or XX+W-x-w > minWaste)\
                               and (y-YY == 0 or y-YY > minWaste)\
                               and (YY+H-y-h == 0 or YY+H-y-h > minWaste)                    
                    
                    if minDimensionsAreOk and minWasteIsOk:
                        return (True, rotateItem, x, y, 0)
                    else:
                        if not printed:
                            print("minDimNotOk", alpha_cut)
                            printed=True
    if rotateItem:
        return (False, rotateItem, 0, 0, 0)
    else:
        print("rotate")
        return best_position(current_node, current_item, True)

def is_real_node(node):
    """
    Input : node = (alpha_cut, id_bin, XX, YY, W, H)
    Output : True if W and H > 0
    """
    (alpha_cut, id_bin, XX, YY, W, H) = node
    b = W > 0 and H > 0
    if b:
        print("error")
    return b
    
def cut(node, item):
    """
    Input:
    * node = (alpha_cut, id_bin, node_id, Type, XX, YY, W, H, Parent)
    * Type :       *  > 0 glass piece index in batch file.
                   * = -1 wasted glass.
                   * = -2 branch.
                   * = -3 residual.
    * item = id
    Output:
    * nodes (max 3, 2 parallel cuts)
    * boolean : True if item cut
    """
    (alpha_cut, id_bin, node_id, Type, XX, YY, W, H, _) = node
    if Type >= 0 or alpha_cut > 3:
        return False, None
    if alpha_cut == 0:
        alpha_cut = 1
    [_, h, w, _, _] = list(batch.loc[item])
    thereIsNewItem = False
    result = []
    (feasible, rotate, x, y, score) = best_position(node, item)
    if rotate:
        h, w = w, h
    if feasible:
        # An alpha-cut ?
        global next_node_id
        if alpha_cut % 2 == 0: # first horizontal cut
            above = YY + H - y - h
            below = y - YY
            if above > 0 or below > 0: # we need a horizontal cut
                # [(bottom, center, up)]
                if below > 0:
                    result.append((alpha_cut, id_bin, next_node_id, -2, XX, YY, W, y - YY, node_id))
                    next_node_id+=1
                result.append(    (alpha_cut, id_bin, next_node_id, -2, XX, y, W, h, node_id))
                next_node_id+=1
                if above > 0:
                    result.append((alpha_cut, id_bin, next_node_id, -2, XX, y + h, W, YY + H - y - h, node_id))
                    next_node_id +=1
            else: # no alpha-cut needed
                # [(left, center , right)]
                thereIsNewItem = True       
                if x - XX > 0:
                    result.append((alpha_cut + 1, id_bin, next_node_id, -2, XX, YY, x - XX, H, node_id))
                    next_node_id+=1
                result.append(    (alpha_cut + 1, id_bin, next_node_id, item, x, y, w, H, node_id))
                next_node_id+=1
                if XX + W - x - w > 0:
                    result.append((alpha_cut + 1, id_bin, next_node_id, -2, x + w, YY, XX + W - x - w, H, node_id))
                    next_node_id+=1
                
        
        else: # first vertical cut
            right = XX + W - x - w
            left = x - XX
            if left > 0 or right > 0: # we need a horizontal cut
                # [(left, center, center)]
                if left > 0:
                    result.append((alpha_cut, id_bin, next_node_id, -2, XX, YY, x - XX, H, node_id))
                    next_node_id+=1
                result.append(    (alpha_cut, id_bin, next_node_id, -2, x, y, w, H, node_id))
                next_node_id+=1
                if right > 0:
                    result.append((alpha_cut, id_bin, next_node_id, -2, x + w, YY, XX + W - x - w, H, node_id))
                    next_node_id+=1
            else:
                # [(bottom, center, up)]
                thereIsNewItem = True
                if y - YY > 0:
                    result.append((alpha_cut + 1, id_bin, next_node_id, -2, XX, YY, W, y - YY, node_id))
                    next_node_id+=1
                result.append(    (alpha_cut + 1, id_bin, next_node_id, item, XX, y, W, h, node_id))
                next_node_id+=1
                if YY + H - y - h > 0:
                    result.append((alpha_cut + 1, id_bin, next_node_id, -2, XX, y + h, W, YY + H - y - h, node_id))
                    next_node_id+=1
        return [thereIsNewItem, result]
    else:
        return False, None


def display_node(node, matrix):
    """
    node = (W if waste or p if piece, node)
    """
    """
    * node = (PlateID; NodeId; X; Y; W; H; TYPE; CUT; PARENT)
    * Type :       *  > 0 glass piece index in batch file.
                   * = -1 wasted glass.
                   * = -2 branch.
                   * = -3 residual.
    """
    (_, _, XX, YY, W, H, Type, alpha_cut, _) = node
    col = None
    if Type == -1:
        col = 10
    else:
        col = alpha_cut
        
    for x in range(XX, XX + W, resolution):
        for y in range(YY, YY + H, resolution):
            x_r = x//resolution
            y_r = y//resolution
            matrix[x_r][y_r] = col
            
            if x == XX or y == YY or XX + W - XX < resolution or YY + H - y < resolution:
                # draw border
                matrix[x_r][y_r] = -5
                
    return matrix
    
def display_defects(matrix, bin = 0):
    defects_bin_id = list(defects.loc[defects.PLATE_ID == bin].DEFECT_ID)
    for d in defects_bin_id:
        [__, _, XX, YY, W, H] = list(defects.loc[d])
        XX = int(XX)
        YY = int(YY)
        W = int(W)
        H = int(H)
        for x in range(XX, XX + W, resolution):
            for y in range(YY, YY + H, resolution):
                x_r = x//resolution
                y_r = y//resolution
                matrix[x_r][y_r] = -10
                
    return matrix
    
def display_nodes_visited(solution, bin = 0):
    
    matrix = np.zeros((widthPlates//resolution + 1, heightPlates//resolution + 1))
    for n in solution:
        if n[0] == bin:
            matrix = display_node(n, matrix)
    
    matrix = display_defects(matrix, bin)
    plt.figure(bin)
    plt.imshow(matrix.T)
    plt.title("Bin {}".format(bin))
    plt.show()
    return matrix
				

if __name__=='__main__':
    instance = "../Datasets/dataset_A/A17"
    defects = instance + "_defects.csv"
    batch = instance + "_batch.csv"
    
    defects = pd.read_csv(defects, sep = ";")
    batch = pd.read_csv(batch, sep = ";")
    
    nPlates = 100
    widthPlates = 6000
    heightPlates = 3210
    minXX = 100
    maxXX = 3500
    minYY = 100
    minWaste = 20

    resolution = 50
    
    colors_alpha_cut = [(0, 0, 1),
        (0, 1, 0),
        (1, 0, 0),
        (1, 0, 1),
        (1, 1, 0),
        (0, 1, 1),
        (0, 1, .5)]
    waste = (0, 0, 0)
    
    # Convert batch into stacks
    nStack = max(batch.STACK)
    list_stacks = []
    
    for k in range(nStack + 1):
        list_stacks.append(list(batch.loc[batch["STACK"] == k].ITEM_ID))
    
    solution = []
    current_bin = 0
    #nodes_to_visit = [(1, idx, -2, 0, 0, widthPlates, heightPlates) for idx in range(99, -1, -1)]
    nodes_to_visit = []
    next_bin_id = 0
    next_node_id = 0
    # First (temporary), we take an available item (and we don't even rotate it)
    current_item = heads(list_stacks)[0]
    remove_item(current_item, list_stacks)
    nodes_visited = []
    thereAreItemsToCut = True
    # While all items are not cut
    while thereAreItemsToCut:
        print(current_item)
        if len(nodes_to_visit) == 0:
            nodes_to_visit.append((0, next_bin_id, next_node_id, -2, 0, 0, widthPlates, heightPlates, None))
            next_bin_id+=1
            next_node_id+=1
        current_node = nodes_to_visit.pop()
        thereIsNewItem, nodes = cut(current_node, current_item)
        nodes_visited.append(current_node)
        if nodes != None:
            for l in range(len(nodes)):
                print(nodes[l])
                nodes_to_visit.append(nodes[((len(nodes)-1) - l)])
            if thereIsNewItem:
                if len(heads(list_stacks)) > 0:
                    current_item = heads(list_stacks)[0]
                    remove_item(current_item, list_stacks)
                else:
                    thereAreItemsToCut = False
    
    while len(nodes_to_visit) > 0:
        nodes_visited.append(nodes_to_visit.pop())
    print("done cutting")
    max_plate = current_node[1]

                    
    #obj : passer de 
    #* node = (alpha_cut, id_bin, node_id, Type, XX, YY, W, H, Parent)
    #* Type :       *  > 0 glass piece index in batch file.
    #               * = -1 wasted glass.
    #               * = -2 branch.
    #               * = -3 residual.
    #à  PlateID; NodeId; X; Y; W; H; TYPE; CUT; PARENT (format csv)
    #il faut déterminer les types...
    print("sorting")
    nodes_visited = sorted(nodes_visited, key=lambda x: x[2])
    print("identifying waste")
    solution = []
    for i in range(len(nodes_visited)):
        node = nodes_visited[i]
        (alpha_cut, id_bin, node_id, Type, XX, YY, W, H, parent) = node
        hasChild = False
        if  node[3]>=0:
            solution.append((id_bin, node_id, XX, YY, W, H, Type, alpha_cut, parent))
        else:
            j = 0
            while j < len(nodes_visited) and not hasChild:
                if nodes_visited[j][-1] == node_id:
                    hasChild = True
                j+=1
            if hasChild:
                solution.append((id_bin, node_id, XX, YY, W, H, -2, alpha_cut, parent))
            else:
                if id_bin == max_plate and alpha_cut == 1 and XX + W == widthPlates:
                    Type = -3 #residuel
                else:
                    Type = -1 #waste
                solution.append((id_bin, node_id, XX, YY, W, H, Type, alpha_cut, parent))
    (alpha_cut, id_bin, node_id, Type, XX, YY, W, H, parent) = nodes_visited[-1]
    solution.append((id_bin, node_id, XX, YY, W, H, -3, alpha_cut, parent))

    print("saving")
    
    df = pd.DataFrame(np.array(solution),\
                       columns=["PLATE_ID","NODE_ID","X","Y","WIDTH","HEIGHT","TYPE","CUT","PARENT"])
    df.to_csv("solutionA17Glouton.csv", sep = ";", index = False)
    
    for k in range(max_plate+1):
        display_nodes_visited(solution, k)    
    
    print("finished")