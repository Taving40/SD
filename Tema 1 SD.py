import random
import timeit

recursion_random=0
recursion_bfprt=0

def bfprt(a): #mediana medianelor
    lung = len(a)
    if lung<5:
        return sorted(a)[lung//2]
    subliste = [sorted(a[i:i+5]) for i in range(0,len(a),5)]
    mediane = [sl[len(sl)//2] for sl in subliste]
    return bfprt(mediane)

def countsort(a): #instabil
    if not a:
        return []
    freq = [0]*(max(a)+1)
    for x in a:
        freq[x]+=1
    rez = []
    for x in range(len(freq)):
        for y in range(freq[x]):
            rez.append(x)
    return rez

def countsort2(a): #stabil
    if not a:
        return []
    freq = [0]*(max(a)+1)
    rez = [0]*len(a)
    for x in a:
        freq[x] += 1
    for i in range(1, len(freq)):
        freq[i] += freq[i-1]
    for i in range(len(a)-1, -1, -1):
        freq[a[i]] -= 1
        rez[freq[a[i]]]=a[i]

    return rez

def interclasare(a,b):
    i=j=0
    rez = []
    while i<len(a) and j<len(b):
        if a[i] > b[j]:
            rez.append(b[j])
            j += 1
        elif a[i]==b[j]:
            rez.append(a[i])
            rez.append(b[j])
            i+=1
            j+=1
        elif a[i] < b[j]:
            rez.append(a[i])
            i+=1
    rez.extend(a[i:])
    rez.extend(b[j:])
    return rez

def mergesort(a):
    if len(a)>1:
        S=a[:len(a)//2]
        D=a[len(a)//2:]
        S=mergesort(S)
        D=mergesort(D)
        return interclasare(S,D)
    else:
        return a

def bubblesort(a):
    if len(a)==1:
        return a
    elif not a:
        return []
    else:
        for x in range(len(a)):
            for y in range(x,len(a)):
                if a[x]>a[y]:
                    a[x],a[y] = a[y],a[x]
    return a

def quicksort_random(a):
    global recursion_random
    try:
        if a == []:
            return []
        else:
            pivot = random.choice(range(len(a)))
            ltoquick1 = [x for x in a[0:pivot] if x < a[pivot]]
            ltoquick2 = [x for x in a[pivot+1:] if x < a[pivot]]
            ltoquick = ltoquick1 + ltoquick2
            gtoquick1 = [x for x in a[0:pivot] if x >= a[pivot]]
            gtoquick2 = [x for x in a[pivot+1:] if x >= a[pivot]]
            pivot = a[pivot]
            gtoquick = gtoquick1 + gtoquick2
            lesser = quicksort_random(ltoquick)
            greater = quicksort_random(gtoquick)
            myList = [] if not lesser else lesser
            myList += [pivot]
            myList += [] if not greater else greater
            return myList
    except RecursionError:
        if not recursion_random:
            print("Quicksort_random a ajuns la un recursion depth mai mare de 1000 si s-a oprit datorita limitarii standard din limbaj.")
            recursion_random=1
        return

def quicksort_bfprt(a):
    global recursion_bfprt
    try:
        if a == []:
            return []
        else:
            pivot = random.choice(range(len(a)))
            ltoquick1 = [x for x in a[0:pivot] if x < a[pivot]]
            ltoquick2 = [x for x in a[pivot+1:] if x < a[pivot]]
            ltoquick = ltoquick1 + ltoquick2
            gtoquick1 = [x for x in a[0:pivot] if x >= a[pivot]]
            gtoquick2 = [x for x in a[pivot+1:] if x >= a[pivot]]
            pivot = a[pivot]
            gtoquick = gtoquick1 + gtoquick2
            lesser = quicksort_bfprt(ltoquick)
            greater = quicksort_bfprt(gtoquick)
            myList = [] if not lesser else lesser
            myList += [pivot]
            myList += [] if not greater else greater
            return myList
    except RecursionError:
        if not recursion_bfprt:
            print("Quicksort_bfprt a ajuns la un recursion depth mai mare de 1000 si s-a oprit datorita limitarii standard din limbaj.")
            recursion_bfprt = 1
        return

def count_10(a, expo):
    rez=[0]*len(a)
    count=[0]*10
    for i in range(0, len(a)):
        cf=(a[i]//expo)%10
        count[cf]+=1
    for i in range(1, 10):
        count[i]+=count[i-1]
    for i in range(len(a)-1,-1,-1):
        cf=(a[i]//expo)%10
        rez[count[cf]-1]=a[i]
        count[cf] -= 1
    return rez

def radixsort_10(a, baza=10):
    if not a:
        return []
    max1=max(a)
    cifra=1
    while max1//cifra>0:
        a=count_10(a, cifra)
        cifra*=baza
    return a

def radixsort_256(a):
    if not a:
        return []
    step=0
    bucket = [[] for _ in range(256)]
    temp= max(a)
    max_cifre = 0
    while temp!=0:
        max_cifre += 1
        temp = temp // 256
    for _ in range(max_cifre):
        for i in range(len(a)):
            bucket[(a[i]>>step)&255].append(a[i])
        index=0
        for i in range(256):
            for x in bucket[i]:
                a[index] = x
                index += 1
            bucket[i] = []
        step += 8
    return a

def test(N, Max, Random=1, Generate=0):
    if Random:
        f = open("date.in", "w")
        for _ in range(N):
            f.write(str(random.randint(0, Max))+"\n")
        f.close()
    if Generate:
        f = open("date.in", "w")
        for x in range(N):
            f.write(str(x) + "\n")
        f.close()
    sorts = [radixsort_10, radixsort_256, quicksort_random, quicksort_bfprt, mergesort, countsort, countsort2, bubblesort]

    f = open("date.in", "r")
    a = [int(x) for x in f.read().split()]
    start_time = timeit.default_timer()
    a.sort()
    elapsed = timeit.default_timer() - start_time
    print("Sort-ul din limbaj a durat {} secunde".format(elapsed))
    f.close()
    for sort in sorts:
        timer(a, "date.in", 100, sort)

def timer(valid, fisier="date.in", repeat=100, functie=radixsort_10):
    f = open("date.in", "r")
    a = [int(x) for x in f.read().split()]
    f.close()
    elapsed=0
    for _ in range(repeat):
        start_time = timeit.default_timer()
        a = functie(a)
        elapsed += timeit.default_timer() - start_time
    print("S-a rulat {} cu numerele generate si citite din {} in {} secunde, iar rezultatul este {}".format(get_name(functie), fisier, elapsed/repeat, "valid" if a==valid else "invalid"))
    if a!=sorted(a):
        print(a)

def get_name(functie):
    functie = str(functie)
    start = 10
    stop = 10
    while functie[stop]!=" ":
        stop+=1
    return functie[start:stop]

# Pentru introducerea unui test random la alegere:
# N = int(input("Introduceti numarul de numere pe care doriti sa se testeze: "))
# Max = int(input("Introduceti numarul maxim care poate aparea in test: "))
# test(N,Max)

x = 1
print("Se ruleaza testul {} cu N: 0; Max: 0. Obs: Cazul fara elemente.".format(x))
test(0,0)
x += 1

print("Se ruleaza testul {} cu N: 10000; Max: 10000. Obs: Cazul general random.".format(x))
test(10000,10000)
x += 1

print("Se ruleaza testul {} cu N: 100000; Max: 10. Obs: Cazul cu multe elemente dar de dimensiuni mici.".format(x))
test(100000,10)
x += 1

print("Se ruleaza testul {} cu N: 10; Max: 1000000. Obs: Cazul cu putine elemente dar de dimensiuni mari.".format(x))
test(10,1000000)
x += 1

print("Se ruleaza testul {} cu N: 100000; Max: 1000000. Obs: Cazul cu multe elemente de dimensiuni mari.".format(x))
test(100000,1000000)
x += 1

print("Se ruleaza testul {} cu N: 10000; Max: 10000. Obs: Cazul cu elemente deja sortate.".format(x))
test(10000,10000,0,1)
x += 1
