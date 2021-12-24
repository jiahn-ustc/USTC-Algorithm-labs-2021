#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <windows.h>
using namespace std;

string out;
int num = 0;
ofstream outfile_result, outfile_time;
void print_lcs(int c[][40], string str1, string str2, int i, int j)
{
    if (i == 0 || j == 0)
    {
        num++;
        string temp=out;
        reverse(temp.begin(), temp.end());
        cout << "temp:"<<temp << endl;
        outfile_result << temp << endl;
        return;
    }
    if (str1[i] == str2[j])
    {
        out += str1[i];
        print_lcs(c, str1, str2, i - 1, j - 1);
        out.pop_back();
    }
    else
    {
        if (c[i - 1][j] > c[i][j - 1])
        {
            print_lcs(c, str1, str2, i - 1, j);
        }
        else if (c[i - 1][j] < c[i][j - 1])
        {
            print_lcs(c, str1, str2, i, j - 1);
        }
        else
        {
            print_lcs(c, str1, str2, i - 1, j);
            print_lcs(c, str1, str2, i, j - 1);
        }
    }
}

int main()
{
    string inpath = "../input/1_2_input.txt";
    string outpath = "../output";
    ifstream infile;
    infile.open(inpath);
    int len = 0;
    outfile_time.open(outpath + "/1_2_time.txt");
    string str1, str2;
    int n = 0;
    string buffer;
    int i = 0, j = 0;
    int c[40][40];
    while (!infile.eof())
    {
        memset(c, 0, sizeof(c));
        getline(infile, buffer);
        if (infile.eof())
            break;
        cout << "buffer: " << buffer << endl;
        cout << "buffer.size = " << buffer.length() << endl;
        n = stoi(buffer);
        cout << "n=" << n << endl;
        getline(infile, str1);
        getline(infile, str2);
        str1 = str1.insert(0, "#");
        str2 = str2.insert(0, "#");
        cout << "str1:" << str1 << endl
             << "str2:" << str2 << endl;
        string temp = outpath + "/result_" + to_string(n) + ".txt";
        cout << "temp: " << temp << endl;
        outfile_result.open(temp);
        cout << "open result" << endl;
        LARGE_INTEGER t1,t2,tc;
        QueryPerformanceFrequency(&tc);
        QueryPerformanceCounter(&t1);
        for (i = 1; i <= n; i++)
        {
            for (j = 1; j <= n; j++)
            {
                if (str1[i] == str2[j])
                {
                    c[i][j] = c[i - 1][j - 1] + 1;
                }
                else if (c[i - 1][j] >= c[i][j - 1])
                {
                    c[i][j] = c[i - 1][j];
                }
                else
                {
                    c[i][j] = c[i][j - 1];
                }
            }
        }
        QueryPerformanceCounter(&t2);
        double time = double(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart;
        for (i = 1; i <= n; i++)
        {
            for (j = 1; j <= n; j++)
            {
                printf("c[%d][%d]=%d\n", i, j, c[i][j]);
            }
        }
        cout << "over!" << endl;
        len = c[n][n];
        out.clear();
        num = 0;
        print_lcs(c, str1, str2, n, n);
        outfile_result << "num = " << num << endl;
        outfile_time<<"time = "<<time *1000<<"ms"<<endl;
        cout << "len=" << len << endl;
        outfile_result.close();
    }
    infile.close();
    outfile_time.close();
    return 0;
}