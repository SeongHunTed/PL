#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <cctype>
using namespace std;

#define RECORDSIZE 1024
#define ROOMRECORD 196
#define USERRECORD 27
#define F_OK 0

class Manage{

private:

    int mode;           // 사용자 선택
    FILE *fp;           // Manage.txt 파일
    int branchNum;      // 지점 번호
    int roomNum;        // 스터디 공간 번호
    int maxNum;         // 할당 인원
    int buf;            // 파일로부터 읽어올 정수 값

    // 지점 추가
    void insertBranch(){
        // 추가할 지점 입력받기
        cout << "[0] : 초기화면\n추가 하실 지점 번호를 입력하세요 : ";
        cin >> branchNum;
        if(branchNum == 0) return;
        
        // 파일에서 기존에 존재하는 지점 값 읽어오기
        fseek(fp, (branchNum - 1) * RECORDSIZE, SEEK_SET);
        fread(&buf, sizeof(int), 1, fp);
        
        // 존재하지 않는 지점이면 파일에 쓰기
        if(branchNum != buf){
            fseek(fp, (branchNum - 1) * RECORDSIZE, SEEK_SET);
            fwrite(&branchNum, sizeof(RECORDSIZE), 1, fp);
            fclose(fp);
            return;
        } else {
            cout << "이미 존재하는 지점 입니다!\n" << endl;
            fclose(fp);
            return;
        }
    }
    
    // 지점 수정
    void manageRoom(){
        
        int zero = 0;
        char recordbuf[ROOMRECORD] = {0};
        int reserveCheck = 0;

        cout << "\n\n[1] 스터디 공간 추가 \n[2] 스터디 공간 수정 \n[3] 스터디 공간 삭제 \n[4] 초기 화면\n\n\n========================\n\nOption :";
        cin >> mode;

        while(1){
            switch (mode)
            {
            case 1:
                cout << "\n\n<스터디 공간 추가> \n[0] : 초기화면\n\n";
                cout << "스터디 공간을 추가할 ||!지점 번호!||를 입력하세요 : ";
                cin >> branchNum;

                // 예외처리
                if(branchNum > 6 || branchNum < 0) cout << "잘못된 접근입니다!\n";
                if(branchNum == 0) return;

                // 지점이 존재하지 않을 때
                if(branchCheck(branchNum) == 0) {
                    cout << "존재하지 않는 지점 입니다!\n";
                    break;
                }

                cout << "추가할 스터디 공간의 번호를 입력하세요 : ";
                cin >> roomNum;
                if(roomNum == 0) return;
                if(roomNum > 5){
                    printf("잘못된 접근입니다!\n");
                    break;
                }

                cout << "스터디 공간의 허용 인원을 설정해주세요 : ";
                cin >> maxNum;
                if(maxNum == 0) return;
                if(maxNum > 10){
                    cout << " 잘못된 접근입니다!\n";
                    return;
                }

                // 존재하지 않았던 스터디 공간일 경우
                if(roomCheck(roomNum, branchNum) == 1) {
                    cout << "이미 존재하는 스터디 공간 입니다!\n";
                    return;
                }

                // 스터디 공간 생성
                fseek(fp, 4 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
                fwrite(&roomNum, sizeof(int), 1, fp);
                // 최대 인원 설정
                fseek(fp, 8 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
                fwrite(&maxNum, sizeof(int), 1, fp);

                sprintf(recordbuf, "%d번 지점의 %d번 방 || %d인 실 || 컴퓨터 보유 : %d ", branchNum, roomNum, maxNum, maxNum);
                fseek(fp, 12 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
                fwrite(recordbuf, sizeof(recordbuf), 1, fp);

                fclose(fp);
                return;
            case 2:
                cout << "\n\n    <스터디 공간 수정> \n    [0] : 초기화면 \n\n";
                cout << "스터디 공간을 수정할 ||!지점 번호!||를 입력하세요 : ";
                cin >> branchNum;

                // 예외처리
                if(branchNum > 6 || branchNum < 0) cout << " 잘못된 접근입니다!\n";
                if(branchNum == 0) return;

                // 지점이 존재하지 않을 때
                if(branchCheck(branchNum) == 0) {
                    cout << "존재하지 않는 지점 입니다!\n";
                    break;
                }

                cout << "수정할 스터디 공간의 번호를 입력하세요 : ";
                cin >> roomNum;

                if(roomNum == 0) return;
                if(roomNum > 5){
                    cout << " 잘못된 접근입니다!\n";
                    break;
                }

                
                fseek(fp, 136 + (roomNum - 1) * ROOMRECORD + (branchNum - 1) * RECORDSIZE, SEEK_SET);
                fread(&reserveCheck, sizeof(int), 1, fp);
                if(reserveCheck == 6){
                    printf(" 이미 예약되어있는 공간은 삭제 할 수 없습니다!");
                    return;
                }


                cout << "스터디 공간의 허용 인원을 설정해주세요 : ";
                cin >> maxNum;
                if(maxNum == 0) return;
                if(maxNum > 10){
                    cout << " 잘못된 접근입니다!\n";
                    return;
                }

                // 존재하지 않았던 스터디 공간일 경우
                if(roomCheck(roomNum, branchNum) == 0) {
                    cout << "존재하지 않은 스터디 공간 입니다!\n";
                    return;
                }

                // 인원 재설정
                fseek(fp, 8 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
                fwrite(&maxNum, sizeof(int), 1, fp);

                // 공간 정보 동기화
                sprintf(recordbuf, "%d번 지점의 %d번 방 || %d인 실 || 컴퓨터 보유 : %d ", branchNum, roomNum, maxNum, maxNum);
                fseek(fp, 12 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
                fwrite(recordbuf, sizeof(recordbuf), 1, fp);
                
                fclose(fp);
                return;

            case 3:
                cout << "\n\n<스터디 공간 삭제> \n[0] : 초기화면\n\n";
                memset(recordbuf, 0, sizeof(ROOMRECORD));

                cout << "\n\n삭제할 스터디 공간의 ||!지점번호!||를 입력하세요 : ";
                cin >> branchNum;
                cout << "\n\n삭제할 스터디 공간의 번호를 입력하세요 : ";
                cin >> roomNum;

                if(roomNum == 0) break;

                if(roomCheck(roomNum, branchNum) == 0){
                    printf("존재하지 않는 지점입니다!\n");
                    break;
                }

                reserveCheck = 0;
                fseek(fp, 136 + (roomNum - 1) * ROOMRECORD + (branchNum - 1) * RECORDSIZE, SEEK_SET);
                fread(&reserveCheck, sizeof(int), 1, fp);
                if(reserveCheck == 6){
                    int deleteForce = 0;
                    cout << "\n\n <Warning> : 예약되어있기 때문에 삭제할 수 없습니다!\n\n";
                    cout << " 0을 누르시면 초기화면으로 이동합니다!\n 그래도 삭제하시려면 1을 누르세요!\n \n Option : ";
                    cin >> deleteForce;
                    if(deleteForce == 0) return;
                }

                fseek(fp, 4 + (roomNum-1)*ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
                fwrite(&zero, sizeof(int), 1, fp);
                fseek(fp, 8 + (roomNum-1)*ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
                fwrite(&zero, sizeof(int), 1, fp);
                fseek(fp, 4 + (roomNum-1)*ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
                fwrite(recordbuf, ROOMRECORD, 1, fp);
                fclose(fp);
                return;
            
            case 4:
                return;
            default:
                cout << "Wrong Option!!" << endl;
                break;
            }
        }
    }

    // 지점 삭제
    void deleteBranch(){
        char recordbuf[RECORDSIZE] = {0};
        int zero = 0;
        int reserveCheck = 0;
        cout << "\n\n[0] : 초기화면\n삭제할 지점의 번호를 입력 하세요 : ";
        cin >> branchNum;

        if(branchCheck(branchNum) == 0){
            cout << "존재하지 않는 지점입니다!\n";
            return;
        }

        // 사용자가 예약을 했다면 해당 지점 삭제 불가
        fseek(fp, 1020 + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fread(&reserveCheck, sizeof(int), 1, fp);
        if(reserveCheck == 6){
            int deleteForce = 0;
            cout << "\n\n <Warning> : 예약되어있기 때문에 삭제할 수 없습니다!\n\n";
            cout << " 0을 누르시면 초기화면으로 이동합니다!\n 그래도 삭제하시려면 1을 누르세요!\n \n Option : ";
            cin >> deleteForce;
            if(deleteForce == 0) return;
        }

        fseek(fp, (branchNum-1)*RECORDSIZE, SEEK_SET);
        fwrite(&zero, sizeof(int), 1, fp);
        fseek(fp, (branchNum-1)*RECORDSIZE, SEEK_SET);
        fwrite(recordbuf, RECORDSIZE, 1, fp);
        fclose(fp);
        return;
    }

public:
    Manage(){
        if(access("Manage", F_OK) < 0){
            fp = fopen("Manage", "w+");
        } else {
            fp = fopen("Manage", "r+");
        }
    }
    ~Manage(){}

    // 관리자 모드
    void manageMode(){
        cout << "\n\n   [1] 지점 추가 \n   [2] 지점 수정 \n   [3] 지점 삭제 \n   [4] 초기 화면 \n\n\nOption : ";
        cin >> mode;
        
        while(1){
        switch (mode)
        {
            case 1:
                cout << "\n\n<지점 추가> \n\n";
                insertBranch();
                return;
            case 2:
                cout << "\n\n<지점별 스터디 공간 관리>\n";
                manageRoom();
                return;
            case 3:
                cout << "\n\n<지점삭제>\n";
                deleteBranch();
                return;
            case 4:
                return;
            default:
                cout << "\n\n잘못된 옵션입니다.\n";
                break;
            }
        }
    }

protected:
    // 지점 존재 여부  || 존재 : 1  || 미존재 : 0
    int branchCheck(int branchNum){
        int buf = 0;
        fseek(fp, (branchNum-1)*RECORDSIZE, SEEK_SET);
        fread(&buf, sizeof(int), 1, fp);

        if(buf != branchNum) return 0;
        else return 1;
    }

    // 스터디 공간 존재 여부  || 존재 : 1  || 미존재 : 0
    int roomCheck(int roomNum, int branchNum){
        int buf = 0;
        fseek(fp, 4 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
        fread(&buf, sizeof(int), 1, fp);

        if(buf != roomNum) return 0;
        else return 1;
    }
};

class User : protected Manage{

private:
    
    int mode;
    int branchNum;
    int roomNum;
    int people;
    int start;
    int duration;
    char id[11];
    char reserveDay[7];
    int a; // 타인 예약 체크 할때 쓰는 정수

    // 지점, 스터디 공간 출력
    void printRoom(){
        FILE *fp;

        if(access("Manage", F_OK) < 0){
            cout << " Tozz 스터디 센터가 없습니다! \n";
        } else {
            fp = fopen("Manage", "r+");
        }
        int maxNum = 0;
        char roominfo[120] = {0};

        for(int i = 0;  i < 6; i++){
            fseek(fp, i * RECORDSIZE, SEEK_SET);
            fread(&branchNum, sizeof(int), 1, fp);
            for(int j = 0; j < 5; j++){
                roomNum = 0;
                fseek(fp, 4 + j * ROOMRECORD +  i * RECORDSIZE, SEEK_SET);
                fread(&roomNum, sizeof(int), 1, fp);
                fseek(fp, 8 + j * ROOMRECORD + i * RECORDSIZE, SEEK_SET);
                fread(&maxNum, sizeof(int), 1, fp);
                fseek(fp, 12 + j * ROOMRECORD + i * RECORDSIZE, SEEK_SET);
                fread(roominfo, sizeof(roominfo), 1, fp);
                if(roomNum > 0){
                    cout << "\n [ "<< branchNum << " 지점 ]  \t [### " << roominfo << " ###]\n";
                }
            }
        }
        fclose(fp);
    }

    // 신규 예약
    void reserveUser(){
        FILE *fp;
        FILE *reserve;
        FILE *user;
        // 오늘 날짜
        struct tm* t;
        time_t base = time(NULL);

        t = localtime(&base);
        
        int today = (t->tm_year - 100) * 10000 + (t->tm_mon + 1) * 100 + (t->tm_mday);

        char filebuf[RECORDSIZE*6] = {0};
        int maxNum = 0;
        int arrTime[14] = {0};
        

        // 기존 관리자 파일로 부터 스터디 공간과 지점에 대한 정보를 가져온다.
        if(access("Manage", F_OK) < 0){
            printf(" 예약 가능한 Tozz 센터가 없습니다!\n");
        } else {
            fp = fopen("Manage", "r+");
            fseek(fp, 0, SEEK_SET);
            fread(filebuf, RECORDSIZE*6, 1, fp);
        }
        fclose(fp);

        // 예약 날짜 받기
        cout << " [0] : 초기화면\n 예약할 날을 입력 햬주세요 <YYMMDD> : ";
        cin >> reserveDay;
        if(atoi(reserveDay) == 0) return;

        if(atoi(reserveDay) < today){
            cout << " 올바르지 않은 날 입니다!" << endl;
            return;
        }

        // 당일 예약 예외 처리
        if(today == atoi(reserveDay)){
            cout << "\n\n 당일 예약은 불가 합니다 \n\n";
            return;
        }

        // 예약 날짜에 대한 파일 생성 및 덮어쓰기
        
        // 개인 예약 정보 파일에 저장
        if(access(id, F_OK) < 0){
            user = fopen(id, "w+");
        } else {
            user = fopen(id, "r+");
        }

        // 예약 날짜 파일
        char reserveBuf[RECORDSIZE*6] = {0};
        if(access(reserveDay, F_OK) < 0){
            reserve = fopen(reserveDay, "w+");
            fwrite(filebuf, RECORDSIZE*6, 1, reserve);
        } else {
            // 관리자가 중간에 추가 지점을 만들었을 경우 대비
            reserve = fopen(reserveDay, "r+");
            fseek(reserve, 0, SEEK_SET);
        }
        
        // 예약 정보 받기
        cout << "\n\n 반드시, 존재하는 지점, 스터디 공간을 확인하세요 \n\n 0을 입력하시면 초기화면으로 이동합니다! \n\n";
        printRoom();
        cout << "\n\n [0] : 초기화면\n 예약할 지점 번호 : ";                  // 없는 공간
        cin >> branchNum;
        if(branchNum == 0) return;
        if(branchCheck(branchNum) == 0){
            cout << " 존재 하지 않는 지점 입니다.\n";
            return;
        }

        cout << "\n\n [0] : 초기화면\n 예약할 스터디 공간 번호 : ";             // 가능한 공간
        cin >> roomNum;
        if(roomNum == 0) return;
        if(roomCheck(roomNum, branchNum) == 0){
            cout << " 존재 하지 않는 스터디 공간 입니다.\n";
            return;
        }
        cout << "\n\n [0] : 초기화면\n 사용 인원 : ";                      // 예외처리 허용인원 초과
        cin >> people;
        if(people == 0) return;
        fseek(reserve, 8 + (roomNum-1)*ROOMRECORD + (branchNum -1) * RECORDSIZE, SEEK_SET);
        fread(&maxNum, sizeof(int), 1, reserve);

        if(people > maxNum){
            cout << "\n\n 허용인원 초과입니다\n";
            return;
        }
        cout << "\n\n [0] : 초기화면\n 사용 시작 시간 <8시~ 22시> : ";       // 예외처리 오전 8시 부터 밤 10시
        cin >> start;
        if(start == 0) return;
        if(start > 22){
            cout << "\n\n 오픈 시간이 아닙니다\n";
            return;
        }
        cout << "\n\n [0] : 초기화면\n 사용 예정 시간 : ";                  // 예외처리 10시를 넘어가는 경우
        cin >> duration;
        if(duration == 0) return;
        if(duration + start > 22){
            cout << "\n\n 마감 시간을 넘겨졌습니다.\n";
            return;
        }

        // 사전에 다른 사람이 예약했는지 확인하기
        for(int i = 0; i < duration; i++){
            fseek(reserve, 140 + 4 * (start-8+i) + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
            fread(&a, sizeof(int), 1, reserve);
            if(a>0){
                cout  << start + i << "시에 예약이 차 있습니다!\n";
                return;
            }
        }

        // 예약이 꽉 차 있지 않다면 예약 하기 
        for(int i = 0; i < duration; i++){
            arrTime[start - 8 + i] = 1; 
        }

        // 기존 예약 정보까지 업데이트
        for(int i = 0; i < 14; i++){
            fseek(reserve, 140 + 4 * i + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
            fread(&a, sizeof(int), 1, reserve);
            if(a>0){
                arrTime[i] = 1;
            }
        }

        fseek(reserve, 140 + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fwrite(arrTime, sizeof(arrTime), 1, reserve);

        // 해당과정까지 수행 완료 후 Manage파일에 삭제 하지 못하도록 표기 -> 지점 삭제 금지 || 공간 삭제 금지
        fp = fopen("Manage", "r+");
        int reserveCheck = 6;
        fseek(fp, 1020 + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fwrite(&reserveCheck, sizeof(int), 1, fp);
        fseek(fp, 136 + (roomNum -1) * ROOMRECORD + (branchNum - 1) * RECORDSIZE, SEEK_SET);
        fwrite(&reserveCheck, sizeof(int), 1, fp);


        // 사용자 예약 파일 읽어 오기
        int count = 0;
        char ch;
        while(1){
            ch = fgetc(user);
            if(ch == EOF) break;
            count++;
        }
        count = count / USERRECORD;

        // 예약자 이름으로 예약 정보 저장하기
        fseek(user, USERRECORD * count, SEEK_SET);
        fwrite(reserveDay, sizeof(reserveDay), 1, user);
        fwrite(&branchNum, sizeof(int), 1, user);
        fwrite(&roomNum, sizeof(int), 1, user);
        fwrite(&people, sizeof(int), 1, user);
        fwrite(&start, sizeof(int), 1, user);
        fwrite(&duration, sizeof(int), 1, user);

        fclose(user);
        fclose(reserve);
        fclose(fp);

        return;

    }
    
    // 예약 수정
    void fixReserve(){
        FILE *reserve;
        FILE *user;

        if(access(id, F_OK) < 0){
            cout << " 귀하의 예약 정보가 없습니다!\n";
            return;
        }

        user = fopen(id, "r+");
        // 사용자 예약 파일 읽어 오기
        int count = 0;
        char ch;
        while(1){
            ch = fgetc(user);
            if(ch == EOF) break;
            count++;
        }
        count = count / USERRECORD;

        memset(reserveDay, (char)0xFF, sizeof(reserveDay));

        for(int i = 0; i < count; i++){
            fseek(user, i * USERRECORD, SEEK_SET);
            fread(reserveDay, sizeof(reserveDay), 1, user);
            fread(&branchNum, sizeof(int), 1, user);
            fread(&roomNum, sizeof(int), 1, user);
            fread(&people, sizeof(int), 1, user);
            fread(&start, sizeof(int), 1, user);
            fread(&duration, sizeof(int), 1, user);

            cout << "\n\n========================\n\n";
            cout << "=======<예약정보>=======\n";
            cout << "[ 예약 날짜 : " << reserveDay << " ]\n [ 스터디 지점 : " << branchNum << " ]\n  스터디 공간 번호 : "<< roomNum
            << "\n  인원 : " << people << "\n  예약 시간 : "<< start << "\n  사용 시간 : " << duration << endl;
        }


        cout << "\n========================\n";
        cout << "[1] : 예약 변경\n\n[2] : 초기화면\n\n\n";
        cout << "Option : ";
        cin >> mode;
        fclose(user);
        fclose(reserve);

        int num = 0;

        switch (mode)
        {
        case 1:
            cout << "\n몇번째 예약을 변경하시겠습니까? : ";
            cin >> num;
            reReserve(num);
            break;
        case 2:
            return;
        default:
            printf("Wrong Option!\n");
            break;
        }

        return;

    }

    void reReserve(int num){
        
        FILE *reserve;
        FILE *user;

        user = fopen(id, "r+");

        int count = 0;
        char ch;
        while(1){
            ch = fgetc(user);
            if(ch == EOF) break;
            count++;
        }
        count = count / USERRECORD;

        if(num > count){
            printf("해당 예약 번호는 존재하지 않습니다!\n");
            return;
        }

        char reset[USERRECORD] = {0};

        fseek(user, (num-1) * USERRECORD, SEEK_SET);
        fread(reserveDay, sizeof(reserveDay), 1, user);
        fread(&branchNum, sizeof(int), 1, user);
        fread(&roomNum, sizeof(int), 1, user);
        fread(&people, sizeof(int), 1, user);
        fread(&start, sizeof(int), 1, user);
        fread(&duration, sizeof(int), 1, user);

        fseek(user, (num-1) * USERRECORD, SEEK_SET);
        fwrite(reset, sizeof(USERRECORD), 1, user);
        reserve = fopen(reserveDay, "r+");

        int timereset = 0;
        int a = 0;
        int arrTime[14] = {0};
        int maxNum = 0;

        // 날짜 예약 파일에 가서 변경전 데이터 지우기
        for(int i = 0; i < duration; i++){
            fseek(reserve, 140 + 4 * (start - 8 + i) + (roomNum - 1) * ROOMRECORD + (branchNum -1) * RECORDSIZE, SEEK_SET);
            fwrite(&timereset, sizeof(int), 1, reserve);
        }
 

        // 예약 정보 받기
        cout << "\n\n 반드시, 존재하는 지점, 스터디 공간을 확인하세요 \n\n 0을 입력하시면 초기화면으로 이동합니다! \n\n";
        printRoom();
        cout << "\n\n [0] : 초기화면\n 예약할 지점 번호 : ";                  // 없는 공간
        scanf("%d", &branchNum);
        if(branchNum == 0) return;
        if(branchCheck(branchNum) == 0){
            cout << " 존재 하지 않는 지점 입니다.\n";
            return;
        }

        cout << "\n\n [0] : 초기화면\n 예약할 스터디 공간 번호 : ";             // 가능한 공간
        cin >> roomNum;
        if(roomNum == 0) return;
        if(roomCheck(roomNum, branchNum) == 0){
            cout << " 존재 하지 않는 스터디 공간 입니다.\n";
            return;
        }
        cout << "\n\n [0] : 초기화면\n 사용 인원 : ";                      // 예외처리 허용인원 초과
        cin >> people;
        if(people == 0) return;

        fseek(reserve, 8 + (roomNum-1)*ROOMRECORD + (branchNum -1) * RECORDSIZE, SEEK_SET);
        fread(&maxNum, sizeof(int), 1, reserve);


        if(people > maxNum){
            cout << "\n\n 허용인원 초과입니다\n";
            return;
        }
        cout << "\n\n [0] : 초기화면\n 사용 시작 시간 <8시~ 22시> : ";       // 예외처리 오전 8시 부터 밤 10시
        cin >> start;
        if(start == 0) return;
        if(start > 22){
            cout << "\n\n 오픈 시간이 아닙니다\n";
            return;
        }
        cout << "\n\n [0] : 초기화면\n 사용 예정 시간 : ";                  // 예외처리 10시를 넘어가는 경우
        cin >> duration;
        if(duration == 0) return;
        if(duration + start > 22){
            cout << "\n\n 마감 시간을 넘겨졌습니다.\n";
            return;
        }

        // 사전에 다른 사람이 예약했는지 확인하기

        for(int i = 0; i < duration; i++){
            fseek(reserve, 140 + 4 * (start-8+i) + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
            fread(&a, sizeof(int), 1, reserve);
            if(a>0){
                printf(" %d시에 예약이 차 있습니다!\n", start + i);
                return;
            }
        }

        // 기존 예약 정보 받아오기
        for(int i = 0; i < 14; i++){
            fseek(reserve, 140 + 4 * i + (roomNum - 1) * RECORDSIZE + (branchNum -1) * RECORDSIZE, SEEK_SET);
            fread(&a, sizeof(int), 1, reserve);
            if(a>0){
                arrTime[i] = 1;
            }
        }

        // 새로 예약한 시간
        for(int i = 0; i < duration; i++){
            arrTime[start - 8 + i] = 1; 
        }

        fseek(reserve, 140 + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fwrite(arrTime, sizeof(arrTime), 1, reserve);


        // 예약자 이름으로 예약 정보 저장하기
        fseek(user, (num -1)*USERRECORD , SEEK_SET);
        fwrite(reserveDay, sizeof(reserveDay), 1, user);
        fwrite(&branchNum, sizeof(int), 1, user);
        fwrite(&roomNum, sizeof(int), 1, user);
        fwrite(&people, sizeof(int), 1, user);
        fwrite(&start, sizeof(int), 1, user);
        fwrite(&duration, sizeof(int), 1, user);

        fclose(user);
        fclose(reserve);

        return;
    }


public:

    User(){};
    ~User(){};

    // 사용자 모드
    void userMode(){

        char exit[5] = "exit";
        while(1){
            while(1){
                int checkDigit = 0;
                int checkChar = 0;
                memset(&id, 0xFF, sizeof(id));
                cout << "\n초기화면으로 돌아가시려면 exit를 입력하세요!\n\n사용자 ID를 입력하세요 : ";
                cin >> id;
                if(strcmp(exit, id) == 0) return;
                for(int i = 0; i < strlen(id); i++){
                    if(isdigit(id[i]) > 0) checkDigit++;
                    else checkChar++;
                }
                if((strlen(id) < 11) && (strlen(id) > 4) && (checkDigit > 0) && (checkChar > 0)) break;
                else cout << " 5글자 이상 10글자 이하 영문, 숫자를 조합하세요! \n";
            }

            cout << "========================";
            cout << "\n\n[1] 스터디 공간 조회 \n[2] 신규 예약 \n[3] 예약 조회 및 수정 \n[4] 초기화면 \n\n========================\n\n";
            cout << "Option : ";
            cin >> mode;

            switch (mode)
            {
            case 1:
                printRoom();
                return;
            case 2:
                reserveUser();
                return;
            case 3:
                fixReserve();
                return;
            case 4:
                cout << "\n\n 초기화면으로 돌아갑니다!\n\n";
                return;
            default:
                break;
            }
        }
    }

};

int main(){
    while(1){
        int select = 0;
        cout << "\n========================";
        cout << "\n\n   [1] 관리자 모드 \n   [2] 사용자 모드 \n   [3] 프로그램 종료\n\n========================\n\n";
        cout << "Option : ";
        cin >> select;
        cout << "\n========================";

        switch (select)
        {
        case 1: {
            cout << "\n\n<관리자 모드>\n\n========================";
            Manage mg;
            mg.manageMode();
            break;
        }
        case 2: {
            cout << "\n\n<사용자 모드>\n\n========================";
            User user;
            user.userMode();
            break;
        }
        case 3:
            cout << "\n\n프로그램을 종료합니다.....";
            exit(0);
        default:
            cout << "\n\nWrong Option!" << endl;
            break;
        }
    }
    return 0;
}