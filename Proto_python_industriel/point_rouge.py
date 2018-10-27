import cv2


class PointRouge:

    def __init__(self, x, y, item_seq):
        self.x = x
        self.y = y
        self.item_seq = item_seq

    def __leq__(self, pointRouge):

        """
        Attention, relation d'ordre uniquement sur le monstre rouge mais pas générale
        """
        return self.x < pointRouge.x or (self.x > pointRouge.x and (self.y < pointRouge.y))

    def __str__(self):
        return "{} : ({}, {})".format(self.item_seq, self.x, self.y)

    def draw(self, img, r=10):
        font = cv2.FONT_HERSHEY_SIMPLEX
        img = cv2.rectangle(img, (0, 0), (int(self.x // r), int(self.y // r)), (0, 0, 255), -1)
        img = cv2.putText(img, str(self.item_seq), (int(self.x // r + 5), int(self.y // r + 5)), font, 1, (255, 255, 255), 2,
                          cv2.LINE_AA)

        return img

    def mange(self, victime):
        """
        Transforme l'objet courant en l'objet ayant absordé 'victime'
        :param victime:
        :return:
        """
        self.item_seq = max(self.item_seq, victime.item_seq)
        self.x = max(self.x, victime.x)
        self.y = max(self.y, victime.y)

    def is_after(self, other):
        return self.item_seq > other.item_seq