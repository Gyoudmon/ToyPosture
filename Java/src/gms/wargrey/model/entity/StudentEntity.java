package gms.wargrey.model.entity;

import gms.wargrey.model.GMSText;
import gms.wargrey.model.GMSEntity;

public final class StudentEntity extends GMSEntity {
	public static boolean match(String line, int offset[]) {
		return GMSText.match(line, GMSText.STUDENT_TAG, offset);
	}
	
	public static String prompt() {
		return "class:nat, No:nat, nickname:str, gender:str, age:byte";
	}
	
	public static String update_prompt() {
		return "No:nat, class:nat, age:byte";
	}

	/*********************************************************************************************/
	public StudentEntity(String line, int offset) {
		String record = (offset > 0) ? line.substring(offset) : line;
		String [] tokens = record.split(GMSText.DELIMITER);
		
		this.clsId = Integer.parseUnsignedInt(tokens[0].trim());
		if (this.clsId == 0) throw new IllegalArgumentException("invalid class identity");
		
		this.No = Integer.parseUnsignedInt(tokens[1].trim());
		if (this.No == 0) throw new IllegalArgumentException("invalid student No.");
		
		this.nickname = tokens[2].trim();
		if (this.nickname.isBlank()) throw new IllegalArgumentException("empty student name");
		
		this.gender = StudentGender.valueOf(tokens[3].trim());
		this.age = Integer.parseUnsignedInt(tokens[4].trim()) % 0x100;
	}
	
	public boolean update(String line, int offset) {
		String record = (offset > 0) ? line.substring(offset) : line;
		String [] tokens = record.split(GMSText.DELIMITER);
		int old_clsId = this.clsId;
		int old_age = this.age;
		
		if (!(tokens[0].isBlank() || tokens[0].equals("-"))) {
			this.clsId = Integer.parseUnsignedInt(tokens[0].trim());
		}
		
		if (!(tokens[1].isBlank() || tokens[1].equals("-"))) {
			this.age = Integer.parseUnsignedInt(tokens[1].trim());
		}
		
		return (this.clsId != old_clsId) || (this.age != old_age);
	}
	
	/*********************************************************************************************/
	public String getNickName() {
		return this.nickname;
	}
	
	public StudentGender getGender() {
		return this.gender;
	}
	
	public int getClassIdentity() {
		return this.clsId;
	}
	
	public int getAge() {
		return this.age;
	}
	
	/*********************************************************************************************/
	@Override public int hashCode() {
		return this.No;
	}
	
	@Override public String toString() {
		return String.format("%c:%d,%d,%s,%s,%d", GMSText.STUDENT_TAG,
				this.clsId, this.No,
				this.nickname, this.gender, this.age);
	}

	/*********************************************************************************************/
	private int No;
	private int clsId;
	private String nickname;
	private StudentGender gender;
	private int age;
}
