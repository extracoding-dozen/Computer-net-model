import sys
from random import randint
sys.stdout = open("input.txt", "wt")
f = open("dictionary.txt")
names = list(map(str, f.read().rstrip().split('\n')))
length = len(names)
used = set()
index = 0
names_i_use = []
cnt = randint(1000, 2000)
print(cnt)
for i in range(cnt):
    index = randint(0, length - 1)
    while index in used:
        index = randint(0, length - 1)
    used.add(index)
    print(names[index])
    print(90)
    names_i_use.append(names[index])


mylen = len(names_i_use)
cnt = randint(40, 50)
components = [[] for i in range(cnt)]
partlen = (mylen + cnt - 1) // cnt
for i in range(mylen):
    components[i // partlen].append(names_i_use[i])

pairs = set()
count = 0
s = ""
for i in range(len(components)):
    size = len(components[i])
    for k in range(size):
        for j in range(randint(size // 20, size // 12)):
            ind = randint(0, size - 1)
            pair = (components[i][k], components[i][ind])
            while k == ind or pair in pairs:
                ind = randint(0, size - 1)
                pair = (components[i][k], components[i][ind])
            pairs.add(pair)
            pairs.add((pair[1], pair[0]))
            s += components[i][k] + '\n' + pair[1] + '\n' + '1\n90\n'
            #print(components[i][k])
            #print(pair[1])
            #print(1)
            count += 1
            #print(90)

print(count)
print(s)





