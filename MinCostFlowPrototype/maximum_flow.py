from collections import deque

debug = True

class Edge:
    def __init__(self, src, dst, capacity):
        self.src = src
        self.dst = dst
        self.capacity = capacity

    def __repr__(self):
        return "%s -(%s)-> %s" % (self.src, self.capacity, self.dst)

def maximum_flow(number_of_nodes, source, target, given_edges):
    # The first element in the edge list is none, this is useful only for the BFS search 
    # The odd indexed elements are forward edges, they are the given ones
    # The even indexed elements are the residual edges, they have 0 capacity to begin with    
    edges = [None]
    # A node is mapped to a list of edges connected to it
    adjacency_list = [[] for _ in range(0, number_of_nodes)]
    for given_edge in given_edges:
        src = given_edge.src
        dst = given_edge.dst
        capacity = given_edge.capacity
        forward_edge = Edge(src, dst, capacity)
        residual_edge = Edge(dst, src, 0)
        adjacency_list[src].append(len(edges))
        edges.append(forward_edge)
        adjacency_list[dst].append(len(edges))
        edges.append(residual_edge)
    flow = 0
    while True:
        # The BFS queue, it contains the edge indexes, that allows us to 
        # get access to the edges quickly
        bfs = deque()

        parents = [-1] * number_of_nodes
        bfs.append(0)
        parents[source] = 0
        target_found = False
        while not target_found and len(bfs) > 0:
            visiting_edge_index = bfs.popleft()
            visiting_edge = edges[visiting_edge_index]
            if visiting_edge is None:
                visiting_target = source
            else:
                visiting_target = visiting_edge.dst
            for neighboring_edge_index in adjacency_list[visiting_target]:
                if edges[neighboring_edge_index].capacity > 0:
                    neighboring_edge = edges[neighboring_edge_index]
                    neighboring_edge_dst = neighboring_edge.dst
                    if parents[neighboring_edge_dst] == -1:
                        bfs.append(neighboring_edge_index)
                        parents[neighboring_edge_dst] = neighboring_edge_index
                        if neighboring_edge_dst == target:
                            target_found = True
                            break
        if not target_found:
            if debug:
                print("No more augmenting path can be found")
            break
        # Recovering the augmenting path and determine how much flow we can augment
        available_capacity = None
        augmenting_edge_indexes = []
        cursor = target
        while True:
            parent_edge_index = parents[cursor]
            if parent_edge_index == 0:
                break
            else:
                augmenting_edge = edges[parent_edge_index]                
                augmenting_edge_indexes.append(parent_edge_index)
                cursor = augmenting_edge.src
                if available_capacity is None or available_capacity > augmenting_edge.capacity:
                    available_capacity = augmenting_edge.capacity
        if debug:
            print("The augmenting path is:")
            augmenting_edge_indexes.reverse()
            for augmenting_edge_index in augmenting_edge_indexes:
                augmenting_edge = edges[augmenting_edge_index]
                print(augmenting_edge)
            augmenting_edge_indexes.reverse()
            print("and we can augment %s" % available_capacity)
        
        flow = flow + available_capacity
        for augmenting_edge_index in augmenting_edge_indexes:
            augmenting_edge = edges[augmenting_edge_index]
            if augmenting_edge_index % 2 == 1:
                partner_edge_index = augmenting_edge_index + 1
            else:
                partner_edge_index = augmenting_edge_index - 1
            partner_edge = edges[partner_edge_index]
            augmenting_edge.capacity = augmenting_edge.capacity - available_capacity
            partner_edge.capacity = partner_edge.capacity + available_capacity
    return flow

def main():
    edges = [
        Edge(0, 1, 20),
        Edge(0, 3, 10),
        Edge(1, 3, 30),
        Edge(1, 2, 10),
        Edge(3, 2, 20)
    ]
    print(maximum_flow(4, 0, 2, edges))
    return 0

if __name__ == "__main__":
    main()