package es.lcssl.mcm;

public class CostMatrixDouble {
	
	public final int dim;
	public final double[][] values;
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
	
	public String getFormat() {
		return fmt;
	}
	
	public void setFormat(final String fmt) {
		this.fmt = fmt;
	}
	
	public String toString() {
		StringBuffer sb = new StringBuffer("{");
		for (int i = 0; i < dim; i++) {
			if (i != 0) {
				sb.append(",\n ");
			}
			sb.append("{");
			for (int j = 0; j < dim; j++) {
				if (j != 0) {
					sb.append(", ");
				}
				sb.append(format(values[i][j]));
			}
			sb.append("}");
		}
		sb.append("}");
		return sb.toString();
	}

} /* class CostMatrixDouble */
