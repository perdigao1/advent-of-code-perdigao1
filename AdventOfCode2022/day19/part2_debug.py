# %%
import numpy as np

def getOptionsNext_opt(blueprint, robots_count, materials_count, bpmaxcost):
    #Returns a index of robot to buy or None for not buying

    #Check which robots I can buy with the materials I have
    canIBuy=np.zeros((4),dtype=bool)
    for ibp, bpitem in enumerate(blueprint):
        canIBuy[ibp] = np.all( (materials_count-bpitem)>=0)

        if canIBuy[ibp] and ibp!=0:
            #if materials_count[ibp] >= ( bpmaxcost[ibp] * 2 ):
            if robots_count[ibp] > bpmaxcost[ibp]+1:
                #No point buying this robot because there are enough robots already to be able to create materials per day for making any other robot
                canIBuy[ibp]=False
        
    #print(f"canIBuy: {canIBuy}")

    buylist = list( np.nonzero(canIBuy)[0] )
    buylist.append(None)
    #print(f"buylist:{buylist}")

    return buylist

def maxAtMinute3(blueprint, robots_count, materials_count, minute, maxminutes, bpmaxcost):
    #print(f"** {minute} mins, robots_count: {robots_count}, materials_count:{materials_count}")
    global maxgeode_global
    global already_calculated
    
    #Check if this state has already been calculated
    if (*robots_count, *materials_count, minute) in already_calculated:
        return already_calculated[(*robots_count, *materials_count, minute)]

    #Check if there is any chance to get a better result. Number of geode robots and geode materials
    minleft = maxminutes-minute
    #sum(i, from k to k+m) = 1/2*(2k+m)(m+1)
    # replace k with robot_count+1, and m with number of minutes
    maxgeodes_possible = materials_count[0] +1+ (2*(robots_count[0]+1)+minleft+1)*(minleft+2)//2
    if maxgeodes_possible<maxgeode_global:
        return materials_count[0] #Get out if it is not possible to do better

    maxgeodes = 0 #default

    # Check if number of robots per material producing for geode is already ok to just create geode robots every minute
    mat_for_geode = blueprint[0]
    if np.all(robots_count>=mat_for_geode) and np.all(materials_count>=mat_for_geode):
        #No need to continue
        #Number of geodes that will be produced will be
        # previous + produced in this minute (ngeoderobots) + (next minute ngeoderobots+1) + ...+ (next minute ngeoderobots+minleft)
        #sum(i, from k to k+m) = 1/2*(2k+m)(m+1)
        maxgeodes = materials_count[0] + (2*robots_count[0]+minleft)*(minleft+1)//2
        return maxgeodes
    
    buylist = getOptionsNext_opt(blueprint, robots_count, materials_count, bpmaxcost)
    #print(f"buylist:{buylist}")
    
    maxgeode0=0

    for buyrobot0 in buylist:
        materials_count1 = materials_count.copy() #Hopefully, np array copy properly
        robots_count1 = robots_count.copy()
        #print(f"Buying robot {buyrobot0}.")

        if not buyrobot0 is None:
            #Test materials that will be left if we buy the robot
            materials_count1 -= blueprint[buyrobot0]
    
            #print(f"After buying this robot the materials left is {materials_count1}")

        #Update materials based in the number of robots
        materials_count1+= robots_count
        #print(f"Robots collect materials, leading to {materials_count1}")
    
        #Update robot count at end of minute if there was a purchase
        if not buyrobot0 is None:
            robots_count1[buyrobot0]+=1
            #print(f"New robot acquired, robot_count1 is now {robots_count1}")

        minute1 = minute+1
        #Check if reached maximum number of minutes
        if minute1>maxminutes:
            return materials_count1[0]

        #print(f"Entering {minute1} mins...")
        maxgeode0 = maxAtMinute3(blueprint,robots_count1,materials_count1,minute1, maxminutes, bpmaxcost)
        #print(f"{minute} mins, maxgeode0: {maxgeode0}")

        maxgeodes= max(maxgeodes, maxgeode0)

    if maxgeodes>maxgeode_global:
        #print(f"{minute} min, new geode max: {maxgeodes}")
        maxgeode_global=maxgeodes
        
    already_calculated[(*robots_count, *materials_count, minute)] = maxgeodes

    return maxgeodes

def getMaxCost(blueprint):
    costs_np = np.array(blueprint)
    max_cost = np.max(costs_np, axis=0) #Maximum along column
    return max_cost

def getMaxGeodesForBlueprint3(blueprint, minutes):
    global already_calculated
    global maxgeode_global
    #reset
    maxgeode_global=0
    already_calculated={}

    bpmaxcost = getMaxCost(blueprint)
    #print(f"bpmaxcost:{bpmaxcost}")

    robots_count_init = np.array([0,0,0,1])
    materials_count_init = np.array([0,0,0,0])
    return maxAtMinute3(blueprint, robots_count_init, materials_count_init, 1, minutes, bpmaxcost)



blueprint = [np.array([0, 7, 0, 2]),
  np.array([ 0,  0, 14,  3]),
  np.array([0, 0, 0, 2]),
  np.array([0, 0, 0, 4])]

# minutes=32
# exp_result = 56
# print ( "opt3,",getMaxGeodesForBlueprint3( blueprint , minutes), f", expected {exp_result}" )


# #Test minute 5 decision
# maxgeode_global=0
# already_calculated={}
# bpmaxcost = getMaxCost(blueprint)
# robots_count_init = np.array([0,0,0,1])
# materials_count_init = np.array([0,0,0,4])
# minutes=32
# minute=5 #from this minute
# maxAtMinute3(blueprint, robots_count_init, materials_count_init, minute, minutes, bpmaxcost)

#Test minute 5 decision
maxgeode_global=0
already_calculated={}
bpmaxcost = getMaxCost(blueprint)
robots_count_init = np.array([0,0,6,2])
materials_count_init = np.array([0,0,15,3])
minutes=32
minute=13 #from this minute. ok:25,22, 18, 14, x10, x12, 13
#Something wrong is happening from minute 13. OK now
print(maxAtMinute3(blueprint, robots_count_init, materials_count_init, minute, minutes, bpmaxcost))
