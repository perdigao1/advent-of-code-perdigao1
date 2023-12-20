#sliding window sum compare

import numpy as np

#test input
#depth = np.array([199 , 200, 208, 210, 200, 207, 240,269,260,263])

depth = np.loadtxt("day 1/input.txt")

print("Input")
print(depth)

sum3 = depth[0:-2]+depth[1:-1] + depth[2:]

print("sum3")
print(sum3)

nincreases = 0

v0 = sum3[0] #initialise

for i in range(1,len(sum3)):
    vi = sum3[i]
    if vi>v0:
        nincreases+=1
    v0=vi

print(f"nincreases = {nincreases}")