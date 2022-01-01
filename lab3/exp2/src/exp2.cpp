#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int max_weight = 214748;

//弧的结构
typedef struct Edge
{
    int h, t; //该边的源结点与尾结点编号
    int w;    //弧的权值
} Edge;

//顶点的结构
typedef struct Vertex
{
    int id;     //顶点编号
    int degree; //顶点的出度
    int d;
    struct Vertex *pi;
    vector<Edge *> edges; //以该点为起始点的边的集合
} Vertex;

typedef struct
{
    map<int, Vertex *> vertexs; //id and address
    int vexnum, edgenum;        //图的当前顶点数和弧数
    map<pair<int, int>, Edge *> edges;
} Graph;

Graph *new_graph()
{
    Graph *graph = new Graph();
    graph->vertexs.clear();
    graph->vexnum = graph->edgenum = 0;
    graph->edges.clear();
    return graph;
}

Vertex *createVertex(Graph *G, int id)
{
    Vertex *vertex = new Vertex();
    vertex->id = id;
    vertex->degree = 0;
    vertex->pi = nullptr;
    vertex->edges.clear();
    G->vertexs[id] = vertex;
    G->vexnum++;
}
Edge *createEdge(Graph *G, int h_id, int t_id, int w)
{
    Edge *edge = new Edge();
    edge->h = h_id;
    edge->t = t_id;
    edge->w = w;
    pair<int, int> p(h_id, t_id);
    G->edges[p] = edge;
    G->edgenum++;
    Vertex *h_vertex = G->vertexs[h_id];
    h_vertex->degree++;
    h_vertex->edges.push_back(edge);
}

void addVertexsAndEdges(Graph *G, int id, vector<int> w)
{
    for (int i = 0; i < w.size(); i++)
    {
        if (w[i] != 0)
        {
            createEdge(G, id, i, w[i]);
        }
    }
}
void destroy(Graph *G)
{
    for (auto i = 0; i < G->vexnum; i++)
    {
        delete G->vertexs[i];
    }
    for (auto iter = G->edges.begin(); iter != G->edges.end(); iter++)
    {
        delete iter->second;
    }
    delete G;
}

void initialize_single_source(Graph *G, int s)
{
    for (int i = 0; i < G->vexnum; i++)
    {
        auto v = G->vertexs[i];
        v->d = max_weight;
        v->pi = nullptr;
    }
    G->vertexs[s]->d = 0;
}

void relax(Vertex *u, Vertex *v, int w)
{
    if (v->d > u->d + w)
    {
        v->d = u->d + w;
        v->pi = u;
    }
}

bool bellman_ford(Graph *G, int s)
{
    initialize_single_source(G, s);
    for (int i = 1; i < G->vexnum; i++)
    {
        for (auto iter = G->edges.begin(); iter != G->edges.end(); iter++)
        {
            Vertex *u = G->vertexs[iter->first.first];
            Vertex *v = G->vertexs[iter->first.second];
            int w = iter->second->w;
            relax(u, v, w);
        }
    }
    for (auto iter = G->edges.begin(); iter != G->edges.end(); iter++)
    {
        Vertex *u = G->vertexs[iter->first.first];
        Vertex *v = G->vertexs[iter->first.second];
        int w = iter->second->w;
        if (v->d > u->d + w)
            return false;
    }
    return true;
}

void dijkstra(Graph *G, int s)
{
    //cout << "enter dijkstra" << endl;
   // cout << "s=" << s << endl;
    initialize_single_source(G, s);
    list<int> S;
    list<int> Q;
    list<int>::iterator q_iter;
    list<int>::iterator iter;
    for (int i = 0; i < G->vexnum - 1; i++)
        Q.push_back(i);
    iter = find(Q.begin(), Q.end(), s);
    if (iter != Q.end())
    {
        Q.erase(iter);
    }
    S.push_back(s);
    int start = 0;
    while (!Q.empty())
    {
       // cout << "Q.size() = " << Q.size() << endl;
        if (start == 0)
        {
            Vertex *u = G->vertexs[s];
            for (auto edge : u->edges)
            {
                Vertex *v = G->vertexs[edge->t];
                relax(u, v, edge->w);
            }
            start = 1;
        }
        else
        {
            Vertex *temp = G->vertexs[*Q.begin()];
            int min_temp = G->vertexs[*Q.begin()]->d;
            q_iter = Q.begin();
            //cout<<"enter the second loop"<<endl;
            for (iter = Q.begin(); iter != Q.end(); iter++)
            {
                /* cout<<"*iter="<< *iter<<endl;
                cout<<"d="<<G->vertexs[*iter]->d<<endl;*/
                if (min_temp > G->vertexs[*iter]->d)
                {
                    min_temp = G->vertexs[*iter]->d;
                    temp = G->vertexs[*iter];
                    q_iter = iter;
                    /*
                    cout<<"in the loop"<<endl;
                    cout<<"min_temp="<<min_temp<<endl;
                    cout<<"*q_iter="<<*q_iter<<endl;*/
                }
            }
            /*
            cout<<"out the loop"<<endl;
            cout<<"min_temp="<<min_temp<<endl;
            cout<<"temp->id="<<temp->id<<endl;*/
            if (q_iter != Q.end())
            {
               // cout << "*q_iter=" << *q_iter << endl;
                Q.erase(q_iter);
            }
            S.push_back(temp->id);
            for (auto edge : temp->edges)
            {
                Vertex *v = G->vertexs[edge->t];
                relax(temp, v, edge->w);
            }
        }
    }
  //  cout << "exit dijkstra" << endl;
}

void johnson(Graph *G, map<pair<int, int>, int> &D)
{
    map<int, int> h;
    int N = G->vexnum;
    createVertex(G, N);
    for (int i = 0; i < N; i++)
    {
        createEdge(G, N, i, 0);
    }
    /*
    for (auto iter = G->edges.begin(); iter != G->edges.end(); iter++)
    {
        int h_id = iter->first.first;
        int t_id = iter->first.second;
        G->edges[make_pair(h_id, t_id)] += (h[h_id] - h[t_id]);
        printf("%d->%d=%d\n", h_id, t_id,G->edges[make_pair(h_id, t_id)]->w);
    }
*/
    if (!bellman_ford(G, N))
        cout << "the input graph contains a negative-weight cycle" << endl;
    else
    {
        //cout << "enter important complent" << endl;
        for (int i = 0; i < G->vexnum; i++)
        {
            Vertex *v = G->vertexs[i];
            h[v->id] = v->d;
        }
        for (auto iter = G->edges.begin(); iter != G->edges.end(); iter++)
        {
            int h_id = iter->first.first;
            int t_id = iter->first.second;
            G->edges[make_pair(h_id, t_id)]->w += (h[h_id] - h[t_id]);
        }
        for (int i = 0; i < N; i++)
        {
            Vertex *u = G->vertexs[i];
            dijkstra(G, u->id);

            for (int j = 0; j < N; j++)
            {
               // printf("u=%d,v=%d\n", u->id, j);
               // printf("distance(u,v)=%d,h(v)=%d,h(u)=%d\n", G->vertexs[j]->d, h[j], h[u->id]);
                D[make_pair(u->id, j)] = G->vertexs[j]->d + h[j] - h[u->id];
            }
        }
    }
}

int main()
{
    string inPath = "../input/input11.txt";
    string resultPath = "../output/result11.txt";
    string timePath = "../output/time.txt";
    ifstream inFile;
    ofstream resultFile, timeFile;
    string buffer;
    double time;
    int row,N;
    LARGE_INTEGER t1, t2, tc;
    map<pair<int, int>, int> D;

    //input11.txt
    inFile.open(inPath);
    resultFile.open(resultPath);
    timeFile.open(timePath);
    Graph *G = new_graph();
    N = 27;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input11,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;


    //input12.txt
    inPath = "../input/input12.txt";
    resultPath= "../output/result12.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 27;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input12,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;


    //input21.txt
    inPath = "../input/input21.txt";
    resultPath= "../output/result21.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 81;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input21,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;



    //input22.txt
    inPath = "../input/input22.txt";
    resultPath= "../output/result22.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 81;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input22,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;

    //input31.txt
    inPath = "../input/input31.txt";
    resultPath= "../output/result31.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 243;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input31,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;


    //input32.txt
    inPath = "../input/input32.txt";
    resultPath= "../output/result32.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 243;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input32,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;

    //input41.txt
    inPath = "../input/input41.txt";
    resultPath= "../output/result41.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 729;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input41,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;


    //input42.txt
    inPath = "../input/input42.txt";
    resultPath= "../output/result42.txt";
    inFile.open(inPath);
    resultFile.open(resultPath);
    G = new_graph();
    N = 729;
    for (int i = 0; i < N; i++)
    {
        createVertex(G, i);
    }
    row = 0;
    while (getline(inFile, buffer))
    {
        row++;
        vector<int> w;
        w.clear();
        stringstream line(buffer);
        string temp;
        while (getline(line, temp, ','))
        {
            w.push_back(stoi(temp));
        }
        addVertexsAndEdges(G, row - 1, w);
    }
    D.clear();
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    johnson(G, D);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input42,time = " << time * 1000 << "ms" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int w = D[make_pair(i, j)];
            if (w < max_weight)
                resultFile << to_string(w) << ' ';
            else
                resultFile << "X ";
        }
        resultFile<<"\n";
    }
    inFile.close();
    resultFile.close();
    destroy(G);
    G = nullptr;

    timeFile.close();
    return 0;
}