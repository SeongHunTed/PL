#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define RECORDSIZE 1024
#define ROOMRECORD 200
// 1024 byte : 4 byte : 지점 번호 || 200 byte : 스터디 공간 { 4byte : 스터디 공간 번호 4byte : 스터디 공간 수용 인원 ~~~~}

// 관리자 모드
// 지점과 스터디공간 번호는 중복을 허용하지 않음
// 지점 관리 1 ~ 6
//      1. 지점 추가    2. 지점 수정    3. 지점 삭제
// 2. 지점 수정 - > 스터디 공간 관리 1 ~ 5
//      1. 스터디 공간 추가     2. 스터디 공간 수정     3. 스터디 공간 삭제
//      스터디 공간 사용 인원 : 1 ~ 10
//      스터디 공간 : 이름, 층, 책상 개수, 보유 컴퓨터 개수 임의 지정

int managerMode();                                      // 관리자 모드
int insertBranch(FILE *fp);                             // 지점 추가
int manageRoom(FILE *fp);                               // 지점별 스터디 공간 관리
int deleteBranch(FILE *fp);                             // 지점 삭제
int bemptycheck(FILE *fp, int branchNum);               // 지점 존재 확인
int remptycheck(FILE *fp, int roomNum, int branchNum);  // 스터디 공간 존재 확인


int userMode();                                         // 사용자 모드
void printRoom();                                       // 스터디 공간 조회

int main(){
    while(1){
        int select = 0;
        printf("========================");
        printf("\n\n [1] 관리자 모드 \n [2] 사용자 모드 \n [3] 프로그램 종료 \n\n========================\n\n");
        printf(" Option : ");
        scanf("%d", &select);
        printf("\n\n========================");

        switch (select)
        {
        case 1:
            printf("\n\n <관리자 모드>\n\n");
            printf("========================");
            managerMode();
            break;
        case 2:
            printf("\n\n <User Mode>\n\n");
            printf("========================\n");
            userMode();
            break;
        case 3:
            printf("\n\n <Program Exit>\n\n");
            printf("========================");
            exit(0);
        default:
            printf("\n\n <Wrong Option!>\n\n");
            printf("========================");
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
        return 0;
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
    printf(" 추가 하실 지점 번호를 입력하세요 : ");
    scanf("%d", &branchNum);
            
    fseek(fp, (branchNum - 1) * RECORDSIZE, SEEK_SET);
    fread(&buf, sizeof(int), 1, fp);
        
    if(branchNum > 6){
        printf(" 잘못된 접근입니다!\n");
    }

    if(branchNum != buf){
        fseek(fp, (branchNum - 1) * RECORDSIZE, SEEK_SET);
        fwrite(&branchNum, sizeof(int), 1, fp);
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
    char recordbuf[180] = {0};
    printf("\n\n [1] 스터디 공간 추가 \n [2] 스터디 공간 수정 \n [3] 스터디 공간 삭제 [4] 초기 화면\n\n\n");
    printf(" Option : ");
    scanf("%d", &select);

    switch (select)
    {
    case 1:
        printf("\n\n <스터디 공간 추가> \n\n");
        
        printf(" 스터디 공간을 추가할 ||!지점 번호!||를 입력하세요 : ");
        scanf("%d", &branchNum);

        if(branchNum > 6){
            printf(" 잘못된 접근입니다!\n");
        }

        // 지점이 존재 하지 않을 경우 
        if(bemptycheck(fp, branchNum) == 0) {
            printf(" 존재하지 않는 지점 입니다!\n");
            return 0;
        }

        printf(" 추가할 스터디 공간의 번호를 입력하세요 : ");
        scanf("%d", &roomNum);

        if(roomNum > 5){
            printf(" 잘못된 접근입니다!\n");
            return 0;
        }

        printf(" 스터디 공간의 허용 인원을 설정해주세요 : ");
        scanf("%d", &maxNum);

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
        printf("\n\n <스터디 공간 수정> \n\n");
        printf(" 스터디 공간을 수정할 ||!지점 번호!||를 입력하세요");
        scanf("%d", &branchNum);
        if(branchNum > 6){
            printf(" 잘못된 접근입니다!\n");
        }

        // 지점이 존재 하지 않을 경우 
        if(bemptycheck(fp, branchNum) == 0) {
            printf(" 존재하지 않는 지점 입니다!\n");
            return 0;
        }

        printf(" 수정할 스터디 공간의 번호를 입력하세요 : ");
        scanf("%d", &roomNum);

        if(roomNum > 5){
            printf(" 잘못된 접근입니다!\n");
            return 0;
        }

        printf(" 스터디 공간의 허용 인원을 수정 해주세요 : ");
        scanf("%d", &maxNum);

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
        printf("\n\n <스터디 공간 삭제> \n\n");
        int deleteRoom = 0;
        int zero = 0;
        char emptyrecord[ROOMRECORD] = {0};
        printf("\n\n 삭제할 스터디 공간의 ||!지점번호!||를 입력하세요 : ");
        scanf("%d", &branchNum);
        printf("\n\n 삭제할 스터디 공간의 번호를 입력하세요 : ");
        scanf("%d", &deleteRoom);

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
    char emptyrecord[RECORDSIZE] = {0};
    printf("\n\n 삭제할 지점의 번호를 입력 하세요 : ");
    scanf("%d", &deleteBranch);

    if(bemptycheck(fp, deleteBranch)==0){
        printf(" 존재하지 않는 지점입니다!\n");
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
int bemptycheck(FILE *fp, int branchNum){
    int buf = 0;
    fseek(fp, (branchNum-1)*RECORDSIZE, SEEK_SET);
    fread(&buf, sizeof(int), 1, fp);

    if(buf != branchNum){
        // printf(" 존재하지 않는 지점입니다! \n");
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

int userMode(){
    FILE *user;

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

    if(access(id, F_OK) < 0){
        user = fopen(id, "w+");
    } else {
        user = fopen(id, "+r");
    }
    

    int select = 0;
    printf("========================");
    printf("\n\n [1] 스터디 공간 조회 \n [2] 신규 예약 \n [3] 예약 수정 \n [4] 초기화면 \n\n========================\n\n");
    printf(" Option : ");
    scanf("%d", &select);
    switch (select)
    {
    case 1:
        printRoom();
        break;
    case 2:

        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }


    return 0;
}

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
    char roominfo[ROOMRECORD] = {0};

    for(int i = 0;  i < 6; i++){
        fseek(fp, i * RECORDSIZE, SEEK_SET);
        fread(&branchNum, sizeof(int), 1, fp);
        for(int j = 0; j < 5; j++){
            fseek(fp, 4 + j * ROOMRECORD +  i * RECORDSIZE, SEEK_SET);
            fread(&roomNum, sizeof(int), 1, fp);
            fseek(fp, 8 + j * ROOMRECORD + i * RECORDSIZE, SEEK_SET);
            fread(&maxNum, sizeof(int), 1, fp);
            fseek(fp, 12 + j * ROOMRECORD + i * RECORDSIZE, SEEK_SET);
            fread(roominfo, sizeof(ROOMRECORD), 1, fp);
            if(roomNum > 0){
                printf("\n [ %d 지점 ] \t [ #%d 번 스터디 공간# 허용 인원 : %d | %s ]\n", branchNum, roomNum, maxNum, roominfo);
            }
        }
    }
}