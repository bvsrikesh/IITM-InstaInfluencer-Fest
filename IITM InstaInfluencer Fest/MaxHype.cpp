#include "ValidOrder.cpp"

class maxHype : public GraphAlgorithm
{
    private: 
        std::vector < int > top_scc;      // topsort of scc's
        std::vector < long long > hypemax;  
    public:
        void Query (Graph &G) override;
};

void maxHype :: Query (Graph &G) 
{
    G.DFS();
    G.SCC();
    G.SCC_Graph();
    G.DFS_SCC();
    G.Calc_MaxHype_SCC();
}