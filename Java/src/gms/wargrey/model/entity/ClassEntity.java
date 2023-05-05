package gms.wargrey.model.entity;

import gms.wargrey.model.GMSText;
import gms.wargrey.model.GMSEntity;

public final class ClassEntity extends GMSEntity {
	public static boolean match(String line, int offset[]) {
		return GMSText.match(line, GMSText.CLASS_TAG, offset);
	}
	
	public static String prompt() {
		return "seq:nat";
	}

	/*********************************************************************************************/
	public ClassEntity(String line, int offset) {
		this.seq = Integer.parseInt(((offset > 0) ? line.substring(offset) : line).trim());
		if (this.seq == 0L) throw new IllegalArgumentException("invalid class identity");
	}
	
	/*********************************************************************************************/
	@Override public int hashCode() {
		return this.seq;
	}
	
	@Override public String toString() {
		return String.format("%c:%d", GMSText.CLASS_TAG, this.seq);
	}

	/*********************************************************************************************/
	private int seq;
}
