from item import *


class Emplacement:

    def __init__(self, item, x, y, rotated, position):
        self.item = item
        self.x = x
        self.y = y
        self.rotated = rotated
        self.position = position  # position dans la séquence

    def get_w(self):
        return self.item.get_w(self.rotated)

    def get_h(self):
        return self.item.get_h(self.rotated)

    def get_id(self):
        return self.item.id

    def get_xw(self):
        return self.x + self.get_w()

    def get_yh(self):
        return self.y + self.get_h()

    def get_stack(self):
        return self.item.id_stack

    def __str__(self):
        return "{} : ({}, {}) - {}, {}".format(str(self.item), self.x, self.y, self.rotated, self.position)