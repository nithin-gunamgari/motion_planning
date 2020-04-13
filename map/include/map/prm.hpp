#ifndef PRM_INCLUDE_GUARD_HPP
#define PRM_INCLUDE_GUARD_HPP
/// \file
/// \brief PRM Library to build a Probabilistic Roadmap.
#include <map/map.hpp>
#include <unordered_set>
#include <unordered_map>

namespace map
{
    using rigid2d::Vector2D;

    struct Edge
    {
        // ID of next node connected by edge
        // init to -1 for error checking
        int next_id = -1;
        // Euclidean Distance between Nodes (Vertices)
        int distance;
    };

    struct Vertex
    {
        // \brief Constructor with Coordinates
        Vertex(const Vector2D coords_);

        // ID of current Node/Vertex
        // init to -1 for error checking
        int id = -1;
        // Cartesian Coordinates of Vertex
        Vector2D coords;
        // Edges connected to this node
        std::vector<Edge> edges;
        // IDs of Adjacent Nodes
        // O(1) best case or O(n) worst case
        std::unordered_set<int> id_set;

        // Helps with search
        bool visited;

        /// \brief Check if edge exists in ID Hash Table
        /// \param check_id: ID of node to connect to
        /// \returns True if current Vertex connected to given ID.
        bool edge_exists(const int & check_id) const;
    };

    /// \brief stores Obstacle(s) to construct basic PRM. Inherits from Map in map.hpp.
    class PRM : public Map
    {
        // Inherits Constructors
        using Map::Map;

    public:

        // \brief Constructs a Roadmap.
        // \param n: number of nodes to put in the Roadmap.
        // \param k: number of closest neighbours to examine for each configuration.
        // \param thresh: Euclidean Distance Threshold for valid Edge.
        void build_map(const int & n, const int & k, const double & thresh);

        // \brief Sample free space Q for configurations q. Steps 3-8 of algorithm.
        // \param n: number of nodes to put in the Roadmap.
        void sample_configurations(const int & n);

        // \brief For a given configuration q, assigns indeces of K Nearest Neighbours to q id_set. Step 10 of algorithm.
        // \param q: the Vertex being examined. Not const because id_set is modified.
        // \param k: number of closest neighbours to examine for each configuration.
        // NOTE: Using Brute Force Now, replace with KD-Tree + Rebalance
        void find_knn(Vertex & q, const int & k);

        // \brief Check is the Edge between two nodes is valid (no collision, and above some euclidean distance)
        // \param q: the main Vertex being examined
        // \param q: the second Vertex being examined
        // \param thresh: Euclidean Distance Threshold for valid Edge.
        bool edge_valid(const Vertex & q, const Vertex & q_prime, const double & thresh);

        // \brief Checks whether a potential Vertex lies on an Obstacle. 'map::PRM::sample_configurations' calls this function.
        // \param q: the Vertex being examined
        bool no_collision(const Vertex & q);

        // \brief Checks whether a potential Edge intersects an Obstacle. 'map::PRM::edge_valid' calls this function.
        // \param q: the main Vertex being examined
        // \param q: the second Vertex being examined
        // \param inflate_robot: approximate robot radius used for collision checking.
        bool no_collision(const Vertex & q, const Vertex & q_prime, const double & inflate_robot);

        // \brief Return Probabilistic Road Map
        // \returns Probabilistic Road Map 
        std::unordered_map<int, Vertex> return_prm();
    private:
        // Hash Table
        std::unordered_map<int, Vertex>  configurations;
    };
}

#endif
