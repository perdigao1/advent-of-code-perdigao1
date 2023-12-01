#with open('inputs/day-09.txt') as file:
#with open('input_test.txt') as file:
with open('input.txt') as file:
    moves = file.read().split('\n')

moves = [x.split(' ') for x in moves]

def moveNext(h, t):
    if t[0] < h[0]-1:
        if t[1] == h[1]:
            t[0] += 1
        elif t[1] > h[1]:
            t[0] += 1
            t[1] -= 1
        elif t[1] < h[1]:
            t[0] += 1
            t[1] += 1
    elif t[0] > h[0]+1:
        if t[1] == h[1]:
            t[0] -= 1
        elif t[1] > h[1]:
            t[0] -= 1
            t[1] -= 1
        elif t[1] < h[1]:
            t[0] -= 1
            t[1] += 1
    elif t[1] < h[1]-1:
        if t[0] == h[0]:
            t[1] += 1
        elif t[0] < h[0]:
            t[0] += 1
            t[1] += 1
        elif t[0] > h[0]:
            t[0] -= 1
            t[1] += 1
    elif t[1] > h[1]+1:
        if t[0] == h[0]:
            t[1] -= 1
        elif t[0] < h[0]:
            t[0] += 1
            t[1] -= 1
        elif t[0] > h[0]:
            t[0] -= 1
            t[1] -= 1

def moveHead(d, h):
    if d == 'R':
        h[0] += 1
    if d == 'L':
        h[0] -= 1
    elif d == 'U':
        h[1] += 1
    elif d == 'D':
        h[1] -= 1

# Part 1
visited = set()
h, t = [0,0], [0,0]
for d, n in moves:
    for _ in range(int(n)):
        visited.add(tuple(t))
        moveHead(d, h)
        moveNext(h, t)
print('Day 09 Part 1:', len(visited))

# Part 2
visited = set()
s = [[0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0], [0,0]]
for d, n in moves:
    for _ in range(int(n)):
        moveHead(d, s[0])
        for i in range(1,10):
            moveNext(s[i-1], s[i])
        visited.add(tuple(s[9]))
print('Day 09 Part 2:', len(visited))
