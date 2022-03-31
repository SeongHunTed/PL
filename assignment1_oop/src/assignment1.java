import java.util.Scanner;
import java.time.Duration;
import java.time.Instant;

public class assignment1 {
    public static void selection_sort ( int[] a, int size){
        for (int i = 0; i < size - 1; i++) {
            int min = i;
            for (int j = i + 1; j < size; j++) {
                if (a[j] < a[min]) {
                    min = j;
                }
            }
            swap(a, min, i);
        }
    }

    public static void swap ( int[] a, int i, int j){
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }

    public static int gcd( int a, int b)
    {
        if (a == 0)
            return b;
        else
            return gcd(b % a, a);
    }

    public static int gcdarray(int[] a, int n)
    {
        int result = a[0];
        for (int i = 1; i < n; i++) {
            result = gcd(a[i], result);
            if (result == 1) {
                return 1;
            }
        }
        return result;
    }

    public static int getChe(int a, int b)
    {
        int flag, i;
        int count = 0;
        while(a<b)
        {
            flag = 0;
            for(i = 2; i<=a/2; ++i)
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

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int num = 30;
        int flag = 0;
        int[] arr = new int[num];

        while(true) {
            System.out.print("Input the number of number to process : ");
            num = sc.nextInt();
            if(num > 30 || num < 2){
                flag = 1;
            }
            if(flag == 1){
                System.out.println("Input only natural number between 2 and 30");
            } else {
                break;
            }

        }

        while(true) {
            flag = 0;
            System.out.print("Input the number to be processed : ");
            for (int i = 0; i < num; i++) {
                arr[i] = sc.nextInt();
            }
            for(int i = 0; i<num; i++) {
                if (arr[i] > 100000 || arr[i] < 1) {
                    flag = 1;
                }
            }
            if(flag == 1) {
                System.out.println("Input only natural number under 100001");
            } else {
                break;
            }
        }

        Instant start = Instant.now();

        System.out.print("GCD of input number is ");
        int a = gcdarray(arr, num);
        System.out.print(a);

        selection_sort(arr, num);

        int[] nArr = new int[num];
        for(int i =0; i<num; i++)
            nArr[i] = 0;
        int count = 0;
        for(int i =0; i<num; i++)
            if(arr[i] != arr[i+1])
                nArr[count++] = arr[i];

        for(int i = 0; i<count-1; i++)
        {
            System.out.println("Number of prime numbers between " + nArr[i] + ", " + nArr[i+1] + " : "
                    + getChe(nArr[i], nArr[i+1]));
        }

        Instant finish = Instant.now();
        double elapsedTime = Duration.between(start, finish).toMillis()/1000.0;
        System.out.println("Total execution time using JAVA is " + elapsedTime + " seconds");
    }
}

