class indepComponent : public GraphAlgorithm
{
    public:
        void Query (Graph &G) override
        {
            G.DFS();
            G.SCC();
        }
};