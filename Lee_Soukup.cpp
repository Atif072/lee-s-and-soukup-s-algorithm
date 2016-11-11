#include<bits/stdc++.h>

using namespace std;

char Lboard[100][100],Sboard[100][100];
int n,nblocks,bx,by,ntarget,sx,sy,desx,desy;
bool Lblock[10000],Sblock[10000];
int Lpath[10000],Spath[10000], dx[4]={-1,0,1,0},dy[4]={0,-1,0,1};
int srcX[100],srcY[100],desX[100],desY[100];
int super_id;


int TO_TARGET(int curr,int target) {
    int dis;
    int tx=target/n,ty=target%n,cx=curr/n,cy=curr%n;
    dis=abs(tx-cx)+abs(ty-cy);
    return dis;
}

int DIRECTION(int next,bool visit[],int target) {
    int dis=INT_MAX;
    int tx=target/n,ty=target%n,nx=next/n,ny=next%n;

    for(int i=0;i<4;i++) {
        int x=nx+dx[i],y=ny+dy[i];
        int id=x*n+y;
        if((x>=0 && x<n && y>=0 && y<n) && visit[id]==false && Sblock[id]==false) {
            if(dis>abs(tx-x)+abs(ty-y)) {
                dis=abs(tx-x)+abs(ty-y);
                super_id=id;
            }
        }
    }
    return dis;
}

int NGHBR_IN_DIR(int curr,bool visit[],int des) {
    int x=curr/n,y=curr%n;
    bool flag=true;
    int id;

    if(DIRECTION(curr,visit,des)<=TO_TARGET(curr,des)) {
        flag=false;
        id=super_id;
    }
    return flag==true?-1:id;
}

bool SoukupAlgo(int x1,int y1,int x2,int y2) {
    bool visit[n*n];
    memset(visit,false,(n*n)+1);

    int src=x1*n+y1,des=x2*n+y2;

    stack<int>plist;
    queue<int>nlist;
    cout<<"in plist "<<src<<endl;
    plist.push(src);
    Spath[src]=-1;
    visit[src]=true;

    int cnt=0;

    while(!plist.empty()) {
        int pid=plist.top();

        if(pid==des) {
            return true;
        }

        if(DIRECTION(pid,visit,des) <= TO_TARGET(pid,des)) {
            int id=super_id;
            //cout<<"in plist "<<id<<endl;
            plist.push(id);
            visit[id]=true;
            Spath[id]=pid;
            if(id==des) {
                return true;
            }

            while(NGHBR_IN_DIR(id,visit,des)>=0) {
                int new_id=NGHBR_IN_DIR(id,visit,des);
                //cout<<"in plist "<<new_id<<endl;
                plist.push(new_id);
                visit[new_id]=true;
                Spath[new_id]=id;
                if(new_id==des) {
                    return true;
                }
                id=new_id;
            }
        }
        while(!plist.empty()) {
            pid=plist.top();
            int tx=pid/n,ty=pid%n;
            for(int i=0;i<4;i++) {
                int x=tx+dx[i],y=ty+dy[i];
                int id=x*n+y;
                if((x>=0 && x<n && y>=0 && y<n)&& visit[id]==false && Sblock[id]==false) {
                    //cout<<"in nlist "<<id<<endl;
                    nlist.push(id);
                    visit[id]=true;
                    Spath[id]=pid;
                }
            }

            //cnt++;
            //if(cnt==50) return false;
            plist.pop();
        }
        while(!nlist.empty()) {
            plist.push(nlist.front());
            nlist.pop();
        }
    }
    return false;
}

bool LeeAlgo(int x1,int y1,int x2,int y2) {
    bool visit[n*n];
    memset(visit,false,(n*n)+1);

    int src=x1*n+y1,des=x2*n+y2;
    queue<int>q;
    q.push(src);
    Lpath[src]=-1;

    while(!q.empty()) {
        int p=q.front();
        visit[p]=true;
        int x=p/n,y=p%n;

        for(int i=0;i<4;i++) {
            int tx=x+dx[i],ty=y+dy[i];
            int id=tx*n+ty;

            if((tx>=0 && tx<n && ty>=0 && ty<n)&&(visit[id]==false)&&(Lblock[id]==false)) {
                if(id==des) {
                     Lpath[id]=p;
                     return true;
                }
                q.push(id);
                Lpath[id]=p;
            }
        }
        q.pop();
    }
    return false;
}

int main()
{
    ifstream in("input.txt");
    ofstream out1("lee.txt");
    ofstream out2("soukup.txt");
    in>>n;
    in>>nblocks;

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            Lboard[i][j]='0';
            Sboard[i][j]='0';
        }
    }
    memset(Lblock,false,(n*n)+1);
    memset(Sblock,false,(n*n)+1);

    for(int i=0;i<nblocks;i++) {
        in>>bx>>by;
        Lboard[bx][by]='#';
        Sboard[bx][by]='#';
        Lblock[bx*n+by]=true;
        Sblock[bx*n+by]=true;
    }
    bool ans[2][100];

    in>>ntarget;

    for(int i=0;i<ntarget;i++) {
        in>>sx>>sy>>desx>>desy;
        srcX[i]=sx;
        srcY[i]=sy;
        desX[i]=desx;
        desY[i]=desy;
        Lboard[sx][sy]='S';
        Lboard[desx][desy]='T';
        Sboard[sx][sy]='S';
        Sboard[desx][desy]='T';
    }
    //input process finish

    //--------------------For Lee's Algorithm---------------------------
    out1<<"Lee's Algorithm"<<endl;
    for(int i=0;i<ntarget;i++) {
        sx=srcX[i];
        sy=srcY[i];
        desx=desX[i];
        desy=desY[i];
        //for Lee's algorithm
        if(LeeAlgo(sx,sy,desx,desy)==true) {    //find destination and return Lpath;
            int id=desx*n+desy;
            Lblock[id]=true;
            Lblock[sx*n+sy]=true;

            while(Lpath[id]!=-1) {
                Lboard[Lpath[id]/n][Lpath[id]%n]='*';
                Lblock[Lpath[id]]=true;
                id=Lpath[id];
            }
            Lboard[sx][sy]='S';
            ans[0][i]=true;
        }
        else {      //there is no route
            Lboard[sx][sy]='s';
            Lboard[desx][desy]='t';
            ans[0][i]=false;
           // cout<<endl;
           // cout<<"Net ("<<sx<<","<<sy<<")->("<<desx<<","<<desy<<") cannot be routed."<<endl;
        }
    }
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) out1<<Lboard[i][j]<<" ";
        out1<<endl;
    }
    out1<<endl;
    for(int i=0;i<ntarget;i++) {
        if(ans[0][i]==false) {
            out1<<"Net ("<<sx<<","<<sy<<")->("<<desx<<","<<desy<<") cannot be routed."<<endl;
        }
    }

//--------------------------for Soukup Algorithm---------------------
    out2<<"Soukup Algorithm : "<<endl;
    for(int i=0;i<ntarget;i++) {
        sx=srcX[i];
        sy=srcY[i];
        desx=desX[i];
        desy=desY[i];
        //for Lee's algorithm
        if(SoukupAlgo(sx,sy,desx,desy)==true) {    //find destination and return Lpath;

            int id=desx*n+desy;
            Sblock[id]=true;
            Sblock[sx*n+sy]=true;
            //cout<<"fuck me"<<endl;
            while(Spath[id]!=-1) {
                Sboard[Spath[id]/n][Spath[id]%n]='*';
                Sblock[Spath[id]]=true;
                id=Spath[id];
            }
            Sboard[sx][sy]='S';
            ans[1][i]=true;
        }
        else {      //there is no route
            Sboard[sx][sy]='s';
            Sboard[desx][desy]='t';
            ans[1][i]=false;
        }
    }

    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) out2<<Sboard[i][j]<<" ";
        out2<<endl;
    }
    out2<<endl;
    for(int i=0;i<ntarget;i++) {
        if(ans[1][i]==false) {
            out2<<"Net ("<<sx<<","<<sy<<")->("<<desx<<","<<desy<<") cannot be routed."<<endl;
        }
    }

    //for soukup algorithm


    return 0;
}


