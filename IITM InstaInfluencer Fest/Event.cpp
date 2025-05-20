struct Event
{
    int  hype_score;   
    int  dt,ft;          // discovered time and finsih time in DFS forest
    int  color = 1;      // 1 is white  0 is grey -1 is black
    int  pi = 0;         // the parent in one of the shortest paths from which dfs is performed
};