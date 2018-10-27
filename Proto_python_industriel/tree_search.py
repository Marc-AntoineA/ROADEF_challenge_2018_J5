import numpy as np
from item import *
from monstre_rouge import *
from emplacement import *
from tree import *
from functools import reduce
from copy import deepcopy
import pandas as pd

BLANK_LOCATION = -1


class TreeSearch:

    def __init__(self, instance):

        self.instance = instance
        self.nb_items = self.instance.nb_items()
        self.n_bin = 100
        self.best_score = np.inf
        self.best_items_cuts = []
        self.previous_monsters = [[MonstreRouge() for k in range(self.n_bin)]] + [[] for k in range(self.nb_items)]
        self.current_bin_id = 0
        self.items_cuts = []
        self.depth = 0
        self.score = np.inf
        self.stacks_available = list(range(len(self.instance.stacks)))
        self.current_bin_id = 0
        self.place_perdue = 0
        self.best_max_x = np.inf

        self.x_max = 0
        self.area = 0
        self.monsters = [MonstreRouge() for k in range(self.n_bin)]
        self.items_cuts = [[] for bin_id in range(0, self.n_bin)]

        self.heuristic_params = {
            "weight_x": 100,
            "weight_y": 200,
            "weight_nb_defects": 10,
            "weight_random": 10,
            "weight_size_hull": 100,
            "weight_area": 4,
            "y_complete": 1000,
            "x_complete": 50000
        }

    def heuristic(self, location):

        return self.heuristic_params["weight_x"] * location.get_xw() - \
                self.heuristic_params["weight_y"] * location.get_yh() - \
                self.heuristic_params["y_complete"] * (location.get_yh() - 2900) + \
                self.heuristic_params["x_complete"] * (location.get_xw() - 6700)

    def locations_red_monster_and_free_of_defects(self, bin_id, item, position):
        locations_red_monster = self.monsters[bin_id].locations_monster(item, position)

        if len(locations_red_monster) > 0:
            locations = reduce(lambda x, y: x + y,
                               [self.instance.locs_free_of_defects(bin_id, emplacement) for emplacement in locations_red_monster])
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

    def get_prioritized_locations(self):

        if len(self.stacks_available) == 0:
            return []

        prioritized_locations = []

        # Parcourir tous les items possibles
        for s in self.stacks_available:
            id_item = self.instance.stacks[s].top()
            (w, h) = self.instance.width_height_item(id_item)
            item = Item(w, h, id_item, s)

            locations = self.locations_red_monster_and_free_of_defects(self.current_bin_id, item, self.depth)

            for location in locations:
                try:
                    tree = Tree(self.instance, self.current_bin_id)
                    tree.gen_tree(self.items_cuts[self.current_bin_id] + [location])
                    # TODO gérer les priorités

                    prioritized_locations.append((self.heuristic(location), location))

                except Exception as error:
                    print(error)

        prioritized_locations.sort(key=lambda x: x[0], reverse=True)
        prioritized_locations = [x[1] for x in prioritized_locations]

        if len(prioritized_locations) == 0:
            self.current_bin_id += 1
            self.x_max = 0
            self.compute_place_perdue()
            return self.get_prioritized_locations()

        return prioritized_locations

    def compute_place_perdue(self):
        s = 0
        # On compte large, sur les bins déjà terminés
        for bin_id in range(self.current_bin_id):
            for emplacement in self.items_cuts[bin_id]:
                s += emplacement.item.get_area()

        self.place_perdue = self.current_bin_id * self.instance.widthPlates * self.instance.heightPlates - s

    def compute_max_x(self):
        return max([0] + [emplacement.get_xw() for emplacement in self.items_cuts[self.current_bin_id]])

    def cost_roadef(self):
        return (self.x_max + self.current_bin_id)*6000 - self.area

    def cost_roadef_total(self):

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

    def rollback(self):
        if self.depth < 1:
            raise Exception("Impossible depth in rollback")

        self.depth -= 1
        self.monsters = self.previous_monsters[self.depth]
        location = self.pop_items_cuts()
        self.add_stacks_available(location.get_stack())
        self.x_max = self.compute_max_x()
        self.area -= location.item.get_area()

    def append_items_cuts(self, location):
        self.items_cuts[self.current_bin_id].append(location)

    def pop_items_cuts(self):
        if len(self.items_cuts[self.current_bin_id]) == 0:
            self.current_bin_id -= 1
            self.x_max = self.compute_max_x()
            #self.compute_place_perdue()
        return self.items_cuts[self.current_bin_id].pop()

    def attempt(self, location):
        try:
            self.previous_monsters[self.depth] = deepcopy(self.monsters)
            self.depth += 1
            self.remove_stacks_available(location.get_stack())
            self.append_items_cuts(location)
            self.monsters[self.current_bin_id].incr_red_monster(location)
            self.area += location.item.get_area()
            self.x_max = max(self.x_max, location.get_xw())
        except:
            pass

    def update_best_state(self):
        if self.score < self.best_score:
            self.best_items_cuts = deepcopy(self.items_cuts)
            self.best_score = self.score
            self.best_max_x = self.max_x()

    # TODO (pas ici => gérer le chgt de bin)
    # TODO
    def pruned(self):

        if self.max_x() > self.best_max_x or self.place_perdue > self.best_score:
            return True

        return False

    def solve(self):
        # Pile des emplacements à regarder
        locations_stack = []

        # Initialisation des stacks
        prioritized_locations = self.get_prioritized_locations()
        # Ajout des emplacements dans le bon ordre
        #  TODO : gérer l'ordre
        locations_stack.extend(prioritized_locations)

        # profondeur de l'item courant de 0 à n - 1
        depth = 0

        while len(locations_stack) > 0:  #  il reste des états à parcourir

            # 1. On charge l'emplacement courant
            location = locations_stack.pop()
            if location == BLANK_LOCATION:
                self.rollback()
                continue

            if location.x > self.best_max_x:
                depth -= 1
                continue

            # 2. On mets à jour l'étoile rouge etc. self.attempt renvoie false si pas possible.
            self.attempt(location)

            if self.pruned():
                depth -= 1  # On remonte dans l'arbre
                self.rollback()
                continue

            # 3. Est-ce qu'on est au bout du bout ?
            if self.depth == self.nb_items:
                self.update_best_state()
                print("{} : {}".format(self.score, self.best_score))
                depth -= 1
                self.rollback()
                continue

            # 3. Est-ce ça vaut encore le coup de continuer cet état ?


            # 4. L'emplacement est possible et pas trop mauvais (a priori)
            # On calcule tous les emplacements possibles avec une priorité
            # (plus c'est faible, mieux c'est = heuristique de recherche)
            # les emplacements
            prioritized_locations = self.get_prioritized_locations()

            # On rajoute un emplacement 'rollback'
            # 5.
            locations_stack.append(BLANK_LOCATION)

            # 6. On rajoute les emplacements en les coupant avec la connaissance actuelle du système
            for location in prioritized_locations:
                locations_stack.append(location)

    def search_rec(self, depth):
        if depth < 0:
            return self.cost_roadef()

        score = np.inf
        locations = self.get_prioritized_locations()
        if len(locations) == 0:
            return self.cost_roadef()

        for location in locations:
            self.attempt(location)
            score = min(self.search_rec(depth - 1), score)
            self.rollback()

        return score

    def greedy_rollback(self, max_depth = 4):
        self.current_bin_id = 0
        while len(self.stacks_available) > 0:

            locations = self.get_prioritized_locations()

            best_score = np.inf
            best_location = None

            for location in locations:
                self.attempt(location)
                score = self.search_rec(max_depth)
                if score <= best_score:
                    best_score = score
                    best_location = location
                self.rollback()

            print("Best location ", best_location, best_score)
            self.attempt(best_location)

        print(self.cost_roadef())
        self.to_file()

    def to_file(self):

        def add_rec_mat(node, mat, k, parent, bin_id):
            """
            k est l'id
            """
            mat.append([bin_id, k, int(node.x), int(node.y), int(node.w),
                        int(node.h), int(node.type), int(node.depth), parent])

            c = k
            for s in node.sons:
                c = add_rec_mat(s, mat, c + 1, k, bin_id)

            return c

        mat = []
        c = -1
        for bin_id in range(100):
            if len(self.items_cuts[bin_id]) == 0:
                break

            tree = Tree(self.instance, bin_id)
            tree.gen_tree(self.items_cuts[bin_id])
            c = add_rec_mat(tree.node, mat, c + 1, "", bin_id)

        df = pd.DataFrame(mat, columns = ['PLATE_ID', 'NODE_ID', 'X', 'Y', 'WIDTH', 'HEIGHT', 'TYPE', 'CUT', 'PARENT'])

        df.to_csv(self.instance.path + "{}_{}_solution.csv".format(self.instance.instance_name, self.cost_roadef()), sep=";", index=False)
