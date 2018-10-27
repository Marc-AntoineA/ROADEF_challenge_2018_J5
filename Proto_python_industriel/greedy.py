import numpy as np
from item import *
from instance import *
from monstre_rouge import *
from functools import reduce
from tree import *
from random import randint


class Greedy:

    def __init__(self, instance):
        self.instance = instance
        self.n_bin = 100

        # TODO pourquoi pas un set ?

        instance.reset_stacks()
        self.stacks_available = list(range(len(self.instance.stacks)))
        self.current_bin_id = 0

        self.monstres = [MonstreRouge() for k in range(self.n_bin)]
        self.trees = [Tree(self.instance, bin_id) for bin_id in range(0, self.n_bin)]
        self.items_cuts = [[] for bin_id in range(0, self.n_bin)]

        # Score à minimiser
        self.score = 0
        self.best_score = 0
        self.best_cut = [[] for bin_id in range(0, self.n_bin)]

    def locations_monstre_rouge_and_free_of_defects(self, bin_id, item):
        locations_monstre_rouge = self.monstres[bin_id].locations_monstre(item)

        if len(locations_monstre_rouge) > 0:
            locations = reduce(lambda x, y: x + y,
                               [self.instance.locs_free_of_defects(bin_id, emplacement) for emplacement in locations_monstre_rouge])

            return locations
        return []

    def remove_stacks_available(self, s):
        id_item = self.instance.stacks[s].pop()
        if self.instance.stacks[s].is_empty():
            del self.stacks_available[self.stacks_available.index(s)]
        return id_item

    def add_stacks_available(self, s):
        if self.instance.stacks[s].is_empty():
            self.stacks_available.append(s)
        self.instance.stacks[s].push()

    def cost_roadef(self):

        s = 0
        n = len([i for i in range(len(self.items_cuts)) if len(self.items_cuts[i]) > 0])
        for bin_id in range(n):
            for emplacement in self.items_cuts[bin_id]:
                s += emplacement.item.get_area()

        list_residuals = [emplacement.get_xw() for emplacement in self.items_cuts[n - 1]]

        # Normalement, ça ne devrait pas arriver…
        if len(list_residuals) == 0:
            return 0

        return ((n - 1) * self.instance.widthPlates + max(list_residuals)) * self.instance.heightPlates - s



    # TODO
    def evaluate_objective(self):
        """
        Calcule dynamiquement le score en utilisant self.score
        Met à jour le best score.
        :return:
        """
        self.score = 0

    # TODO
    def pruned(self):
        """
        Détermine quelles directions sont absurdes.
        Renvoie True si elles le sont, False sinon
        :return:
        """
        return False

    def solve(self):

        # Faire attention à ce que les paramètres soient bien resets
        # Todo gérer les itérations et la recherche complète dans l'arbre
        operations_stack = []

        while len(self.stacks_available) > 0:

            best_score = np.inf
            current_score = best_score
            best_stack = -1
            best_emplacement = None

            # Parcourir tous les items possibles
            for s in self.stacks_available:
                id_item = self.instance.stacks[s].top()
                (w, h) = self.instance.width_height_item(id_item, False)
                item = Item(w, h, id_item)

                # Pour chaque item, parcourir tous les emplacements possibles
                emplacements = self.locations_monstre_rouge_and_free_of_defects(self.current_bin_id, item)

                for emplacement in emplacements:

                    try:
                        self.trees[self.current_bin_id].gen_tree(self.items_cuts[self.current_bin_id] + [emplacement])
                        self.items_cuts[self.current_bin_id].append(emplacement)
                        current_score = self.cost_roadef()
                        self.items_cuts[self.current_bin_id].pop(-1)

                    except Exception as error:
                        print(error)

                    if current_score < best_score:
                        best_score = current_score
                        best_stack = s
                        best_emplacement = emplacement

            if best_score == np.inf:
                self.current_bin_id += 1
                print("-------------------------------")
                print("   Changement de bin, ", self.current_bin_id)

            else:
                print(self.score)
                print(best_emplacement)
                self.score = best_score
                self.remove_stacks_available(best_stack)
                self.items_cuts[self.current_bin_id].append(best_emplacement)
                self.monstres[self.current_bin_id].incr_monstre_rouge(best_emplacement)


