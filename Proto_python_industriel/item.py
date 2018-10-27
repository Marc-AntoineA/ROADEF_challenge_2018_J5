class Item:

    def __init__(self, w, h, id_item, id_stack):
        self.w = w
        self.h = h
        self.id = id_item
        self.id_stack = id_stack

    def get_w(self, rotated):
        return self.h if rotated else self.w

    def get_h(self, rotated):
        return self.w if rotated else self.h

    def get_area(self):
        return self.w * self.h

    def __str__(self):
        return "({}, {}, {})".format(self.id, self.w, self.h)
