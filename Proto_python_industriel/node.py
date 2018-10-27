# coding : utf8

import cv2


font = cv2.FONT_HERSHEY_SIMPLEX
colors_stacks = [(255, 62, 150), (205, 105, 201), (0, 0, 255),
          (188, 210, 238), (0, 206, 209), (0, 205, 102),
          (255, 255, 0), (128, 128, 0), (255, 165, 0),
          (255, 140, 0), (255, 99, 71), (170, 170, 170)]
BRANCH = -1
WASTE = -2
RESIDUAL = -3


class Node:

    def __init__(self, x, y, w, h, depth, sons, t, items=[]):
        self.sons = sons
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.type = t
        self.depth = depth
        if len(items) > 0:
            self.gen_node(items)

    def draw(self, img, instance, r=10):
        """
        Ajoute un noeud sur l'image img
        :param img:
        :param instance:
        :param r:
        :return:
        """

        # Placement des pièces = couleur + id
        if self.type > -1:
            img = cv2.rectangle(img, (int(self.x // r), int(self.y // r)),
                                (int((self.x + self.w) // r), int((self.y + self.h) // r)),
                                colors_stacks[min(11, instance.get_stack(self.type))], -1)
            cv2.putText(img, str(self.type), (int(self.x // r), int((self.y + self.h // 2) // r)), font, 1, (255, 255, 255), 2,
                        cv2.LINE_AA)

        # Dessin du contour (en rouge)
        img = cv2.rectangle(img, (int(self.x // r), int(self.y // r)), (int((self.x + self.w) // r), int((self.y + self.h) // r)), (255, 0, 0), 2)

        for node in self.sons:
            img = node.draw(img, instance, r)

        return img

    def gen_node(self, emplacements):
        """
        Génére un arbre à partir d'une liste d'items au format suivant
        -2 branch, -1 waste, -3 residual
        :param emplacements:
        :return:
        """

        if self.w < 20 or self.h < 20:
            raise Exception("Waste too small")

        n = len(emplacements)
        if n == 0:
            self.type = WASTE
            return

        if n == 1 and \
            emplacements[0].x == self.x and emplacements[0].y == self.y and \
            emplacements[0].get_w() == self.w and \
                emplacements[0].get_h() == self.h:

            self.type = emplacements[0].get_id()
            return

        if self.depth > 3:
            # print(node)
            raise Exception("Tree too depth")

        if self.depth % 2 == 0:  # découpe verticale
            # + 0.1 pour faire en sorte que la pièce commence juste après la découpe
            cuts_available = [(emplacement.x + .1, -1) for emplacement in emplacements] + \
                [(emplacements[k].get_xw(), k) for k in range(n)]
            cuts_available.sort()
            cuts = []
            opened = 0
            max_item = -1
            n_items = 0

            for x in cuts_available:
                if opened == 0 and max_item + 1 == n_items:
                    cuts.append((int(x[0]), n_items))

                if x[1] == -1:
                    opened += -1
                else:
                    opened += 1
                    max_item = max(max_item, x[1])
                    n_items += 1
                    if opened == 0 and max_item + 1 == n_items:
                        cuts.append((int(x[0]), n_items))

            xprev = self.x
            if self.depth == 0:
                for (x, ni) in cuts:
                    if ni > 0 and (0 < x - xprev < 100 or x - xprev > 3500):
                        raise Exception("1-cut constrain violated")

                    xprev = x

            xprev = self.x
            niprev = 0
            for (x, ni) in cuts:
                try:
                    # print(x)
                    # print(items[niprev:ni])
                    # print(niprev, ni)
                    if x != xprev:
                        self.sons.append(
                            Node(xprev, self.y, x - xprev, self.h, self.depth + 1, [], BRANCH, emplacements[niprev:ni]))
                    xprev = x
                    niprev = ni

                except Exception as error:
                    raise error

            if self.x + self. w != xprev:
                self.sons.append(
                    Node(xprev, self.y, self.x + self.w - xprev, self.h, self.depth + 1, [], BRANCH, []))

            return

        else:  # découpe horizontale
            # + 0.1 pour faire en sorte que la pièce commence juste après la découpe
            cuts_available = [(emplacements[k].y + 0.1, -1) for k in range(n)] + \
                [(emplacements[k].get_yh(), k) for k in range(n)]
            cuts_available.sort()

            cuts = []
            opened = 0
            max_item = -1
            n_items = 0
            for x in cuts_available:
                if opened == 0 and max_item + 1 == n_items:
                    cuts.append((int(x[0]), n_items))

                if x[1] == -1:
                    opened += -1
                else:
                    opened += 1
                    max_item = max(max_item, x[1])
                    n_items += 1
                    if opened == 0 and max_item + 1 == n_items:
                        cuts.append((x[0], n_items))
                    # le reste, il n'y a pas de défauts

            # print(cuts)
            yprev = self.y
            if self.depth == 0:
                for (y, ni) in cuts:
                    if ni > 0 and 0 < y - yprev < 100:
                        raise Exception("2-cut constrain violated")

                    yprev = y
                    niprev = ni

            check_trimming = 0
            yprev = self.y
            niprev = 0
            for (y, ni) in cuts:
                try:
                    # print(node)
                    if y != yprev:
                        self.sons.append(
                            Node(self.x, yprev, self.w, y - yprev, self.depth + 1, [], BRANCH, emplacements[niprev:ni]))
                        check_trimming += 1
                    yprev = y
                    niprev = ni

                except Exception as error:
                    raise error

            if self.y + self.h != yprev:
                check_trimming += 1
                self.sons.append(
                    Node(self.x, yprev, self.w, self.y + self.h - yprev, self.depth + 1, [], BRANCH, []))

            if self.depth == 3 and check_trimming > 2:
                raise Exception("Trimming failed")
            return

    def to_matrix(self, mat, item_id, parent, bin_id):
        """
        :param mat:
        :param item_id:
        :param parent:
        :param bin_id:
        :return:
        """

        mat.append([int(bin_id), int(item_id), int(self.x), int(self.y), int(self.w),
                    int(self.h), int(self.type), int(self.depth), parent])

        current_id = item_id
        for son in self.sons:
            current_id = son.to_matrix(mat, current_id + 1, item_id, bin_id)

        return current_id
