package gms.wargrey.model.entity;

import gms.wargrey.model.GMSText;
import gms.wargrey.model.GMSEntity;

public final class DisciplineEntity extends GMSEntity {
	public static boolean match(String line, int offset[]) {
		return GMSText.match(line, GMSText.DISCIPLINE_TAG, offset);
	}
	
	public static String prompt() {
		return "code:nat, name:str, credit:nat";
	}
	
	public static String update_prompt() {
		return "code:nat, credit:nat";
	}

	/*********************************************************************************************/
	public DisciplineEntity(String line, int offset) {
		String record = (offset > 0) ? line.substring(offset) : line;
		String [] tokens = record.split(GMSText.DELIMITER);
			
		this.code = Integer.parseUnsignedInt(tokens[0].trim());
		if (this.code == 0) throw new IllegalArgumentException("invalid discipline code");
		
		this.type = DisciplineType.valueOf(tokens[1].trim());
		this.credit = Integer.parseUnsignedInt(tokens[2].trim());
	}
	
	public boolean update(String line, int offset) {
		String record = (offset > 0) ? line.substring(offset) : line;
		String [] tokens = record.split(GMSText.DELIMITER);
		int old_credit = this.credit;
		
		if (!(line.isBlank() || line.equals("-"))) {
			this.credit = Integer.parseUnsignedInt(tokens[0].trim());
		}
		
		return this.credit != old_credit;
	}
	
	/*********************************************************************************************/
	public DisciplineType getType() {
		return this.type;
	}
	
	public int getCredit() {
		return this.credit;
	}
	
	/*********************************************************************************************/
	@Override public int hashCode() {
		return this.code;
	}
	
	@Override public String toString() {
		return String.format("%c:%d,%s,%d", GMSText.DISCIPLINE_TAG, this.code, this.type, this.credit);
	}

	/*********************************************************************************************/
	private int code;
	private int credit;
	private DisciplineType type;
}
