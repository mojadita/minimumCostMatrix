package es.lcssl.mcm;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import java.util.Set;

public class OptimizableCostMatrixDouble extends CostMatrixDouble {

	private List<Node> theList;
	private Node root;

	public class Node {

		private final Node parent;
		private final int col, row;
		private final double cost;
		private final Node[] children;
		private int n_children;

		public Node(final Node parent, final int row, final int col) {
			this.parent = parent;
			this.col = col;
			this.row = row;
			this.cost = parent != null ? parent.getCost() + getValue(row, col)
					: 0.0;
			this.children = new Node[dim];
			this.n_children = 0;
			if (parent != null) {
				parent.n_children++;
				parent.children[row] = this;
			}
		}

		public double getCost() {
			return cost;
		}

		public int getN_children() {
			return n_children;
		}

		public void setN_children(int n_children) {
			this.n_children = n_children;
		}

		public Node getParent() {
			return parent;
		}

		public int getRow() {
			return row;
		}

		public int getCol() {
			return col;
		}

		public Node[] getChildren() {
			return children;
		}

		private String toString(Node node) {
			if (node == root)
				return "ROOT";
			StringBuffer sb = new StringBuffer();
			if (node.parent != null) {
				sb.append(toString(node.parent));
				sb.append(", ");
			}
			return sb.toString() + "<" + node.getRow() + ", " + node.getCol()
					+ ": " + node.getCost() + ">";
		}

		@Override
		public String toString() {
			if (this == root)
				return "ROOT";
			return "{" + toString(this) + "}";
		}

		public boolean isFinal() {
			return col == dim;
		}
	}

	public OptimizableCostMatrixDouble(int dim) {
		super(dim);
		init_process();
	}

	private void init_process() {
		theList = new ArrayList<Node>();
		theList.add(root = new Node(null, -1, -1));
	}

	private Node process_one() {
		double minCost = Double.MAX_VALUE;
		int row = -1, col = -1;
		Node prnt = null;
		ArrayList<Node> newList = new ArrayList<Node>();
		for (Node n : theList) { // for all viable parents
		// System.out.println("Visiting node " + n);
			int candidates = 0;
			for (int r = 0; r < dim; r++) { // test all rows
			// System.out.println("  Row #" + r);
				if (n.row == r) {// this node's row.
				// System.out.println("    row is this node's row");
					continue;
				}
				if (n.children[r] != null) { // already added a node in this
												// row.
				// System.out.println("    already have a child at this node's row");
					continue;
				}

				{ // search up in the tree for a node at row r
					Node v;
					for (v = n; v != root; v = v.parent)
						if (v.row == r)
							break; // already visited.
					if (v != root) {
						// System.out.println("    already have a parent at this node's row");
						continue; // a node already visited this row.
					}
				} // block

				candidates++;
				int c = n.col + 1; // next column
				double cost = n.getCost() + values[r][c];
				// System.out.print("    candidate at row=" + r + ", col="+ c +
				// ", cost=" + cost);
				if (cost < minCost) {
					// System.out.print(" is a CANDIDATE");
					prnt = n;
					row = r;
					col = c;
					minCost = cost;
				}
				// System.out.println();
			}
			// System.out.println("  candidates left, after scanning: " +
			// candidates);
			if (candidates > 0)
				newList.add(n);
		}

		theList = newList;
		if (prnt != null) {
			Node result = new Node(prnt, row, col);
			// System.out.println("Candidate is " + result);
			newList.add(result);
			return result;
		}
		return null;
	}

	public static void main(String[] args) {
		final int DIM = 10;
		Random r = new Random();
		OptimizableCostMatrixDouble m = new OptimizableCostMatrixDouble(DIM);
		for (int row = 0; row < m.dim; row++)
			for (int col = 0; col < m.dim; col++)
				m.setValue(row, col,
						row == 0 || col == 0 
							? 1.0 
							: m.getValue(row - 1, col)
							+ m.getValue(row, col - 1));
		m.init_process();
		Node sol;
		int npos = 0;
		do {
			sol = m.process_one();
			System.out.println("found so far: " + sol);
			npos++;
		} while (sol.getCol() < m.dim - 1);
		System.out.println(m);
		System.out.println("Solution is " + sol);
		System.out.println("positions searched: " + npos);
	}
}
