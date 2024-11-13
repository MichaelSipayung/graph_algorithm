import <vector>;
import <utility>;
import <iostream>;
import algo_graph;

using std::vector;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

int main() {
    Graph g(13);
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
    cout<<"g2 pass"<<endl;

    Graph g3(6);
    vector<pair<unsigned int, unsigned int> > edges2 = {
        make_pair(1, 2), make_pair(1, 5), make_pair(2,5),make_pair(4, 5),
        make_pair(2,4), make_pair(2, 3), make_pair(4, 3),
    };
    auto temp=0;
    for (auto e: edges2) {
        g3.add_edge(e.first, e.second);
    }
    cout << g3._to_string() << endl;
    return 0;
}
