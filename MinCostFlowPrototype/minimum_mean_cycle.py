from collections import namedtuple

debug = True

edge = namedtuple('edge',['src','dst','cost'])

class minimum_mean_cycle_states:
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
        # This represents the currently found best cycle
        self.best_cycle = None
        # This represents the mean of the currently found best cycle
        self.best_cycle_mean = None

def minimum_mean_cycle(number_of_nodes, adjacency_list, edges):
    states = minimum_mean_cycle_states(number_of_nodes, edges)
    for node in range(0, number_of_nodes):
        if states.start_time[node] is None:
            minimum_mean_cycle_helper(node, adjacency_list, states)
    return states.best_cycle

def minimum_mean_cycle_helper(node, adjacency_list, states):
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
            minimum_mean_cycle_helper(edge.dst, adjacency_list, states)
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
        connected_component_edge_indexes = []
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
                connected_component_edge_indexes.append(edge_index)
        for connected_component_node in connected_component_nodes:
            states.instack[connected_component_node] = 3
        if debug:
            print("A connected component is found")
            print("connected component nodes: %s" % connected_component_nodes)
            print("connected component edges: %s" % [states.edges[connected_component_edge_index] for connected_component_edge_index in connected_component_edge_indexes])
        minimum_mean_cycle_within_connected_component(connected_component_nodes, connected_component_edge_indexes, states)

def minimum_mean_cycle_within_connected_component(connected_component_nodes, connected_component_edge_indexes, states):
    number_of_nodes = len(connected_component_nodes)
    mapping = [0] * number_of_nodes
    for i in range(0, number_of_nodes):
        mapping[connected_component_nodes[i]] = i

    # costs[v][k] represents the minimal cost to reach node v from 0 through exactly k edges
    # None represents node v is unreachable from node s
    costs = [[None for i in range(0, number_of_nodes + 1)] for j in range(0, number_of_nodes)]

    # parents[v][k] represents the edge index we chose to reach the parent
    parents = [[None for i in range(0, number_of_nodes + 1)] for j in range(0, number_of_nodes)]

    # In 0 step, the only reachable node from 0 is 0, and its cost is 0
    costs[0][0] = 0

    # The best cost to reach node v through exactly k edge can be found by relaxing all the edges
    for step in range(1, number_of_nodes + 1):
        for edge_index in connected_component_edge_indexes:
            edge = states.edges[edge_index]
            src = mapping[edge.src]
            dst = mapping[edge.dst]
            if costs[src][step - 1] is not None:
                new_cost = costs[src][step - 1] + edge.cost
                if costs[dst][step] == None or new_cost < costs[dst][step]:
                    costs[dst][step] = new_cost
                    parents[dst][step] = edge_index

    # Karp's formula, computing the max ratios
    ratios = [None] * number_of_nodes
    for node in range(0, number_of_nodes):
        for step in range(0, number_of_nodes):
            if costs[node][number_of_nodes] is not None and costs[node][step] is not None:
                new_ratio = (costs[node][number_of_nodes] - costs[node][step])/(number_of_nodes - step)
                if ratios[node] is None or new_ratio > ratios[node]:
                    ratios[node] = new_ratio

    # Minimizing the max ratios over all nodes
    best_ratio = None
    best_node = None
    for node in range(0, number_of_nodes):
        if ratios[node] is not None and (best_ratio is None or ratios[node] < best_ratio):
            best_ratio = ratios[node]
            best_node = node

    if debug:
        print("Cost table, each row is a step")
        for step in range(0, number_of_nodes + 1):
            for node in range(0, number_of_nodes):
                print(costs[node][step], end="\t")
            print()
        print()
        print("Parent table, each row is a step")
        for step in range(0, number_of_nodes + 1):
            for node in range(0, number_of_nodes):
                if parents[node][step] is None:
                    print(None, end="\t")
                else:
                    print(mapping[states.edges[parents[node][step]].src], end="\t")
            print()
        print()
        print("Ratio table")
        for i in range(0, number_of_nodes):
            print(ratios[i], end="\t")
        print()
        print()
        print("best_ratio = %s" % best_ratio)
        if best_node is not None:
            print("best_node = %s" % connected_component_nodes[best_node])

    if best_node is not None:
        # There must be a cycle on a path of n nodes - find it
        steps = [None] * number_of_nodes
        node_cursor = best_node
        step_cursor = number_of_nodes
        cycle = []
        while True:
            if steps[node_cursor] is None:
                # On each newly discovered node, mark it with the current step number
                steps[node_cursor] = step_cursor
                # And walk through the parent chain
                node_cursor = mapping[states.edges[parents[node_cursor][step_cursor]].src]
                step_cursor = step_cursor - 1
            else:
                # We have found the cycle, recover it by walking the cycle again starting
                # from the original step number
                stop_cursor = step_cursor
                step_cursor = steps[node_cursor]
                while step_cursor > stop_cursor:
                    cycle.append(parents[node_cursor][step_cursor])
                    node_cursor = mapping[states.edges[parents[node_cursor][step_cursor]].src]
                    step_cursor = step_cursor - 1
                break
        # The discovery order was in parent chain, so reverse it.
        cycle.reverse()
        if debug:
            cycle_nodes = []
            for edge_index in cycle:
                cycle_nodes.append(states.edges[edge_index].dst)
            print()
            print("The best cycle found within the strongly connected component is %s" % cycle_nodes)
            print()
        if states.best_cycle_mean is None or states.best_cycle_mean > best_ratio:
            states.best_cycle_mean = best_ratio
            states.best_cycle = cycle

def main():
    edges = [
        edge(0,1,1),
        edge(1,3,2),
        edge(3,7,1),
        edge(7,4,2),
        edge(3,4,1),
        edge(2,0,-1),
        edge(1,2,3),
        edge(6,1,1),
        edge(5,6,2),
        edge(4,5,-1),
    ]
    adjacency_list = [[] for _ in range(0, 8)]
    for i in range(0, len(edges)):
        adjacency_list[edges[i].src].append(i)
    print(minimum_mean_cycle(8, adjacency_list, edges))
    return 0

if __name__ == "__main__":
    main()