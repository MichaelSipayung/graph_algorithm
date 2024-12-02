import <vector>;
import <utility>;
import <iostream>;
import <sstream>;
import algo_graph;

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

int main() {
  std::cout << "input the data ... " << std::endl;
  std::istringstream iss("6 8\n0 5\n2 4\n2 3\n1 2\n0 1\n3 4\n3 5\n0 2\n");

  Graph g(iss);
  g.reverse_order();
  // connected_dfs(g, 0);
  test_breadth_first(g);
  test_depth_first(g);
  Graph sample_2(13);
  vector<pair<unsigned int, unsigned int>> edges = {
      make_pair(0, 5),  make_pair(4, 3), make_pair(0, 1), make_pair(9, 12),
      make_pair(6, 4),  make_pair(5, 4), make_pair(0, 2), make_pair(11, 12),
      make_pair(9, 10), make_pair(0, 6), make_pair(7, 8), make_pair(9, 11),
      make_pair(5, 3)};
  for (auto e : edges) {
    sample_2.add_edge(e.first, e.second);
  }
  sample_2.reverse_order();
  test_connected_components(sample_2);
  test_connected_components_show(sample_2);

  test_acyclic(g);
  //test_acylic(sample_2);

  /*std::istringstream read_g("2 1\n0 1\n");
  Graph g3(read_g);
  g3.reverse_order();
  test_acylic(g3);
  */
  return 0;
}

void test_connected_components(const Graph &data) {
  cout << "test connected components" << endl;
  auto cc = Connected_Components(data);
  // simple connectivity test
  cout << "is(0,3) connected components: " << cc.connected(0, 3) << endl;
  cout << "is(0,7) connected components: " << cc.connected(0, 7) << endl;
  cout << "is(7,12) connected components: " << cc.connected(7, 12) << endl;
  cout << endl;
}

void test_connected_components_show(const Graph &data) {
  cout << "show connected components" << endl;
  const auto cc = Connected_Components(data);
  vector<vector<unsigned int>> components;
  auto count = cc.count();
  components.resize(count);
  for (unsigned int i = 0; i < data.vertex_length(); i++)
    components[static_cast<adj_list::size_type>(cc.id(i))].push_back(i);
  // optional, make it's same with bag data type
  for (unsigned int i = 0; i < components.size(); i++)
    std::reverse(components[static_cast<adj_list::size_type>(i)].begin(),
                 components[static_cast<adj_list::size_type>(i)].end());
  for (unsigned int i = 0; i < count; i++) {
    for (auto v : components[static_cast<adj_list::size_type>(i)])
      cout << v << " ";
    cout << endl;
  }
}

void test_breadth_first(const Graph &data) {
  cout << "test breadth first - shortest path" << endl;
  std::cout << finding_path_test_bfs(data, 0) << endl;
}

void test_depth_first(const Graph &data) {
  cout << "test depth first - path to" << endl;
  std::cout << finding_path_test(data, 0) << endl;
}
void test_acyclic(Graph const &data) {
  std::cout << "test acyclic" << std::endl;
  auto cycle = Cycle(data);
  std::cout<<"acyclic graph: "<<cycle.has_cycle()<<std::endl;
}
