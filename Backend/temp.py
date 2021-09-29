import random
g=0
for i in range(0,36):
    k=random.randint(0,1)
    if(k==1):
        g=g+k
print(g/36)