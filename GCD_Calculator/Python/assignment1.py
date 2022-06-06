import time
import math

def sort(lst, n):
    for i in range(0,n):
        min = i
        for j in range(i+1, n):
            if lst[j] < lst[min]:
                min = j
        lst[i], lst[min] = lst[min], lst[i]

def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)

def gcdarray(arr):
    gcd_arr = None
    for i in range(len(arr)):
        if i == 0:
            gcd_arr = arr[i]
        else:
            gcd_arr = gcd(gcd_arr, arr[i])
    return gcd_arr

def getChe(a, b):
    flag = 0
    count = 0
    while(a<b):
        flag = 0
        if(a <= 1):
            a += 1
            continue
        for i in range (2, int(a/2)):
            if(a%i == 0):
                flag = 1
                break
        if(flag == 0):
            count += 1
        a += 1
    return count

num = 30
arr = []
err = 0

while(True):
    try:
        num = int(input("Input the number of numbers to process : "))
    except ValueError:
        print("Input number between 2 and 30")
    if num not in range(2, 30):
        print("Input integer between 2 and 30")
    else:
        break

while(True):
    err = 0
    arr = list(map(int, input("Input the numbers to be processed : ").split()))
    for i in range(0, num):
        if arr[i] > 100000 or arr[i] < 1:
            err = 1
    if err > 0:
        print("Input number between 1 and 100000")
    else:
        break

start = time.time()
math.factorial(100000)
end = time.time()
print(f"GCD of input number is : {gcdarray(arr)}")

sort(arr, num)
nArr = []
count = 0

for i in range(0, len(arr)):
    if i == 0:
        nArr.append(arr[i])
    elif arr[i] != arr[i-1]:
        nArr.append(arr[i])

for i in range(1, len(nArr)):
    print("Number of prime number between", nArr[i-1], ", ", nArr[i], " : ", getChe(nArr[i-1], nArr[i]))

print(f"Total execution time using python is {end - start:.5f} seconds")