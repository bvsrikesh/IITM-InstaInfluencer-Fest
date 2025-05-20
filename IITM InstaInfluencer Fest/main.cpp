#include "MaxHype.cpp"

int  main()
{
    int N,M;
    std::cin >> N >> M;
    Graph G(N,M);
    int hypescore;
    for ( int i = 1 ; i <= N ; i++)
    {
        std::cin >> hypescore;
        Event event ;
        event . hype_score = hypescore;
        G.addEvent(event);
    }
    int Event1,Event2;
    for ( int i = 0 ; i < M ; i++)
    {
        std::cin >> Event1 >> Event2;
        G.addEdge(Event1,Event2);
    }
    int Q,q;      // no of queries,query no
    std::cin >> Q;
    Graph M1 = G;
    GraphAlgorithm* iscycle = new isCycle;
    iscycle -> Query(M1);

    Graph M2 = G;
    GraphAlgorithm* indecomp = new indepComponent;
    indecomp -> Query(M2);

    Graph M3 = G;
    GraphAlgorithm* valord = new validOrder;
    valord -> Query(M3);

    Graph M4 = G;
    GraphAlgorithm* max_hypescorepe = new maxHype;
    max_hypescorepe -> Query(M4);

    for ( int i = 0 ; i < Q ; i++)
    {
        std::cin >> q;
        if (q == 1)
        {
            M1.printIscyclic();
        }
        else if (q == 2)
        {
            std::cout << M2.getno_scc() <<" "<< M2.getmax_scc_size() << "\n";
        }
        else if (q == 3)
        {
            M3.printTopsort();
        }
        else if (q == 4)
        {
            std::cout << M4.getmax_hypescore() << "\n";
        }
    }
}