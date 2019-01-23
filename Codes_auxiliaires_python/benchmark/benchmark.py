#------------------------------------------------------
# Code pour benchmarker l'algo utilisé pour la ROADEF
#------------------------------------------------------

import shutil
import subprocess
import os
import re

INSTANCE_PATH = '../../checker_instances/instances_checker/'
CHECKER_PATH = '../../checker_instances/'

instance = 'A2'

instance_filename = INSTANCE_PATH + instance

batch_filename = instance_filename + '_batch.csv'
defects_filename = instance_filename + '_defects.csv'

def test_instance(instance, solution_filename):
    shutil.copy2(solution_filename, INSTANCE_PATH + instance + '_solution.csv')
    
    os.chdir(CHECKER_PATH)
    out = subprocess.check_output(['./checker', instance, '0'], shell=False)
    out = out.decode('utf8')
    regex = instance + ' => VALID SOLUTION\\t \(waste,OF\) :  \| (\d*\.\d*) \| (\d*)\\n'
    match = re.search(regex, out)
    if not match:
        return
    score = int(match.group(2))
    surface = float(match.group(1))
    return (score, surface)

solution_filename = '../../Code_cpp/src/build/bin/' + instance + '.csv'    
print(test_instance(instance, solution_filename))
