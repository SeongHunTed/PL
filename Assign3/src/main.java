import java.util.Scanner;
import java.io.*;


import static java.lang.System.exit;

public class main {

    public static void main(String[] args) throws IOException {

        Manager mg = new Manager();
        Scanner sc = new Scanner(System.in);

        while(true){

            System.out.println("========================\n\n [1] : 관리자 모드 \n [2] : 사용자 모드 \n [3] : 프로그램 종료\n\n========================");
            System.out.print(" Option : ");
            int select = sc.nextInt();

            switch (select){
                case 1:
                    System.out.println(" 관리자 모드 입니다");
                    mg.manageService();
                    break;
                case 2:
                    System.out.println(" 사용자 모드 입니다");
                    break;
                case 3:
                    System.out.println(" 프로그램을 종료합니다!");
                    exit(0);
            }

        }
    }
}
