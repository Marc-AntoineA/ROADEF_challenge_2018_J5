

class Defect:

    def __init__(self, x, y, w, h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    # TODO
    def __leq__(self, defect):
        return True

    # TODO (mais semble inutile car l'algo du Jupyter Notebook est faux
    def is_before_loc(self, x, y):
        """
        :param x: Abscisse de l'emplacement à tester
        :param y: Ordonnée de l'emplacement à tester
        :return: True si le défaut est avant l'item (en x ou en y)
        """
        return self.x + self.w <= x or self.y + self.h <= y


def loc_not_includes_defects(defects_included, x, y):
    """

    :param defects_included:
    :param x: Abscisse de l'emplacement à tester
    :param y: Ordonnée de l'emplacemnt à tester
    :return: True si l'ensemble des défauts de defects_included sont avant (Defect.is_before_loc) l'item
    """
    i = 0
    while i < len(defects_included) and defects_included[i].is_before_loc(x, y):
        i += 1

    return not(i < len(defects_included))
