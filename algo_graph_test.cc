import <vector>;
import <utility>;
import <iostream>;
import <sstream>;
import <stack>;
import <queue>;
import algo_graph;
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <cassert>

import <algorithm>;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::vector;
using std::string;

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

void test_mst_from_file(const string &filename);

void test_mst_from_file_eager(const string &filename);

int main()
{
    test_mst_from_file("tiny.txt");
    test_mst_from_file("medium.txt");
    test_mst_from_file("large.txt");

    test_mst_from_file_eager("tiny.txt");
    test_mst_from_file_eager("medium.txt");
    test_mst_from_file_eager("large.txt");
    // test_minindex_pq();
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
    const auto cycle = Cycle(data);
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

void test_mst_from_file(const string &filename)
{
    const auto edge_weight = EdgeWeightedGraph(filename);
    fmt::println("total vertex: {} edge: {}", edge_weight.v(), edge_weight.e());
    const auto lazy_prim = LazyPrimMST(edge_weight);
    /*for (const auto &i : lazy_prim.edges_to_vector())
        fmt::print("[{} {} {}]", i.either(), i.other(i.either()),
            i.weight());
    fmt::println("");*/

    // fmt::println("lazy version: {}", lazy_prim.weight());
    std::cout<<"lazy version: "<< lazy_prim.weight()<<endl;
}

void test_mst_from_file_eager(const string &filename)
{
    auto g = EdgeWeightedGraph(filename);
    fmt::println("total vertex: {} edge: {}", g.v(), g.e());
    const auto mst_eager = PrimMST(g);
    /*for (const auto &i : mst_eager.mst_edge())
        fmt::print("[{} {} {}]", i.either(), i.other(i.either()),
            i.weight());
    fmt::println("");*/
    auto wg = mst_eager.weight();
    std::cout << "eager version : "<<wg << std::endl;
}