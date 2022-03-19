#include <iostream>
#include <ctime>
using namespace std;

void Sort(int* a, int n);
static int gcd(int x, int y);
static int gcdArray(int* a, int n);
int getChe(int a, int b);

int main()
{   
    clock_t start, finish;
    double duration;
    int num = 30;
    int arr[num];

    while (true)
    {   
        cout << "입력받을 배열의 인덱스" << endl;
        cout << "Input the number of numbers to process : ";
        cin >> num;
        try
        {
            if(!cin)
                throw num;
            else if(num <= 1 || num > 30)
                throw num;
            else
                break;
        }
        catch(int num)
        {
            cout << "Input the number over 0 or only Positive Interger! " << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        
        
    }
    
    
    // 입력오류 예외처리
    cout << "배열원소들 입력" << endl;
    cout << "Input the numbers to be processed :  ";
    start = clock();
    for(int i = 0; i<num; i++)
    {
        cin >> arr[i];
    }
    //입력오류 예외처리

    // 배열내 최대공약수 구하기
    cout << "GCD of input numbers is " << gcdArray(arr, num) << endl;

    // 최대공약수 구한 직후 배열 상태
    cout << endl;
    cout << "최대공약수 구한 직후 배열 상태" << endl;
    for(int i =0; i<num; i++)
    {
        cout << arr[i] << " ";
    }

    // 배열 정렬
    Sort(arr, num);

    cout << endl;
    cout << endl;
    cout << "정렬 후 배열 상태" << endl;
    for(int i =0; i<num; i++)
    {
        cout << arr[i] << " ";
    }

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

    cout << endl;
    cout << endl;
    cout << "중복제거 후 배열상태" << endl;
    for(int i = 0; i<count; i++)
    {
        cout << nArr[i] << " ";
    }

    cout << endl;
    cout << endl;
    cout << "두 수 사이 소수개수 구하기" << endl;

    for(int i = 0; i<count-1; i++)
    {
        cout << "Number of prime numbers between " << nArr[i] << 
        ", " << nArr[i+1] << " : " << getChe(nArr[i], nArr[i+1]) << endl;
    }
    finish = clock();

    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    cout << "Total execution time using C++ is " << duration << " seconds!" << endl;



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

        if(a<= 1)
        {
            ++a;
            continue;
        }

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