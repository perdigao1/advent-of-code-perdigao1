#This code takes forever
#It is too recursive to be timely pratical to complete
#It can take up to 44 to 50 rolls to reach a winning decision

#Maybe I have to think about counting by moving backwards

p1_start = 4
p2_start = 8
#p1_start = 1
#p2_start = 5

winscore = 21

rollvalues = [1,2,3]

p1_wins = 0
p2_wins = 0

def newRollUniverse(p1_pos, p1_score, p2_pos, p2_score, nextrollvalue, rollindex, nrolls=0):
    #rollindex will have values ranging from 0 to 5.
    # 0,1,2 to roll p1 ; 3,4,5 to roll p2
    #When reaching 2, check p1 score
    #when reaching 5 check p2 score

    global p1_wins, p2_wins
    
    #print(f"nrolls = {nrolls}; p1_pos={p1_pos}, p1_score={p1_score}, p2_pos={p2_pos}, p2_score={p2_score}")
    if rollindex<=2:
        p1_pos += nextrollvalue
        if p1_pos>=10:
            p1_pos -=10

        if rollindex==2:
            #Set new score
            p1_score += p1_pos

            #Check if win
            if p1_score >= winscore:
                p1_wins+=1
                print(f"p1 win , p1_wins={p1_wins}")
                print(f"nrolls = {nrolls}; p1_pos={p1_pos}, p1_score={p1_score}, p2_pos={p2_pos}, p2_score={p2_score}")
                return
    else:
        #Assume nextrollindex>2
        p2_pos += nextrollvalue
        if p2_pos>=10:
            p2_pos -=10

        if rollindex==5:
            #Set new score
            p2_score += p2_pos

            #Check if win
            if p2_score >= winscore:
                p2_wins+=1
                #print(f"p2 win , p2_wins={p2_wins}")
                #print(f"nrolls = {nrolls}; p1_pos={p1_pos}, p1_score={p1_score}, p2_pos={p2_pos}, p2_score={p2_score}")
                return
    
    #Setup for new universes
    rollindex0 = rollindex+1
    if rollindex0>5:
        rollindex0=0

    nrolls0 = nrolls+1
    #Recursive
    for irolls in rollvalues:
        newRollUniverse(p1_pos, p1_score, p2_pos, p2_score, irolls, rollindex0, nrolls0)


p1_pos = p1_start
p2_pos = p2_start

for iroll0 in rollvalues:
    print (f"root: iroll0={iroll0}")
    newRollUniverse(p1_start, 0, p2_start, 0 , iroll0, 0, 0)

print(f"p1_wins = {p1_wins} ; p2_wins = {p2_wins}")