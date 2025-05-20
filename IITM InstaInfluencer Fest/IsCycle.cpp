class isCycle : public GraphAlgorithm      
{                                            // there is cycle iff there is a back edge 
    public:                                 // at time of exploration of u v edge if v is gray then u v is back edge
        void Query (Graph &G) override
        {
            G.DFS();
        }
};