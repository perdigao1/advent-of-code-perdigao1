import numpy as np

#input
#depth = [199 , 200, 208, 210, 200, 207, 240,269,260,263]

depth = np.loadtxt("input.txt")

print("Input")
print(depth)

d0 = depth[0]

nincreases = 0

for i in range(1,len(depth)):
    di = depth[i]
    if di>d0:
        nincreases+=1
    d0=di

print(f"nincreases = {nincreases}")