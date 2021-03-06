#include <iostream>
#include <ctime>

#define MAX_NUMBER 100000
using namespace std;

void swap(int &a, int &b); // 위치교환
void Sort(int* a, int n); // 정렬함수
static int gcd(int x, int y); // 두 정수의 최대 공약수
static int gcdArray(int* a, int n); // 배열 내 최대 공약수
int getChe(int a, int b); // 소수함수

int main()
{   
    clock_t start, finish; // 시간측정
    double duration; // 시간
    int num = 30; // 배열 인덱스 수 초기화
    int arr[num]; // 입력값 배열

    while(true)
    {
        cout << "Input the number of numbers to process : ";
        cin >> num;

        if(num >= 2 && num <= 30)
            break;
        else if( num < 2 || num >30)
        {
            cout << "The number should be between 2 to 30 and Integer" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        } else {
            cout << "Follow the rule (interger between 2 and 30)" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }

    while(true)
    {
        int flag = 0;
        cout << "Input the numbers to be processed :  ";

        for(int i = 0; i<num; i++)
        {
            cin >> arr[i];    
        }
        for(int i = 0; i<num; i++)
        {
            if(arr[i] > MAX_NUMBER || arr[i] < 1)
            {
                flag = 1;
            }
        }
        if(flag > 0) cout << "input number between 1 and 100000\n" << endl;
        else break;
    }
    
    start = clock();

    // 배열내 최대공약수 구하기  
    cout << "GCD of input numbers is " << gcdArray(arr, num) << "\n" << endl;

    // 배열 정렬
    Sort(arr, num);

    // 중복제거하기
    int nArr[num];
    for(int i = 0; i < num; i++)
    {
        nArr[i] = 0;
    }
    int count = 0;
    for(int i = 0; i < num; i++)
    {
        if(arr[i] != arr[i+1])
            nArr[count++] = arr[i];
        
    }

    // 두 수 사이의 Prime number 구하기
    for(int i = 0; i<count-1; i++)
    {
        cout << "Number of prime numbers between " << nArr[i] << 
        ", " << nArr[i+1] << " : " << getChe(nArr[i], nArr[i+1]) << endl;
    }

    finish = clock();
    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    cout << "\n" <<"Total execution time using C++ is " << duration << " seconds!" << endl;

    return 0;
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void Sort(int* a, int n)
{
    for(int i=0; i<n; i++)
    {
        int minn = a[i];
        int location = i;
        for(int j = i+1; j<n; j++)
        {
            if(minn>a[j])
            {
                minn = a[j];
                location = j;
            }
        }
        swap(a[i],a[location]);
    }
}


static int gcd(int a, int b)
{
    if(a == 0)
        return b;
    else
        return gcd(b%a, a);
}

static int gcdArray(int* a, int n)
{   
    int result = a[0];
    for(int i = 1; i<n; i++)
    {
        result = gcd(a[i], result);

        if(result == 1)
        {
            return 1;
        }
    }
    return result;
}

// 두 정수사이의 소수갯수
int getChe(int a, int b)
{   
    int flag, i;
    int count = 0;
    while( a < b)
    {
        flag = 0;
        for(i = 2; i <= a/2; ++i)
        {
            if(a%i == 0)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
            ++count;
        ++a;
    }
    return count;
}
