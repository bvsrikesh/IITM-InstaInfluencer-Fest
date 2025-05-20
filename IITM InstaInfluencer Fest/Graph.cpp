#include "Event.cpp"

class Graph
{
    private:
        std::vector < Event > events;        
        std::vector< std::set < std::pair < int ,int > > > dependencies;   // the index of vertices which have edge and the type of edge 
        // -1 for back edge 0 tree edge 1 for forward edge 2 for cross edge
        int  N,M;         //  N num of Events , M no of dependencies
        bool cyclic = false;  // if it has a cyclic or not 
        std::vector < std::set < int > > inedges;     // inedges of the  vertices(reverse of edges/dependencies)
        int max_scc_size = 0;  // max size of scc
        int  no_scc= 0; // no of scc's
        std::vector < int > toposort;  // toplogical sort of graph
        long long  max_hypescore = 0;  // maximum hype_score of events 
        int t = 0;   // dfs time
        std::vector < int > reverse_finishetime;  // order of events in reverse finish times
        std::map < int , int > components;   // vertices to scc-component mapping 
        std::vector < long long > hype_components;    // hype score of scc components
        std::vector < std::set < int > > comp_edges;        // scc_component to set of scc_components edges(edges between scc_components)
        std::vector < bool > scc_visited;   // scc_visited vector for the scc's components
        std::vector < bool > inedgecomp;  // does scc_comp has inedge or not 
        std::vector < int > top_scc;      // topsort of scc's
        std::vector < long long > hypemax;  
    public:
        Graph( int n, int m)
        {
            N = n;
            M = m;
            events.resize(1);
            inedges.resize(N+1);
            dependencies.resize(N+1);   // intializing 0 index as we are using 1 index
        }
        ~Graph(){}
        void operator = ( Graph& G);
        void addEvent(Event event);
        void addEdge(int event1, int event2);
        void DFS();
        void DFS_VISIT (int i, int &dfs_count, long long &sum_hype, int comp);
        void SCC_Graph();
        void Gtranspose (Graph &GT);
        void SCC();
        void DFS_SCC();
        void DFS_SCC_VISIT (int i);
        void topsort (std::set < int > &vertices, int m);
        void Calc_MaxHype_SCC();
        void printTopsort();
        void printIscyclic(); 
        bool isCyclic()
        {
            return cyclic;
        }
        int getno_scc()
        {
            return no_scc;
        }
        int getmax_scc_size()
        {
            return max_scc_size;
        }
        long long getmax_hypescore()
        {
            return max_hypescore;
        }
        int getN()
        {
            return N;
        }
};

void Graph :: operator = ( Graph& G)
{                                // for this question
    G.events = events;
    G.dependencies = dependencies;
    G.inedges = inedges;
    G.N = N;
    G.M = M;
}

void Graph :: addEvent (Event event)
{
    events.push_back(event);
}

void Graph :: addEdge(int event1, int event2)
{
    dependencies[event1].insert({event2,0});
    inedges[event2].insert(event1);
}

void Graph :: DFS()
{
    t = 0;
    for ( int i = 1; i <= N ; i++)
    {
        if (events[i].color == 1)
        {
            int  s = 0;
            long long  shype=0;
            int  comp = -1;
            DFS_VISIT (i,s,shype,comp);
        }
    }
}

void Graph :: DFS_VISIT (int i, int &dfs_count, long long &sum_hype, int comp)
{
    dfs_count++;
    t = t + 1;
    events[i].dt = t;
    events[i] . color = 0;
    sum_hype += events[i]. hype_score;
    auto s = dependencies[i];        // s is the set of directed edge vertices 
    for (auto m : s)
    {
        if (events[m.first] . color == 0) {
            m.second = -1;  // if color is grey its back edge
            cyclic = true;
        }
        else if (events[m.first] . color == -1) m.second=1; // if color is black its forward or cross edge
        else if (events[m.first] . color == 1)  // if color is whitw its tree edge
        {
            events[m.first] . pi = i;
            DFS_VISIT (m.first,dfs_count,sum_hype,comp);
        }
    }
    t = t+1;
    events[i] . color = -1;
    events[i] . ft = t;
    reverse_finishetime.push_back(i);
    if (comp!=-1) components[i] = comp;
}

void Graph :: SCC_Graph()
{
    comp_edges.resize(no_scc+1);
    inedgecomp.resize(no_scc+1,false);
    for (int i = 1; i <= N ; i++)
    {   
        auto s = dependencies[i];         
        for (auto m: s)          // constucting scc_graph if there is an edge between vertices and if both aren't in same scc add edge between scc's
        {
            if (components[m.first] != components[i])
            {
                comp_edges[components[i]].insert(components[m.first]);
                inedgecomp[m.first] = true;
            }        
        }
    }
}

void Graph :: Gtranspose (Graph &GT)
{
    GT.events = events;
    GT.N = N;
    GT.M = M;
    GT.dependencies.resize(N+1);       // constructing G transpose
    for (int i = 1; i <= N ; i++)
    {
        GT.events[i] . color = 1; 
        for (auto m: dependencies[i])
        {
            GT.dependencies[m.first].insert({i,0});
        }
    }
}

void Graph :: SCC()
{
    Graph GT(N,M);
    Gtranspose(GT);
    GT.t = 0;
    int i = reverse_finishetime.size() - 1;
    GT.no_scc = 0;
    GT.hype_components.resize(N+1);
    hype_components.resize(N+1);
    for(; i >= 0 ; i--)
    {
        if(GT.events[reverse_finishetime[i]] . color == 1)
        {
            int s = 0;
            long long hype = 0;
            GT.no_scc++;
            GT.DFS_VISIT(reverse_finishetime[i],s,hype,GT.no_scc);
            hype_components[GT.no_scc] = hype;
            if (GT.max_scc_size <= s)
            {
                GT.max_scc_size = s;
            }
        }
    }
    components = GT.components;
    max_scc_size = GT.max_scc_size;
    no_scc = GT.no_scc;
    max_hypescore = GT.max_hypescore;
}

void Graph :: DFS_SCC()
{
    scc_visited.resize(no_scc+1,false);
    for ( int i = 1 ; i <= no_scc ; i++)
    {
        if (scc_visited[i] == false)
        {   
            DFS_SCC_VISIT(i);
        }
    }
}

void Graph :: DFS_SCC_VISIT (int i)
{   
    auto s = &comp_edges[i];           // DFS on SCC graph for calculating topsort of scc graph
    scc_visited[i] = true;  
    for (auto m: *s)
    {
        if (!scc_visited[m])
            DFS_SCC_VISIT(m);
    }
    top_scc.push_back(i);
}

void Graph :: topsort (std::set < int > &vertices, int m)
{
    if (inedges[m].empty())
    {
        vertices.erase(m);
        toposort.push_back(m);
        for (auto x : dependencies[m])
        {
            inedges[x.first].erase(m);
        }
    }
    else 
    {
        auto itr = inedges[m].begin();
        topsort(vertices,*itr);
    }
}

void Graph :: Calc_MaxHype_SCC()
{
    long long hy = 0;
    hypemax.resize(no_scc+1,0);
    std::reverse(top_scc.begin(),top_scc.end());
    for ( int i = 0 ; i < (int)top_scc.size() ; i++)
    {
        hypemax[top_scc[i]] += hype_components[top_scc[i]];
        hy = std::max(hy, hypemax[top_scc[i]]);
        for (auto &mv: comp_edges[top_scc[i]])
        {
            hypemax[mv] = std::max(hypemax[top_scc[i]], hypemax[mv]);
        }
    }
    max_hypescore = hy;
} 

void Graph :: printTopsort()
{
    if(cyclic)
    {
        std::cout << "NO\n";
        return;
    }
    int size = toposort.size();
    for( int i = 0; i < size ; i++)
    {
        std::cout << toposort[i] << " ";
    }
    std::cout << "\n";
}

void Graph :: printIscyclic()
{
    if(cyclic) std::cout << "YES\n";
    else std::cout << "NO\n";
}