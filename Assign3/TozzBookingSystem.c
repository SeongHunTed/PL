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

int managerMode();  // 관리자 모드
int manageRoom(FILE *fp); // 지점별 스터디 공간 관리
int deleteBranch(FILE *fp);
int bemptycheck(FILE *fp, int branchNum);
int remptycheck(FILE *fp, int roomNum, int branchNum);

// int userMode();     // 사용자 모드

int main(){
    
    int select = 0;

    while(1){
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
            printf("========================");
            // userMode();
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

int managerMode(){

    FILE *fp;
    if(access("Manage", F_OK) < 0){
        fp = fopen("Manage", "w+");
    } else {
        fp = fopen("Manage", "r+");
    }

    int select = 0;

    printf("\n\n [1] 지점 추가 \n [2] 지점 수정 \n [3] 지점 삭제 \n [4] 초기화면 \n\n\n");
    printf(" Option : ");
    scanf("%d", &select);
    int branchNum;
    int buf = 0;

    switch (select)
    {
    case 1:
        printf("\n\n <지점 추가> \n\n");
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
        } else {
            printf(" 이미 존재하는 지점 입니다! \n");
            return 0;
        }

        break;
        
    case 2:
        printf("\n\n <지점별 스터디 공간 관리>\n");
        manageRoom(fp);
        break;

    case 3:
        printf("\n\n <지점 삭제>\n");
        deleteBranch(fp);
        return 0;
    default:
        printf("Wrong Option!\n");
        break;
    }

    return 1;

}

int manageRoom(FILE *fp){

    int select = 0;     // 관리자 옵션 값
    int branchNum = 0;  // 지점 번호
    int roomNum = 0;    // 스터디 공간 번호
    int maxNum = 0;     // 스터디 공간 허용 인원
    char recordbuf[180] = {0};
    printf("\n\n [1] 스터디 공간 추가 \n [2] 스터디 공간 수정 \n [3] 스터디 공간 삭제 \n\n\n");
    printf(" Option : ");
    scanf("%d", &select);

    switch (select)
    {
    case 1:
        printf("\n\n <스터디 공간 추가> \n\n");
        
        printf(" 스터디 공간을 추가할 지점 번호를 입력하세요 : ");
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
    
    default:
        printf(" Wrong Option!\n");
        break;
    }
    return 0;
}

int deleteBranch(FILE *fp){
    int deleteBranch = 0;
    char emptyrecord[RECORDSIZE] = {0};
    printf("\n\n 삭제할 지점의 번호를 입력 하세요 : ");
    scanf("%d", &deleteBranch);

    if(bemptycheck(fp, deleteBranch)==0){
        printf(" 존재하지 않는 지점입니다!\n");
        return 0;
    }

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

    if(buf != roomNum){
        return 0;
    } else return 1;
}
