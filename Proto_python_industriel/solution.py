from tree import *
import pandas as pd


class Solution:

    def __init__(self):
        self.trees = []

    def nb_trees(self):
        return len(self.trees)

    def to_mat(self):
        mat = []
        c = -1
        for bin_id in range(n):
            c = self.trees[bin_id].to_mat(mat, c + 1, "", bin_id)
        return mat

    def trees_to_file(self, instance_path):
        mat = self.to_mat()
        # print(mat)

        df = pd.DataFrame(mat, columns=['PLATE_ID', 'NODE_ID', 'X', 'Y', 'WIDTH', 'HEIGHT', 'TYPE', 'CUT', 'PARENT'])
        df.to_csv("{}_solution.csv".format(instance_path), sep=";", index=False)


