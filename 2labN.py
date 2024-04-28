import random
import time

class Node:
    def __init__(self, weight):
        self.weight = weight
        self.children = []

def find_subtrees_with_min_max_ratio(root):
    min_ratio = float('inf')
    max_ratio = float('-inf')
    min_subtrees = []
    max_subtrees = []

    def dfs(node):
        nonlocal min_ratio, max_ratio, min_subtrees, max_subtrees
        total_weight = node.weight
        total_nodes = 1
        
        for child in node.children:
            child_weight, child_nodes = dfs(child)
            total_weight += child_weight
            total_nodes += child_nodes
        
        ratio = total_weight / total_nodes
        
        if ratio < min_ratio:
            min_ratio = ratio
            min_subtrees = [node]
        elif ratio == min_ratio:
            min_subtrees.append(node)
        
        if ratio > max_ratio:
            max_ratio = ratio
            max_subtrees = [node]
        elif ratio == max_ratio:
            max_subtrees.append(node)
        
        return total_weight, total_nodes

    dfs(root)
    
    return min_subtrees, max_subtrees

def dump(node, prefix='', is_root=True, is_last=True):
    print(prefix + ('' if is_root else ('|_ ' if is_last else '|_ ')) + str(node.weight))
    if not node or not node.children:
        return
    children = node.children
    for i in range(len(children)):
        dump(children[i], prefix + ('' if is_root else ('    ' if is_last else '|   ')), False, i + 1 >= len(children))

def create_random_tree(arity, num_nodes):
    if num_nodes < 1:
        return None
    
    root = Node(random.randint(1, 100))
    remaining_nodes = num_nodes - 1
    
    while remaining_nodes > 0:
        node = root
        for _ in range(random.randint(0, arity - 1)):
            if node.children:
                node = random.choice(node.children)
        new_node = Node(random.randint(1, 100))
        node.children.append(new_node)
        remaining_nodes -= 1
    
    return root

def create_tree_from_input():
    root_weight = int(input("Введите вес корневого узла: "))
    root = Node(root_weight)
    queue = [root]
    
    while queue:
        node = queue.pop(0)
        num_children = int(input(f"Введите количество дочерних узлов для узла {node.weight} (0 для пропуска): "))
        for _ in range(num_children):
            child_weight = int(input(f"Введите вес дочернего узла для узла {node.weight}: "))
            child = Node(child_weight)
            node.children.append(child)
            queue.append(child)
    
    return root

# Пример использования
choice = int(input("Выберите способ создания дерева:\n1. Создание дерева случайным образом\n2. Создание дерева вручную\nВведите номер: "))

if choice == 1:
    arity = int(input("Введите арность дерева: "))
    num_nodes = int(input("Введите количество узлов: "))
    root = create_random_tree(arity, num_nodes)
else:
    root = create_tree_from_input()

# Измерение времени работы и использования памяти
start_time = time.time()


min_subtrees, max_subtrees = find_subtrees_with_min_max_ratio(root)

end_time = time.time()


print("\nДерево:")
dump(root)
print("\nПоддерево(-ья) с минимальным отношением:")
for subtree in min_subtrees:
    dump(subtree)
print("\nПоддерево(-ья) с максимальным отношением:")
for subtree in max_subtrees:
    dump(subtree)

print(f"\nВремя работы: {end_time - start_time:.6f} секунд")

