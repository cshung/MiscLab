from collections import namedtuple

debug = True

edge = namedtuple('edge',['src','dst'])

class strongly_connected_components_state:
    def __init__(self, number_of_nodes, edges):
        self.time = 0
        # This represents the time when the node is reached
        self.start_time = [None] * number_of_nodes
        # This represents whether a node is in the stack
        # 0 implies it is never pushed on the stack
        # 1 implies it is currently on the stack
        # 2 implies it is currently being popped from the stack but we have not reported the strongly connected component yet
        # 3 implies it left the stack
        self.instack = [0] * number_of_nodes
        # This represents, among all the nodes that are not currently declared as a strongly connected component
        # The one with the smallest start time that can be reached through its subtree only.
        self.low = [None] * number_of_nodes
        self.node_stack = []
        self.edge_stack = []
        self.edges = edges

def strongly_connected_components(number_of_nodes, adjacency_list, edges):
    states = strongly_connected_components_state(number_of_nodes, edges)
    for node in range(0, number_of_nodes):
        if states.start_time[node] is None:
            strongly_connected_components_helper(node, adjacency_list, states)

def strongly_connected_components_helper(node, adjacency_list, states):
    states.start_time[node] = states.time
    states.time = states.time + 1
    states.low[node] = states.start_time[node]
    states.node_stack.append(node)
    states.instack[node] = 1
    for edge_index in adjacency_list[node]:
        edge = states.edges[edge_index]
        if states.start_time[edge.dst] is None:
            # All the tree edges enters the edge stack
            states.edge_stack.append(edge_index)
            strongly_connected_components_helper(edge.dst, adjacency_list, states)
            if states.low[edge.src] > states.low[edge.dst]:
                states.low[edge.src] = states.low[edge.dst]
        elif states.instack[edge.dst] == 1:
            # So are all the edges that may end up in the strongly connected component
            # We might encounter a back edge in this branch, and back edges always end up in a
            # strongly connected component.
            # But we might also encounter a cross edge or a forward edge in this branch, in that case
            # the edge might cross two strongly connected components, and there is no good way
            # to tell at this point.
            states.edge_stack.append(edge_index)
            if states.low[edge.src] > states.start_time[edge.dst]:
                states.low[edge.src] = states.start_time[edge.dst]
        else:
            # Otherwise the edge is known to point to some other strongly connected component
            # This is an edge we can safely skip from entering the edge stack
            if not states.instack[edge.dst] == 3:
                raise ValueError()
    if states.start_time[node] == states.low[node]:
        connected_component_nodes = []
        connected_component_edges = []
        while True:
            stack_node = states.node_stack.pop()
            # The node is marked as "being popped", not quite done with popping
            states.instack[stack_node] = 2
            connected_component_nodes.append(stack_node)
            if node == stack_node:
                break
        # The popping should stop after we have emptied the stack (in case the node is the root of the depth first search)
        while len(states.edge_stack) > 0:
            # I claim that all edges within the strongly connected component must be currently at the
            # top of the stack
            edge_index = states.edge_stack.pop()
            edge = states.edges[edge_index]
            # If we find an edge that is going downwards to node, it has to be a tree edge.
            # # This is because it is the first time when the node returns, it is impossible to be a forward edge.
            if edge.dst == node and states.start_time[edge.src] < states.start_time[edge.dst]:
                # Once we have found the tree edge to node, we can stop.
                break
            # I claim that states.instack[edge.src] == 2
            if not states.instack[edge.src] == 2:
                raise ValueError()
            # The edge could be forward edge or cross edge to other strongly connected component
            # Therefore we check if the destination of the edge is within the current strongly connected component
            if states.instack[edge.dst] == 2:
                connected_component_edges.append(edge)
        for connected_component_node in connected_component_nodes:
            states.instack[connected_component_node] = 3
        if debug:
            print("A connected component is found")
            print("connected component nodes: %s" % connected_component_nodes)
            print("connected component edges: %s" % connected_component_edges)

def main():
    edges = [
        edge(1, 0),
        edge(2, 1),
        edge(3, 2),
        edge(2, 3),
        edge(0, 4),
        edge(4, 1),
        edge(5, 4),
        edge(6, 5),
        edge(5, 6),
        edge(6, 2),
        edge(7, 6),
        edge(7, 7),
    ]
    adjacency_list = [[] for _ in range(0, 8)]
    for i in range(0, len(edges)):
        adjacency_list[edges[i].src].append(i)
    strongly_connected_components(8, adjacency_list, edges)
    return 0

if __name__ == "__main__":
    main()