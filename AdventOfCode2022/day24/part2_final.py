import numpy as np

#input_file='./input_test.txt'
input_file='day24/input.txt'

with open(input_file,'r') as f:
    lines=f.read().split('\n')

lines.remove('')

lines

# First line tells initial position of elf, and width size of the area
elf_pos_initial=(-1,lines[0].index('.')-1) #y=-1 and x given
elf_pos_initial

elf_aim_position=(len(lines)-2,lines[-1].index('.')-1)
elf_aim_position

valley_width = len(lines[0])-2
valley_height = len(lines)-2
print(valley_height)
print(valley_width)

# Get initial state of blizzard

blizzard_up_initial=np.zeros((valley_height,valley_width), dtype=bool)
blizzard_down_initial=np.zeros((valley_height,valley_width), dtype=bool)
blizzard_right_initial=np.zeros((valley_height,valley_width), dtype=bool)
blizzard_left_initial=np.zeros((valley_height,valley_width), dtype=bool)
for irow,line in enumerate(lines[1:-1]):
    for icol, char in enumerate(line[1:-1]):
        if char=='^':
            blizzard_up_initial[irow,icol]=True
            continue
        if char=='>':
            blizzard_right_initial[irow,icol]=True
            continue
        if char=='v':
            blizzard_down_initial[irow,icol]=True
            continue
        if char=='<':
            blizzard_left_initial[irow,icol]=True

# In order to find next movement, it is best to find the next blizzard position(s)

def getNextBlizzard(blizzard_up, blizzard_right, blizzard_down, blizzard_left):
    blizzard_up_next = np.zeros_like(blizzard_up)
    blizzard_up_next[0:-1,:] = blizzard_up[1:,:]
    blizzard_up_next[-1,:]=blizzard_up[0,:]

    blizzard_down_next = np.zeros_like(blizzard_down)
    blizzard_down_next[1:,:] = blizzard_down[0:-1,:]
    blizzard_down_next[0,:]=blizzard_down[-1,:]

    blizzard_right_next = np.zeros_like(blizzard_right)
    blizzard_right_next[:,1:] = blizzard_right[:,0:-1]
    blizzard_right_next[:,0]=blizzard_right[:,-1]

    blizzard_left_next = np.zeros_like(blizzard_left)
    blizzard_left_next[:,0:-1] = blizzard_left[:,1:]
    blizzard_left_next[:,-1]=blizzard_left[:,0]

    return (blizzard_up_next, blizzard_right_next, blizzard_down_next, blizzard_left_next)

def printBlizzardMap(blizzard_up, blizzard_right, blizzard_down, blizzard_left):
    irows,icols = blizzard_up.shape
    blizzards_total = blizzard_up.astype(np.uint8)+blizzard_right.astype(np.uint8)+blizzard_down.astype(np.uint8)+blizzard_left.astype(np.uint8)
    print('#'*(icols+2))
    for irow in range(irows):
        s='#'
        for icol in range(icols):
            bl_tot0=blizzards_total[irow,icol]
            if bl_tot0==0:
                s+='.'
                continue
            if bl_tot0>1:
                s+=str(blizzards_total[irow,icol])
                continue
            #Only one blizzard, put the right character
            if blizzard_up[irow,icol]:
                s+='^'
                continue
            if blizzard_right[irow,icol]:
                s+='>'
                continue
            if blizzard_down[irow,icol]:
                s+='v'
                continue
            if blizzard_left[irow,icol]:
                s+='<'
                continue
        s+='#'
        print(s)
    print('#'*(icols+2)) 

def get_manhatan_distance(vector0, vector1):
    v1=np.array(vector0)
    v2=np.array(vector1)

    return np.sum(np.abs(v1-v2))

# Check Manh distance from start to end, for curiosity

no_blizz_distance= get_manhatan_distance(elf_pos_initial,elf_aim_position)
no_blizz_distance

blizz_start = (blizzard_up_initial, blizzard_right_initial, blizzard_down_initial, blizzard_left_initial)

nmins1=286
nmins2=255

import time
def get_minimum_minutes_from_downagain(elf_loc, blizz_state, n_cur_minutes, elf_aim_pos, progress0,progressDiv):
    global valley_width
    global valley_height
    global global_minmins
    global global_max_mins
    global global_elfpos_mins_history
    global t0
    
    t1=time.time()
    if t1-t0>60: #Check progress every minute
        print(f"elf_loc:{elf_loc}, n_cur_minutes:{n_cur_minutes}, global_minmins:{global_minmins}, progress:{progress0}")
        t0=t1
    #print(f"elf_loc:{elf_loc}, n_cur_minutes:{n_cur_minutes}, global_minmins:{global_minmins}, progress:{progress0}")

    mdist = get_manhatan_distance(elf_loc,elf_aim_pos)

    if mdist==1:
        #Next move is to exit, regardless of blizzard
        print(f"Exit after:{n_cur_minutes+1}")
        print(f"elf_loc:{elf_loc}, n_cur_minutes:{n_cur_minutes}, global_minmins:{global_minmins}, progress:{progress0}")
        return n_cur_minutes+1

    if (elf_loc,n_cur_minutes) in global_elfpos_mins_history:
        #already checked from this position
        return None
    else:
        global_elfpos_mins_history.append( (elf_loc,n_cur_minutes) )

    #Check current position against the global minmins
    if not global_minmins is None:
        if n_cur_minutes+get_manhatan_distance(elf_loc,elf_aim_pos) > global_minmins:
            #No point trying to find a route as it will take more minutes than minimum set
            return None
    
    if n_cur_minutes>global_max_mins:
        return None
        
    #Update blizzard to new state
    blizz_next = getNextBlizzard(*blizz_state)

    #with this new blizzard check possible movements
    next_positions=[]
    #Preference down or right

    bl0,bl1,bl2,bl3= blizz_next
    #Blizard any
    bl_any = bl0 | bl1 | bl2 | bl3

    if elf_loc[0]<0:
        #Starting position: only move down or stationary allowed
        pos1= (elf_loc[0]+1,elf_loc[1])
        next_positions.append(pos1)

        pos1= (elf_loc[0],elf_loc[1])
        next_positions.append(pos1)
    elif elf_loc[0]==valley_height:
        #Starting position from bottom: only move up or stationary allowed
        pos1= (elf_loc[0]-1,elf_loc[1])
        next_positions.append(pos1)

        pos1= (elf_loc[0],elf_loc[1])
        next_positions.append(pos1)
    else:
         #Check right
        if elf_loc[1]<valley_width-1:
            pos1= (elf_loc[0],elf_loc[1]+1)
            if not bl_any[pos1[0], pos1[1]]:
                next_positions.append(pos1)

        #Check down
        if elf_loc[0]<valley_height-1:
            pos1=(elf_loc[0]+1,elf_loc[1])
            if not bl_any[pos1[0], pos1[1]]:
                next_positions.append(pos1)

        #Check stationary
        pos1= (elf_loc[0],elf_loc[1])
        if not bl_any[pos1[0], pos1[1]]:
            next_positions.append(pos1)
        
        #Check left
        if elf_loc[1]>0:
            pos1= (elf_loc[0],elf_loc[1]-1)
            if not bl_any[pos1[0], pos1[1]]:
                next_positions.append(pos1)
        
        #Check up
        if elf_loc[0]>0:
            pos1=(elf_loc[0]-1,elf_loc[1])
            if not bl_any[pos1[0], pos1[1]]:
                next_positions.append(pos1)

    #At this point next_positions should have all possible positions to try if any
    if len(next_positions)==0:
        #No movements of stationary is possible, returning None
        #print("No movements")
        return None
    else:
        n_cur_minutes0=n_cur_minutes+1 #for next iteration
        progressDiv1=progressDiv/len(next_positions)
        progress1=progress0
        for nextpos0 in next_positions:
            mins=get_minimum_minutes_from_downagain(nextpos0, blizz_next, n_cur_minutes0, elf_aim_pos, progress1, progressDiv1)

            #Process mins
            if not mins is None:
                if global_minmins is None:
                    global_minmins=mins
                else:
                    global_minmins=min(global_minmins,mins)
            progress1+=progressDiv1
        return global_minmins

bliz3=blizz_start

for i in range(nmins1+nmins2):
    bliz3=getNextBlizzard(*bliz3)
print(elf_pos_initial)
print(elf_aim_position)

global_minmins=None
global_max_mins= 319
print(f"global_max_mins:{global_max_mins}")
global_elfpos_mins_history=[]

progress=0
t0=time.time()
nmins3=get_minimum_minutes_from_downagain(elf_pos_initial,bliz3,0, elf_aim_position,0.0,100.0)
print(nmins3)

 


