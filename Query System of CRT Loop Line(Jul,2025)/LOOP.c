#include<stdio.h>
#include<string.h>
//注：1.编号方式：
//    以重庆北站南广场(ChongqingNorthStationSouthSquare)为原点，编号为0;
//    设逆时针方向为正，共33个车站，编号范围0-32.
//    2.运行方向：
//     内环：顺时针方向；外环：逆时针方向。
//    3.直快车：全程为唐家沱（Tangjiatuo)-跳蹬（Tiaodeng).程序设计时只考虑环线部分,即：民安大道（Min'anAve),冉家坝（Ranjiaba),沙坪坝（Shapingba）,
//      重庆图书馆（ChongqingLibrary),上桥（Shangqiao),重庆西站（ChongqingWestStation）.
//    4.票价计算：仅考虑环线部分。与实际票价出入，可能是因为存在多次换乘的更短路径。计算路径时亦只考虑内环外环两个方向，通过其他线路换乘的更优路径均不考虑。
//    5.所有信息来源截至25.7.1.在建、暂缓开通的线路和换乘均不考虑。
//    ***一句话：很简单，只要搞明白下面两件事：任一个车站，要么是普通站，要么是换乘站；要么停直快车和普通车，要么只停普通车。

//*1.自定义结构体存储车站信息：自定义名为Stations.

typedef struct{
    char name[100]; //车站名称（Name）.（如Ranjiaba（冉家坝））
    int num;//车站编号（Number）.（0-32）
    int isTransfer;//是否为换乘站（IsTransfer）.（0/1）
    char Tra[100];//换乘线路（Transfer）.用字符串表示。设计多条线路换乘的，用or连接）
    int isExpress;//是否为直快车停靠站（IsExpress）.（0/1）
    float dis;//到下一站距离（Distance）.单位为km.
    char cft[10];//内环方向首班车时间（Clockwise First Train）
    char ccft[10];//外环方向首班车时间（Counter-clockwise First Train)
    char clt[10];//内环方向末班车时间（Clockwise Last Train）
    char cclt[10];//外环方向末班车时间（Counter-clockwise Last Train）
}Stations;



//*2:设Stations为全局变量Sta，方便后续函数调用。

Stations Sta[33];

typedef struct{
    char Tname[100];
    int  Tsnum;
    int  Tnum;
    char d1[100];
    char d2[100];
    char d1ft[10];
    char d1lt[10];
    char d2ft[10];
    char d2lt[10];
}TransInfo;

TransInfo Tra[15];



int PRINT(int NUM){
    printf("Line %d\n",Tra[NUM].Tnum);
    printf("=================================\n");
    printf("                            First Train          Last Train\n");
    printf("To %s:%19s%22s\n",Tra[NUM].d1,Tra[NUM].d1ft,Tra[NUM].d1lt);
    printf("To %s:%19s%22s\n",Tra[NUM].d2,Tra[NUM].d2ft,Tra[NUM].d2lt);
    return 0;
}



int SingleTra(int NUM){
    printf("Transfer Info:\n");
    PRINT(NUM);
    return 0;
}

int MultiTra(int NUM,char s[100]){
    int Target;
    for(int i=0;i<14;i++){
        if(strcmp(Tra[i].Tname,Tra[i+1].Tname)==0&&(strcmp(s,Tra[i].Tname)==0)){
            printf("Select a line To check:\n");
            scanf("%d",&Target);
            if(Target==Tra[i].Tnum){
                PRINT(i);
                return 0;
            }
            else if(Target==Tra[i+1].Tnum){
                PRINT(i+1);
                return 0;
            }
            else{
                printf("Error:Invalid Transfer Line Option.\n");
                return 0;
            }
        }

    }
}

int CHECK(char s[100]){
    int j=0;
    int NUM=-1;
    for(int i=0;i<=14;i++){
        if(strcmp(s,Tra[i].Tname)==0){
            NUM=i;
            j++;
        }
    }
   switch(j){
        case 1:
        SingleTra(NUM);
        break;
        case 2:
        MultiTra(NUM,s);
        break;
        default:
        printf("   ");
        return 0;
   }
}


int StartTransfer(){
    FILE *fp;
    fp=fopen("LOOPT.txt","r");
    for(int i=0;i<15;i++){
        if(fscanf(fp,"%s %d %d %s %s %s %s %s %s",Tra[i].Tname,&Tra[i].Tsnum,&Tra[i].Tnum,Tra[i].d1,Tra[i].d2,Tra[i].d1ft,Tra[i].d2ft,Tra[i].d1lt,Tra[i].d2lt)!=9){
            printf("Error:File\n.");
            return 0;
        }
    }
    char s[100];
    printf("Insert Station:\n");
    scanf("%s",s);
    CHECK(s);
    return 0;
}






//*3：换乘信息输出：若第i站为换乘站，则输出可换乘线路。
//                    (例：i=4：冉家坝（Transfer to Line 5or6)).

int Transfer(int i){
    if((Sta[i].isTransfer)==1){
        printf("(Transfer to Line %s)\n",Sta[i].Tra);
    }
}

//*4.直快信息输出：判断能否乘坐直快车前往目的地。

int EXPRESS(int st,int ed){
    printf("===================================\n");
    //条件判断1：若直快车可达，则起点和终点站都必须是直快车停靠站。
    if((Sta[st].isExpress)==1&&(Sta[ed].isExpress)==1){
    printf("Train:Local/Express\n");
    //输出直快车和普通车均可达的信息。输出直快车途径站：
    printf("Express Via:\n");
    if(st<ed){
        //条件判断2：内环/外环方向途径站输出顺序不同，循环条件不同，需要分类判断。
        //          -外环方向（st<ed）：正序输出；-内环方向（st>ed）：倒序输出。
        for(int i=st;i<=ed;i++){
            if((Sta[i].isExpress)==1){
            printf("%s\n",Sta[i].name);
            Transfer(i);
        }
    }
    }

    else if(st>ed){
        for(int i=st;i>=ed;i--){
         if((Sta[i].isExpress)==1){
            printf("%s\n",Sta[i].name);
            Transfer(i);
}
        }
    }

    }
    //*条件判断1：若起点/终点任一站不是直快车停靠站，则只能乘坐普通车到达。
    else{
        printf("Train:Local\n");
    }
}

//*5：票价计算：根据Route函数中叠加的路程，分别对应以下票价：
//             0-6公里2元，6-11公里3元，以此类推，封顶7元。
int Fare(float dis){
    int total=0;
    if((dis>=0)&&(dis<=6)){
        total=2;
    }
    else if((dis>6)&&(dis<=11)){
        total=3;
    }
    else if((dis>11)&&(dis<=17)){
        total=4;
    }
    else if((dis>17)&&(dis<=24)){
        total=5;
    }
    else if((dis>24)&&(dis<=32)){
        total=6;
    }
    else if(dis>32){
        total=7;
    }
    printf("Prize:¥%d\n",total);
    return 0;
}

//***6：核心部分1：计算路径。
int Route(Stations Sta[]){
    int st=-1,ed=-1;//初始化起点st，终点ed编号为-1.
    char start[100];//start、end记录用户输入的车站名.
    char end[100];
    printf("From:");
    scanf("%s",start);//请用户输入起点站.
    printf("To:");
    scanf("%s",end);//请用户输入终点站.
    float dis=0;//初始化路程为0km.
    printf("==================================\n");
    //整个结构体线性遍历检索：若能找到用户输入的车站，更新st/ed为车站编号。
    for(int i=0;i<33;i++){
        if(strcmp(start,Sta[i].name)==0){
            st=i;
        }
        if(strcmp(end,Sta[i].name)==0){
            ed=i;
        }
    }
    //若任一车站序号没有被更新，则表示输入无效，退出。
    if(st==-1||ed==-1){
        printf("Error:Invalid Insert\n");
        return 0;
    }   

    //***路径计算：分为4种情况：是否越过原点，起终点编号大小比较。
    //例：0-10，10-0，33-4,4-33.
    if(st<ed){
    //*若起点站编号小于终点站，则外环方向为正。外环途径站数为ed-st.内环途径站数为33-（ed-st）.
    //例：0-10：外环=10-0=0；内环=33-10=33+0-10=23.
    //    4-32：外环=32-4=28；内环=33-8=33-32+4=5.显然，4-33若选择内环方向，需要先从4坐4站到0；越过原点（编号更为33）后，再坐一站到32.共5站。
        int cck1=ed-st;
        int clk1=st+33-ed;
        if(cck1<clk1){
            //若外环方向站数更少，则外环方向更近。输出推荐搭乘外环方向列车。
            printf("Direction:Counter-clockwise Loop\n");
            printf("Via:\n");
            // 输出外环方向普通车途径站点和换乘信息：
            for(int i=st;i<=ed;i++){
                printf("%s\n",Sta[i].name);
                Transfer(i);
            //累加计算通过的路程.
                dis +=Sta[i].dis;
            }
        }
        else if(cck1>clk1){
            ////若内环方向站数更少，则内环方向更近。输出推荐搭乘内环方向列车。
            printf("Direction:Clockwise Loop\n");
            printf("Via:\n");
            //输出内环方向普通车途径站点和换乘信息：
            //注意：当起点编号小于终点编号且内环方向更近时，显然，要越过原点。
            //例：4-32,6-31.需分段输出途径车站信息。
            for(int j=st;j>=0;j--){
                //先输出内环方向第一段途径信息：起点-原点(含，0号点）。
                printf("%s\n",Sta[j].name);
                Transfer(j);
                dis +=Sta[j].dis;
            }
            for(int j=32;j>=ed;j--){
                //再输出内环方向第二段途径信息：原点（不含，33号点）-终点。
                printf("%s\n",Sta[j].name);
                Transfer(j);
                dis +=Sta[j].dis;
            }

    }
}
//**st>ed的情况完全与正方向对称。这里仅做简单注释。
else if(st>ed){
    int clk2=st-ed;
    int cck2=33-st+ed;
    if(clk2<cck2){
        printf("Direction:Clockwise Loop\n");
        printf("Via:\n");
        for(int i=st;i>=ed;i--){
            printf("%s\n",Sta[i].name);
            Transfer(i);
            dis +=Sta[i].dis;
        }

    }
    else if(clk2>cck2){
        printf("Direction:Counter-clockwise Loop\n");
        printf("Via:\n");
        for(int i=st;i<=32;i++){
            printf("%s\n",Sta[i].name);
            Transfer(i);
            dis +=Sta[i].dis;
        }
        for(int i=0;i<=ed;i++){
            printf("%s\n",Sta[i].name);
            Transfer(i);
            dis +=Sta[i].dis;
        }
    }
}
EXPRESS(st,ed);
Fare(dis);
}


int Info(int i){
    printf("==================================\n");
    printf("Info about %s station:\n",Sta[i].name);
    printf("==================================\n");
    printf("First&Last Train Departure Schedule:\n");
    printf("                            First Train          Last Train\n");
    printf("Clockwise Loop:%19s%22s\n",Sta[i].cft,Sta[i].clt);
    printf("Counter-Clockwise Loop:%11s%22s\n",Sta[i].ccft,Sta[i].cclt);
        printf("==================================\n");
    printf("Transfer:%s\n",Sta[i].Tra);
        printf("==================================\n");
    if((Sta[i].isExpress)==1){
        printf("Train:Express/Local\n");
    }
    else{
        printf("Train:Local\n");
    }
    printf("==================================\n");
    printf("Next Station:\n");
    if((i!=0)&&(i!=32)){
    printf("→Counter-clockwise Loop Direction:%s\n",Sta[i+1].name);
    Transfer(i+1);
    printf("→Clockwise Loop Direction:%s\n",Sta[i-1].name);
    Transfer(i-1);
}

    else if(i==0){
    printf("→Counter-clockwise Loop Direction:%s\n",Sta[i+1].name);
    Transfer(i+1);
    printf("→Counter-clockwise Loop Direction:%s\n",Sta[32].name);
    Transfer(32);
    }

    else if(i==32){
        printf("→Counter-clockwise Loop Direction:%s\n",Sta[0].name);
        Transfer(0);
        printf("→Clockwise Loop Direction:%s\n",Sta[i-1].name);
        Transfer(i-1);
    }
    if(Sta[i].isExpress==1){
       printf("=================================\n");
       printf("Express Next stations:\n\n");
       printf("→To Tiaodeng:\n");
       for(int j=i+1;j<=32;j++){
        if(Sta[j].isExpress==1){
            printf("%s\n",Sta[j].name);
            Transfer(j);
        }
       }
       printf("\n");
         printf("→To Tangjiatuo:\n");
            for(int j=i-1;j>=0;j--){
                if(Sta[j].isExpress==1){
                    printf("%s\n",Sta[j].name);
                    Transfer(j);
                }
            }
        }
    }



int CHECKSINGLE(Stations Sta[]){
    char s[100];
    printf("Insert the station:\n");
    scanf("%s",s);
    for(int i=0;i<33;i++){
        if(strcmp(s,Sta[i].name)==0){
            Info(i);
            return 0;
        }
    }
    printf("Error:Station Not Found.\n");
    return 0;
}


int Start(){
    FILE *fp;
    fp=fopen("LOOP.txt","r");
    for(int i=0;i<=32;i++){
        if(fscanf(fp,"%s %d %d %s %d %f %s %s %s %s",Sta[i].name,&Sta[i].num,&Sta[i].isTransfer,Sta[i].Tra,&Sta[i].isExpress,&Sta[i].dis,Sta[i].cft,Sta[i].ccft,Sta[i].clt,Sta[i].cclt)!=10){
            printf("ERROR:FILE\n");
            return 0;
        }
    }
    printf("Welcome to Chongqing Rail Transit LOOP Line\n");
}    


int main(){
    if(Start()){
    int m;
    printf("==================================\n");
    printf("1:Route&Fare;2:Station Info;3.Transfer Schedule Info\nSELECT:");
    scanf("%d",&m);
    printf("==================================\n");
    switch(m){
        case 1:
        Route(Sta);
        break;

        case 2:
        CHECKSINGLE(Sta);
        break;

        case 3:
        StartTransfer();

        default:
        printf("Error:Invalid insert.\n");
        return 0;
    }
}
printf("==================================\n");
printf("Thank u for travelling on the Chongqing Rail Transit LOOP Line\n");
return 0;
}