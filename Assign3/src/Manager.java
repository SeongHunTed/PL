import java.io.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Scanner;
import java.util.Set;

public class Manager {

    private int branchNum = 0;
    private int roomNum = 0;
    private int maxNum = 0;

    private String branch;

    private Scanner sc;
    File file = new File("./src/files/manage.txt");
    static BufferedWriter w = null;
    static BufferedReader r = null;
    HashMap<String, String> map = new HashMap<String, String>();

    public Manager() throws IOException {
        if(!file.exists()){
            file.createNewFile();
        }
        sc = new Scanner(System.in);
    }

    private void addBranch(){
        try{
            w = new BufferedWriter(new FileWriter("./src/files/manage.txt", true));
            System.out.print(" 추가 하실 지점 번호를 입력 해주세요 : ");
            branchNum = sc.nextInt();
            // 지점 번호 예외 처리
            if(branchNum > 6 || branchNum < 0) {System.out.println(" Wrong Option!"); return;}
            // 지점이 기 존재 하는 경우
//            String s;
//            String [] sArray;
//            r = new BufferedReader(new FileReader("./src/files/manage.txt"));
//            while((s = r.readLine()) != null){
//                sArray = s.split(":");
//                map.put(sArray[0], sArray[1]);
//            }
//            Set<String> keylist = map.keySet();
//            Iterator<String> itr = keylist.iterator();
//            while(itr.hasNext()){
//                String a = itr.next();
//                System.out.println(a);
//            }
//            r.close();
            branch = "지점 : " + branchNum + "\n\n\n\n\n";
            w.write(branch);
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
