import os

instances = list(range(19, 21))

for i in instances:
    os.system("./toukeute -t 180 -p instances_checker/A{} -o instances_checker/A{}_solution.csv -s 0".format(i, i))