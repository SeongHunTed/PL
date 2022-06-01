#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define RECORDSIZE 1024
#define ROOMRECORD 196
#define USERRECORD 27

// 1. 해당 날 : 22년 05월 26로 설정
// 2. 관리자가 설정을 마치고 사용자가 예약을 하는 관점에서 다루었음
// 3. 한 사람은 한 예약만 한다고 가정함.
// 4. 당일 예약 보다 뒤의 경우는 따로 처리 하지 않음


// 1024 byte : 4 byte : 지점 번호 || 200 byte : 스터디 공간 { 4byte : 스터디 공간 번호 4byte : 스터디 공간 수용 인원 ~~~~}

// 관리자 모드
// 지점과 스터디공간 번호는 중복을 허용하지 않음
// 지점 관리 1 ~ 6
//      1. 지점 추가    2. 지점 수정    3. 지점 삭제
// 2. 지점 수정 - > 스터디 공간 관리 1 ~ 5
//      1. 스터디 공간 추가     2. 스터디 공간 수정     3. 스터디 공간 삭제
//      스터디 공간 사용 인원 : 1 ~ 10
//      스터디 공간 : 이름, 보유 컴퓨터 개수 임의 지정

// 현재 날짜 구하기
// typedef struct _TIME{
//     int year;
//     int month;
//     int date;
// }time;

// 관리자용
int managerMode();                                                                                  // 관리자 모드
int insertBranch(FILE *fp);                                                                         // 지점 추가
int manageRoom(FILE *fp);                                                                           // 지점별 스터디 공간 관리
int deleteBranch(FILE *fp);                                                                         // 지점 삭제
int bemptycheck(FILE *reserve, int branchNum);                                                           // 지점 존재 확인
int remptycheck(FILE *fp, int roomNum, int branchNum);                                              // 스터디 공간 존재 확인

// 사용자용
int userMode();                                                                                     // 사용자 모드
void printRoom();                                                                                   // 스터디 공간 조회
int reserve(char *id);                                                                              // 신규 예약
int fixReserve(char *id);                                                                           // 예약조회
// int resetFile(char *reserveDay, int branchNum, int roomNum, int people, int start, int duration);   // 예약 변경시 예약파일 수정
int reReserve(int num, char* id);

int main(){
    
    while(1){
        int select = 0;
        printf("\n========================\n");
        printf("\n [1] 관리자 모드 \n [2] 사용자 모드 \n [3] 프로그램 종료 \n\n========================\n\n");
        printf(" Option : ");
        scanf("%d", &select);
        printf("\n\n========================\n");

        switch (select)
        {
        case 1:
            printf("\n\n <관리자 모드>\n");
            printf("\n========================\n");
            managerMode();
            break;
        case 2:
            printf("\n\n <사용자 모드>\n");
            printf("\n========================\n");
            userMode();
            break;
        case 3:
            printf("\n\n <프로그램 종료>\n");
            printf("\n========================\n");
            exit(0);
        default:
            printf("\n\n <Wrong Option!>\n");
            printf("\n========================\n");
            break;
        }
    }

    return 0;
}

// 관리자 모드
int managerMode(){

    FILE *fp;
    if(access("Manage", F_OK) < 0){
        fp = fopen("Manage", "w+");
    } else {
        fp = fopen("Manage", "r+");
    }

    int select = 0;

    printf("\n\n [1] 지점 추가 \n [2] 지점 수정 \n [3] 지점 삭제 \n [4] 초기 화면 \n\n\n");
    printf(" Option : ");
    scanf("%d", &select);
    

    switch (select)
    {
    case 1:
        printf("\n\n <지점 추가> \n\n");
        insertBranch(fp);
        break;
        
    case 2:
        printf("\n\n <지점별 스터디 공간 관리>\n");
        manageRoom(fp);
        break;

    case 3:
        printf("\n\n <지점 삭제>\n");
        deleteBranch(fp);
        break;
    case 4:
        break;
    default:
        printf("Wrong Option!\n");
        break;
    }

    return 1;

}

// 지점 생성
int insertBranch(FILE *fp){
    int branchNum;
    int buf = 0;
    printf(" [0] : 초기화면\n 추가 하실 지점 번호를 입력하세요 : ");
    scanf("%d", &branchNum);
    if(branchNum == 0) return 0;
    fseek(fp, (branchNum - 1) * RECORDSIZE, SEEK_SET);
    fread(&buf, sizeof(int), 1, fp);
        
    if(branchNum > 6){
        printf(" 잘못된 접근입니다!\n");
    }

    if(branchNum != buf){
        fseek(fp, (branchNum - 1) * RECORDSIZE, SEEK_SET);
        fwrite(&branchNum, sizeof(RECORDSIZE), 1, fp);
        fclose(fp);
        return 1;
    } else {
        printf(" 이미 존재하는 지점 입니다! \n");
        fclose(fp);
        return 0;
    }
}

// 스터디 공간 관리
int manageRoom(FILE *fp){

    int select = 0;     // 관리자 옵션 값
    int branchNum = 0;  // 지점 번호
    int roomNum = 0;    // 스터디 공간 번호
    int maxNum = 0;     // 스터디 공간 허용 인원
    char recordbuf[ROOMRECORD-60] = {0};
    printf("\n\n [1] 스터디 공간 추가 \n [2] 스터디 공간 수정 \n [3] 스터디 공간 삭제 \n [4] 초기 화면\n\n\n");
    printf(" Option : ");
    scanf("%d", &select);

    switch (select)
    {
    case 1:
        printf("\n\n <스터디 공간 추가> \n [0] : 초기화면\n\n");
        
        printf(" 스터디 공간을 추가할 ||!지점 번호!||를 입력하세요 : ");
        scanf("%d", &branchNum);

        if(branchNum > 6 || branchNum < 0){
            printf(" 잘못된 접근입니다!\n");
        }
        if(branchNum == 0) return 0;

        // 지점이 존재 하지 않을 경우 
        if(bemptycheck(fp, branchNum) == 0) {
            printf(" 존재하지 않는 지점 입니다!\n");
            return 0;
        }

        printf(" 추가할 스터디 공간의 번호를 입력하세요 : ");
        scanf("%d", &roomNum);
        if(roomNum == 0) return 0;
        if(roomNum > 5){
            printf(" 잘못된 접근입니다!\n");
            return 0;
        }

        printf(" 스터디 공간의 허용 인원을 설정해주세요 : ");
        scanf("%d", &maxNum);
        if(maxNum == 0) return 0;

        if(maxNum > 10){
            printf(" 잘못된 접근입니다!\n");
            return 0;
        }

        // 존재하지 않았던 스터디 공간일 경우
        if(remptycheck(fp, roomNum, branchNum) == 1) {
            printf(" 이미 존재하는 스터디 공간 입니다!\n");
            return 0;
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
        break;
    
    case 2:
        printf("\n\n <스터디 공간 수정> \n [0] : 초기화면 \n\n");
        printf(" 스터디 공간을 수정할 ||!지점 번호!||를 입력하세요 : ");
        scanf("%d", &branchNum);
        if(branchNum > 6 || branchNum < 0){
            printf(" 잘못된 접근입니다!\n");
        }
        if(branchNum == 0) return 0;

        // 지점이 존재 하지 않을 경우 
        if(bemptycheck(fp, branchNum) == 0) {
            printf(" 존재하지 않는 지점 입니다!\n");
            return 0;
        }

        printf(" 수정할 스터디 공간의 번호를 입력하세요 : ");
        scanf("%d", &roomNum);
        if(roomNum == 0) return 0;
        if(roomNum > 5){
            printf(" 잘못된 접근입니다!\n");
            return 0;
        }

        int reserveCheck = 0;
        fseek(fp, 136 + (roomNum - 1) * ROOMRECORD + (branchNum - 1) * RECORDSIZE, SEEK_SET);
        fread(&reserveCheck, sizeof(int), 1, fp);
        if(reserveCheck == 6){
            printf(" 이미 예약되어있는 공간은 삭제 할 수 없습니다!");
            return 0;
        }

        printf(" 스터디 공간의 허용 인원을 수정 해주세요 : ");
        scanf("%d", &maxNum);
        if(maxNum == 0) return 0;

        if(maxNum > 10){
            printf(" 잘못된 접근입니다!\n");
            return 0;
        }

        if(remptycheck(fp, roomNum, branchNum) == 0) {
            printf(" 존재하지 않은 스터디 공간 입니다!\n");
            return 0;
        }

        // 최대 인원 재설정
        fseek(fp, 8 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
        fwrite(&maxNum, sizeof(int), 1, fp);

        // 공간 정보 동기화
        sprintf(recordbuf, "%d번 지점의 %d번 방 || %d인 실 || 컴퓨터 보유 : %d ", branchNum, roomNum, maxNum, maxNum);
        fseek(fp, 12 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
        fwrite(recordbuf, sizeof(recordbuf), 1, fp);



        fclose(fp);
        break;
    
    case 3:
        printf("\n\n <스터디 공간 삭제> \n [0] : 초기화면\n\n");
        int deleteRoom = 0;
        int zero = 0;
        char emptyrecord[ROOMRECORD] = {0};
        printf("\n\n 삭제할 스터디 공간의 ||!지점번호!||를 입력하세요 : ");
        scanf("%d", &branchNum);
        if(branchNum == 0) break;
        printf("\n\n 삭제할 스터디 공간의 번호를 입력하세요 : ");
        scanf("%d", &deleteRoom);
        if(deleteRoom == 0) break;

        // 기 예약된 공간일 경우
        reserveCheck = 0;
        fseek(fp, 136 + (deleteRoom - 1) * ROOMRECORD + (branchNum - 1) * RECORDSIZE, SEEK_SET);
        fread(&reserveCheck, sizeof(int), 1, fp);
        if(reserveCheck == 6){
            printf(" 이미 예약되어있는 공간은 삭제 할 수 없습니다!");
            return 0;
        }

        if(remptycheck(fp, deleteRoom, branchNum) == 0){
            printf(" 존재하지 않는 지점입니다!\n");
            return 0;
        }

        fseek(fp, 4 + (deleteRoom-1)*ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fwrite(&zero, sizeof(int), 1, fp);
        fseek(fp, 8 + (deleteRoom-1)*ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fwrite(&zero, sizeof(int), 1, fp);
        fseek(fp, 4 + (deleteRoom-1)*ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fwrite(emptyrecord, ROOMRECORD, 1, fp);
        fclose(fp);
        break;
    case 4:
        break;
    default:
        printf(" Wrong Option!\n");
        break;
    }
    return 0;
}

// 지점 삭제
int deleteBranch(FILE *fp){
    int deleteBranch = 0;
    int zero = 0;
    int reserveCheck = 0;
    char emptyrecord[RECORDSIZE] = {0};
    printf("\n\n [0] : 초기화면\n 삭제할 지점의 번호를 입력 하세요 : ");
    scanf("%d", &deleteBranch);

    if(bemptycheck(fp, deleteBranch)==0) return 0;

    // 사용자가 예약을 했다면 해당 지점 삭제 불가
    fseek(fp, 1020 + (deleteBranch-1) * RECORDSIZE, SEEK_SET);
    fread(&reserveCheck, sizeof(int), 1, fp);
    if(reserveCheck == 6){
        printf(" 예약되어있기 때문에 삭제할 수 없습니다!");
        return 0;
    }

    fseek(fp, (deleteBranch-1)*RECORDSIZE, SEEK_SET);
    fwrite(&zero, sizeof(int), 1, fp);
    fseek(fp, (deleteBranch-1)*RECORDSIZE, SEEK_SET);
    fwrite(emptyrecord, RECORDSIZE, 1, fp);

    fclose(fp);
    return 0;
}

// 지점 존재 여부 확인
int bemptycheck(FILE *reserve, int branchNum){
    int buf = 0;
    fseek(reserve, (branchNum-1)*RECORDSIZE, SEEK_SET);
    fread(&buf, sizeof(int), 1, reserve);

    if(buf != branchNum){
        printf(" 존재하지 않는 지점입니다! \n");
        return 0;
    } else return 1;
}

// 스터디 공간 존재 여부 확인
int remptycheck(FILE *fp, int roomNum, int branchNum){
    int buf = 0;
    fseek(fp, 4 + (roomNum-1)*ROOMRECORD + (branchNum-1)*RECORDSIZE, SEEK_SET);
    fread(&buf, sizeof(int), 1, fp);

    // 0 -> 존재 하지 않았던 경우
    // 1 -> 이미 존재하는 경우
    if(buf != roomNum){
        return 0;
    } else return 1;
}

// 사용자 모드
int userMode(){
    
    // 사용자 ID
    char id[10] = {0};

    while(1){
        memset(&id, 0xFF, sizeof(id));
        printf(" 사용자 ID를 입력하세요 : ");
        scanf("%s", id);
        if(strlen(id) < 11 && strlen(id) > 4){
            break;
        } else {
            printf(" 5글자 이상 10글자 이하 영문, 숫자를 조합하세요! \n");
        }
    }

    int select = 0;
    printf("========================");
    printf("\n\n [1] 스터디 공간 조회 \n [2] 신규 예약 \n [3] 예약 조회 및 수정 \n [4] 초기화면 \n\n========================\n\n");
    printf(" Option : ");
    scanf("%d", &select);

    switch (select)
    {
    case 1:
        printRoom();
        break;
    case 2:
        reserve(id);
        break;
    case 3:
        fixReserve(id);
        break;
    case 4:
        printf("\n\n 초기화면으로 돌아갑니다!\n\n");
        break;
    default:
        break;
    }


    return 0;
}

// 스터디 지점, 공간 조회
void printRoom(){
    FILE *fp;

    if(access("Manage", F_OK) < 0){
        printf(" Tozz 스터디 센터가 없습니다! \n");
    } else {
        fp = fopen("Manage", "r+");
    }

    int branchNum = 0;
    int roomNum = 0;
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
                printf("\n [ %d 지점 ] \t [### %s  ###]\n", branchNum, roominfo);
            }
        }
    }
    fclose(fp);
}

// 신규 예약
int reserve(char *id){
    // 오늘 날짜
    struct tm* t;
    time_t base = time(NULL);

    t = localtime(&base);
    
    int today = (t->tm_year - 100) * 10000 + (t->tm_mon + 1) * 100 + (t->tm_mday);
    // 예약 날짜
    char reserveDay[7] = {0};
    time_t timer;
    
    // 복사할 buffer
    char filebuf[RECORDSIZE*6] = {0};
    // 저장된 허용인원
    int maxNum = 0;
    // 예약 시간 56byte 짜리 배열 196-56 140
    int arrTime[14] = {0};
    int a = 0; // 해당 시간 비었는 지 체크

    // 사용자 예약 입력값 정보
    int branchNum = 0;
    int roomNum = 0;
    int people = 0;
    int start = 0;
    int duration = 0;

    // fp -> 기존 파일 reserve -> 예약파일
    FILE *fp;
    FILE *reserve;

    // 기존 관리자 파일로 부터 스터디 공간과 지점에 대한 정보를 가져온다.
    if(access("Manage", F_OK) < 0){
        printf(" 예약 가능한 Tozz 센터가 없습니다!\n");
    } else {
        fp = fopen("Manage", "r+");
        fseek(fp, 0, SEEK_SET);
        fread(filebuf, RECORDSIZE*6, 1, fp);
    }

    // 예약 날짜에 대한 예약 파일을 새로 만든다. -> 존재하면 만들지 않는다.
    printf(" [0] : 초기화면\n 예약할 날을 입력 햬주세요 <YYMMDD> : ");
    scanf("%s", reserveDay);
    if(atoi(reserveDay) == 0) return 0;

    if(today == atoi(reserveDay)){
        printf("\n\n 당일 예약은 불가 합니다 \n\n");
        return 0;
    }

    // 개인 예약 정보 파일에 저장
    FILE *user;

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
        reserve = fopen(reserveDay, "r+");
        fread(reserveBuf, sizeof(RECORDSIZE*6), 1, reserve);
        fseek(reserve, 0, SEEK_SET);
        fwrite(filebuf, RECORDSIZE*6, 1, reserve);
        fseek(reserve, 0, SEEK_SET);
        fwrite(reserveBuf, sizeof(RECORDSIZE*6), 1, reserve);
    }

    // 예약 정보 받기
    printf("\n\n 반드시, 존재하는 지점, 스터디 공간을 확인하세요 \n\n 0을 입력하시면 초기화면으로 이동합니다! \n\n");
    printRoom();
    printf("\n\n [0] : 초기화면\n 예약할 지점 번호 : ");                  // 없는 공간
    scanf("%d", &branchNum);
    if(branchNum == 0) return 0;
    if(bemptycheck(reserve, branchNum) == 0){
        printf(" 존재 하지 않는 지점 입니다.\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 예약할 스터디 공간 번호 : ");             // 가능한 공간
    scanf("%d", &roomNum);
    if(roomNum == 0) return 0;
    if(remptycheck(reserve, roomNum, branchNum) == 0){
        printf(" 존재 하지 않는 스터디 공간 입니다.\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 사용 인원 : ");                      // 예외처리 허용인원 초과
    scanf("%d", &people);
    if(people == 0) return 0;
    fseek(reserve, 8 + (roomNum-1)*ROOMRECORD + (branchNum -1) * RECORDSIZE, SEEK_SET);
    fread(&maxNum, sizeof(int), 1, reserve);
    if(people > maxNum){
        printf("\n\n 허용인원 초과입니다\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 사용 시작 시간 <8시~ 22시> : ");       // 예외처리 오전 8시 부터 밤 10시
    scanf("%d", &start);
    if(start == 0) return 0;
    if(start > 22){
        printf("\n\n 오픈 시간이 아닙니다\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 사용 예정 시간 : ");                  // 예외처리 10시를 넘어가는 경우
    scanf("%d", &duration);
    if(duration == 0) return 0;
    if(duration + start > 22){
        printf("\n\n 마감 시간을 넘겨졌습니다.\n");
        return 0;
    }

    // 사전에 다른 사람이 예약했는지 확인하기
    for(int i = 0; i < duration; i++){
        fseek(reserve, 140 + 4 * (start-8+i) + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fread(&a, sizeof(int), 1, reserve);
        if(a>0){
            printf(" %d시에 예약이 차 있습니다!\n", start + i);
            return 0;
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

    return 0;
    
}

// 예약 수정
int fixReserve(char *id){

    // 예약자로 저장된 파일 읽어올 포인터
    FILE *user; 
    // 예약정보 받아올 변수
    char reserveDay[7] = {0};
    int branchNum = 0;
    int roomNum = 0;
    int people = 0;
    int start = 0;
    int duration = 0;

    if(access(id, F_OK) < 0){
        printf(" 귀하의 예약 정보가 없습니다!\n");
        return 0;
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
    
    for(int i = 0; i < count; i++){
        fseek(user, i * USERRECORD, SEEK_SET);
        fread(reserveDay, sizeof(reserveDay), 1, user);
        fread(&branchNum, sizeof(int), 1, user);
        fread(&roomNum, sizeof(int), 1, user);
        fread(&people, sizeof(int), 1, user);
        fread(&start, sizeof(int), 1, user);
        fread(&duration, sizeof(int), 1, user);

        printf("\n\n========================\n\n");
        printf("=======<예약정보>=======\n");
        printf(" [ 예약 날짜 : %s ]\n [ 스터디 지점 : %d ]\n  스터디 공간 번호 : %d\n  인원 : %d\n  예약 시간 : %d\n  사용 시간 : %d\n", reserveDay,branchNum, roomNum, people, start, duration);
    }

    int select = 0;

    printf("\n========================\n");
    printf(" [1] : 예약 변경\n\n [2] : 초기화면\n\n\n");
    printf(" Option : ");
    scanf("%d", &select);
    // fclose(user);

    int num = 0;

    switch (select)
    {
    case 1:
        printf("\n 몇번째 예약을 변경하시겠습니까? : ");
        scanf("%d", &num);
        reReserve(num, id);
        break;
    case 2:
        break;
    default:
        printf(" Wrong Option!\n");
        break;
    }

    return 0;

}

int reReserve(int num, char *id){

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
        printf(" 해당 예약 번호는 존재하지 않습니다!\n");
        return 0;
    }

    char reset[USERRECORD] = {0};
    char reserveDay[7] = {0};
    int branchNum = 0;
    int roomNum = 0;
    int maxNum = 0;
    int people = 0;
    int start = 0;
    int duration = 0;
    
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

    // 날짜 예약 파일에 가서 변경전 데이터 지우기
    for(int i = 0; i < duration; i++){
        fseek(reserve, 140 + 4 * (start - 8 + i) + (roomNum - 1) * RECORDSIZE + (branchNum -1) * RECORDSIZE, SEEK_SET);
        fwrite(&timereset, sizeof(int), 1, reserve);
    }

    // 예약 정보 받기
    printf("\n\n 반드시, 존재하는 지점, 스터디 공간을 확인하세요 \n\n 0을 입력하시면 초기화면으로 이동합니다! \n\n");
    printRoom();
    printf("\n\n [0] : 초기화면\n 예약할 지점 번호 : ");                  // 없는 공간
    scanf("%d", &branchNum);
    if(branchNum == 0) return 0;
    if(bemptycheck(reserve, branchNum) == 0){
        printf(" 존재 하지 않는 지점 입니다.\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 예약할 스터디 공간 번호 : ");             // 가능한 공간
    scanf("%d", &roomNum);
    if(roomNum == 0) return 0;
    if(remptycheck(reserve, roomNum, branchNum) == 0){
        printf(" 존재 하지 않는 스터디 공간 입니다.\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 사용 인원 : ");                      // 예외처리 허용인원 초과
    scanf("%d", &people);
    if(people == 0) return 0;
    fseek(reserve, 8 + (roomNum-1)*ROOMRECORD + (branchNum -1) * RECORDSIZE, SEEK_SET);
    fread(&maxNum, sizeof(int), 1, reserve);
    if(people > maxNum){
        printf("\n\n 허용인원 초과입니다\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 사용 시작 시간 <8시~ 22시> : ");       // 예외처리 오전 8시 부터 밤 10시
    scanf("%d", &start);
    if(start == 0) return 0;
    if(start > 22){
        printf("\n\n 오픈 시간이 아닙니다\n");
        return 0;
    }
    printf("\n\n [0] : 초기화면\n 사용 예정 시간 : ");                  // 예외처리 10시를 넘어가는 경우
    scanf("%d", &duration);
    if(duration == 0) return 0;
    if(duration + start > 22){
        printf("\n\n 마감 시간을 넘겨졌습니다.\n");
        return 0;
    }

    // 사전에 다른 사람이 예약했는지 확인하기
    for(int i = 0; i < duration; i++){
        fseek(reserve, 140 + 4 * (start-8+i) + (roomNum-1) * ROOMRECORD + (branchNum-1) * RECORDSIZE, SEEK_SET);
        fread(&a, sizeof(int), 1, reserve);
        if(a>0){
            printf(" %d시에 예약이 차 있습니다!\n", start + i);
            return 0;
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

    return 0;
}

// 예약 파일 수정
// int resetFile(char *reserveDay, int branchNum, int roomNum, int people, int start, int duration){

//     FILE *reserve;
//     reserve = fopen(reserveDay, "r+");

//     int timereset = 0;
//     // 기존 파일에서 타인의 예약 정보는 남기기 위한 구현부
//     int a = 0;
//     int arrTime[14] = {0};

//     for(int i = 0; i < 14; i++){
//         fseek(reserve, 140 + 4 * i + (roomNum - 1) * RECORDSIZE + (branchNum -1) * RECORDSIZE, SEEK_SET);
//         fread(&a, sizeof(int), 1, reserve);
//         if(a>0){
//             arrTime[i] = 1;
//         }
//     }

//     for(int i = 0; i < duration; i++){
//         fseek(reserve, 140 + 4 * (start - 8 + i) + (roomNum - 1) * RECORDSIZE + (branchNum -1) * RECORDSIZE, SEEK_SET);
//         fwrite(&timereset, sizeof(int), 1, reserve);
//     }

//     fclose(reserve);

//     return 0;
// }