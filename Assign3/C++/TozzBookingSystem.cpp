#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

#define RECORDSIZE 1024
#define ROOMRECORD 196

class Manage{

private:
    FILE *fp;
    int branchNum;
    int studyRoom;

public:
    Manage(){
        fp = fopen("Manage.txt", "w+");
        branchNum = NULL;
        studyRoom = NULL;
    }


    void insertBranch(){
        cout << "생성할 지점 번호 : ";
        cin >> branchNum;
        if(branchNum == 0) return;
        fwrite(&branchNum, sizeof(RECORDSIZE), 1, fp);
        fclose(fp);
    }

};

class User{

};

int main(){
    Manage mg;
    mg.insertBranch();
}