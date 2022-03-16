#include <iostream>
using namespace std;

void Sort(int a[], int n);
static int gcd(int x, int y);
static int gcdArray(int* a, int start, int n);

int main()
{
    int num = 0;
    int arr[num];

    cout << "Input the number of numbers to process : ";
    cin >> num;
    // 입력오류 예외처리

    cout << "Input the numbers to be processed : ";
    for(int i = 0; i<num; i++)
    {
        cin >> arr[i];
    }
    //입력오류 예외처리

    cout << "GCD of input numbers is " << gcdArray(arr, 0 , num) << endl;

    for(int i =0; i<num; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    Sort(arr, num);

    for(int i =0; i<num; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

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
    for(int i = 0; i<count; i++)
    {
        cout << nArr[i] << " ";
    }


    return 0;

}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void Sort(int a[], int n)
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

static int gcd(int x, int y)
{
    if(y == 0)
        return x;
    else
        return gcd(y, x%y);
}

static int gcdArray(int* a, int start, int n)
{
    if(n == 1)
    {
        return a[start];
    } else if(n == 2)
    {
        return gcd(a[start], a[start+1]);
    } else {
        return gcd(a[start], gcdArray(a, start+1, n-1));
    }
}