import <vector>;
import <utility>;
import <iostream>;
import <sstream>;
import algo_graph;

using std::vector;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

int main() {
    /*Graph g(13);
    vector<pair<unsigned int, unsigned int> > edges = {
        make_pair(0, 5), make_pair(4, 3), make_pair(0, 1), make_pair(9, 12),
        make_pair(6, 4), make_pair(5, 4), make_pair(0, 2), make_pair(11, 12),
        make_pair(9, 10), make_pair(0, 6), make_pair(7, 8), make_pair(9, 11),
        make_pair(5, 3)
    };
    for (auto e: edges) {
        g.add_edge(e.first, e.second);
    }
    cout << g._to_string() << endl;

    std::istream &is = std::cin;
    Graph g2(is);
    cout << g2._to_string() << endl;
    cout << "g2 pass" << endl;

    Graph g3(6);
    vector<pair<unsigned int, unsigned int> > edges2 = {
        make_pair(1, 2), make_pair(1, 5), make_pair(2, 5), make_pair(4, 5),
        make_pair(2, 4), make_pair(2, 3), make_pair(4, 3),
    };
    for (auto e: edges2) {
        g3.add_edge(e.first, e.second);
    }
    cout << g3._to_string() << endl;

    // connected component test (graph processing)
    connected_dfs(g, 0);
    connected_dfs(g, 9);

    // connected case
    const Graph connect_case(std::cin);
    cout << connect_case._to_string() << endl;
    connected_dfs(connect_case, 0);
    connected_dfs(connect_case, 1);
    connected_dfs(connect_case, 2);
    connected_dfs(connect_case, 3);

    // try to connect
    g3.add_edge(0, 1);
    g3.add_edge(0, 2);
    connected_dfs(g3, 0);*/
    std::cout<<"input the data ... " << std::endl;
    std::istringstream iss("6 8\n0 5\n2 4\n2 3\n1 2\n0 1\n3 4\n3 5\n0 2\n");

    Graph g(iss);
    g.reverse_order();
    //connected_dfs(g, 0);
    std::cout<< finding_path_test(g,0);
    std::cout<<"breadth_first_search_test"<<std::endl;
    std::cout<< finding_path_test_bfs(g,0);
    return 0;
}
