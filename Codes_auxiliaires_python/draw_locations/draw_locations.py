import re
from matplotlib import pyplot as plt
import cv2
import numpy as np

input_file = "output14.txt"

regex = "(\d*) : \((\d*), (\d*), (\d*), (\d*)\)"

rects = []
file = open(input_file)
for line in file.readlines():
    result = re.match(regex, line)
    if not result is None:
        rect = [result.group(1), result.group(2), result.group(3), result.group(4), result.group(5)]
        rect = [int(r) for r in rect]
        rects.append(rect)
        
r = 10
width_plates = 6000
height_plates = 3210
font = cv2.FONT_HERSHEY_SIMPLEX

img = np.zeros((height_plates//r, width_plates//r, 3), np.uint8)

for rect in rects[:]:
    [id, x, y, xw, yh] = rect
    img = cv2.rectangle(img, (int(x//r), int(y//r)), 
                            (int(xw//r), int(yh//r)), 
                            (125, 0, 0), -1)
    img = cv2.rectangle(img, (int(x//r), int(y//r)), (int((xw)//r), int((yh)//r)), (255, 0, 0), 2)
    cv2.putText(img, str(id), (int((x)//(r)), int((y + yh)//2)//r) , font, 1, (255, 255, 255), 2, cv2.LINE_AA)
        
plt.imshow(img)
plt.xticks(np.arange(0, width_plates + 1, 1000)//r, np.arange(0, width_plates + 1, 1000))
plt.yticks(np.arange(0, height_plates + 1, 500)//r, np.arange(0, height_plates + 1, 500))
plt.show()