#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


typedef struct Node
{
    int rank;
    struct Node *parent;
} Node;

typedef struct Forest
{
    list<Node *> roots;
    int count;
} Forest;

Node *make_set(Forest *F)
{
    Node *node = new Node();
    node->rank = 0;
    node->parent = node;
    F->roots.push_back(node);
    F->count++;
    return node;
}

Node *find_set(Node *x)
{
    if (x != x->parent)
    {
        x->parent = find_set(x->parent);
    }
    return x->parent;
}

Node *link(Forest *F, Node *x, Node *y)
{
    if (x->rank > y->rank)
    {
        y->parent = x;
        auto iter = find(F->roots.begin(), F->roots.end(), y);
        F->roots.erase(iter);
        F->count--;
    }
    else
    {
        x->parent = y;
        auto iter = find(F->roots.begin(), F->roots.end(), x);
        F->roots.erase(iter);
        F->count--;
        if (x->rank == y->rank)
            y->rank++;
    }
}

Node *union_tree(Forest *F, Node *x, Node *y)
{
    Node* x_root = find_set(x);
    Node* y_root = find_set(y);
    if(x_root != y_root)
        link(F, x_root, y_root);
}

int main()
{
    string inpath = "../input/2_2_input.txt";
    string outpath_result = "../output/result.txt";
    string outpath_time = "../output/time.txt";

    int n_10[10][10], n_15[15][15], n_20[20][20], n_30[30][30], n_25[25][25];
    ifstream infile;
    ofstream outfile_result, outfile_time;
    infile.open(inpath);
    outfile_result.open(outpath_result);
    outfile_time.open(outpath_time);
    string buffer;
    vector<int> T;
    while (getline(infile, buffer))
    {
        string temp;
        istringstream data(buffer);
        while (data >> temp)
        {
            T.push_back(stoi(temp));
        }
    }
    //cout<<"T.size() = "<<T.size()<<endl;
    int count = 0;
    while (count < T.size())
    {
        if (count <= 99)
        {
            for (int i = 0; i < 10; i++)
                for (int j = 0; j < 10; j++)
                    n_10[i][j] = T[count++];
        }
        else if (count >= 100 && count <= 324)
        {
            for (int i = 0; i < 15; i++)
                for (int j = 0; j < 15; j++)
                    n_15[i][j] = T[count++];
        }
        else if (count >= 325 && count <= 724)
        {
            for (int i = 0; i < 20; i++)
                for (int j = 0; j < 20; j++)
                    n_20[i][j] = T[count++];
        }
        else if (count >= 725 && count <= 1349)
        {
            for (int i = 0; i < 25; i++)
                for (int j = 0; j < 25; j++)
                    n_25[i][j] = T[count++];
        }
        else if (count >= 1350 && count <= 2249)
        {
            for (int i = 0; i < 30; i++)
                for (int j = 0; j < 30; j++)
                    n_30[i][j] = T[count++];
        }
    }
#ifdef DEBUG
    cout << "n_25: " << endl;
    for (int i = 0; i < 25; i++)
    {
        cout << endl;
        for (int j = 0; j < 25; j++)
            cout << n_25[i][j] << " ";
    }
#endif

    double time;
    LARGE_INTEGER t1, t2, tc;

    //n=10
    Forest *F_10 = new Forest();
    F_10->count = 0;
    F_10->roots.clear();
    vector<Node *>person_10;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    for(int i = 0; i < 10; i++)
    {
        person_10.push_back(make_set(F_10));

    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if(n_10[i][j]==1)
            {
                union_tree(F_10,person_10[i],person_10[j]);
            }
                
        }
    }
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time<<"n=10 time = "<<time * 1000<<"ms"<<endl;
    cout<<"F_10->count="<<F_10->count<<endl;
    outfile_result<<"n=10 family numer is "<<F_10->count<<endl;


    //n=15
    
    Forest *F_15 = new Forest();
    F_15->count = 0;
    F_15->roots.clear();
    vector<Node *>person_15;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    for(int i = 0; i < 15; i++)
    {
        person_15.push_back(make_set(F_15));

    }
    for (int i = 0; i < 15; i++)
    {
        for (int j = i + 1; j < 15; j++)
        {
            if(n_15[i][j]==1)
            {
                union_tree(F_15,person_15[i],person_15[j]);
            }
                
        }
    }
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time<<"n=15 time = "<<time * 1000<<"ms"<<endl;
    cout<<"F_15->count="<<F_15->count<<endl;
    outfile_result<<"n=15 family numer is "<<F_15->count<<endl;


    //n=20

    Forest *F_20 = new Forest();
    F_20->count = 0;
    F_20->roots.clear();
    vector<Node *>person_20;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    for(int i = 0; i < 20; i++)
    {
        person_20.push_back(make_set(F_20));

    }
    for (int i = 0; i < 20; i++)
    {
        for (int j = i + 1; j < 20; j++)
        {
            if(n_20[i][j]==1)
            {
                union_tree(F_20,person_20[i],person_20[j]);
            }
                
        }
    }
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time<<"n=20 time = "<<time * 1000<<"ms"<<endl;
    cout<<"F_20->count="<<F_20->count<<endl;
    outfile_result<<"n=20 family numer is "<<F_20->count<<endl;

    //n=25
    Forest *F_25 = new Forest();
    F_25->count = 0;
    F_25->roots.clear();
    vector<Node *>person_25;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    for(int i = 0; i < 25; i++)
    {
        person_25.push_back(make_set(F_25));

    }
    for (int i = 0; i < 25; i++)
    {
        for (int j = i + 1; j < 25; j++)
        {
            if(n_25[i][j]==1)
            {
                union_tree(F_25,person_25[i],person_25[j]);
            }
                
        }
    }
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time<<"n=25 time = "<<time * 1000<<"ms"<<endl;
    cout<<"F_25->count="<<F_25->count<<endl;
    outfile_result<<"n=25 family numer is "<<F_25->count<<endl;

    //30
    Forest *F_30 = new Forest();
    F_30->count = 0;
    F_30->roots.clear();
    vector<Node *>person_30;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    for(int i = 0; i < 30; i++)
    {
        person_30.push_back(make_set(F_30));

    }
    for (int i = 0; i < 30; i++)
    {
        for (int j = i + 1; j < 30; j++)
        {
            if(n_30[i][j]==1)
            {
                union_tree(F_30,person_30[i],person_30[j]);
            }
                
        }
    }
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time<<"n=30 time = "<<time * 1000<<"ms"<<endl;
    cout<<"F_30->count="<<F_30->count<<endl;
    outfile_result<<"n=30 family numer is "<<F_30->count<<endl;



    infile.close();
    outfile_result.close();
    outfile_time.close();

    return 0;
}