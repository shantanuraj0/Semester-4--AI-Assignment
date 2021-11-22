class Node:
    def __init__(self, missionaries, cannibals, boat):
        self.missionaries = missionaries
        self.cannibals = cannibals
        self.boat = boat


# ___________________________________Global Variables_______________________________________________

MAX_DEPTH = 1000
cost = []
parent = []
visited = []
generated = []
root = Node(3, 3, 0)


# ___________________________________ DFS _______________________________________________


def run_dfs():
    _init_global()
    dfs(root, [], 0, MAX_DEPTH)
    print_cost()


def dfs(node: Node, expanded: list, depth, max_depth):
    visited[node.missionaries][node.cannibals][node.boat] = True

    print_details(expanded)

    expanded.append(generated.pop(0))

    if (node.missionaries == 0) & (node.cannibals == 0):
        return True

    if depth == max_depth:
        return False

    print('Expanding ({} , {} , {})'.format(node.missionaries, node.cannibals, node.boat))

    child = []

    generate_successors(node, child)

    for node in reversed(child):
        generated.insert(0, node)

    res = False

    for i in range(len(child)):
        res = res | dfs(generated[0], expanded, depth + 1, max_depth)

    return res


# ___________________________________ BFS _______________________________________________


def run_bfs():
    _init_global()
    bfs()
    print_cost()


def bfs():
    expanded = []
    while len(generated) > 0:
        print_details(expanded)
        n = len(generated)
        for i in range(n):
            node = generated.pop(0)
            visited[node.missionaries][node.cannibals][node.boat] = True
            expanded.append(node)
            print('Expanding ({} , {} , {})'.format(node.missionaries, node.cannibals, node.boat))
            generate_successors(node, generated)

# ___________________________________ Iterative DFS _______________________________________________


def run_iterative_dfs():
    for k in range(MAX_DEPTH):
        print('\nk = {}\n'.format(k))
        _init_global()
        if dfs(root, [], 0, k):
            break
    print_cost()


# ___________________________________ UCS _______________________________________________


def run_ucs():
    _init_global()
    ucs()
    print_cost()


def ucs():
    expanded = []
    while True:
        print_details(expanded)
        min_node_ind = minCostNode()
        node = generated[min_node_ind]
        del generated[min_node_ind]
        visited[node.missionaries][node.cannibals][node.boat] = True
        expanded.append(node)
        if (node.missionaries == 0) & (node.cannibals == 0):
            return
        print('Expanding ({} , {} , {})'.format(node.missionaries, node.cannibals, node.boat))
        generate_successors(node, generated)


# ___________________________________ Util Functions _______________________________________________


def generate_successors(node, child):
    if node.boat == 0:

        if node.missionaries > 1:  # send 2 missionaries
            generate_node(node, node.missionaries - 2, node.cannibals, 1, node.missionaries, node.cannibals, 0,
                          child, 20)
        if node.cannibals > 1:  # send 2 cannibal
            generate_node(node, node.missionaries, node.cannibals - 2, 1, node.missionaries, node.cannibals, 0,
                          child, 40)
        if (node.cannibals > 0) & (node.missionaries > 0):  # send 1 missionaries , 1 cannibal
            generate_node(node, node.missionaries - 1, node.cannibals - 1, 1, node.missionaries, node.cannibals, 0,
                          child, 30)
        if node.missionaries > 0:  # send 1 missionary
            generate_node(node, node.missionaries - 1, node.cannibals, 1, node.missionaries, node.cannibals, 0,
                          child, 10)
        if node.cannibals > 0:  # send 1 cannibal
            generate_node(node, node.missionaries, node.cannibals - 1, 1, node.missionaries, node.cannibals, 0,
                          child, 20)
    else:

        if node.missionaries < 2:  # send 2 missionaries
            generate_node(node, node.missionaries + 2, node.cannibals, 0, node.missionaries, node.cannibals, 1,
                          child, 20)
        if node.cannibals < 2:  # send 2 cannibal
            generate_node(node, node.missionaries, node.cannibals + 2, 0, node.missionaries, node.cannibals, 1,
                          child, 40)
        if (node.cannibals < 3) & (node.missionaries < 3):  # send 1 missionaries , 1 cannibal
            generate_node(node, node.missionaries + 1, node.cannibals + 1, 0, node.missionaries, node.cannibals, 1,
                          child, 30)
        if node.missionaries < 3:  # send 1 missionary
            generate_node(node, node.missionaries + 1, node.cannibals, 0, node.missionaries, node.cannibals, 1,
                          child, 10)
        if node.cannibals < 3:  # send 1 cannibal
            generate_node(node, node.missionaries, node.cannibals + 1, 0, node.missionaries, node.cannibals, 1,
                          child, 20)


def generate_node(node, new_missionaries, new_cannibals, new_boat, missionaries, cannibals, boat, child, weight):
    if ((new_missionaries > 0) & (new_missionaries < new_cannibals)) | (
            (new_missionaries < 3) & (new_cannibals < new_missionaries)):
        return

    if not visited[new_missionaries][new_cannibals][new_boat]:
        if cost[new_missionaries][new_cannibals][new_boat] != -1:
            if cost[new_missionaries][new_cannibals][new_boat] + weight < \
                    cost[new_missionaries][new_cannibals][new_boat]:
                cost[new_missionaries][new_cannibals][new_boat] = cost[missionaries][cannibals][boat] + weight
                parent[new_missionaries][new_cannibals][new_boat] = node
        else:
            print('Generating ({} , {}, {})'.format(new_missionaries, new_cannibals, new_boat))
            cost[new_missionaries][new_cannibals][new_boat] = cost[missionaries][cannibals][boat] + weight
            child.append(Node(new_missionaries, new_cannibals, new_boat))
            parent[new_missionaries][new_cannibals][new_boat] = node


def minCostNode():
    min_val = 1000_000
    min_node = None
    for i in range(len(generated)):
        if cost[generated[i].missionaries][generated[i].cannibals][generated[i].boat] < min_val:
            min_val = cost[generated[i].missionaries][generated[i].cannibals][generated[i].boat]
            min_node = i
    return min_node


def print_details(expanded):
    print("Expanded Nodes : ")
    print("{ ", end="")
    for node in expanded:
        print("({},{},{})-{} ".format(node.missionaries, node.cannibals, node.boat,
                                      cost[node.missionaries][node.cannibals][node.boat]), end=",")
    print(" }")
    print("Generated Nodes : ")
    print("{ ", end="")
    for node in generated:
        print("({},{},{})-{} ".format(node.missionaries, node.cannibals, node.boat,
                                      cost[node.missionaries][node.cannibals][node.boat]), end=",")
    print(" }")


def print_cost():
    print()
    print("cost to reach goal : {}".format(cost[0][0][1]))
    cur = parent[0][0][1]
    res = [Node(0, 0, 1)]
    while cur != -1:
        res.append(cur)
        cur = parent[cur.missionaries][cur.cannibals][cur.boat]
    res.reverse()
    print()
    print("path to reach goal: ")
    for node in res:
        print(' ({},{},{}) '.format(node.missionaries, node.cannibals, node.boat), end='->')
    print()


def _init_global():
    global cost, parent, visited, generated
    cost = [[[-1 for k in range(2)] for j in range(4)] for i in range(4)]
    parent = [[[-1 for k in range(2)] for j in range(4)] for i in range(4)]
    visited = [[[False for k in range(2)] for j in range(4)] for i in range(4)]
    cost[3][3][0] = 0
    generated = [root]


def main():

    run_dfs()
    # run_bfs()
    # run_iterative_dfs()
    # run_ucs()


if __name__ == "__main__":
    main()
