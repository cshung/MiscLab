package LeftLeaningRedBlackTree;

public class LLRB<Key extends Comparable<Key>, Value> {
	private static boolean TwoThreeTree = true;
	private static final boolean RED = true;
	private static final boolean BLACK = false;
	private Node root;

	private class Node {
		private Key key;
		private Value val;
		private Node left, right;
		private boolean color;

		Node(Key key, Value val) {
			this.key = key;
			this.val = val;
			this.color = RED;
		}

		public String toString() {
			return "[" + key + "," + val + "]";
		}
	}

	public Value get(Key key) {
		Node x = root;
		while (x != null) {
			int cmp = key.compareTo(x.key);
			if (cmp == 0) {
				return x.val;
			} else if (cmp < 0) {
				x = x.left;
			} else if (cmp > 0) {
				x = x.right;
			}
		}
		return null;
	}

	public void put(Key key, Value value) {
		root = insert(root, key, value);
		root.color = BLACK;
	}

	private Node insert(Node h, Key key, Value value) {
		if (h == null) {
			return new Node(key, value);
		}
		int cmp = key.compareTo(h.key);
		if (cmp < 0) {
			h.left = insert(h.left, key, value);
		} else if (cmp > 0) {
			h.right = insert(h.right, key, value);
		} else {
			h.val = value;
		}
		if (isRed(h.right) && !isRed(h.left)) {
			h = rotateLeft(h);
		}
		if (isRed(h.left) && isRed(h.left.left)) {
			h = rotateRight(h);
		}
		if (isRed(h.left) && isRed(h.right)) {
			colorFlip(h);
		}
		return h;
	}

	private Node fixUp(Node h) {
		if (isRed(h.right) && !isRed(h.left)) {
			h = rotateLeft(h);
		}
		if (isRed(h.left) && isRed(h.left.left)) {
			h = rotateRight(h);
		}
		if (isRed(h.left) && isRed(h.right)) {
			colorFlip(h);
		}
		return h;
	}

	public void delete(Key key) {
		root = delete(root, key);
		if (root != null) {
			root.color = BLACK;
		}
	}

	private Node delete(Node h, Key key) {
		if (key.compareTo(h.key) < 0) {
			if (!isRed(h.left) && !isRed(h.left.left))
				h = moveRedLeft(h);
			h.left = delete(h.left, key);
		} else {
			if (isRed(h.left))
				h = rotateRight(h);

			if (key.compareTo(h.key) == 0 && (h.right == null))
				return null;

			if (!isRed(h.right) && !isRed(h.right.left))
				h = moveRedRight(h);

			if (key.compareTo(h.key) == 0) {
				Node minNode = null;
				h.val = minNode.val;
				h.key = minNode.key;

				h.right = deleteMin(h.right);
			} else
				h.right = delete(h.right, key);
		}
		return fixUp(h);
	}

	public void deleteMin() {
		root = deleteMin(root);
		root.color = BLACK;
	}

	private Node deleteMin(Node h) {
		if (h.left == null) {
			return null;
		}
		if (!isRed(h.left) && !isRed(h.left.left)) {
			h = moveRedLeft(h);
		}
		h.left = deleteMin(h.left);
		return fixUp(h);
	}

	Node rotateLeft(Node h) {
		Node x = h.right;
		h.right = x.left;
		x.left = h;
		x.color = h.color;
		h.color = RED;
		return x;
	}

	Node rotateRight(Node h) {
		Node x = h.left;
		h.left = x.right;
		x.right = h;
		x.color = h.color;
		h.color = RED;
		return x;
	}

	void colorFlip(Node h) {
		h.color = !h.color;
		h.left.color = !h.left.color;
		h.right.color = !h.right.color;
	}

	private Node moveRedLeft(Node h) {
		colorFlip(h);
		if (isRed(h.right.left)) {
			h.right = rotateRight(h.right);
			h = rotateLeft(h);
			colorFlip(h);
		}
		return h;
	}

	private Node moveRedRight(Node h) {
		colorFlip(h);
		if (isRed(h.left.left)) {
			h = rotateRight(h);
			colorFlip(h);
		}
		return h;
	}

	boolean isRed(Node x) {
		if (x == null) {
			return false;
		}
		return x.color;
	}

	private void verify() {
		if (root == null) {
			return;
		}
		if (root.color == RED) {
			throw new IllegalStateException();
		}
		verify(root, false);
	}

	private int verify(Node n, boolean requireBlack) {
		if (n == null) {
			return 0;
		}
		if (n.color == RED && requireBlack) {
			throw new IllegalStateException();
		}
		if (n.left != null && n.left.color == BLACK && n.right != null && n.right.color == RED) {
			throw new IllegalStateException();
		}
		if (TwoThreeTree) {
			if (n.left != null && n.left.color == RED && n.right != null && n.right.color == RED) {
				throw new IllegalStateException();
			}
		}
		int left = verify(n.left, n.color == RED);
		int right = verify(n.right, n.color == RED);
		if (left != right) {
			throw new IllegalStateException();
		}
		return (n.color == BLACK ? 1 : 0) + left;
	}

	public static void main(String[] args) {
		LLRB<Integer, Integer> tree = new LLRB<Integer, Integer>();
		for (int i = 0; i < 50; i++) {
			tree.put(i, i);
			tree.verify();
		}
		for (int i = 0; i < 50; i++) {
			tree.delete(i);
			tree.verify();
		}
		System.out.println("Done");
	}
}
