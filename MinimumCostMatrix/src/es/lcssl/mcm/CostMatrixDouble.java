package es.lcssl.mcm;

public class CostMatrixDouble {

	protected final int dim;
	protected final double[][] values;
	private String fmt = "%f";

	public CostMatrixDouble(int dim) {
		this.dim = dim;
		this.values = new double[dim][];
		for (int i = 0; i < dim; i++) {
			this.values[i] = new double[dim];
		}
	}

	protected String format(final double val) {
		return String.format(fmt, val);
	}

	public String getFmt() {
		return fmt;
	}

	public void setFmt(String fmt) {
		this.fmt = fmt;
	}

	public int getDim() {
		return dim;
	}

	public void setFormat(final String fmt) {
		this.fmt = fmt;
	}

	public double getValue(int row, int col) {
		return values[row][col];
	}

	public void setValue(int row, int col, double val) {
		values[row][col] = val;
	}

	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer("{");
		for (int row = 0; row < dim; row++) {
			if (row != 0) {
				sb.append(",\n ");
			}
			sb.append("{");
			for (int col = 0; col < dim; col++) {
				if (col != 0) {
					sb.append(", ");
				}
				sb.append(format(values[row][col]));
			}
			sb.append("}");
		}
		sb.append("}");
		return sb.toString();
	}

} /* class CostMatrixDouble */
