from copy import copy
from point_rouge import *
from emplacement import *
from etoile_verte import *
import numpy as np
import matplotlib.pyplot as plt


class MonstreRouge:
    heightPlates = 3210
    widthPlates = 6000

    # TODO remettre le paramètre
    def __init__(self, points=[]):
        self.points = []

    def draw(self, r=10):

        img = np.zeros((self.heightPlates // r, self.widthPlates // r, 3), np.uint8)
        for point in self.points:
            point.draw(img, r)

        plt.imshow(img)
        plt.show()

    def clear_monster(self, emplacement):

        k = 0
        while k < len(self.points) - 1:

            # Si le plus gros élément est à gauche (1, 0, 2)
            if self.points[k].is_after(self.points[k + 1]):
                if emplacement.x >= self.points[k].x and emplacement.y <= self.points[k + 1].y:
                    self.points[k].mange(self.points[k + 1])
                    del self.points[k + 1]

            else:
                if emplacement.x <= self.points[k].x and emplacement.y >= self.points[k].y:
                    self.points[k].mange(self.points[k + 1])
                    del self.points[k + 1]

            k += 1

    def get_area(self):

        area = 0
        if len(self.points) > 0:
            area = self.points[0].x * self.points[0].y

            k = 0
            while k < len(self.points) - 1:
                area += (self.points[k + 1].x - self.points[k].x) * self.points[k].y
                k += 1
        return area

    def incr_etoile_verte(self, pointRouge):
        """
        Du nom de l'ancienne étoile verte, exactement le même algo adapté à la nouvelle structure de données
        :param emplacement:
        :return:
        """

        k = 0
        while k < len(self.points):

            x = self.points[k].x
            if x == pointRouge.x:
                del self.points[k]
                continue

            if x > pointRouge.x:
                break

            y = self.points[k].y
            if y < pointRouge.y:
                del self.points[k]
                continue

            k += 1

        self.points.insert(k, pointRouge)

    def incr_red_monster(self, emplacement):

        self.clear_monster(emplacement)

        point_rouge = PointRouge(emplacement.get_xw(), emplacement.get_yh(), emplacement.position)
        self.incr_etoile_verte(point_rouge)

    def locations_monster(self, item, pos):

        w = item.w
        h = item.h

        x_max_rot = self.widthPlates - h
        y_max_rot = self.heightPlates - w
        x_max = self.widthPlates - w
        y_max = self.heightPlates - h

        emplacements = []
        n = len(self.points)

        if n > 0:

            # TODO gérer le x qui avance
            (x, y) = (0, self.points[0].y)
            if y <= y_max:
                emplacements.append(Emplacement(item, x, y, False, pos))

            if y <= y_max_rot:
                emplacements.append(Emplacement(item, x, y, True, pos))

            k = 0
            # TODO Etre plus précis sur la sélection de x et limiter les emplacements
            while k < n - 1 and x < max(x_max, x_max_rot):
                (x, y) = (self.points[k].x, self.points[k + 1].y)

                if y < y_max:
                    emplacements.append(Emplacement(item, x, y, False, pos))

                if y < y_max_rot:
                    emplacements.append(Emplacement(item, x, y, True, pos))

                k += 1

            (x, y) = (self.points[-1].x, 0)

            # TODO (Idea) on pourrait maintenir pour chaque item une liste d'emplacements possibles
            if x <= x_max:
                emplacements.append(Emplacement(item, x, y, False, pos))

            if x <= x_max_rot:
                emplacements.append(Emplacement(item, x, y, True, pos))

            return emplacements

        return [Emplacement(item, 0, 0, False, pos), Emplacement(item, 0, 0, True, pos)]
