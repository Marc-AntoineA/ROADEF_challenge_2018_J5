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

def areaIsFreeOfDefects(x, y, w, h, id_bin):
    list_defects = list(defects.loc[defects["PLATE_ID"] == id_bin].DEFECT_ID)
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
    (alpha_cut, id_bin, XX, YY, W, H) = current_node
    [_, h, w, _, _] = list(batch.loc[current_item])
    
    if rotateItem:
        h, w = w, h
    
    for x in range(XX, XX + W):
        for y in range(YY, YY + H):
            # Verifying constraints
            # Avoiding defects
            if x + w <= XX + W\
            and y + h <= YY + H\
            and areaIsFreeOfDefects(x, y, w, h, id_bin):
            
                if alpha_cut == 1:
                    minDimensionsAreOk = (x-XX == 0 or x-XX > minXX)\
                                     and (w > minXX)\
                                     and (XX+W-x-w == 0 or XX+W-x-w > minXX)\
                                     and (y-YY == 0 or y-YY > minYY)\
                                     and (h > minYY)\
                                     and (YY+H-y-h == 0 or XX+W-x-w > minYY)
                elif alpha_cut == 2:
                    minDimensionsAreOk = (y-YY == 0 or y-YY > minYY)\
                                     and (h > minYY)\
                                     and (YY+H-y-h == 0 or XX+W-x-w > minYY)
                else:
                    minDimensionsAreOk = True
                if minDimensionsAreOk:
                    return (True, rotateItem, x, y, 0)
    if rotateItem:
        return (False, rotateItem, 0, 0, 0)
    else:
        return best_position(current_node, current_item, True)

def is_real_node(node):
    """
    Input : node = (alpha_cut, id_bin, XX, YY, W, H)
    Output : True if W and H > 0
    """
    (alpha_cut, id_bin, XX, YY, W, H) = node
    return W > 0 and H > 0
    
def cut(node, item):
    """
    Input:
    * node = (alpha_cut, id_bin, XX, YY, W, H)
    * item = id
    Output:
    * nodes (max 3, 2 parallel cuts)
    * boolean : True if item cut
    """
    (alpha_cut, id_bin, XX, YY, W, H) = node
    [_, h, w, _, _] = list(batch.loc[item])
    if alpha_cut > 3:
        return None
    
    (feasible, rotate, x, y, score) = best_position(node, item)
    if rotate:
        h, w = w, h
    if feasible:
        # An alpha-cut ?
        
        if alpha_cut % 2 == 0: # first horizontal cut
            above = YY + H - y - h
            below = y - YY
            if above > 0 or below > 0: # we need a horizontal cut
                # [(bottom, up, center)]
                return [
                    (alpha_cut, id_bin, XX, YY, W, y - YY), 
                    (alpha_cut, id_bin, x, y + h, W, YY + H - y - h),
                    (alpha_cut, id_bin, x, y, W, h)]
                    
                    
            else: # no alpha-cut needed
                # [(left, right, center)]
                return [
                    (alpha_cut + 1, id_bin, XX, YY, x - XX, H),
                    (alpha_cut + 1, id_bin, x + w, y, XX + W - x - w, H),
                    (alpha_cut + 1, id_bin, x, y, w, H)]
                
        
        else: # first vertical cut
            right = XX + W - x - w
            left = x - XX
            if left > 0 or right > 0: # we need a horizontal cut
                # [(left, right, center)]
                return [
                    (alpha_cut, id_bin, XX, YY, x - XX, H),
                    (alpha_cut, id_bin, x + w, y, XX + W - x - w, H),
                    (alpha_cut, id_bin, x, y, w, H)]
            
            else:
                # [(bottom, up, center)]
                return [ 
                    (alpha_cut + 1, id_bin, XX, YY, W, y - YY), 
                    (alpha_cut + 1, id_bin, x, y + h, W, YY + H - y - h),
                    (alpha_cut + 1, id_bin, x, y, W, h)]
    else:
        return None


def display_node(node, matrix):
    """
    node = (W if waste or p if piece, node)
    """
    (type, (alpha_cut, id_bin, XX, YY, W, H)) = node
    col = None
    if type == "W":
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
                
    plt.imshow(matrix)
    plt.show()
    #plt.pause(1)
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
    
def display_nodes_visited(nodes_visited, bin = 0):
    
    matrix = np.zeros((widthPlates//resolution + 1, heightPlates//resolution + 1))
    for n in nodes_visited:
        if n[1][1] == bin:
            matrix = display_node(n, matrix)
    display_nodes_visited(nodes_visited, k)
    matrix = display_defects(matrix, bin)
    plt.imshow(matrix.T)
    plt.title("Bin {}".format(bin))
    plt.show()
    plt.pause(3)
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
    nodes_to_visit = [(1, idx, 0, 0, widthPlates, heightPlates) for idx in range(99, -1, -1)]
    
    
    # First (temporary), we take an available item (and we don't even rotate it)
    current_item = heads(list_stacks)[0]
    
    nodes_visited = []
    # While all items are not cut
    while len(heads(list_stacks)) > 0 and len(nodes_to_visit) > 0:
        current_node = nodes_to_visit.pop()
        nodes = cut(current_node, current_item)
        if nodes == None:
            nodes_visited.append(("W", current_node))
        else:
            if not(is_real_node(nodes[0])) and not (is_real_node(nodes[1])):
                remove_item(current_item, list_stacks)
                nodes_visited.append(("P", current_node))
                if len(heads(list_stacks)) > 0:
                    current_item = heads(list_stacks)[0]
            else:
                for n in nodes:
                    print(n)
                    if is_real_node(n):
                        nodes_to_visit.append(n)
    
    max_plate = current_node[1]
    #for k in range(max_plate + 1):
     #   display_nodes_visited(nodes_visited, k)
    
    
    
    
    
    
    
    
    
    
    

    
    
    
    
    
    
    
    
    