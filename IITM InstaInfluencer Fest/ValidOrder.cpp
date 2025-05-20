#include "IndepComponent.cpp"

class validOrder : public GraphAlgorithm     
{
    public:
        std::set < int > vertices;
        void Query (Graph &G) override;
};

void validOrder :: Query (Graph &G)
{
    GraphAlgorithm* iscyclic = new isCycle;
    iscyclic -> Query(G);
    if (G.isCyclic())
    {
        return;
    }
    for ( int i = 1 ; i <= G.getN() ; i++)
    {
        vertices.insert(i);  
    }
    while (!vertices.empty())          // call DFS and find source vertex recursively
    {
        auto itr = vertices.begin();
        G.topsort(vertices,*itr);
    }
}