from greedy import *
from instance import *
from tree import *
import random
import numpy as np
from tree_search import *

random.seed = 3

instance = Instance("A1")

treeSearch = TreeSearch(instance)
treeSearch.greedy_rollback(10)
print(treeSearch.score())
#print(treeSearch.solve())
#greedy = Greedy(instance)
#greedy.solve()

#s = greedy.cost_roadef()


#for tree in greedy.trees:
#    tree.draw_tree()


# 56737170
# 53584950
#  3688710

#make_residual_trees(trees)

#trees_to_file(name, trees)
#trees_to_file(name + "_" + str(cost_roadef(items_cut)), trees)