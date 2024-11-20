module;
export module algo_graph;
import <iostream>;
import <vector>;
import <istream>;
import <string>;
import <stack>;
import <queue>;
using std::queue;
using std::vector;
typedef std::vector<vector<unsigned int> > adj_list;

// graph interface
export class Graph {
public:
    // explicitly ask total vertex to create
    explicit Graph(const unsigned int v) : _v(v) {
        initialize(v);
    }

    // ctor require input from user
    explicit Graph(std::istream &is) {
        unsigned int v_read = 0;
        is >> v_read;
        _v = v_read;
        initialize(v_read);
        unsigned int e_read;
        is >> e_read;
        for (unsigned int i = 0; i < e_read; i++) {
            unsigned int v, w;
            is >> v >> w; // read a vertex and another vertex
            add_edge(v, w); //connecting them
        }
    }

    // add edge v-w
    void add_edge(unsigned int v, unsigned int w) {
        check_invalid_vertex(v, _v);
        check_invalid_vertex(w, _v);
        _adj[static_cast<adj_list::size_type>(v)].push_back(w); // add w to v's list
        _adj[static_cast<adj_list::size_type>(w)].push_back(v); // add v to w's list
        ++_e;
    }

    // reverse the order, assume the structure is equal to book
    auto reverse_order() {
        for (auto i = 0; i < _adj.size(); i++)
            std::reverse(_adj[static_cast<adj_list::size_type>(i)].begin(),
                         _adj[static_cast<adj_list::size_type>(i)].end());
    }

    // check total vertex
    [[nodiscard]] constexpr auto vertex_length() const {
        return _v;
    }

    //check total edge
    [[nodiscard]] constexpr auto edge_length() const {
        return _e;
    }

    static auto degree(Graph const &g, const int v) {
        unsigned int deg = 0;
        for (auto i: g.adj(v))
            deg++;
        return deg;
    }

    // return array which adjacent to v
    [[nodiscard]] constexpr vector<unsigned int> adj(auto v) const {
        return _adj[v];
    }

    static constexpr auto max_degree(Graph const &g) {
        unsigned int max_deg = 0;
        for (auto v = 0; v < g.vertex_length(); v++)
            if (degree(g, v) > max_deg)
                max_deg = degree(g, v);
        return max_deg;
    }

    static constexpr auto avg_degree(Graph const &g) {
        return 2 * g.edge_length() / g.vertex_length();
    }

    static constexpr auto num_self_loops(Graph const &g) {
        unsigned int num_self_loops = 0;
        for (auto v = 0; v < g.vertex_length(); v++)
            for (auto f: g.adj(v))
                if (f == v)
                    num_self_loops++;
        return num_self_loops;
    }

    // store the result vertex and edges on a string
    [[nodiscard]] constexpr auto _to_string() const {
        auto s = std::to_string(_v);
        s.append(" vertices ");
        s.append(std::to_string(_e));
        s.append(" edges \n");
        for (auto v = 0; v < _v; v++) {
            s.append(std::to_string(v));
            s.append(": ");
            for (auto w: adj(v)) {
                s.append(std::to_string(w));
                s.append(" ");
            }
            s.append("\n");
        }
        return s;
    }

private:
    constexpr void initialize(const unsigned int v) {
        _adj.resize(v);
    }

    adj_list _adj; // adjacency lists
    unsigned int _e = 0; // number of edges
    unsigned int _v = 0; // number of vertices
    static void check_invalid_vertex(const unsigned int v, unsigned int _limit) {
        if (v >= _limit)
            throw std::out_of_range("Vertex index out of range, limit : "
                                    + std::to_string(_limit));
    }
};

//  depth first search interface
export class DepthFirstSearch {
public:
    DepthFirstSearch(Graph const &g, unsigned int s) {
        _marked.resize(g.vertex_length());
        dfs(g, s);
    }

    [[nodiscard]] constexpr auto marked(unsigned int w) const {
        return _marked[static_cast<adj_list::size_type>(w)];
    }

    [[nodiscard]] constexpr auto count() const {
        return _count;
    }

private:
    vector<bool> _marked;
    unsigned int _count = 0;

    void dfs(Graph const &g, unsigned int v) {
        _marked[static_cast<adj_list::size_type>(v)] = true;
        _count++;
        for (auto w: g.adj(v))
            if (!_marked[static_cast<adj_list::size_type>(w)])
                dfs(g, w);
    }
};

// check whether given s as vertex is connected component on g
export void connected_dfs(Graph const &g, unsigned int s) {
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
export class DepthFirstPaths {
public:
    DepthFirstPaths(Graph const &g, unsigned int s) : _s{s} {
        check_input(s, g.vertex_length());
        _marked.resize(g.vertex_length(), false);
        _edge_to.resize(g.vertex_length());
        dfs(g, s);
    }

    void dfs(Graph const &g, unsigned int v) {
        _marked[static_cast<adj_list::size_type>(v)] = true;
        for (auto w: g.adj(v)) {
            if (!_marked[static_cast<adj_list::size_type>(w)]) {
                // v-w was the edge used to access w for the first time
                _edge_to[static_cast<adj_list::size_type>(w)] = v;
                dfs(g, w);
            }
        }
    }

    [[nodiscard]] bool has_path_to(unsigned int v) const {
        return _marked[static_cast<adj_list::size_type>(v)];
    }

    // return the path to v
    [[nodiscard]] auto path_to(unsigned int v) const {
        std::vector<unsigned int> temp;
        if (!has_path_to(v))
            return temp;
        std::stack<unsigned int> path;
        for (auto x = v; x != _s; x = _edge_to[static_cast<adj_list::size_type>(x)])
            path.push(x);
        path.push(_s);
        while (!path.empty()) {
            temp.push_back(path.top());
            path.pop();
        }
        return temp;
    }

private:
    vector<bool> _marked; // has dfs been called for this vertex ?
    vector<unsigned int> _edge_to; // last vertex on known path to this vertex
    unsigned int _s = 0; // source
    static void check_input(unsigned int _source, unsigned int _expected) {
        if (_source >= _expected)
            throw std::out_of_range("Source is not a valid input");
    }
};

export auto finding_path_test(Graph const &g, unsigned int s) {
    std::string temp;
    const auto search = DepthFirstPaths(g, s);
    for (unsigned int v = 0; v < 6; v++) {
        temp.append(std::to_string(s));
        temp.append(" to ");
        temp.append(std::to_string(v));
        temp.append(": ");
        if (search.has_path_to(v)) {
            for (auto x: search.path_to(v)) {
                if (x == s) temp.append(std::to_string(x));
                else {
                    temp.append("-");
                    temp.append(std::to_string(x));
                }
            }
        }
        temp.append("\n");
    }
    return temp;
}

class BreadthFirstPaths {
public:
    BreadthFirstPaths(Graph const &g, unsigned int s): _s{s} {
        _marked.resize(g.vertex_length(), false);
        _edge_to.resize(g.vertex_length());
        bfs(g, s);
    }

    void bfs(Graph const &g, unsigned int s) {
        queue<unsigned int> q;
        _marked[static_cast<adj_list::size_type>(s)] = true;
        q.push(s);
        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            for (auto w: g.adj(v)) {
                if (!_marked[static_cast<adj_list::size_type>(w)]) {
                    // save last edge on a shortest path
                    _edge_to[static_cast<adj_list::size_type>(w)] = v;
                    // matk it because path is known
                    _marked[static_cast<adj_list::size_type>(w)] = true;
                    q.push(w); // and add it to the queue
                }
            }
        }
    }

    [[nodiscard]] bool has_path_to(unsigned int v) const {
        return _marked[static_cast<adj_list::size_type>(v)];
    }

    [[nodiscard]] auto path_to(unsigned int v) const {
        vector<unsigned int> temp;
        if (!has_path_to(v))
            return temp;
        std::stack<unsigned int> path;
        for (auto x = v; x != _s; x = _edge_to[static_cast<adj_list::size_type>(x)])
            path.push(x);
        path.push(_s);
        while (!path.empty()) {
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

export auto finding_path_test_bfs(Graph const &g, unsigned int s) {
    std::string temp;
    const auto search = BreadthFirstPaths(g, s);
    for (unsigned int v = 0; v < 6; v++) {
        temp.append(std::to_string(s));
        temp.append(" to ");
        temp.append(std::to_string(v));
        temp.append(": ");
        if (search.has_path_to(v)) {
            for (auto x: search.path_to(v)) {
                if (x == s) temp.append(std::to_string(x));
                else {
                    temp.append("-");
                    temp.append(std::to_string(x));
                }
            }
        }
        temp.append("\n");
    }
    return temp;
}

export class Connected_Components {
public:
    explicit Connected_Components(Graph const &g) {
        _marked.resize(g.vertex_length(), false);
        _id.resize(g.vertex_length());
        for (unsigned int v = 0; v < g.vertex_length(); v++) {
            if(!_marked[static_cast<adj_list::size_type>(v)]) {
                dfs(g, static_cast<adj_list::size_type>(v));
                _count++;
            }
        }
    }
    void dfs(const Graph &g, unsigned int v) {
        _marked[static_cast<adj_list::size_type>(v)] = true;
        _id[static_cast<adj_list::size_type>(v)] = _count;
        for (auto w: g.adj(v)) {
            if (!_marked[static_cast<adj_list::size_type>(w)])
                dfs(g, w);
        }
    }
    auto connected(unsigned int v, unsigned int w) {
        return _id[static_cast<adj_list::size_type>(v)]
            ==_id[static_cast<adj_list::size_type>(w)];
    }
    [[nodiscard]] unsigned int id(unsigned int v) const {
        return _id[static_cast<adj_list::size_type>(v)];
    }
    [[nodiscard]] unsigned int count() const {
        return _count;
    }
private:
    vector<bool> _marked;
    vector<unsigned int> _id; // store the connected component
    unsigned int _count=0;
};