#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

typedef struct heap_node
{
    int key;
    int degree;
    list<struct heap_node *> children;
    //struct heap_node *children;
    struct heap_node *parent;
    struct heap_node *left;
    struct heap_node *right;
    bool mark;
} heap_node;

typedef struct heap
{
    struct heap_node *min;
    list<struct heap_node *> roots;
    int n;
} heap;

vector<heap_node *> N(1001);

heap *make_heap()
{
    heap *H = new heap();
    H->min = nullptr;
    H->roots.clear();
    H->n = 0;
    return H;
}

void addNodeInRoots(heap *H, heap_node *node)
{
#ifdef DEBUG
    cout << "enter addNodeInRoots" << endl;
    cout << "children->key= " << node->key << endl;
#endif
    H->n = H->n + 1;
    node->parent = nullptr;
    if (H->min != nullptr)
    {
        heap_node *temp = H->min->left;
        node->right = H->min;
        H->min->left = node;
        temp->right = node;
        node->left = temp;
    }
    else
    {
        node->right = node->left = node;
    }

    H->roots.push_back(node);
#ifdef DEBUG
    cout << "roots: ";
    for (auto val : H->roots)
        cout << val->key << " ";
    cout << "exit addNodeInRoots" << endl;
#endif
}
heap_node *removeNodeFromRoots(heap *H, heap_node *node)
{
#ifdef DEBUG
    cout << "enter removeNodeFromRoots" << endl;
    cout << "node->key = " << node->key << endl;
#endif

    auto iter = find(H->roots.begin(), H->roots.end(), node);

#ifdef DEBUG
    if (iter == H->roots.end())
        cout << "cant find node in roots" << endl;
#endif

    H->roots.erase(iter);

#ifdef DEBUG
    cout << "have erase " << node->key << endl;
    for (auto val : H->roots)
        cout << val->key << " ";
    cout << endl;
#endif

    H->n = H->n - 1;
    heap_node *left_node = node->left;
    heap_node *right_node = node->right;
    left_node->right = right_node;
    right_node->left = left_node;

#ifdef DEBUG
    cout << endl
         << "H->n = " << H->n << endl;
    cout << "exit removeNodeFromRoots" << endl;
#endif
}

void heap_insert(heap *H, int x)
{
    heap_node *node = new heap_node();
    N[x] = node;
    node->degree = 0;
    node->parent = nullptr;
    node->children.clear();
    node->mark = false;
    node->key = x;
    //node->left=node->right=node;
    if (H->min == nullptr)
    {
        H->min = node;
        H->roots.push_back(node);
        node->left = node->right = node;
    }
    else
    {
        H->roots.push_back(node);
        heap_node *temp = H->min->left;
        node->right = H->min;
        H->min->left = node;
        temp->right = node;
        node->left = temp;
        if (node->key < H->min->key)
            H->min = node;
    }
    H->n = H->n + 1;
}

int heap_minimum(heap *H)
{
    return H->min->key;
}

void addChildInParent(heap *H, heap_node *p, heap_node *child)
{
    if (p->children.size() == 0)
    {
        child->left = child->right = child;
    }
    else
    {
        auto right_node = p->children.front();
        auto left_node = p->children.back();
        left_node->right = child;
        child->left = left_node;
        right_node->left = child;
        child->right = right_node;
    }
    p->children.push_back(child);
    p->degree++;
    child->parent = p;
    H->n = H->n + 1;

#ifdef DEBUG
    cout << endl
         << "H->n=" << H->n << endl;
#endif
}

void heap_link(heap *H, heap_node *y, heap_node *x)
{
#ifdef DEBUG
    cout << "enter heap_link" << endl;
    if (y == nullptr)
        cout << "y is nullptr" << endl;
    cout << "y->key = " << y->key << endl;
    cout << "x->key = " << x->key << endl;
#endif

    removeNodeFromRoots(H, y);
    addChildInParent(H, x, y);
    y->mark = false;

#ifdef DEBUG
    cout << "exit heap_link" << endl;
#endif
}

void consoliDate(heap *H)
{
#ifdef DEBUG
    cout << "enter consoliDate" << endl;
#endif

    int max_degree = 0;
    max_degree = H->n;
    map<int, heap_node *> A;
    for (int i = 0; i <= max_degree; i++)
        A[i] = nullptr;

#ifdef DEBUG
    cout << "max_degree = " << max_degree << endl;
    cout << "A.size()=" << A.size() << endl;
#endif

    vector<heap_node *> temp_roots;
    temp_roots.assign(H->roots.begin(), H->roots.end());
    for (heap_node *w : temp_roots)
    {
        heap_node *x = w;
        int d = x->degree;
        while (A[d] != nullptr)
        {
            heap_node *y = A[d];
            if (x->key > y->key)
            {
                heap_node *temp = y;
                y = x;
                x = temp;
            }

            #ifdef DEBUG
            cout << "root.size() = " << H->roots.size() << endl;
            cout << "d = " << d << endl;
            cout << "x->key = " << x->key << "x->degree = " << x->degree << endl;
            cout << "y->key =" << y->key << "y->degree= " << y->degree << endl;
            #endif

            heap_link(H, y, x);
            A[d] = nullptr;
            d = d + 1;
        }
        A[d] = x;
    }
    H->min = nullptr;
    H->roots.clear();
    for (int i = 0; i <= max_degree; i++)
    {
        if (A[i] != nullptr)
        {
            if (H->min == nullptr)
            {

                addNodeInRoots(H, A[i]);
                H->n = H->n - 1;
                H->min = A[i];
            }
            else
            {
                addNodeInRoots(H, A[i]);
                H->n = H->n - 1;
                if (A[i]->key < H->min->key)
                    H->min = A[i];
            }
        }
    }
#ifdef DEBUG
    cout << "exit consoliDate" << endl;
#endif
}

heap_node *removeChildFromNode(heap *H, heap_node *p, heap_node *child)
{
#ifdef DEBUG
    cout << "enter removeChildFromNode" << endl;
#endif

    auto iter = find(p->children.begin(), p->children.end(), child);
    if (iter == p->children.end())
    {
        return nullptr;
    }
    p->children.erase(iter);
    p->degree--;
    auto left_node = child->left;
    auto right_node = child->right;
    left_node->right = right_node;
    right_node->left = left_node;
    H->n = H->n - 1;

#ifdef DEBUG
    cout << endl;
    cout << "H->n = " << H->n << endl;
    cout << endl;
    cout << "exit removeNodeFromNode" << endl;
#endif

    return child;
}

heap_node *heap_extract_min(heap *H)
{
#ifdef DEBUG
    cout << "enter heap_extract_min" << endl;
#endif

    heap_node *node = H->min;

#ifdef DEBUG
    cout << "H->min->key = " << node->key << endl;
    cout << "It has " << node->degree << " children" << endl;
    cout << "It has " << node->children.size() << " children" << endl;
#endif

    if (node != nullptr)
    {
        vector<heap_node *> temp;
        temp.assign(node->children.begin(), node->children.end());
        for (auto child : temp)
        {
            removeChildFromNode(H, node, child);
            addNodeInRoots(H, child);
        }
        /*
        while (node->children.size() != 0)
        {
            heap_node *child = node->children[node->children.size() - 1];
            removeChildFromNode(H,node,child);
            addNodeInRoots(H, child);
        }*/
        removeNodeFromRoots(H, node);
        if (node->right == node)
            H->min = nullptr;
        else
        {
            H->min = node->right;

            consoliDate(H);
        }
    }

#ifdef DEBUG
    cout << "exit heap_extract_min" << endl;
#endif

    return node;
}

heap *heap_union(heap *H1, heap *H2)
{
    heap *H = make_heap();
    H->min = H1->min;
    H->roots.assign(H1->roots.begin(), H1->roots.end());
    H->roots.insert(H->roots.end(), H2->roots.begin(), H2->roots.end());
    if (H1->min == nullptr || (H2->min != nullptr && H2->min->key < H1->min->key))
        H->min = H2->min;
    H->n = H1->n + H2->n;
    return H;
}

void heap_cut_node(heap *H, heap_node *x, heap_node *y)
{
#ifdef DEBUG
    cout << "enter heap_cut_node" << endl;
#endif

    auto child = removeChildFromNode(H, y, x);

#ifdef DEBUG
    cout << endl
         << "111111H->n= " << H->n << endl;
#endif

    addNodeInRoots(H, child);

#ifdef DEBUG
    cout << endl
         << "H->n= " << H->n << endl;
#endif

    child->mark = false;

#ifdef DEBUG
    cout << "exit heap_cut_node" << endl;
#endif
}

void cascading_heap_cut(heap *H, heap_node *y)
{
#ifdef DEBUG
    cout << "enter cascading_heap_cut" << endl;
#endif

    auto z = y->parent;
    if (z != nullptr)
    {
        if (!y->mark)
        {
            y->mark = true;
        }
        else
        {
            heap_cut_node(H, y, z);
            cascading_heap_cut(H, z);
        }
    }


#ifdef DEBUG
    cout << "exit cascading_heap_cut" << endl;
#endif
}

void heap_decrease_key(heap *H, heap_node *x, int k)
{
#ifdef DEBUG
    cout << "enter heap_decrease_key" << endl;
#endif


    if (k > x->key)
        return;
    x->key = k;
    auto y = x->parent;


#ifdef DEBUG
    if (y == nullptr)
    {
        cout << "y is nullptr" << endl;
    }
    else
    {
        cout << "y->key = " << y->key << endl;
    }
    cout << "x->key = " << x->key << endl;
#endif

    if (y != nullptr && x->key < y->key)
    {
        heap_cut_node(H, x, y);


#ifdef DEBUG
        cout << endl
             << "H->n= " << H->n << endl;
        cascading_heap_cut(H, y);
        cout << endl
             << "H->n= " << H->n << endl;
#endif

    }

    if (x->key < H->min->key)
        H->min = x;

#ifdef DEBUG
    cout << "exit heap_decrease_key" << endl;
    cout << "H->n= " << H->n << endl;
#endif
}
void heap_delete_node(heap *H, heap_node *x)
{
    heap_decrease_key(H, x, -1);
    heap_extract_min(H);
}

int main()
{
    for (int i = 0; i < N.size(); i++)
        N[i] = nullptr;
    string inpath = "../input/2_1_input.txt";
    string outpath_result = "../output/result.txt";
    string outpath_time = "../output/time.txt";
    ifstream infile;
    ofstream outfile_result, outfile_time;
    infile.open(inpath);
    outfile_result.open(outpath_result);
    outfile_time.open(outpath_time);
    vector<int> H1_insert;
    vector<int> H2_insert;
    vector<int> H3_insert;
    vector<int> H4_insert;
    int temp = 0;
    string buffer;
    while (!infile.eof())
    {
        getline(infile, buffer);
        temp++;
        if (temp <= 50)
        {
            H1_insert.push_back(stoi(buffer));
        }
        else if (temp >= 51 && temp <= 150)
        {
            H2_insert.push_back(stoi(buffer));
        }
        else if (temp >= 151 && temp <= 300)
        {
            H3_insert.push_back(stoi(buffer));
        }
        else if (temp >= 301 && temp <= 500)
        {
            H4_insert.push_back(stoi(buffer));
        }
    }
    //step1, set up H1-H4
    LARGE_INTEGER t1, t2, tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    heap *H1 = make_heap();
    for (auto val : H1_insert)
        heap_insert(H1, val);
    heap *H2 = make_heap();
    for (auto val : H2_insert)
        heap_insert(H2, val);
    heap *H3 = make_heap();
    for (auto val : H3_insert)
        heap_insert(H3, val);
    heap *H4 = make_heap();
    for (auto val : H4_insert)
        heap_insert(H4, val);

    QueryPerformanceCounter(&t2);
    double time_step1 = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;

    int temp_int;
    heap_node *temp_node;
    double time;
    //step2
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    outfile_result << "H1" << endl;

    heap_insert(H1, 249);
    outfile_result << H1->n << ",";

    heap_insert(H1, 830);
    outfile_result << H1->n << ",";

    temp_int = heap_minimum(H1);
    outfile_result << temp_int << ",";

    heap_delete_node(H1, N[127]);
    outfile_result << H1->n << ",";

    heap_delete_node(H1, N[141]);
    outfile_result << H1->n << ",";

    temp_int = heap_minimum(H1);
    outfile_result << temp_int << ",";

    heap_decrease_key(H1, N[75], 61);
    outfile_result << H1->min->key << ",";

    heap_decrease_key(H1, N[198], 169);
    outfile_result << H1->min->key << ",";

    temp_node = heap_extract_min(H1);
    outfile_result << temp_node->key << ",";

    temp_node = heap_extract_min(H1);
    outfile_result << temp_node->key << endl;

    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time << "step2:" << endl;
    outfile_time << "time = " << time * 1000 << "ms" << endl;

    //step3
    outfile_result << "H2" << endl;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    heap_insert(H2, 816);
    outfile_result << H2->n << ",";

    temp_int = heap_minimum(H2);
    outfile_result << temp_int << ",";

    heap_insert(H2, 345);
    outfile_result << H2->n << ",";

    temp_node = heap_extract_min(H2);
    outfile_result << temp_node->key << ",";

    heap_delete_node(H2, N[504]);
    outfile_result << H2->n << ",";

    heap_delete_node(H2, N[203]);
    outfile_result << H2->n << ",";

    heap_decrease_key(H2, N[296], 87);
    outfile_result << H2->min->key << ",";

    heap_decrease_key(H2, N[278], 258);
    outfile_result << H2->min->key << ",";

    temp_int = heap_minimum(H2);
    outfile_result << temp_int << ",";

    temp_node = heap_extract_min(H2);
    outfile_result << temp_node->key << endl;

    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time << "step3: " << endl;
    outfile_time << "time = " << time * 1000 << "ms" << endl;

    //step4
    outfile_result << "H3:" << endl;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    temp_node = heap_extract_min(H3);
    outfile_result << temp_node->key << ",";

    temp_int = heap_minimum(H3);
    outfile_result << temp_int << ",";

    heap_insert(H3, 262);
    outfile_result << H3->n << ",";

    temp_node = heap_extract_min(H3);
    outfile_result << temp_node->key << ",";

    heap_insert(H3, 832);
    outfile_result << H3->n << ",";

    temp_int = heap_minimum(H3);
    outfile_result << temp_int << ",";

    heap_delete_node(H3, N[134]);
    outfile_result << H3->n << ",";

    heap_delete_node(H3, N[177]);
    outfile_result << H3->n << ",";

    heap_decrease_key(H3, N[617], 360);
    outfile_result << H3->min->key << ",";

    heap_decrease_key(H3, N[889], 353);
    outfile_result << H3->min->key << endl;

    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time << "step4:" << endl;
    outfile_time << "time = " << time * 1000 << "ms" << endl;

    //step5
    outfile_result << "H4:" << endl;

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    temp_int = heap_minimum(H4);
    outfile_result << temp_int << ",";

    heap_delete_node(H4, N[708]);
    outfile_result << H4->n << ",";

    heap_insert(H4, 281);
    outfile_result << H4->n << ",";

    heap_insert(H4, 347);
    outfile_result << H4->n << ",";

    temp_int = heap_minimum(H4);
    outfile_result << temp_int << ",";

    heap_delete_node(H4, N[415]);
    outfile_result << H4->n << ",";

    temp_node = heap_extract_min(H4);
    outfile_result << temp_node->key << ",";

    heap_decrease_key(H4, N[620], 354);
    outfile_result << H4->min->key << ",";

    heap_decrease_key(H4, N[410], 80);
    outfile_result << H4->min->key << ",";

    temp_node = heap_extract_min(H4);
    outfile_result << temp_node->key << endl;

    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time << "step5: " << endl;
    outfile_time << "time = " << time * 1000 << "ms" << endl;

    //step6
    heap *H12 = heap_union(H1, H2);
    heap *H34 = heap_union(H3, H4);
    heap *H5 = heap_union(H12, H34);

    //step7
    outfile_result << "H5:" << endl;

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    temp_node = heap_extract_min(H5);
    outfile_result << temp_node->key << ",";

    temp_int = heap_minimum(H5);
    outfile_result << temp_int << ",";

    heap_delete_node(H5, N[800]);
    outfile_result << H5->n << ",";

    heap_insert(H5, 267);
    outfile_result << H5->n << ",";

    heap_insert(H5, 351);
    outfile_result << H5->n << ",";

    temp_node = heap_extract_min(H5);
    outfile_result << temp_node->key << ",";

    heap_decrease_key(H5, N[478], 444);
    outfile_result << H5->min->key << ",";

    heap_decrease_key(H5, N[559], 456);
    outfile_result << H5->min->key << ",";

    temp_int = heap_minimum(H5);
    outfile_result << temp_int << ",";

    heap_delete_node(H5, N[929]);
    outfile_result << H5->n << endl;

    QueryPerformanceCounter(&t2);
    time = double(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    outfile_time << "step7: " << endl;
    outfile_time << "time = " << time * 1000 << "ms" << endl;

    infile.close();
    outfile_result.close();
    outfile_time.close();
    for (int i = 0; i < N.size(); i++)
        delete N[i];
    delete H1;
    delete H2;
    delete H12;
    delete H3;
    delete H4;
    delete H34;
    delete H5;
    return 0;
}