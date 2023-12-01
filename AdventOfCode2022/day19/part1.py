import numpy as np

file_input = 'day19/input_test.txt'
#file_input = 'day19/input.txt'

with open(file_input) as f:
    filestr = f.read()

ores_dict = {'geode':0, 'obsidian':1, 'clay':2,'ore':3}


blupr_str = filestr.split("Blueprint")
blueprints={}

for b0 in blupr_str:

    if b0!='':
        #Initialise blueprint
        blueprint = [0,0,0,0]
        # for o0 in ores:
        #     blueprint[o0]={}
        
        b0_split = b0.split(':')
        print(f"b0_split:{b0_split}")
        bp_number = int(b0_split[0])
        bp_key = bp_number
        bp_value={}

        b_rest = b0_split[1]

        b_details_split = b_rest.split('.\n')
        print(f"b_details_split:{b_details_split}")

        for b1 in b_details_split:
            if 'costs' in b1:
                # #Initialise costs
                # robot_costs = {}
                # for o0 in ores:
                #     robot_costs[o0]=0

                b2=b1.split('costs')
                print(f"b2:{b2}")

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
                print(f"b2_costs_split:{b2_costs_split}")

                robot_costs = np.zeros((4),dtype=np.int32)
                for b3 in b2_costs_split:

                    cost_key=''
                    cost_value=0

                    b3_split = b3.split(' ')
                    print(f"b3_split:{b3_split}")
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


#Assume materials_count and robots count is is a numpy array with 4 values
# with the order 'geode', 'obsidian', 'clay','ore'

#robot_buy_order = ['geode', 'obsidian', 'clay','ore', None]
robot_buy_order_i = [0, 1, 2,3, None]
maxgeode_global = 0

def maxAtMinute(blueprint, robots_count, materials_count, minute, maxminutes):
    #print(f"** {minute} mins, robots_count: {robots_count}, materials_count:{materials_count}")
    maxgeodes = 0 #default
    
    global maxgeode_global
    
    #for robot_type in robot_buy_order:
    for robot_type in robot_buy_order_i:
        #print(f"Checking if can buy {robot_type}")

        maxgeode0=0
        materials_count1 = materials_count.copy() #Hopefully, np array copy properly
        robots_count1 = robots_count.copy()

        if not robot_type is None:
            #Check if materials are enough to buy the robot
            robot_costs = blueprint[robot_type]

            #Test materials that will be left if we buy the robot
            materials_count1 -= robot_costs
            
            #If all the materials count is positive then we can buy the robot

        #print(f"If we could use this {robot_type} robot, the materials left would be {materials_count1}")
        bCanBuy=True
        if np.min(materials_count1)<0:
            #print(f"Cannot buy this robot: {robot_type}")
            pass
        else:
            #print("Can buy")
            #Go to next day
            #Update materials with robot collection values
            #Each robot collects one of each material
            materials_count1+= robots_count

            #print(f"Robots collect materials, so we are left with {materials_count1}")
            
            if not robot_type is None:
                #robots_count1[ores_dict[robot_type]]+=1
                robots_count1[robot_type]+=1
                
                #print(f"New robot acquired, robot_count1 is now {robots_count1}")

            minute1 = minute+1
            #Check if reached maximum number of minutes
            if minute1>maxminutes:
                return materials_count1[0]

            #print(f"Entering {minute1} mins...")
            maxgeode0 = maxAtMinute(blueprint,robots_count1,materials_count1,minute1, maxminutes)
            #print(f"{minute} mins, maxgeode0: {maxgeode0}")

            maxgeodes = max(maxgeodes, maxgeode0)
            
            if maxgeodes>maxgeode_global:
                print(f"Min {minute} min, new geode max: {maxgeodes}")
                maxgeode_global=maxgeodes
            
        
    #print(f"{minute} mins. Checked all options, maxgeodes collected are {maxgeodes}")

    return maxgeodes

robots_count_init = np.array([0,0,0,1])
materials_count_init = np.array([0,0,0,0])
maxgeode_global=0

res = maxAtMinute(blueprints[1], robots_count_init, materials_count_init, 1, 24)
print(f"Final result: {res}")
