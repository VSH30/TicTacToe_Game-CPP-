#include<iostream>
#include<conio.h>
#include<string.h>
#include<windows.h>
#include<mysql.h>
#include<sstream>
#include<iomanip>
#include<math.h>

using namespace std;

char x[2][30];
char arr[10]={'0','1','2','3','4','5','6','7','8','9'};

int rev(int a){
    int rev=0;
    while(a!=0){
        rev = rev*10 + a%10;
        a = a/10;
    }
    return rev;
}

void pattern(const char p1[30],const char p2[30]){
    char *g1, *g2;
    g1 = new char(strlen(p1)+1);
    strcpy(g1,p1);
    g2 = new char(strlen(p2)+1);
    strcpy(g2,p2);
	cout<<endl;
    cout<<g1<<" : X"<<endl;
    cout<<g2<<" : O"<<endl;
    cout<<"-------------------"<<endl;
    cout<<"|     |     |     |"<<endl;
    cout<<"|  "<<arr[1]<<"  |  "<<arr[2]<<"  |  "<<arr[3]<<"  |"<<endl;
    cout<<"|     |     |     |"<<endl;
    cout<<"-------------------"<<endl;
    cout<<"|     |     |     |"<<endl;
    cout<<"|  "<<arr[4]<<"  |  "<<arr[5]<<"  |  "<<arr[6]<<"  |"<<endl;
    cout<<"|     |     |     |"<<endl;
    cout<<"-------------------"<<endl;
    cout<<"|     |     |     |"<<endl;
    cout<<"|  "<<arr[7]<<"  |  "<<arr[8]<<"  |  "<<arr[9]<<"  |"<<endl;
    cout<<"|     |     |     |"<<endl;
    cout<<"-------------------"<<endl;
}

int status(){
    if((arr[1]==arr[2] && arr[2]==arr[3]) || (arr[4]==arr[5] && arr[5]==arr[6]) || (arr[7]==arr[8] && arr[8]==arr[9]) ||
       (arr[1]==arr[4] && arr[4]==arr[7]) || (arr[2]==arr[5] && arr[5]==arr[8]) || (arr[3]==arr[6] && arr[6]==arr[9]) ||
       (arr[1]==arr[5] && arr[5]==arr[9]) || (arr[3]==arr[5] && arr[5]==arr[7]))
        return 1;
    else if(arr[9]!='9' && arr[1]!='1' && arr[2]!='2' && arr[3]!='3' && arr[4]!='4' && arr[5]!='5' && arr[6]!='6' && arr[7]!='7' && arr[8]!='8')
        return 0;
    else
        return -1;
}

void game(MYSQL *conn,int solution=0){
    strcpy(arr,"0123456789");
    int op, result, p=1;
    char xo, stat[30];
    int temp=solution;
    int z=0;

    do{
        if(temp==0){
            system("CLS");
        }
        pattern(x[0],x[1]);
        p = (p%2)?1:2;

        cout<<x[p-1]<<"'s chance:"<<endl;
        cout<<"Enter a Choice : ";
        if(solution==0)
            cin>>op;
        else{
            int sss=solution%10;
            cout<<sss<<endl;
            op=sss;
            solution=solution/10;
        }
        //cout<<endl;

        xo = (p==1)?'X':'O';

        if((op==9 && arr[op]=='9') || (op==1 && arr[op]=='1') || (op==2 && arr[op]=='2') ||
           (op==3 && arr[op]=='3') || (op==4 && arr[op]=='4') || (op==5 && arr[op]=='5') ||
           (op==6 && arr[op]=='6') || (op==7 && arr[op]=='7') || (op==8 && arr[op]=='8')){
			arr[op]=xo;
			z = z*10 + op;
        }else{
            cout<<"*****Not Possible!!!*****"<<endl<<endl;
            p = p-1;
        }
        result = status();
        if(result==1)
            p = p - 1;
        else if(result==-1)
            p = p + 1;
    }while(result==-1);
    if(temp==0){
            system("CLS");
    }
    pattern(x[0],x[1]);

    if(result==0){
        cout<<"*****DRAW*****"<<endl;
        strcpy(stat,"DRAW");
    }else if(result==1){
        cout<<"*****"<<x[p]<<" WINS*****"<<endl;
        //string zz = x[p] + " Wins";
        strcpy(stat,x[p]);
    }
    if(temp==0){
        if(result==0 || result==1){
            //cout<<z<<endl;
            z=rev(z);
            cout<<z<<endl;
            stringstream ss;
            ss<<"INSERT INTO xox(player_A, player_B, status, moves) VALUES('"<<x[0]<<"','"<<x[1]<<"','"<<stat<<"',"<<z<<")";

            //INSERT
            int xstate=0;
            string qq = ss.str();
            const char*q = qq.c_str();
            xstate=mysql_query(conn,q);
            if(xstate==0)
                cout<<"Record inserted"<<endl<<endl;
            else
                cout<<"FAILED insertion"<<endl<<endl;
        }
    }
}

void detail(MYSQL *conn,int gid){
    //int count=0;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;
    int sol;

    stringstream ss;
    ss<<"SELECT * FROM xox WHERE gameid="<<gid;
    string qq = ss.str();
    const char*q = qq.c_str();

    qstate = mysql_query(conn,q);
    if(!qstate){
        res = mysql_store_result(conn);
        while((row = mysql_fetch_row(res))){
            strcpy(x[0],row[1]);
            strcpy(x[1],row[2]);
            sol = stoi(row[4]);
        }
        game(conn,sol);
    }else{
        cout<<"FAILED TO FETCH"<<endl;
    }
}

void line(){
    for(int i=0;i<112;i++)
            cout<<"-";
}

void history(MYSQL *conn){
    int count=0;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;
    qstate = mysql_query(conn,"SELECT * FROM xox ORDER BY gameid DESC");
    if(!qstate){
        res = mysql_store_result(conn);
        line();
        cout<<endl<<"|"<<setw(6)<<"GameID"<<"|"
            <<setw(30)<<"Player_A"<<"|"
            <<setw(30)<<"Player_B"<<"|"
            <<setw(30)<<"Status"<<"|"
            <<setw(10)<<"Moves"<<"|"<<endl;
        line();cout<<endl;
        while((row = mysql_fetch_row(res)) && count<10){
            cout<<"|"<<setw(6)<<row[0]<<"|"
            <<setw(30)<<row[1]<<"|"
            <<setw(30)<<row[2]<<"|"
            <<setw(30)<<row[3]<<"|"
            <<setw(10)<<row[4]<<"|"<<endl;
            count++;
        }
        line();cout<<endl;
        cout<<endl;
    }else{
        cout<<"FAILED TO FETCH"<<endl;
    }
}

int main(){
    MYSQL *conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn,"localhost","root","","cppgame",0,NULL,0);
    int id;
    int option;
    char enter[3];
    cout<<endl<<"+++++++ Welcome to VSH's XO Game +++++++"<<endl<<endl;
    cout<<"Type Go and press Enter to Continue: ";
    cin>>enter;
    system("CLS");
    while(1){
        cout<<"-----MENU-----"<<endl;
        cout<<"1. Play Game"<<endl;
        cout<<"2. Show History"<<endl;
        cout<<"3. Show Game Details"<<endl;
        cout<<"0. Exit"<<endl;

        cout<<endl<<"Enter your option: ";
        cin>>option;

        switch(option){
            case 1:
                //char stat[30];
                //int res;
                cout<<"Enter player 1 Name : ";
                cin>>x[0];
                cout<<"Enter player 2 Name : ";
                cin>>x[1];
                game(conn);
                break;
            case 2:
                system("CLS");
                history(conn);
                break;
            case 3:
                system("CLS");
                cout<<"Enter GameID : ";
                cin>>id;
                detail(conn,id);
                break;
            case 0:
                exit(0);
        }
    }
    return 0;
}


