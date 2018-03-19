# -*- coding: utf-8 -*-
#contains a small nb of geometric functions

def contient_point(x1, y1, w1, h1, x2, y2):
    return x1 < x2 and x2 < x1 + w1 and \
           y1 < y2 and y2 < y1 + h1
            
def intersecte_rectangle(x1, y1, w1, h1, x2, y2, w2, h2):
    #note : il ne faut pas rajouter d'espace apres les antislash
    return contient_point(x1, y1, w1, h1, x2, y2) or       \
           contient_point(x1, y1, w1, h1, x2 + w2, y2) or  \
           contient_point(x1, y1, w1, h1, x2, y2 + h2) or  \
           contient_point(x1, y1, w1, h1, x2 + w2, y2 + h2)
           
if __name__=='__main__':
    intersecte_rectangle(1 ,1, 1, 1, 0, 0, 1.5, 1.5)
    contient_point(1, 1, 1, 1, 1.5, 1.5)
    intersecte_rectangle(1 ,1, 1, 1, 0, 0, 0.5, 0.5)