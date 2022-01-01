#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int max_weight = 21474836;

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

void Graphprint(Graph *G, int i, ofstream &outFile)
{
    auto vertex = G->vertexs[i];
    auto temp = vertex;
    vector<int> vertexId;
    vertexId.clear();
    while (temp != nullptr)
    {
        vertexId.push_back(temp->id);
        temp = temp->pi;
    }

    for (int j = vertexId.size() - 1; j >= 0; j--)
    {
        if (j == 0)
        {
            outFile << to_string(vertexId[j]);
            //cout << to_string(vertexId[j])<<endl;
        }
        else
        {
            outFile << to_string(vertexId[j]) + ',';
            // cout << to_string(vertexId[j]) + ','<<endl;
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

int main()
{
    string inPath = "../input/input11.txt";
    string resultPath = "../output/result11.txt";
    string timePath = "../output/time.txt";
    ifstream inFile;
    ofstream resultFile, timeFile;
    string buffer;
    double time;
    int row;
    LARGE_INTEGER t1, t2, tc;

    //input11.txt
    inFile.open(inPath);
    resultFile.open(resultPath);
    timeFile.open(timePath);
    Graph *G = new_graph();
    int N = 27;
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

    //测试图的构造是否正确
    /*
    int num = 0;
    for (int i = 0; i < N; i++)
    {
        auto v = G->vertexs[i];
        for (auto edge : v->edges)
        {
            printf("%d -> %d : %d\n", v->id, edge->t, edge->w);
            num++;
        }
    }
    printf("num=%d\n", num);
    printf("vertex_num:%d, edge_num=%d\n", G->vexnum, G->edgenum);
    for (auto iter = G->edges.begin(); iter != G->edges.end(); iter++)
    {
        int h_id = iter->first.first;
        int t_id = iter->first.second;
        Edge *edge = iter->second;
        printf("%d -> %d : %d\n", h_id, t_id, edge->w);
    }
    cout << "test is ok" << endl;*/

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input11,time = " << time * 1000 << "ms" << endl;
    /*
    for (int i = 0; i < N; i++)
    {
        auto v = G->vertexs[i];
        if (v->pi != nullptr)
            printf("%d->pi=%d\n", i, v->pi->id);
    }*/

    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input12.txt
    inPath = "../input/input12.txt";
    resultPath = "../output/result12.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input12,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input21.txt
    inPath = "../input/input21.txt";
    resultPath = "../output/result21.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input21,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input22.txt
    inPath = "../input/input22.txt";
    resultPath = "../output/result22.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input22,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input31.txt
    inPath = "../input/input31.txt";
    resultPath = "../output/result31.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input31,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input32.txt
    inPath = "../input/input32.txt";
    resultPath = "../output/result32.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input32,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input41.txt
    inPath = "../input/input41.txt";
    resultPath = "../output/result41.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input41,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();

    //input42.txt
    inPath = "../input/input42.txt";
    resultPath = "../output/result42.txt";
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
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    bellman_ford(G, 0);
    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    timeFile << "input42,time = " << time * 1000 << "ms" << endl;
    for (int i = 1; i < G->vexnum; i++)
    {
        if (G->vertexs[i]->d < max_weight)
        {
            resultFile << to_string(0) + ',' + to_string(i) + ',' + to_string(G->vertexs[i]->d) + ';';
            Graphprint(G, i, resultFile);
            resultFile << endl;
        }
    }
    destroy(G);
    G = nullptr;
    inFile.close();
    resultFile.close();


    timeFile.close();
    return 0;
}