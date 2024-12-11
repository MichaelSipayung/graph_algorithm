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
    explicit Graph(std::istream &is)
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

    // add edge v-w
    void add_edge(unsigned int v, unsigned int w)
    {
        check_invalid_vertex(v, _v);
        check_invalid_vertex(w, _v);
        _adj[static_cast<adj_list::size_type>(v)].push_back(w); // add w to v's list
        _adj[static_cast<adj_list::size_type>(w)].push_back(v); // add v to w's list
        ++_e;
    }

    // reverse the order, assume the structure is equal to book
    auto reverse_order()
    {
        for (auto i = 0; i < _adj.size(); i++)
            std::reverse(_adj[static_cast<adj_list::size_type>(i)].begin(),
                         _adj[static_cast<adj_list::size_type>(i)].end());
    }

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

    static auto degree(Graph const &g, const int v)
    {
        unsigned int deg = 0;
        for ([[maybe_unused]] auto i : g.adj(v))
            deg++;
        return deg;
    }

    // return array which adjacent to v
    [[nodiscard]] constexpr vector<unsigned int> adj(auto v) const
    {
        return _adj[v];
    }

    static constexpr auto max_degree(Graph const &g)
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

    void dfs(Graph const &g, unsigned int v)
    {
        _marked[static_cast<adj_list::size_type>(v)] = true;
        _count++;
        for (auto w : g.adj(v))
            if (!_marked[static_cast<adj_list::size_type>(w)])
                dfs(g, w);
    }
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
    DepthFirstPaths(Graph const &g, unsigned int s) : _s{s}
    {
        check_input(s, g.vertex_length());
        _marked.resize(g.vertex_length(), false);
        _edge_to.resize(g.vertex_length());
        dfs(g, s);
    }

    void dfs(Graph const &g, unsigned int v)
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

    [[nodiscard]] bool has_path_to(unsigned int v) const
    {
        return _marked[static_cast<adj_list::size_type>(v)];
    }

    // return the path to v
    [[nodiscard]] auto path_to(unsigned int v) const
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
    BreadthFirstPaths(Graph const &g, unsigned int s) : _s{s}
    {
        _marked.resize(g.vertex_length(), false);
        _edge_to.resize(g.vertex_length());
        bfs(g, s);
    }

    void bfs(Graph const &g, unsigned int s)
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

    [[nodiscard]] bool has_path_to(unsigned int v) const
    {
        return _marked[static_cast<adj_list::size_type>(v)];
    }

    [[nodiscard]] auto path_to(unsigned int v) const
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
    explicit Connected_Components(Graph const &g)
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

    void dfs(const Graph &g, unsigned int v)
    {
        _marked[static_cast<adj_list::size_type>(v)] = true;
        _id[static_cast<adj_list::size_type>(v)] = _count;
        for (auto w : g.adj(v))
        {
            if (!_marked[static_cast<adj_list::size_type>(w)])
                dfs(g, w);
        }
    }

    auto connected(unsigned int v, unsigned int w) const
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
    explicit Cycle(Graph const &g)
    {
        _marked.resize(g.vertex_length(), false);
        for (unsigned int v = 0; v < g.vertex_length(); v++)
            if (!_marked[static_cast<adj_list::size_type>(v)])
                dfs(g, v, v);
    }

    void dfs(Graph const &g, unsigned int v, unsigned int u)
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
    explicit TwoColor(Graph const &g)
    {
        _marked.resize(g.vertex_length(), false);
        _color.resize(g.vertex_length());
        for (unsigned int v = 0; v < g.vertex_length(); v++)
            if (!_marked[static_cast<adj_list::size_type>(v)])
                dfs(g, v);
    }

    // run dfs with additional step, coloring and checking the color
    void dfs(Graph const &g, unsigned int v)
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
    explicit SymbolGraph(const string &filename, char sp = ' ') : _graph(0)
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
    explicit Digraph(std::istream &is)
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
    void reverse_order()
    {
        for (unsigned int i = 0; i < _v; i++)
            std::reverse(_adj[i].begin(), _adj[i].end());
    }
    [[nodiscard]] vector<unsigned int> adj(unsigned int v) const
    {
        return _adj[v];
    }
    // reverse of this graph
    [[nodiscard]] Digraph reverse() const
    { // example v point to w, now w point to v
        auto dr = Digraph(_v);
        for (unsigned int i = 0; i < _v; i++)
            for (auto w : _adj[i])
                dr.add_edge(w, i);
        return dr;
    }
    // add edge given vertex v and w or connect v to w
    void add_edge(unsigned int v, unsigned int w)
    {
        if (v >= _v || w >= _v)
            bad_input();
        _adj[v].push_back(w); // the connecting process is only one time
        ++_e;
    }

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
    DirectedDFS(const Digraph &digraph, const vector<unsigned int> &s)
    {
        _marked.resize(digraph.vertex_length());
        for (auto v : s)
            if (!_marked[v])
                dfs(digraph, v);
    }
    // check if v is marked
    [[nodiscard]] bool marked(unsigned int v) const
    {
        return _marked[v];
    }

  private:
    vector<bool> _marked;
    // run depth first search for processing the graph
    void dfs(const Digraph &digraph, unsigned int v)
    {
        _marked[v] = true;
        for (auto w : digraph.adj(v))
            if (!_marked[w])
                dfs(digraph, w); // not marked the adjacency list, call dfs recursively
    }
};

// DirectedCycle: interface for finding directed cycle on a digraph using dfs
export class DirectedCycle
{
  public:
    explicit DirectedCycle(const Digraph &digraph)
    {
        _on_stack.resize(digraph.vertex_length());
        _edge_to.resize(digraph.vertex_length());
        _marked.resize(digraph.vertex_length());
        for (unsigned int i = 0; i < digraph.vertex_length(); i++)
            if (!_marked[i])
                dfs(digraph, i);
    }
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
    void dfs(const Digraph &digraph, const unsigned int v)
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
};

// DepthFirstOrder: depth first search vertex ordering in a digraph
export class DepthFirstOrder
{
  public:
    explicit DepthFirstOrder(const Digraph &digraph)
    {
        _marked.resize(digraph.vertex_length());
        for (unsigned int i = 0; i < digraph.vertex_length(); i++)
            if (!_marked[i])
                dfs(digraph, i);
    }
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
    [[nodiscard]] auto revers_post_order() const
    {
        return _reverse_post;
    }

  private:
    queue<unsigned int> _pre;               // put the vertex on a queue before the recursive calls
    queue<unsigned int> _post;              // put the vertex on a queue after the r... calls
    std::stack<unsigned int> _reverse_post; // put ... stack after r... calls
    vector<bool> _marked;
    void dfs(const Digraph &digraph, const unsigned int v)
    {
        _pre.push(v);
        _marked[v] = true;
        for (const auto w : digraph.adj(v))
            if (!_marked[w])
                dfs(digraph, w);
        _post.push(v);
        _reverse_post.push(v);
    }
};