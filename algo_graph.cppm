module;
export module algo_graph;
import <iostream>;
import <vector>;
import <istream>;
import <string>;
import <stack>;
import <queue>;
import <fstream>;
import <map>;

using std::map;
using std::queue;
using std::string;
using std::vector;
typedef std::vector<vector<unsigned int>> adj_list;

// graph interface
export class Graph
{
  public:
    // explicitly ask total vertex to create
    explicit Graph(const unsigned int v) : _v(v)
    {
        initialize(v);
    }

    // ctor require input from user
    explicit Graph(std::istream &is);

    // add edge v-w
    void add_edge(unsigned int v, unsigned int w);

    // reverse the order, assume the structure is equal to book
    auto reverse_order();

    // check total vertex
    [[nodiscard]] constexpr auto vertex_length() const
    {
        return _v;
    }

    // check total edge
    [[nodiscard]] auto edge_length() const
    {
        return _e;
    }

    static unsigned int degree(Graph const &g, int v);

    // return array which adjacent to v
    [[nodiscard]] constexpr vector<unsigned int> adj(auto v) const
    {
        return _adj[v];
    }

    static constexpr unsigned int max_degree(Graph const &g)
    {
        unsigned int max_deg = 0;
        for (auto v = 0; v < g.vertex_length(); v++)
            if (degree(g, v) > max_deg)
                max_deg = degree(g, v);
        return max_deg;
    }

    static constexpr auto avg_degree(Graph const &g)
    {
        return 2 * g.edge_length() / g.vertex_length();
    }

    static constexpr auto num_self_loops(Graph const &g)
    {
        unsigned int num_self_loops = 0;
        for (auto v = 0; v < g.vertex_length(); v++)
            for (auto f : g.adj(v))
                if (f == v)
                    num_self_loops++;
        return num_self_loops;
    }

    // store the result vertex and edges on a string
    [[nodiscard]] constexpr auto _to_string() const
    {
        auto s = std::to_string(_v);
        s.append(" vertices ");
        s.append(std::to_string(_e));
        s.append(" edges \n");
        for (auto v = 0; v < _v; v++)
        {
            s.append(std::to_string(v));
            s.append(": ");
            for (auto w : adj(v))
            {
                s.append(std::to_string(w));
                s.append(" ");
            }
            s.append("\n");
        }
        return s;
    }

  private:
    constexpr void initialize(const unsigned int v)
    {
        _adj.resize(v);
    }

    adj_list _adj;       // adjacency lists
    unsigned int _e = 0; // number of edges
    unsigned int _v = 0; // number of vertices
    static void check_invalid_vertex(const unsigned int v, unsigned int _limit)
    {
        if (v >= _limit)
            throw std::out_of_range("Vertex index out of range, limit : " + std::to_string(_limit));
    }
};

//  depth first search interface
export class DepthFirstSearch
{
  public:
    DepthFirstSearch(Graph const &g, unsigned int s)
    {
        _marked.resize(g.vertex_length());
        dfs(g, s);
    }

    [[nodiscard]] constexpr auto marked(unsigned int w) const
    {
        return _marked[static_cast<adj_list::size_type>(w)];
    }

    [[nodiscard]] constexpr auto count() const
    {
        return _count;
    }

  private:
    vector<bool> _marked;
    unsigned int _count = 0;

    void dfs(Graph const &g, unsigned int v);
};

// check whether given s as vertex is connected component on g
export void connected_dfs(Graph const &g, unsigned int s)
{
    DepthFirstSearch data(g, s);
    std::cout << "vertex : " << s << std::endl;
    for (unsigned int v = 0; v < g.vertex_length(); v++)
        if (data.marked(v))
            std::cout << v << " ";
    std::cout << std::endl;
    if (data.count() != g.vertex_length())
        std::cout << "not ";
    std::cout << "connected" << std::endl;
}

// dfs interface to find paths in a graph
export class DepthFirstPaths
{
  public:
    DepthFirstPaths(Graph const &g, unsigned int s);

    void dfs(Graph const &g, unsigned int v);

    [[nodiscard]] bool has_path_to(unsigned int v) const
    {
        return _marked[static_cast<adj_list::size_type>(v)];
    }

    // return the path to v
    [[nodiscard]] vector<unsigned> path_to(unsigned int v) const;

  private:
    vector<bool> _marked;          // has dfs been called for this vertex ?
    vector<unsigned int> _edge_to; // last vertex on known path to this vertex
    unsigned int _s = 0;           // source
    static void check_input(unsigned int _source, unsigned int _expected)
    {
        if (_source >= _expected)
            throw std::out_of_range("Source is not a valid input");
    }
};

export auto finding_path_test(Graph const &g, unsigned int s)
{
    std::string temp;
    const auto search = DepthFirstPaths(g, s);
    for (unsigned int v = 0; v < 6; v++)
    {
        temp.append(std::to_string(s));
        temp.append(" to ");
        temp.append(std::to_string(v));
        temp.append(": ");
        if (search.has_path_to(v))
        {
            for (auto x : search.path_to(v))
            {
                if (x == s)
                    temp.append(std::to_string(x));
                else
                {
                    temp.append("-");
                    temp.append(std::to_string(x));
                }
            }
        }
        temp.append("\n");
    }
    return temp;
}

// BreadthFirstPaths: interface for finding the shortest path for given graph
class BreadthFirstPaths
{
  public:
    BreadthFirstPaths(Graph const &g, unsigned int s);

    void bfs(Graph const &g, unsigned int s);

    [[nodiscard]] bool has_path_to(unsigned int v) const
    {
        return _marked[static_cast<adj_list::size_type>(v)];
    }

    [[nodiscard]] vector<unsigned> path_to(unsigned int v) const;

  private:
    std::vector<bool> _marked;
    std::vector<unsigned int> _edge_to;
    unsigned int _s = 0;
};

export auto finding_path_test_bfs(Graph const &g, unsigned int s)
{
    std::string temp;
    const auto search = BreadthFirstPaths(g, s);
    for (unsigned int v = 0; v < 6; v++)
    {
        temp.append(std::to_string(s));
        temp.append(" to ");
        temp.append(std::to_string(v));
        temp.append(": ");
        if (search.has_path_to(v))
        {
            for (auto x : search.path_to(v))
            {
                if (x == s)
                    temp.append(std::to_string(x));
                else
                {
                    temp.append("-");
                    temp.append(std::to_string(x));
                }
            }
        }
        temp.append("\n");
    }
    return temp;
}

// Connected_Components: interface for test if given 2 vertex is connected
export class Connected_Components
{
  public:
    explicit Connected_Components(Graph const &g);

    void dfs(const Graph &g, unsigned int v);

    [[nodiscard]] auto connected(unsigned int v, unsigned int w) const
    {
        return _id[static_cast<adj_list::size_type>(v)] == _id[static_cast<adj_list::size_type>(w)];
    }

    [[nodiscard]] unsigned int id(unsigned int v) const
    {
        return _id[static_cast<adj_list::size_type>(v)];
    }

    [[nodiscard]] unsigned int count() const
    {
        return _count;
    }

  private:
    vector<bool> _marked;
    vector<unsigned int> _id; // store the connected component
    unsigned int _count = 0;
};

// Cycle interface: the purpose is to test the given graph is acyclic or not
export class Cycle
{
  public:
    explicit Cycle(Graph const &g);

    void dfs(Graph const &g, unsigned int v, unsigned int u);

    [[nodiscard]] auto has_cycle() const
    {
        return _has_cycle;
    }

  private:
    vector<bool> _marked;
    bool _has_cycle = false;
};

// TwoColor: interface for test if given graph is bipartite
export class TwoColor
{
  public:
    explicit TwoColor(Graph const &g);

    // run dfs with additional step, coloring and checking the color
    void dfs(Graph const &g, unsigned int v);

    // check if given graph is bipartite
    [[nodiscard]] auto is_bipartite() const
    {
        return _is_two_colorable;
    }

  private:
    vector<bool> _marked;
    vector<bool> _color;
    bool _is_two_colorable = true;
};

// symbol graph implementation
export class SymbolGraph
{
  public:
    explicit SymbolGraph(const string &filename, char sp = ' ');

    // check whether symbol table contains s
    [[nodiscard]] bool contains(const string &s) const
    {
        return _st.find(s) != _st.end();
    }

    // get the index given s from symbol table
    [[nodiscard]] unsigned int index(const string &s) const
    {
        return _st.at(s);
    }

    // get name or string given key v from symbol table
    [[nodiscard]] string name(unsigned int v) const
    {
        return _keys.at(v);
    }

    // get built graph result from symbol table
    [[nodiscard]] const Graph &get_graph() const
    {
        return _graph;
    }

  private:
    map<string, unsigned int> _st{};
    vector<string> _keys{};
    Graph _graph;
};

// directed graph implementation
export class Digraph
{
  public:
    // ctor with v is total vertex
    explicit Digraph(unsigned int v) : _v(v)
    {
        _adj.resize(v);
    }
    // read the digraph from input stream
    explicit Digraph(std::istream &is);
    // return total vertex
    [[nodiscard]] unsigned int vertex_length() const
    {
        return _v;
    }
    // return total edge
    [[nodiscard]] unsigned int edge_length() const
    {
        return _e;
    }
    // reverse the structure corresponding to the book
    void reverse_order();
    [[nodiscard]] vector<unsigned int> adj(unsigned int v) const
    {
        return _adj[v];
    }
    // reverse of this graph
    [[nodiscard]] Digraph reverse() const;
    // add edge given vertex v and w or connect v to w
    void add_edge(unsigned int v, unsigned int w);

  private:
    unsigned int _v = 0; // vertex and edge
    unsigned int _e = 0;
    adj_list _adj; // adjacency list representation
    void bad_input() const
    {
        throw std::runtime_error("Input is not a valid graph, max: " + std::to_string(_v));
    }
};

// reachability in digraph, directed dfs
export class DirectedDFS
{
  public:
    // ctor with digraph and given s as a source
    DirectedDFS(const Digraph &digraph, unsigned int s)
    {
        _marked.resize(digraph.vertex_length());
        dfs(digraph, s);
    }
    // ctor with digraph and given list of sources
    DirectedDFS(const Digraph &digraph, const vector<unsigned int> &s);
    // check if v is marked
    [[nodiscard]] bool marked(unsigned int v) const
    {
        return _marked[v];
    }

  private:
    vector<bool> _marked;
    // run depth first search for processing the graph
    void dfs(const Digraph &digraph, unsigned int v);
};

// DirectedCycle: interface for finding directed cycle on a digraph using dfs
export class DirectedCycle
{
  public:
    explicit DirectedCycle(const Digraph &digraph);
    [[nodiscard]] bool has_cycle() const
    {
        return !_cycle.empty();
    }
    [[nodiscard]] auto cycle() const
    {
        return _cycle;
    }

  private:
    vector<bool> _marked;
    vector<unsigned int> _edge_to;
    std::stack<unsigned int> _cycle; // vertices on a cycle (if one exist)
    vector<bool> _on_stack;          // vertices on recursive call stack
    void dfs(const Digraph &digraph, unsigned int v);
};

// DepthFirstOrder: depth first search vertex ordering in a digraph
export class DepthFirstOrder
{
  public:
    explicit DepthFirstOrder(const Digraph &digraph);
    // return preorder ordering vertex
    [[nodiscard]] auto pre_order() const
    {
        return _pre;
    }
    // ...post order ordering vertex
    [[nodiscard]] auto post_order() const
    {
        return _post;
    }
    // return reverse post ordering vertex
    [[nodiscard]] auto reverse_post_order() const
    {
        return _reverse_post;
    }

  private:
    queue<unsigned int> _pre;               // put the vertex on a queue before the recursive calls
    queue<unsigned int> _post;              // put the vertex on a queue after the r... calls
    std::stack<unsigned int> _reverse_post; // put ... stack after r... calls
    vector<bool> _marked;
    void dfs(const Digraph &digraph, unsigned int v);
};

// Topological sort: topological order for directed acyclic graph (DAG)
export class Topological
{
  public:
    explicit Topological(const Digraph &digraph);
    [[nodiscard]] auto order() const // return topological order
    {
        return _order;
    }
    [[nodiscard]] auto is_dag() const // check if the graph is DAG
    {
        return !_order.empty();
    }

  private:
    std::stack<unsigned int> _order;
};

// Kosaraju's algorithm: computing connected components in digraph
export class KosarajuSCC
{
  public:
    explicit KosarajuSCC(const Digraph &digraph);
    [[nodiscard]] auto strongly_connected(const unsigned int v, const unsigned int w) const
    {
        return _id[v] == _id[w];
    }
    [[nodiscard]] auto id(const unsigned int v) const
    {
        return _id[v];
    }
    [[nodiscard]] auto count() const
    {
        return _count;
    }
    [[nodiscard]] auto identifiers() const
    {
        return _id;
    }

  private:
    vector<bool> _marked;     // reached vertices
    vector<unsigned int> _id; // components identifiers
    unsigned int _count = 0;  // number of strong components
    void dfs(const Digraph &digraph, unsigned int v);
};
Graph::Graph(std::istream &is)
{
    unsigned int v_read = 0;
    is >> v_read;
    _v = v_read;
    initialize(v_read);
    unsigned int e_read;
    is >> e_read;
    _e = e_read;
    for (unsigned int i = 0; i < e_read; i++)
    {
        unsigned int v, w;
        is >> v >> w;   // read a vertex and another vertex
        add_edge(v, w); // connecting them
    }
}
void Graph::add_edge(unsigned int v, unsigned int w)
{
    check_invalid_vertex(v, _v);
    check_invalid_vertex(w, _v);
    _adj[static_cast<adj_list::size_type>(v)].push_back(w); // add w to v's list
    _adj[static_cast<adj_list::size_type>(w)].push_back(v); // add v to w's list
    ++_e;
}
auto Graph::reverse_order()
{
    for (auto i = 0; i < _adj.size(); i++)
        std::reverse(_adj[static_cast<adj_list::size_type>(i)].begin(),
                     _adj[static_cast<adj_list::size_type>(i)].end());
}
unsigned int Graph::degree(Graph const &g, const int v)
{
    unsigned int deg = 0;
    for ([[maybe_unused]] auto i : g.adj(v))
        deg++;
    return deg;
}
void DepthFirstSearch::dfs(Graph const &g, unsigned int v)
{
    _marked[static_cast<adj_list::size_type>(v)] = true;
    _count++;
    for (auto w : g.adj(v))
        if (!_marked[static_cast<adj_list::size_type>(w)])
            dfs(g, w);
}
DepthFirstPaths::DepthFirstPaths(Graph const &g, unsigned int s) : _s{s}
{
    check_input(s, g.vertex_length());
    _marked.resize(g.vertex_length(), false);
    _edge_to.resize(g.vertex_length());
    dfs(g, s);
}
void DepthFirstPaths::dfs(Graph const &g, unsigned int v)
{
    _marked[static_cast<adj_list::size_type>(v)] = true;
    for (auto w : g.adj(v))
    {
        if (!_marked[static_cast<adj_list::size_type>(w)])
        {
            // v-w was the edge used to access w for the first time
            _edge_to[static_cast<adj_list::size_type>(w)] = v;
            dfs(g, w);
        }
    }
}
vector<unsigned> DepthFirstPaths::path_to(unsigned int v) const
{
    std::vector<unsigned int> temp;
    if (!has_path_to(v))
        return temp;
    std::stack<unsigned int> path;
    for (auto x = v; x != _s; x = _edge_to[static_cast<adj_list::size_type>(x)])
        path.push(x);
    path.push(_s);
    while (!path.empty())
    {
        temp.push_back(path.top());
        path.pop();
    }
    return temp;
}
BreadthFirstPaths::BreadthFirstPaths(Graph const &g, unsigned int s) : _s{s}
{
    _marked.resize(g.vertex_length(), false);
    _edge_to.resize(g.vertex_length());
    bfs(g, s);
}
void BreadthFirstPaths::bfs(Graph const &g, unsigned int s)
{
    queue<unsigned int> q;
    _marked[static_cast<adj_list::size_type>(s)] = true;
    q.push(s);
    while (!q.empty())
    {
        auto v = q.front();
        q.pop();
        for (auto w : g.adj(v))
        {
            if (!_marked[static_cast<adj_list::size_type>(w)])
            {
                // save last edge on the shortest path
                _edge_to[static_cast<adj_list::size_type>(w)] = v;
                // mark it because path is known
                _marked[static_cast<adj_list::size_type>(w)] = true;
                q.push(w); // and add it to the queue
            }
        }
    }
}
vector<unsigned> BreadthFirstPaths::path_to(unsigned int v) const
{
    vector<unsigned int> temp;
    if (!has_path_to(v))
        return temp;
    std::stack<unsigned int> path;
    for (auto x = v; x != _s; x = _edge_to[static_cast<adj_list::size_type>(x)])
        path.push(x);
    path.push(_s);
    while (!path.empty())
    {
        temp.push_back(path.top());
        path.pop();
    }
    return temp;
}
Connected_Components::Connected_Components(Graph const &g)
{
    _marked.resize(g.vertex_length(), false);
    _id.resize(g.vertex_length());
    for (unsigned int v = 0; v < g.vertex_length(); v++)
    {
        if (!_marked[static_cast<adj_list::size_type>(v)])
        {
            dfs(g, static_cast<adj_list::size_type>(v));
            _count++;
        }
    }
}
void Connected_Components::dfs(const Graph &g, unsigned int v)
{
    _marked[static_cast<adj_list::size_type>(v)] = true;
    _id[static_cast<adj_list::size_type>(v)] = _count;
    for (auto w : g.adj(v))
    {
        if (!_marked[static_cast<adj_list::size_type>(w)])
            dfs(g, w);
    }
}
Cycle::Cycle(Graph const &g)
{
    _marked.resize(g.vertex_length(), false);
    for (unsigned int v = 0; v < g.vertex_length(); v++)
        if (!_marked[static_cast<adj_list::size_type>(v)])
            dfs(g, v, v);
}
void Cycle::dfs(Graph const &g, unsigned int v, unsigned int u)
{
    _marked[static_cast<adj_list::size_type>(v)] = true;
    for (auto w : g.adj(v))
    {
        if (!_marked[static_cast<adj_list::size_type>(w)])
            dfs(g, w, v);
        else if (w != u) // the adjacent vertex w is not the parent vertex u
            _has_cycle = true;
    }
}
TwoColor::TwoColor(Graph const &g)
{
    _marked.resize(g.vertex_length(), false);
    _color.resize(g.vertex_length());
    for (unsigned int v = 0; v < g.vertex_length(); v++)
        if (!_marked[static_cast<adj_list::size_type>(v)])
            dfs(g, v);
}
void TwoColor::dfs(Graph const &g, unsigned int v)
{
    _marked[static_cast<adj_list::size_type>(v)] = true;
    for (auto w : g.adj(v))
    {
        if (!_marked[static_cast<adj_list::size_type>(w)])
        {
            _color[static_cast<adj_list::size_type>(w)] = !_color[static_cast<adj_list::size_type>(v)];
            dfs(g, w);
        }
        else if (_color[static_cast<adj_list::size_type>(w)] == _color[static_cast<adj_list::size_type>(v)])
        {
            _is_two_colorable = false;
        }
    }
}
SymbolGraph::SymbolGraph(const string &filename, char sp) : _graph(0)
{
    std::fstream file(filename, std::ios::in);
    if (!file.is_open())
        throw std::runtime_error("Could not open file " + filename);
    string line, first_line, second_line;
    vector<string> a = {" ", " "};
    while (std::getline(file, line))
    { // first pass, build the index
        auto pos = line.find_first_of(sp);
        first_line = line.substr(0, pos);
        second_line = line.substr(pos + 1);
        a[0] = first_line;
        a[1] = second_line;
        for (const auto &s : a)
            if (_st.find(s) == _st.end())
                _st[s] = _st.size();
    }
    // inverted index to get string keys
    _keys.resize(_st.size());
    for (const auto &k : _st)
    {
        _keys[k.second] = k.first;
    }
    // second pass, build the graph, by connecting the first vertex
    // on each line to all the others
    file.close();
    file.open(filename, std::ios::in);
    _graph = Graph(_st.size());
    while (std::getline(file, line))
    {
        auto pos = line.find_first_of(sp);
        first_line = line.substr(0, pos);
        second_line = line.substr(pos + 1);
        a[0] = first_line;
        a[1] = second_line;
        unsigned int v = _st[first_line];
        for (unsigned int i = 1; i < a.size(); i++)
            _graph.add_edge(v, _st[a[i]]);
    }
    file.close();
}
Digraph::Digraph(std::istream &is)
{
    unsigned vertex, edge; // total vertex to read and it's edge
    is >> vertex >> edge;
    _adj.resize(vertex);
    _v = vertex;
    unsigned int v, w;
    for (unsigned int i = 0; i < edge; i++)
    {
        is >> v >> w;
        add_edge(v, w);
    }
}
void Digraph::reverse_order()
{
    for (unsigned int i = 0; i < _v; i++)
        std::reverse(_adj[i].begin(), _adj[i].end());
}
Digraph Digraph::reverse() const
{ // example v point to w, now w point to v
    auto dr = Digraph(_v);
    for (unsigned int i = 0; i < _v; i++)
        for (const auto w : _adj[i])
            dr.add_edge(w, i);
    return dr;
}
void Digraph::add_edge(unsigned int v, unsigned int w)
{
    if (v >= _v || w >= _v)
        bad_input();
    _adj[v].push_back(w); // the connecting process is only one time
    ++_e;
}
DirectedDFS::DirectedDFS(const Digraph &digraph, const vector<unsigned int> &s)
{
    _marked.resize(digraph.vertex_length());
    for (auto v : s)
        if (!_marked[v])
            dfs(digraph, v);
}
void DirectedDFS::dfs(const Digraph &digraph, unsigned int v)
{
    _marked[v] = true;
    for (auto w : digraph.adj(v))
        if (!_marked[w])
            dfs(digraph, w); // not marked the adjacency list, call dfs recursively
}
DirectedCycle::DirectedCycle(const Digraph &digraph)
{
    _on_stack.resize(digraph.vertex_length());
    _edge_to.resize(digraph.vertex_length());
    _marked.resize(digraph.vertex_length());
    for (unsigned int i = 0; i < digraph.vertex_length(); i++)
        if (!_marked[i])
            dfs(digraph, i);
}
void DirectedCycle::dfs(const Digraph &digraph, const unsigned int v)
{
    _on_stack[v] = true; // set true on entry to dfs(g,v)
    _marked[v] = true;
    for (auto w : digraph.adj(v))
    {
        if (has_cycle())
            return;
        else if (!_marked[w])
        {
            _edge_to[w] = v;
            dfs(digraph, w);
        }
        else if (_on_stack[w])
        {
            for (unsigned x = v; x != w; x = _edge_to[x])
                _cycle.push(x);
            _cycle.push(w);
            _cycle.push(v); // back to the head, this is cycled
        }
    }
    _on_stack[v] = false; // set false on exit
}
DepthFirstOrder::DepthFirstOrder(const Digraph &digraph)
{
    _marked.resize(digraph.vertex_length());
    for (unsigned int i = 0; i < digraph.vertex_length(); i++)
        if (!_marked[i])
            dfs(digraph, i);
}
void DepthFirstOrder::dfs(const Digraph &digraph, const unsigned int v)
{
    _pre.push(v);
    _marked[v] = true;
    for (const auto w : digraph.adj(v))
        if (!_marked[w])
            dfs(digraph, w);
    _post.push(v);
    _reverse_post.push(v);
}
Topological::Topological(const Digraph &digraph)
{
    if (const auto cycle_finder = DirectedCycle(digraph); !cycle_finder.has_cycle())
    {
        const auto dfs = DepthFirstOrder(digraph);
        _order = dfs.reverse_post_order();
    }
}
KosarajuSCC::KosarajuSCC(const Digraph &digraph)
{
    _marked.resize(digraph.vertex_length());
    _id.resize(digraph.vertex_length());
    auto order = DepthFirstOrder(digraph.reverse()).reverse_post_order();
    vector<unsigned int> temp;
    while (!order.empty())
    {
        temp.push_back(order.top());
        order.pop();
    }
    for (const auto w : temp)
        if (!_marked[w])
        {
            dfs(digraph, w);
            _count++;
        }
}
void KosarajuSCC::dfs(const Digraph &digraph, const unsigned int v)
{
    _marked[v] = true;
    _id[v] = _count;
    for (const auto w : digraph.adj(v))
        if (!_marked[w])
            dfs(digraph, w);
}

// Edge: weighted edge data type implementation
export class Edge
{
  public:
    explicit Edge(const unsigned int v = 0, const unsigned int w = 0, const double weight = 0)
        : _v(v), _w(w), _weight(weight)
    {
    }
    [[nodiscard]] auto weight() const
    {
        return _weight;
    }
    [[nodiscard]] auto either() const
    {
        return _v;
    }
    [[nodiscard]] auto other(unsigned int vertex) const
    {
        if (vertex == _v)
            return _w;
        else if (vertex == _w)
            return _v;
        else
            throw std::invalid_argument("Inconsistent edge");
    }
    friend bool operator<(const Edge &e1, const Edge &e2)
    {
        return e1._weight < e2._weight;
    }

  private:
    unsigned int _v = 0;
    unsigned int _w = 0;
    double _weight = 0;
};

// EdgeWeightedGraph: Edge-Weighted graph data type
export class EdgeWeightedGraph
{
  public:
    explicit EdgeWeightedGraph(const unsigned int v) : _v(v)
    {
        _adj.resize(v);
    }
    [[nodiscard]] auto v() const
    {
        return _v;
    }
    [[nodiscard]] auto e() const
    {
        return _e;
    }
    void add_edge(const Edge &e);
    [[nodiscard]] vector<Edge> adj(unsigned int v) const
    {
        return _adj[v];
    }
    [[nodiscard]] vector<Edge> edges() const;

  private:
    unsigned int _v = 0;       // number of vertices
    unsigned int _e = 0;       // number of edges
    vector<vector<Edge>> _adj; // adjacency list
};

void EdgeWeightedGraph::add_edge(const Edge &e)
{
    auto v = e.either();
    auto w = e.other(v);
    _adj[v].push_back(e);
    _adj[w].push_back(e);
    _e++;
}
// gathering all the edges in an edge weighted graph
vector<Edge> EdgeWeightedGraph::edges() const
{
    vector<Edge> res;
    for (auto i = 0; i < _v; i++)
        for (Edge e : _adj[i])
            if (e.other(i) > i)
                res.push_back(e);
    return res;
}

// BinaryHeap: priority queue implementation, for prim's algorithm
export template <typename Comparable> class BinaryHeap
{
  public:
    explicit BinaryHeap(unsigned int capacity = 100);
    explicit BinaryHeap(const vector<Comparable> &items);
    [[nodiscard]] bool is_empty() const;
    const Comparable &find_min() const;
    void insert(const Comparable &e);
    void delete_min();
    [[nodiscard]] unsigned int size()const {return _curr_size;};

  private:
    unsigned int _curr_size;   // the number of elements in heap
    vector<Comparable> _array; // the heap array
    void build_heap();
    void percolate_down(unsigned int i);
};
template <typename Comparable> BinaryHeap<Comparable>::BinaryHeap(unsigned int capacity) : _curr_size(0)
{
    _array.resize(capacity);
}
template <typename Comparable>
BinaryHeap<Comparable>::BinaryHeap(const vector<Comparable> &items)
    : _curr_size(items.size()), _array(items.size() + 10)
{
    for (unsigned int i = 0; i < items.size(); i++)
        _array[i + 1] = items[i];
    build_heap();
}
template <typename Comparable> bool BinaryHeap<Comparable>::is_empty() const
{
    return _curr_size == 0;
}
template <typename Comparable> const Comparable &BinaryHeap<Comparable>::find_min() const
{
    if (is_empty())
        throw std::invalid_argument("Heap is empty");
    return _array[1];
}
template <typename Comparable> void BinaryHeap<Comparable>::insert(const Comparable &e)
{
    if (_curr_size == _array.size() - 1)
        _array.resize(_array.size() * 2);
    // percolate up
    auto hole = ++_curr_size;
    auto copy = e;
    _array[0] = std::move(copy);
    for (; e < _array[hole / 2]; hole /= 2)
        _array[hole] = std::move(_array[hole / 2]);
    _array[hole] = std::move(_array[0]);
}
template <typename Comparable> void BinaryHeap<Comparable>::delete_min()
{
    if (is_empty())
        throw std::invalid_argument("Heap is empty");
    _array[1] = std::move(_array[_curr_size--]);
    percolate_down(1);
}
template <typename Comparable> void BinaryHeap<Comparable>::build_heap()
{
    for (unsigned int i = _curr_size / 2; i > 0; --i)
        percolate_down(i);
}
template <typename Comparable> void BinaryHeap<Comparable>::percolate_down(unsigned int i)
{
    unsigned int child;
    Comparable tmp = std::move(_array[i]);
    for (; i * 2 <= _curr_size; i = child)
    {
        child = i * 2;
        if (child != _curr_size && _array[child + 1] < _array[child])
            ++child;
        if (_array[child] < tmp)
            _array[i] = std::move(_array[child]);
        else
            break;
    }
    _array[i] = std::move(tmp);
}

// Lazy version of Prim's MST algorithm
export class LazyPrimMST
{
  public:
    typedef BinaryHeap<Edge> Priority;
    explicit LazyPrimMST(const EdgeWeightedGraph &g);
    void visit(const EdgeWeightedGraph &g, unsigned int v);
    [[nodiscard]] auto edges() const
    {
        return _mst;
    }
    [[nodiscard]] double weight() const;
    [[nodiscard]] vector<Edge> edges_to_vector()const;
  private:
    vector<bool> _marked; // MST Vertices
    queue<Edge> _mst;     // MST edges
    Priority _pq;            // crossing (and  ineligible) edges
};
LazyPrimMST::LazyPrimMST(const EdgeWeightedGraph &g)
{
    _marked.resize(g.v());
    visit(g, 0); // assume G is connected
    while (!_pq.is_empty())
    {
        auto e = _pq.find_min(); // get the lowest weight edge from pq
        _pq.delete_min();
        const auto v = e.either();
        const auto w = e.other(v);
        if (_marked[v] && _marked[w])
            continue; // skip if ineligible
        _mst.push(e); // add edge to the tree
        if (!_marked[v])
            visit(g, v); // add vertex to tree
        if (!_marked[w])
            visit(g, w); // either v or w
    }
}
void LazyPrimMST::visit(const EdgeWeightedGraph &g, const unsigned int v)
{
    // mark v and add to pq all edges from v to unmarked vertices
    _marked[v] = true;
    for (Edge e : g.adj(v))
    {
        if (!_marked[e.other(v)])
            _pq.insert(e);
    }
}
double LazyPrimMST::weight() const
{
    const auto vec = edges_to_vector();
    auto sum =0.0;
    for (const auto &e : vec)
        sum+=e.weight();
    return sum;
}
vector<Edge> LazyPrimMST::edges_to_vector() const
{
    auto mst = _mst;
    vector<Edge> res;
    while (!mst.empty())
    {
        res.push_back(mst.front());
        mst.pop();
    }
    return res;
}