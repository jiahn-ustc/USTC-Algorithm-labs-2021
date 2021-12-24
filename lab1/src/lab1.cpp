#include<iostream>
#include<algorithm>
#include<fstream>
#include<string>
#include<vector>
#include<cstring>
#include<windows.h>
using namespace std;
const unsigned long long MAX_VALUE = 18446744073709551600ULL;


string str;
void print_OPTIMAL(int s[][30],int i,int j){
    if(i==j){
        cout<<"A"<<i;
        str = str + "A" + to_string(i);
    }   
    else{
        cout<<"(";
        str+="(";
        print_OPTIMAL(s,i,s[i][j]);
        print_OPTIMAL(s,s[i][j]+1,j);
        cout<<")";
        str+=")";
    }
}

int main(){
    string buffer1;
    string buffer2;
    string inpath = "../input/1_1_input.txt";
    //cout<<inpath<<endl;
    string outpath = "../output";
    ifstream infile;
    ofstream outfile_result,outfile_time;
    infile.open(inpath);
    int i=0,j=0,n=0;
    unsigned long long  m[30][30];
    int s[30][30];
    vector<unsigned long long> values;
    outfile_result.open(outpath+"/1_1_result.txt");
    outfile_time.open(outpath+"/1_1_time.txt");
    while(!infile.eof()){
        cout<<"Solutions: "<<endl;
        while(!values.empty()){
            values.pop_back();
        }
        getline(infile,buffer1);
        //cout << "buffer1: "<<buffer1 << endl;
        if(infile.eof())
            break;
        n = stoi(buffer1);
        getline(infile,buffer2);
        //cout<<"buffer2: "<<buffer2 << endl;
        for(i=0,j=0;i<buffer2.length();i++){
            if(buffer2[i]==' '){
                values.push_back(stoi(buffer2.substr(j,i-j)));
                j=i+1;
            }
        }
        values.push_back(stoi(buffer2.substr(j,i)));
        /*for(i=0;i<=n;i++){
            for(j=0;j<=n;j++){
                m[i][j]=0;
            }
        }*/
        memset(m,0,sizeof(m));
        memset(s,0,sizeof(s));
        i=0;
        j=0;
        cout<<"n="<<n<<endl;
        LARGE_INTEGER t1,t2,tc;
        QueryPerformanceFrequency(&tc);
        QueryPerformanceCounter(&t1);
        for(int l=2;l<=n;l++){
            for(i=1;i<=n-l+1;i++){
                j=i+l-1;
                //cout<<"i="<<i<<",j="<<j<<endl;
                m[i][j]=MAX_VALUE;
                for(int k=i;k<=j-1;k++){
                    unsigned long long temp = m[i][k]+m[k+1][j]+values[i-1]*values[k]*values[j];
                    if(temp<m[i][j]){
                        /*
                        if(n==5){
                            cout<<"k= "<<k<<endl;
                            cout<<"i="<<i<<endl;
                            cout<<"j="<<j<<endl;
                            cout<<"m[i][j]= "<<m[i][j]<<endl;
                            cout<<"values[i-1]= "<<values[i-1]<<endl;
                            cout<<"values[k]="<<values[k]<<endl;
                            cout<<"values[j]="<<values[j]<<endl;
                            cout<<"temp="<<temp<<endl;
                        }*/
                        m[i][j]=temp;
                        s[i][j]=k;
                        //cout<<"s["<<i<<"]["<<j<<"]="<<s[i][j]<<endl;
                    }
                }
            }
        }
        QueryPerformanceCounter(&t2);
        double time = double(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart;
        if(n==5){
            for(i=1;i<=n;i++){
                for(j=i;j<=n;j++){
                    cout<<"m["<<i<<"]["<<j<<"]="<<m[i][j]<<endl;
                }
            }
            for(i=1;i<=n;i++){
                for(j=i+1;j<=n;j++){
                    cout<<"s["<<i<<"]["<<j<<"]="<<s[i][j]<<endl;
                }
            }
        }
        str.clear();
        print_OPTIMAL(s,1,n);
        outfile_result<<"num="<<m[1][n]<<endl;
        outfile_result<<str<<endl;
        
        cout<<endl;
        
        cout<<"time = "<<time * 1000<<"ms"<<endl;
        
        outfile_time<<"time="<<time * 1000<<"ms"<<endl;
        
    }
    infile.close();
    outfile_result.close();
    outfile_time.close();
    return 0;
}