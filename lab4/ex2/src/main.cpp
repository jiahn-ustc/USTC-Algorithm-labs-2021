#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

map<pair<int, int>, vector<int>> result;
map<pair<int, int>, vector<int>> matchs;

void rabin_karp_matcher(string T, string P, int d, int q)
{
    int n = T.length();
    int m = P.length();
    int h=d % q;
    for(int i=1;i<=m-2;i++)
    {
        h = (h*(d%q))%q;
    }
    int p = 0;
    vector<int> t(n);
    t[0] = 0;
    for (int i = 1; i <= m; i++)
    {
        p = (d * p %q+ P[i - 1]%q) % q;
        t[0] = (d * t[0]%q + T[i - 1]%q) % q;
    }
    //cout<<"p= "<<p<<endl;
    //cout<<"t[0]= "<<t[0]<<endl;
    
    int sum = 0;
    vector<int> match;
    for (int s = 0; s <= n - m; s++)
    {
        if ((p - t[s]) % q == 0)
        {
            if (P == T.substr(s, m))
            {
                match.push_back(s + 1);
               // cout<<"s= "<<s<<"---------------------------------------------------------------- "<<endl;
            }
            else
                sum++;
        }
        if (s < n - m)
        {
            t[s + 1] = (d * (t[s] - T[s] * h)%q + T[s + m]%q) % q;
            //cout<<"t["<<s+1<<"]= "<<t[s+1]<<endl;
        }
    }
    /*
    cout<<"n="<<n<<",m="<<m<<",d="<<d<<",q="<<q<<endl;
    cout<<"sum="<<sum<<endl;
    cout<<"match:";
    for(int i=0;i<match.size(); i++)
        cout<<to_string(match[i])+" ";
    cout<<endl;*/
    
    result[make_pair(n, m)].push_back(sum);
    matchs[make_pair(n, m)]=match;
}

void solution(vector<string> T, vector<string> P, int d, int q, ofstream &timefile)
{
    timefile << "(" << d << "," << q << ")" << endl;
    LARGE_INTEGER t1, t2, tc;
    double time;
    for (int i = 0; i < T.size(); i++)
    {
        QueryPerformanceFrequency(&tc);
        QueryPerformanceCounter(&t1);
        rabin_karp_matcher(T[i], P[i], d, q);
        QueryPerformanceCounter(&t2);
        time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
        timefile << time * 1000 << "ms" << endl;
    }
}

int main()
{
    string inpath = "../input/4_2_input.txt";
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
    int d = 2, q = 13;
    
    solution(T, P, 2, 13, timefile);
    solution(T, P, 2, 1009, timefile);
    solution(T, P, 10, 13, timefile);
    solution(T, P, 10, 1009, timefile);
    
    for (auto iter = result.begin(); iter != result.end(); iter++)
    {
        //cout<<"n="<<iter->first.first<<",m="<<iter->first.second<<endl;
        auto sums = result[make_pair(iter->first.first, iter->first.second)];
        auto match = matchs[make_pair(iter->first.first, iter->first.second)];
        outfile<<to_string(match.size())<<endl;
        for(int i=0;i<sums.size();i++)
            outfile<<to_string(sums[i])<<" ";
        outfile<<endl;
        for(int i=0;i<match.size(); i++)
            outfile<<to_string(match[i])<<" ";
        outfile<<endl<<endl;
       
    }


 //   rabin_karp_matcher(T[2],P[2],2,13);

    infile.close();
    outfile.close();
    timefile.close();

    return 0;
}