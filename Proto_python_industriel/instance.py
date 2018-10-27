# coding: utf8

from functools import reduce
from defect import *
from emplacement import *
import pandas as pd
from stack import *


class Instance:

    nPlates = 100
    widthPlates = 6000
    heightPlates = 3210
    minXX = 100
    maxXX = 3500
    minYY = 100
    minWaste = 20
    path = "../checker/instances_checker/"

    def __init__(self, instance_name):
        """
        Charge l'instance, accessible via defects et batch
        :param instance_name: nom de l'instance (ex. A19)
        """

        self.instance_name = instance_name

        print("Chargement de l'instance ", instance_name)

        instance_path = self.path + instance_name
        self.defects = pd.read_csv(instance_path + "_defects.csv", sep=";")
        self.batch = pd.read_csv(instance_path + "_batch.csv", sep=";")

        print("\tConversion en stacks du fichier de batch ...")
        self.stacks = []
        self.convert_batch_into_stacks()
        print("\t\t...done")

        print("\tConversion du fichier de batch en np.array...")
        self.batch_mat \
            = self.batch.as_matrix()
        print("\t\t...done")

        print("\tCréation d'une liste des défauts...")
        self.list_def = []
        self.list_def = self.convert_defects_into_list()
        print("\t\t...done")

        print("\t...done")

    def convert_batch_into_stacks(self):
        n_stack = max(self.batch.STACK)
        list_stacks = []

        for k in range(n_stack + 1):
            list_stacks.append(
                list(self.batch.loc[self.batch["STACK"] == k]
                     .sort_values("SEQUENCE").ITEM_ID))

        self.stacks = [Stack(s) for s in list_stacks]

    def reset_stacks(self):
        for s in self.stacks:
            s.reset()

    def convert_defects_into_list(self):
        n_def = max(self.defects.PLATE_ID)
        list_def = []

        for k in range(n_def + 1):
            list_def.append(
                {"x": self.defects.loc[self.defects["PLATE_ID"] == k].sort_values("X").as_matrix(),
                 "y": self.defects.loc[self.defects["PLATE_ID"] == k].sort_values("Y").as_matrix()
                 })

        return list_def

    def get_stack(self, item):
        return self.batch_mat[item, 3]

    def width_height_item(self, item, rotated = False):
        return (self.width_item(item, rotated),
                self.height_item(item, rotated))

    def width_item(self, item, rotated):
        if rotated:
            return self.batch_mat[item, 1]
        else:
            return self.batch_mat[item, 2]

    def height_item(self, item, rotated):
        if rotated:
            return self.batch_mat[item, 2]
        else:
            return self.batch_mat[item, 1]

    def nb_items(self):
        return len(self.batch)

    # TODO : mieux gérer la liste de défauts et l'accessibilité
    def locs_free_of_defects(self, bin_id, emplacement):
        x0 = emplacement.x
        y0 = emplacement.y
        x_max = self.widthPlates - emplacement.get_w()
        y_max = self.heightPlates - emplacement.get_h()

        # 1/ Détecter les positions des défauts qui mangent l'emplacement
        i_min = 0
        i = i_min
        i_max = len(self.list_def[bin_id]["x"][:, 2])
        x = x0
        defects_included = []

        xw0 = emplacement.get_xw()
        yh0 = emplacement.get_yh()

        if x0 > x_max or y0 > y_max:
            return []

        while i < i_max and x < xw0:
            [k, x, y, w, h] = self.list_def[bin_id]["x"][i, [0, 2, 3, 4, 5]]

            if x0 <= x and x + w < xw0 and y0 <= y and y < yh0:
                defects_included.append(Defect(x, y, w, h))

            if x > xw0:
                i_8max = i

            if x < x0:
                i_min += 1

            i += 1

        if len(defects_included) == 0:
            return [emplacement]

        # 2/ Appliquer l'ancien (la V0) algo de l'étoile verte pour
        # obtenir les emplacements qui évitent les défauts

        xx = sorted([x0] + [self.list_def[bin_id]["x"][i, 2] +
                            self.list_def[bin_id]["x"][i, 4] for i in range(i_min, i_max)])
        yy = sorted([y0] + [self.list_def[bin_id]["x"][i, 3] +
                            self.list_def[bin_id]["x"][i, 5] for i in range(i_min, i_max)])

        emplacements = []
        i = 0
        j = 0
        j_max = len(yy)
        i_max = len(xx)
        # print("imax", imax, jmax)

        w0 = emplacement.get_w()
        h0 = emplacement.get_h()
        rotated = emplacement.rotated

        while i < i_max and j < j_max:
            x = xx[i]
            y = yy[j]
            # print(x, y)
            if x > x_max:
                i_max = i
                continue
            if y > y_max:
                j_max = j
                j = 0
                i += 1
                continue

            if loc_not_includes_defects(defects_included, x, y):
                # TODO mieux gérer la copie
                emplacements.append(Emplacement(emplacement.item, x, y, rotated, emplacement.position))
                j_max = j
                j = 0
                i += 1
            else:
                j += 1
                if j == j_max:
                    i += 1
                    j = 0

        # print("loc", locations)
        if len(emplacements) > 0:

            return reduce(lambda x, y: x + y,
                          [self.locs_free_of_defects(bin_id, emplacement) for emplacement in emplacements])
        return []