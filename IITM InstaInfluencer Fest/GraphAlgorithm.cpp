#include "Graph.cpp"

class GraphAlgorithm
{
    public:
        GraphAlgorithm (){}
        virtual ~GraphAlgorithm () {}
        virtual void Query (Graph &G) = 0;
};