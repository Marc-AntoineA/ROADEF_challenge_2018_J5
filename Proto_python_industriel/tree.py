# coding: utf8

import cv2
import numpy as np
from matplotlib import pyplot as plt
from node import *


class Tree:

    widthPlates = 6000
    heightPlates = 3210

    def __init__(self, instance, bin_id):
        self.instance = instance
        self.node = Node(0, 0, self.widthPlates, self.heightPlates, 0, [], WASTE)
        self.bin_id = bin_id

    def draw_tree(self, r=10):
        img = np.zeros((self.heightPlates // r, self.widthPlates // r, 3), np.uint8)
        img = self.node.draw(img, self.instance, r)

        plt.imshow(img)
        plt.xticks(np.arange(0, self.widthPlates + 1, 1000) // r, np.arange(0, self.widthPlates + 1, 1000))
        plt.yticks(np.arange(0, self.heightPlates + 1, 500) // r, np.arange(0, self.heightPlates + 1, 500))
        return img

    def draw_defects(self, r=10):
        # Placement des défauts
        for k in range(len(self.instance.list_def[self.bin_id]["x"][:, 2])):
            [xd, yd, wd, hd] = self.instance.list_def[self.bin_id]["x"][k, [2, 3, 4, 5]]

            # cercle plutôt qu'un rectangle
            # img = cv2.rectangle(img, (int(xd//r), int(yd//r)), (int((xd + wd)//r), int((yd + hd)//r)), (0, 255, 0), -1)

            img = cv2.circle(img, (int((xd + wd / 2) // r), int((yd + hd / 2) // r)), 4, (0, 255, 0), -1)

        return img

    def to_matrix(self, mat, current_id):
        self.node.to_matrix(mat, current_id + 1, "", self.bin_id)

    def gen_tree(self, emplacements):
        self.node = Node(0, 0, self.widthPlates, self.heightPlates, 0, [], WASTE)
        self.node.gen_node(emplacements)
