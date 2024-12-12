import <vector>;
import <utility>;
import <iostream>;
import <sstream>;
import <stack>;
import <queue>;
import algo_graph;
#include <fmt/core.h>
#include <fmt/ranges.h>

using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::vector;

typedef vector<vector<unsigned int>> adj_list;

void test_connected_components(const Graph &data);

void test_connected_components_show(const Graph &data);

void test_breadth_first(const Graph &data);

void test_depth_first(const Graph &data);

void test_acyclic(Graph const &data);

void test_bipartite(Graph const &data);

void test_symbol_table(const std::string &, char sp = ' ');

void test_reachability(const Digraph &d, const vector<unsigned int> &s);

void test_reachability(const Digraph &d, unsigned int s);

void test_directed_cycle(const Digraph &data);

void test_depth_first_order(const Digraph &data);

void test_topological_order(const Digraph &data);

void test_strong_components_simple(const Digraph &data, unsigned int v, unsigned int w);

void test_total_strong_components(const Digraph &data);

void test_total_strong_components_dag(const Digraph &data);

void test_show_strong_components(const Digraph &data);
int main()
{
    std::cout << "input the data ... " << std::endl;
    std::istringstream iss("6 8\n0 5\n2 4\n2 3\n1 2\n0 1\n3 4\n3 5\n0 2\n");

    Graph g(iss);
    g.reverse_order();
    // connected_dfs(g, 0);
    test_breadth_first(g);
    test_depth_first(g);
    Graph sample_2(13);
    vector<pair<unsigned int, unsigned int>> edges = {
        make_pair(0, 5), make_pair(4, 3),  make_pair(0, 1),   make_pair(9, 12), make_pair(6, 4),
        make_pair(5, 4), make_pair(0, 2),  make_pair(11, 12), make_pair(9, 10), make_pair(0, 6),
        make_pair(7, 8), make_pair(9, 11), make_pair(5, 3)};
    for (auto e : edges)
    {
        sample_2.add_edge(e.first, e.second);
    }
    sample_2.reverse_order();
    test_connected_components(sample_2);
    test_connected_components_show(sample_2);

    test_acyclic(g);
    // test_acylic(sample_2);

    /*std::istringstream read_g("2 1\n0 1\n");
    Graph g3(read_g);
    g3.reverse_order();
    test_acylic(g3);
    */
    test_bipartite(g);
    std::istringstream test_bipartitesample("3 2\n 0 1\n 1 2\n");
    Graph bipartite(test_bipartitesample);
    bipartite.reverse_order();
    test_bipartite(bipartite);
    test_bipartite(sample_2);

    std::istringstream iss_2("4 4\n0 1\n0 3\n2 1\n2 3\n");
    Graph graph_2(iss_2);
    graph_2.reverse_order();
    test_bipartite(graph_2);

    std::istringstream iss_3("8 8\n0 4\n0 5\n1 4\n1 6\n2 5\n2 7\n3 6\n3 7\n");
    Graph graph_3(iss_3);
    graph_3.reverse_order();
    test_bipartite(graph_3);

    // test_symbol_table("routes.txt");
    std::istringstream digraphsample("13 22\n 4 2\n 2 3\n 3 2\n 6 0\n 0 1\n 2 0\n 11 12\n 12 9\n 9 10\n"
                                     "9 11\n 8 9\n 10 12\n 11 4\n 4 3\n 3 5\n 7 8\n 8 7\n 5 4\n 0 5\n 6 4\n"
                                     "6 9\n 7 6\n");
    Digraph dg(digraphsample);
    fmt::println("vertex size : {}", dg.vertex_length());
    fmt::println("edge size : {}", dg.edge_length());
    dg.reverse_order();
    fmt::print("source 1 : ");
    test_reachability(dg, 1);

    fmt::print("source 0 : ");
    test_reachability(dg, 0);

    fmt::print("source 2 : ");
    test_reachability(dg, 2);
    fmt::print("source 7 : ");
    test_reachability(dg, 7);
    fmt::print("source 8 : ");
    test_reachability(dg, 8);
    fmt::print("source [1 2 6] : ");
    test_reachability(dg, {1, 2, 6});

    test_directed_cycle(dg); // has cycle return nil means digraph is DAG
    std::istringstream digraphsample2("13 15\n0 6\n 0 1\n 0 5\n"
                                      "2 3\n 2 0\n 3 5\n 5 4\n 6 9\n 6 4\n 7 6\n"
                                      "8 7\n 9 12\n 9 10\n 9 11\n 11 12\n");
    Digraph dg2(digraphsample2);
    dg2.reverse_order();
    test_depth_first_order(dg2);
    test_topological_order(dg2);

    test_strong_components_simple(dg, 7, 8);
    test_strong_components_simple(dg, 9, 12);
    test_strong_components_simple(dg, 0, 1);
    test_total_strong_components(dg);
    test_total_strong_components_dag(dg2);

    test_show_strong_components(dg);
    test_show_strong_components(dg2);

    return 0;
}

void test_connected_components(const Graph &data)
{
    cout << "test connected components" << endl;
    const auto cc = Connected_Components(data);
    // simple connectivity test
    cout << "is(0,3) connected components: " << cc.connected(0, 3) << endl;
    cout << "is(0,7) connected components: " << cc.connected(0, 7) << endl;
    cout << "is(7,12) connected components: " << cc.connected(7, 12) << endl;
    cout << endl;
}

void test_connected_components_show(const Graph &data)
{
    cout << "show connected components" << endl;
    const auto cc = Connected_Components(data);
    vector<vector<unsigned int>> components;
    const auto count = cc.count();
    components.resize(count);
    for (unsigned int i = 0; i < data.vertex_length(); i++)
        components[static_cast<adj_list::size_type>(cc.id(i))].push_back(i);
    // optional, make it's same with bag data type
    for (unsigned int i = 0; i < components.size(); i++)
        std::reverse(components[static_cast<adj_list::size_type>(i)].begin(),
                     components[static_cast<adj_list::size_type>(i)].end());
    for (unsigned int i = 0; i < count; i++)
    {
        for (const auto v : components[static_cast<adj_list::size_type>(i)])
            cout << v << " ";
        cout << endl;
    }
}

void test_breadth_first(const Graph &data)
{
    cout << "test breadth first - shortest path" << endl;
    std::cout << finding_path_test_bfs(data, 0) << endl;
}

void test_depth_first(const Graph &data)
{
    cout << "test depth first - path to" << endl;
    std::cout << finding_path_test(data, 0) << endl;
}
void test_acyclic(Graph const &data)
{
    std::cout << "test acyclic" << std::endl;
    auto cycle = Cycle(data);
    std::cout << "acyclic graph: " << cycle.has_cycle() << std::endl;
}
void test_bipartite(Graph const &data)
{
    const auto two_coloring = TwoColor(data);
    cout << "is bipartite graph: " << two_coloring.is_bipartite() << std::endl;
}

void test_symbol_table(const std::string &file_name, const char sp)
{
    const auto st = SymbolGraph(file_name, sp);
    Graph g = st.get_graph();
    g.reverse_order();
    std::string line;
    while (std::cin >> line)
    {
        fmt::print(" {}", g.adj(st.index(line)));
        fmt::print(" -> ");
        vector<std::string> temp;
        for (auto w : g.adj(st.index(line)))
            temp.push_back(st.name(w));
        fmt::println("{}", temp);
    }
}

void test_reachability(const Digraph &d, const vector<unsigned int> &s)
{
    const auto reachable = DirectedDFS(d, s);
    for (unsigned int i = 0; i < d.vertex_length(); i++)
        if (reachable.marked(i))
            fmt::print("{} ", i);
    fmt::println("");
}

void test_reachability(const Digraph &d, unsigned int s)
{
    const auto reachable = DirectedDFS(d, s);
    for (unsigned int i = 0; i < d.vertex_length(); i++)
        if (reachable.marked(i))
            fmt::print("{} ", i);
    fmt::println("");
}

void test_directed_cycle(const Digraph &data)
{
    if (const auto dr_cycle = DirectedCycle(data); dr_cycle.has_cycle())
        fmt::println("directed cycle : {} ", dr_cycle.cycle());
    else
        fmt::println("no cycle on digraph or digraph is DAG");
}

void test_depth_first_order(const Digraph &data)
{
    const auto dfs_order = DepthFirstOrder(data);
    fmt::println("depth ordering");
    fmt::println("preorder : {}", dfs_order.pre_order());
    fmt::println("postorder : {}", dfs_order.post_order());
    auto col = dfs_order.reverse_post_order();
    fmt::print("reverse post order : [ ");
    while (!col.empty())
    {
        fmt::print("{} ", col.top());
        col.pop();
    }
    fmt::println("]");
    // fmt::println("reverse post ordering : {}", dfs_order.reverse_post_order());
}

void test_topological_order(const Digraph &data)
{
    if (const auto topological_order = Topological(data); topological_order.is_dag())
        fmt::println("Topological order : {}", topological_order.order());
    else
        fmt::println("digraph is not DAG");
}

void test_strong_components_simple(const Digraph &data, unsigned int v, unsigned int w)
{
    if (const auto str = KosarajuSCC(data); str.strongly_connected(v, w))
        fmt::println("strongly connected components v {}, w {}", v, w);
    else
        fmt::println("not strongly connected components");
}

void test_total_strong_components(const Digraph &data)
{
    if (const auto str = KosarajuSCC(data); str.count() != 0)
        fmt::println("total strong components : {}", str.count());
    else
        fmt::println("strong components is nil");
}

void test_total_strong_components_dag(const Digraph &data)
{
    fmt::println("assume DAG, total strong components must be equal to : {}", data.vertex_length());
    if (const auto str = KosarajuSCC(data); str.count() != 0)
        fmt::println("total strong components : {}", str.count());
    else
        fmt::println("not strong components");
}
void test_show_strong_components(const Digraph &data)
{
    const auto str = KosarajuSCC(data);
    vector<vector<unsigned int>> cc;
    cc.resize(str.count());
    for (auto i = 0; i < str.count(); i++)
    {
        for (auto j = 0; j < data.vertex_length(); j++)
            if (str.id(j) == i)
                cc[i].push_back(j);
    }
    fmt::println("{} components ", str.count());
    for (auto &i : cc)
        fmt::println("{} ", i); // print all strong components
}
