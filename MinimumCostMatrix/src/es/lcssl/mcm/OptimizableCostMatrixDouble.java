package es.lcssl.mcm;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Set;

public class OptimizableCostMatrixDouble extends CostMatrixDouble {

	public class Node {

		public final Node parent;
		public final int row, col;
		public final double cost;
		public final int hash;
		public int numChildren;
		
		public Node(final Node parent, final int row, final int col, double cost) {
			this.parent = parent; 
			this.row = row; 
			this.col = col; 
			this.cost = cost;
			this.hash = (parent != null ? parent.hash : 0) + row*dim + col;
			this.numChildren = 0;  // for next process.
			if (parent != null) parent.numChildren++;
		}
		
		public Node() {
			this(null, -1, -1, 0.0);
		}
		
		@Override
		public int hashCode() {
			return hash;
		}

		@Override
		public boolean equals(Object obj) {
			if (this == obj) return true;
			if (!(obj instanceof Node)) return false;
			Node other = (Node) obj;
			if (parent != other.parent) return false;
			if (row != other.row) return false;
			if (col != other.col) return false;
			return true;
		}
		public boolean isFinal() {
			return col == dim;
		}
	}
		
	public OptimizableCostMatrixDouble(int dim) {
		super(dim);
	}
	
	private HashSet<Node> set;
	private Node root;
	
	private void init_process() {
		set = new HashSet<Node>();
		set.add(root = new Node());
	}
	
	private Node process_one() {
		double minCost = Double.MAX_VALUE;
		int row=-1, col=-1;
		Node prnt = null;
		for(Node p: set) { // for all viable parents
			if (p.col + p.numChildren) p.
			for (int r = 0; r < dim; r++) { // test all rows
				// test if already have a node at row r
				for (Node v = p; v != root; v = v.parent)
					if (v.row == r) continue; // already visited.
				int c = p.col+1; // next column
				double cost = p.cost + values[r][c];
				if (cost < minCost) {
					prnt = p;
					row = r; 
					col = p.col+1;
					minCost = 
				}
			}
		}
		return null;
	}

}
