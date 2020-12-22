debug = True

class edge:
    def __init__(self, src, dst, cost):
        self.src = src
        self.dst = dst
        self.cost = cost

    def __repr__(self):
        return "%s --%s--> %s" % (self.src, self.cost, self.dst)

# The edge list is assumed to be representing a strongly connected graph
def minimum_mean_cycle_within_connected_component(number_of_nodes, edges, edge_indexes):
    # costs[v][k] represents the minimal cost to reach node v from 0 through exactly k edges
    # None represents node v is unreachable from node s
    costs = [[None for i in range(0, number_of_nodes + 1)] for j in range(0, number_of_nodes)]

    # parents[v][k] represents the edge index we chose to reach the parent
    parents = [[None for i in range(0, number_of_nodes + 1)] for j in range(0, number_of_nodes)]

    # In 0 step, the only reachable node from 0 is 0, and its cost is 0
    costs[0][0] = 0

    # The best cost to reach node v through exactly k edge can be found by relaxing all the edges
    for step in range(1, number_of_nodes + 1):
        for edge_index in edge_indexes:
            edge = edges[edge_index]
            src = edge.src
            dst = edge.dst
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
                    print(edges[parents[node][step]].src, end="\t")
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
                node_cursor = edges[parents[node_cursor][step_cursor]].src
                step_cursor = step_cursor - 1
            else:
                # We have found the cycle, recover it by walking the cycle again starting
                # from the original step number
                stop_cursor = step_cursor
                step_cursor = steps[node_cursor]
                while step_cursor > stop_cursor:
                    cycle.append(parents[node_cursor][step_cursor])
                    node_cursor = edges[parents[node_cursor][step_cursor]].src
                    step_cursor = step_cursor - 1
                break
        # The discovery order was in parent chain, so reverse it.
        cycle.reverse()
        return cycle

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
    nodes = []
    for edge_index in minimum_mean_cycle_within_connected_component(8, edges, list(range(0, len(edges)))):
        nodes.append(edges[edge_index].dst)
    print(nodes)
    return 0

if __name__ == "__main__":
    main()