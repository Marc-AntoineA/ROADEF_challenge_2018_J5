

import numpy as np
import pandas as pd

df = pd.read_csv("A0_defects.csv", sep=";")
x = df
#x = df.loc[df['PLATE_ID'] == 3]
plt.scatter(x['X'], x['Y'])
plt.show()