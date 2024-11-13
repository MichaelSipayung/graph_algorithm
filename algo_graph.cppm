module;
export module algo_graph;
import <iostream>;
import <vector>;
import <istream>;
import <string>;
using std::vector;
typedef std::vector<vector<unsigned int> > adj_list;

export class Graph {
public:
    explicit Graph(const unsigned int v) : _v(v) {
        initialize(v);
    }

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

    void add_edge(unsigned int v, unsigned int w) {
        if (v >= _v || w >= _v)
            throw std::out_of_range("Vertex index out of range");
        _adj[static_cast<adj_list::size_type>(v)].push_back(w); // add w to v's list
        _adj[static_cast<adj_list::size_type>(w)].push_back(v); // add v to w's list
        ++_e;
    }

    [[nodiscard]] constexpr auto vertex_length() const {
        return _v;
    }

    [[nodiscard]] auto edge_length() const {
        return _e;
    }

    static auto degree(Graph const &g, const int v) {
        unsigned int deg = 0;
        for (auto i: g.adj(v))
            deg++;
        return deg;
    }

    [[nodiscard]] constexpr vector<unsigned int> adj(auto v) const {
        return _adj[v];
    }

    static auto max_degree(Graph const &g) {
        unsigned int max_deg = 0;
        for (auto v = 0; v < g.vertex_length(); v++)
            if (degree(g, v) > max_deg)
                max_deg = degree(g, v);
        return max_deg;
    }

    static constexpr auto avg_degree(Graph const &g) {
        return 2 * g.edge_length() / g.vertex_length();
    }

    static auto num_self_loops(Graph const &g) {
        unsigned int num_self_loops = 0;
        for (auto v = 0; v < g.vertex_length(); v++)
            for (auto f: g.adj(v))
                if (f == v)
                    num_self_loops++;
        return num_self_loops;
    }

    [[nodiscard]] auto _to_string() const {
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
};
