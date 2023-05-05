package gms.wargrey.model.entity;

import gms.wargrey.model.GMSEntity;

import java.io.ByteArrayInputStream;
import java.util.Scanner;

public class ClassEntity extends GMSEntity {
	public static boolean match(String line, int offset[]) {
		return GMSEntity.match(line, GMSEntity.CLASS_TAG, offset);
	}
	
	public static String prompt() {
		return "seq:nat";
	}

	/*********************************************************************************************/
	public ClassEntity(String line, int offset) throws Exception {
		ByteArrayInputStream bin = new ByteArrayInputStream(line.getBytes());
		Scanner parser = new Scanner(bin);
		
		try {
			if (offset > 0) bin.skip(offset);
			parser.useDelimiter(GMSEntity.DELIMITER);
			this.seq = parser.nextLong();
			if (this.seq == 0L) throw new Exception("invalid class identity");
		} finally {
			parser.close();
		}
	}
	
	/*********************************************************************************************/
	@Override
	public long primaryKey() {
		return this.seq;
	}
	
	@Override
	public String toString() {
		return String.format("%c:%d", GMSEntity.CLASS_TAG, this.seq);
	}

	/*********************************************************************************************/
	private long seq;
}
