#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

vector<int> compute_prefix(string P)
{
    //   cout<<"P:"<<P<<endl;
    int m = P.length();
    vector<int> pi(m);
    pi[0] = 0;
    int k = 0;
    for (int q = 2; q <= m; q++)
    {
        while (k > 0 && P[k] != P[q - 1])
            k = pi[k - 1];
        if (P[k] == P[q - 1])
            k = k + 1;
        pi[q - 1] = k;
        // printf("pi[%d]=%d\n",q-1,k);
    }
    // cout<<"compute_prefix is over"<<endl;
    return pi;
}

void kmp_matcher(string T, string P, ofstream &outfile, ofstream &timefile)
{
    int n = T.length();
    int m = P.length();
    LARGE_INTEGER t1, t2, tc;
    double time;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    vector<int> pi = compute_prefix(P);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timefile << "m= "<<m<<",m_time = " << time * 1000 << "ms";
    /*
    for(int i=0;i<pi.size();i++)
        cout<<pi[i]<<' ';*/
    vector<int> location;
    location.clear();
    //outfile << "index:";
    // cout<<endl;
    int q = 0;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    for (int i = 1; i <= n; i++)
    {
        //cout<<"q="<<q<<endl;
        while (q > 0 && P[q] != T[i - 1])
            q = pi[q - 1];
        if (P[q] == T[i - 1])
            q = q + 1;
        if (q == m)
        {
            location.push_back(i-m+1);
            //outfile << to_string(i - m + 1) + ' ';
            //cout<<i-m+1<<endl;
            // cout<<"match is ok"<<endl;
            q = pi[q - 1];
        }
    }
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timefile << ",n= "<<n<<",n_time = " << time * 1000 << "ms" << endl;
    outfile <<to_string(location.size())<<endl;
    for (int i = 0; i < pi.size(); i++)
        outfile << to_string(pi[i]) + ' ';
    outfile<<endl;
    for(int i = 0; i <location.size(); i++)
        outfile << to_string(location[i])+" ";
    
    
    outfile << endl<<endl;
}

int main()
{
    string inpath = "../input/4_1_input.txt";
    string outpath = "../output/result.txt";
    string timepath = "../output/time.txt";
    ifstream infile;
    ofstream outfile;
    ofstream timefile;
    infile.open(inpath);
    outfile.open(outpath);
    timefile.open(timepath);
    string buffer;
    int row = 0;
    vector<string> T;
    vector<string> P;
    while (getline(infile, buffer))
    {
        if (buffer == "\n")
            break;
        row++;
        if (row % 2 == 0)
            T.push_back(buffer);
        else
            P.push_back(buffer);
    }
    LARGE_INTEGER t1, t2, tc;
    double time;
    //kmp_matcher(T[1],P[1]);

    for (int i = 0; i < T.size(); i++)
    {
        kmp_matcher(T[i], P[i], outfile, timefile);
    }
    /*
    for (int i = 0; i < T.size(); i++)
    {
        QueryPerformanceFrequency(&tc);
        QueryPerformanceCounter(&t1);



        QueryPerformanceCounter(&t2);
        time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
        timefile << "input11,time = " << time * 1000 << "ms" << endl;
    }*/
    /*
    vector<int> p = compute_prefix("ababa");
    for(int i=0;i<p.size();i++)
        printf("p[%d]=%d\n",i,p[i]);*/

    infile.close();
    outfile.close();
    timefile.close();

    return 0;
}