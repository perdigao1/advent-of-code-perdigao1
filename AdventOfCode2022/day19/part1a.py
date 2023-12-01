# %%
import numpy as np

# %%
#file_input = 'input_test.txt'
file_input = 'day19/input.txt'

# %%
with open(file_input) as f:
    filestr = f.read()

# %% [markdown]
# Collect information from file

# %%
blupr_str = filestr.split("Blueprint")

# %%
ores = ['geode', 'obsidian', 'clay','ore']

# %%
ores_dict = {'geode':0, 'obsidian':1, 'clay':2,'ore':3}
#ores_dict_inv = {0:'geode'}

# %%
blueprints={}

for b0 in blupr_str:

    if b0!='':
        #Initialise blueprint
        blueprint = [0,0,0,0]
        # for o0 in ores:
        #     blueprint[o0]={}
        
        b0_split = b0.split(':')
        #print(f"b0_split:{b0_split}")
        bp_number = int(b0_split[0])
        bp_key = bp_number
        bp_value={}

        b_rest = b0_split[1]

        b_details_split = b_rest.split('.')
        #print(f"b_details_split:{b_details_split}")

        for b1 in b_details_split:
            b1=b1.strip('\n')
            if 'costs' in b1:
                # #Initialise costs
                # robot_costs = {}
                # for o0 in ores:
                #     robot_costs[o0]=0

                b2=b1.split('costs')
                #print(f"b2:{b2}")

                brobot = ''
                irobot = 0
                b2_robot = b2[0]
                if 'ore' in b2_robot:
                    brobot = 'ore'
                    irobot=3
                if 'clay' in b2_robot:
                    brobot = 'clay'
                    irobot=2
                if 'obsidian' in b2_robot:
                    brobot = 'obsidian'
                    irobot=1
                if 'geode' in b2_robot:
                    brobot = 'geode'
                    irobot=0
                
                b2_costs = b2[1]

                b2_costs_split = b2_costs.split('and')
                #print(f"b2_costs_split:{b2_costs_split}")

                robot_costs = np.zeros((4),dtype=np.int32)
                for b3 in b2_costs_split:

                    cost_key=''
                    cost_value=0

                    b3_split = b3.split(' ')
                    #print(f"b3_split:{b3_split}")
                    for b4 in b3_split:
                        if b4!='':
                            if b4.isnumeric():
                                cost_value=int(b4)
                            else:
                                if 'ore' in b4:
                                    cost_key = 'ore'
                                if 'clay' in b4:
                                    cost_key = 'clay'
                                if 'obsidian' in b4:
                                    cost_key = 'obsidian'
                                if 'geode' in b4: #This may not be necessary
                                    cost_key = 'geode'
                    if cost_key!='':
                        robot_costs[ores_dict[cost_key]] = cost_value

                #blueprint[]= robot_costs
                blueprint[irobot] = robot_costs
        
            blueprints[bp_number] = blueprint


# %% [markdown]
# # Optimizer 2

# %%
#This is not working well
def getOptionsNext(blueprint, robots_count, materials_count):
    #Returns a index of robot to buy or None for not buying

    #Check which robots I can buy with the materials I have
    canIBuy=np.zeros((4),dtype=bool)
    for ibp, bpitem in enumerate(blueprint):
        canIBuy[ibp] = np.all( (materials_count-bpitem)>=0)
    #print(f"canIBuy: {canIBuy}")

    buylist = list( np.nonzero(canIBuy)[0] )
    buylist.append(None)
    #print(f"buylist:{buylist}")

    return buylist


# %%
#Global variables
maxgeode_global = 0
already_calculated={}

# %%
def maxAtMinute2(blueprint, robots_count, materials_count, minute, maxminutes):
    #print(f"** {minute} mins, robots_count: {robots_count}, materials_count:{materials_count}")
    global maxgeode_global
    global already_calculated
    
    #Check if this state has already been calculated
    if (*robots_count, *materials_count, minute) in already_calculated:
        return already_calculated[(*robots_count, *materials_count, minute)]

    #Check if there is any chance to get a better result. Number of geode robots and geode materials
    minleft = maxminutes-minute
    maxgeodes_possible = materials_count[0] +1+ (2*(robots_count[0]+1)+minleft)*(minleft+1)/2
    if maxgeodes_possible<maxgeode_global:
        return 0 #Get out if it is not possible to do better

    maxgeodes = 0 #default
    
    
    buylist = getOptionsNext(blueprint, robots_count, materials_count)
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
        maxgeode0 = maxAtMinute2(blueprint,robots_count1,materials_count1,minute1, maxminutes)
        #print(f"{minute} mins, maxgeode0: {maxgeode0}")

        maxgeodes= max(maxgeodes, maxgeode0)

    if maxgeodes>maxgeode_global:
        #print(f"{minute} min, new geode max: {maxgeodes}")
        maxgeode_global=maxgeodes
        
    already_calculated[(*robots_count, *materials_count, minute)] = maxgeodes

    return maxgeodes

# %%
def getMaxGeodesForBlueprint(blueprint):
    global already_calculated
    global maxgeode_global
    #reset
    maxgeode_global=0
    already_calculated={}

    robots_count_init = np.array([0,0,0,1])
    materials_count_init = np.array([0,0,0,0])
    return maxAtMinute2(blueprint, robots_count_init, materials_count_init, 1, 24)


# %% [markdown]
# # Calculate for all blueprints

# %%
bp_scores = []
values = []
for bp_number, bp in blueprints.items():
    print(f"bp_number:{bp_number}, blueprint:{bp}")
    bpmax= getMaxGeodesForBlueprint(bp)
    values.append(bpmax)
    score = bp_number*bpmax
    bp_scores.append(score)
    print(f"bpmax:{bpmax}, score:{score}")

# %%
answer = np.sum( np.array(bp_scores))
print(answer)




