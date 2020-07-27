from collections import deque
import random

# begin debug
id = 0
# end debug

class TrieNode:
    def __init__(self):
        # begin debug
        global id
        id = id + 1
        self.id = id
        # end debug
        self.children = {}
        self.failure = None
        self.pattern = None
        self.output = None

    def add(self, s):
        self.__add(s, s)

    def __add(self, s, t):
        if len(s) > 0:
            first = s[0]
            butfirst = s[1:]
            if first not in self.children:
                self.children[first] = TrieNode()
            self.children[first].__add(butfirst, t)
        else:
            self.pattern = t

    # begin debug
    def print(self):
        self.__print(" ", 0)
    
    def __print(self, c, indent):
        s = ""
        for i in range(0, indent):
            s = s + " "
        s = s + "%s(%s) has pattern %s" % (c, self.id, self.pattern)
        print(s)
        for child in self.children:
            self.children[child].__print(child, indent + 1)
    # end debug

def bfs(root):
    queue = deque()
    queue.append((None, ' ', root))
    while len(queue) > 0:
        (p, c, v) = queue.popleft()
        while p is not None:
            if p == root:
                break
            if c in p.failure.children:
                v.failure = p.failure.children[c]
                if v.failure.pattern is not None:
                    v.output = v.failure
                elif v.failure.output is not None:
                    v.output = v.failure.output
                break
            p = p.failure
        if v.failure is None:
            v.failure = root
        for child_key in v.children:
            queue.append((v, child_key, v.children[child_key]))

def aho_corasick(text, patterns):
    root = TrieNode()
    for pattern in patterns:
        root.add(pattern)
    bfs(root)
    root.print()
    result = []
    cursor = root
    i = 0
    while True:
        if i == len(text):
            break
        c = text[i]
        if c in cursor.children:
            cursor = cursor.children[c]
            i = i + 1
            if cursor.pattern is not None:
                result.append((i - len(cursor.pattern), cursor.pattern))
            output_cursor = cursor.output
            while output_cursor is not None:
                result.append((i - len(output_cursor.pattern), output_cursor.pattern))
                output_cursor = output_cursor.output
        else:
            if cursor == root:
                i = i + 1
            else:
                cursor = cursor.failure
    return result

# begin test
def random_dna(length):
    result = ""
    for i in range(0, length):
        result = result + random.choice(['A','C','T','G'])
    return result

def test():
    text = random_dna(100000)
    patterns = []
    for i in range(0, 500):
        pattern = random_dna(5)
        if pattern in patterns:
            i = i - 1
        else:
            patterns.append(pattern)
    actual = aho_corasick(text, patterns)
    expect = []
    for pattern in patterns:
        i = 0
        while True:
            f = text.find(pattern, i)
            if f == -1:
                break
            else:
                expect.append((f, pattern))
            i = f + 1
    actual.sort()
    expect.sort()
    if len(actual) != len(expect):
        print("Fail")
        return
    for i in range(0, len(actual)):
        if actual[i] != expect[i]:
            print("Fail")
            return

def main():
    random.seed(0)
    test()
# end test

if __name__ == "__main__":
    main()