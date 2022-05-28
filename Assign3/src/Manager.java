import java.io.*;
import java.util.Scanner;

public class Manager {

    private final int RECORDSIZE = 1024;
    private String filename = "manage.txt";

    private Scanner sc;
    File file = new File("./src/files/manage.txt");

    static FileReader r = null;
    static FileWriter w = null;

    static BufferedReader br = null;


//    HashMap<String, String> map = new HashMap<String, String>();

    public Manager() throws IOException {
        if(!file.exists()){
            file.createNewFile();
        }
        sc = new Scanner(System.in);
    }

    private void addBranch(){
        try{
            w = new FileWriter(filename);
            System.out.print(" 추가 하실 지점 번호를 입력 해주세요 : ");
            int branchNum = sc.nextInt();

            // 지점 번호 예외 처리
            if(branchNum > 6 || branchNum < 0) {System.out.println(" Wrong Option!"); return;}

            // 기존 존재 하는 지점 예외처리
            char [] recordbuf = new char[2];
            br = new BufferedReader(new FileReader(filename));
            br.read(recordbuf, RECORDSIZE * (branchNum - 1), 4);

            System.out.println(recordbuf);
            r.close();

            String branch = branchNum + "지점";
            w.write(branch, RECORDSIZE * (branchNum - 1) , branch.length());
            w.close();

        } catch(IOException e){
            System.err.println(" File Input Error");
        }
    }

    private void addRoom(){

    }

    public void manageService() throws IOException {

        while(true){
            System.out.println("========================\n\n [1] : 지점 추가 \n [2] : 지점 수정 \n [3] : 지점 삭제 \n [4] : 초기 화면 \n\n========================");
            System.out.print(" Option : ");
            int select = sc.nextInt();

            switch (select){
                case 1:
                    addBranch();
                    break;
                case 2:
//                    System.out.println("\"========================\\n\\n [1] : 스터디 공간 추가 \\n [2] : 스터디 공간 수정 \\n [3] : 스터디 공간 삭제 \\n [4] : 초기화면 \\n\\n========================\"");
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }

        }
    }
}
