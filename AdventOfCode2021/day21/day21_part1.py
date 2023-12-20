# p1_start = 4
# p2_start = 8
p1_start = 1
p2_start = 5

winscore = 1000

#notReached1000 = True

class deterministicDice3TimesTotal():
    def __init__(self, maxvalue=100):
        self.maxvalue = maxvalue
        self.value=maxvalue
    
    def __iter__(self):
        return self
    
    def roll(self):
        self.value+=1
        if self.value>self.maxvalue:
            self.value=1
        return self.value

    def __next__(self):
        retvalue = self.roll() + self.roll() + self.roll()

        nrolls =3

        return retvalue, nrolls

dice = iter(deterministicDice3TimesTotal(100))

boardMax = 10

p1_pos = p1_start
p2_pos = p2_start
p1_score = 0
p2_score = 0
nrolls = 0

while True:
    rollv, rolls = next(dice)
    p1_pos += rollv
    nrolls += rolls
    #get new boardposition (remind that there is no zero position)
    newp1_pos = p1_pos % boardMax
    if newp1_pos==0:
        newp1_pos = 10
    p1_pos = newp1_pos
    p1_score += newp1_pos
    print(f"nrolls={nrolls} ; newp1_pos = {newp1_pos} , p1_score={p1_score}")

    if p1_score>=winscore:
        print(f"p1 wins the game")
        print(f"res = { p2_score*nrolls}")
        break

    rollv, rolls = next(dice)
    p2_pos += rollv
    nrolls += rolls
    #get new boardposition (remind that there is no zero position)
    newp2_pos = p2_pos % boardMax
    if newp2_pos==0:
        newp2_pos = 10
    p2_pos = newp2_pos
    p2_score += newp2_pos
    print(f"nrolls={nrolls} ; newp2_pos = {newp2_pos}, p2_score={p2_score}")

    if p2_score>=winscore:
        print(f"p2 wins the game")
        print(f"res = { p1_score*nrolls}")
        break

