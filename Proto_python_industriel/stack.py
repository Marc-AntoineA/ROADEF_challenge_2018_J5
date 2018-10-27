# coding: utf8

from random import randint


class Stack:
    """
    Pile personnalisée afin de ne pas supprimer les items quand on rajoute ou en enlève des éléments
    """

    def __init__(self, L):
        self.list = L
        self.n = len(L)
        self.begin = 0  # premier élément
        self.end = self.n  # juste après le dernier

    def pop(self):
        """
            Renvoie l'élément en tête et avance l'item
        """
        if not self.is_empty():
            self.begin += 1
            return self.list[self.begin - 1]

    def is_empty(self):
        return self.end - self.begin == 0

    def push(self):
        """
            Ajoute artificiellement l'élément en tête
            (décale juste le pointeur begin)
        """
        if self.begin > 0:
            self.begin -= 1

    def reset(self, begin=0, end=-1):
        self.begin = begin

        if end == -1:
            self.end = self.n
        else:
            self.end = end

    def top(self):
        if not self.is_empty():
            return self.list[self.begin]
        else:
            return -1

    def __str__(self):
        return str(self.list[0:self.begin - 1]) + " : " + str(self.list[self.begin:
                                                              ])

def random_seq(stacks):
    """
    Génère une séquence aléatoire vérifiant l'ordre sur les stacks
    :param stacks: Stacks de type stack
    :return: séquence d'objets ([1, 4, 5,...])
    """
    seq = []

    for k in range(len(stacks)):
        stacks[k].reset()
    n = len(stacks)
    k = 0
    while k < sum([s.n for s in stacks]):
        p = randint(0, n - 1)
        if not stacks[p].is_empty():
            k += 1
            seq.append(stacks[p].pop())

    for k in range(n):
        stacks[k].reset()
    return seq


def random_stack(stacks_allowed):
    """
        * stacks est une liste de Stack
        * stacks_allowed est une liste des id stacks non vide [0, 2] si seulement les stacks 0 et 2 sont
            non-vide.
    """
    n = len(stacks_allowed)
    if len(stacks_allowed) == 0:
        return -1
    return stacks_allowed(randint(0, n - 1))
