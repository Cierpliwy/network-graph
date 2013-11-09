#ifndef GRAPH_H
#define GRAPH_H


/**
 * @brief State of a node. Helpful for marking state in path based algorithms.
 */
enum class NodeState
{
    OPEN,
    CLOSED,
    VISITED
};


/**
 * @brief General node structure.
 */
struct Node
{
    Node(unsigned int ID) : id(ID), state(NodeState::OPEN) {}
    Node(unsigned int ID, NodeState nodeState) : id(ID), state(nodeState) {}
    unsigned int id; //!< Node ID Label
    NodeState state; 
};

/**
 * @brief General edge structure.
 */
struct Edge
{
    Edge(float faultProbability) : faultProb(faultProbability) {}
    float faultProb;
};

/**
 * @brief Graph interface.
 */
class Graph
{
public:

    /**
     * @brief Add node to the graph.
     *
     * @param node Node parameters.
     *
     * @return Pointer to newly created node or nullptr if an error occurred.
     */
    virtual Node* addNode(const Node &node) = 0;

    /**
     * @brief Remove node form the graph.
     *
     * @param node Pointer to node to be removed. Also removes all edges
     *             connected with this node.
     *
     * @return True if successful.
     */
    virtual bool removeNode(Node *node) = 0;

    /**
     * @brief Return first node of the graph. It doesn't have to be firstly
     *        created Node in the graph.
     *
     * @return Pointer to node or nullptr if graph doesn't have nodes.
     */
    virtual Node* getFirstNode() = 0;

    /**
     * @brief Return next available node. You can't be certain about order
     *        of returned nodes. It should be used for iterating through all
     *        available nodes in a graph. Nodes cannot be returned twice or
     *        more times.
     *
     * @param node Neighbour node pointer.
     *
     * @return Next pointer to node or nullptr if not available.
     */
    virtual Node* getNextNode(Node *node) = 0;

    /**
     * @brief Return first adjacent node. It doesn't have to be firstly 
     *        created adjacent node.
     *
     * @param node Neighbour node pointer.
     *
     * @return First adjacent node pointer or nullptr if not available.
     */
    virtual Node* getFirstAdjNode(Node *node) = 0;

    /**
     * @brief Return next available adjacent node.
     *
     * @param node Neighbour node pointer.
     *
     * @return Next adjacent node pointer or nullptr if not available.
     */
    virtual Node* getNextAdjNode(Node *node) = 0;

    /**
     * @brief Add undirectional node to the graph.
     *
     * @param source Pointer to source node.
     * @param target Pointer to target node.
     * @param edge Edge parameters.
     *
     * @return Pointer to newly created edge or nullptr if an error occurred.
     */
    virtual Edge* addEdge(Node *source, Node *target, const Edge &edge) = 0;

    /**
     * @brief Remove edge from a graph.
     *
     * @param edge Pointer to edge to be removed.
     *
     * @return True if operation was successful.
     */
    virtual bool removeEdge(Edge *edge) = 0;

    /**
     * @brief Get edge connected with source and target nodes.
     *
     * @param source Source node pointer.
     * @param target Target node pointer.
     *
     * @return Pointer to the edge or nullptr if it doesn't exist.
     */
    virtual Edge* getEdge(Node *source, Node *target) = 0;

    /**
     * @brief Get edge's source node.
     *
     * @param edge Graph's edge pointer.
     *
     * @return Pointer to source node.
     */
    virtual Node* getEdgeSource(Edge *edge) = 0;

    /**
     * @brief Get edge's target node.
     *
     * @param edge Graph's edge pointer.
     *
     * @return Pointer to target node.
     */
    virtual Node* getEdgeTarget(Edge *edge) = 0;
    
    /**
     * @brief Get first graph's edge. It doesn't have to be firstly created
     *        edge in graph.
     *
     * @return Pointer to graph's edge or nullptr if graph doesn't have edges.
     */
    virtual Edge* getFirstEdge() = 0;

    /**
     * @brief Get next available edge in a graph.
     *
     * @param edge Neighbour edge pointer.
     *
     * @return Pointer to next edge or nullptr if not available.
     */
    virtual Edge* getNextEdge(Edge *edge) = 0;

    /**
     * @brief Removes all nodes and edges.
     */
    virtual void reset() = 0;

     /**
     * @brief Clone graph and place it in referenced object.
     *
     * @param graph Graph to be modified.
     *
     * @return True if operation was succesful.
     */
     virtual bool clone(Graph& graph) = 0;
};

#endif //GRAPH_H
