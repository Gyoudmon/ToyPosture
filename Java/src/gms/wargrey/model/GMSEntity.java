package gms.wargrey.model;

public abstract class GMSEntity {
	protected static final String DELIMITER = ",";
	protected static final char CLASS_TAG = 'c';
	protected static final char DISCIPLINE_TAG = 'd';
	protected static final char STUDENT_TAG = 's';
	
    protected static boolean match(String line, char tag, int offset[]) {
		boolean okay = false;
		
		if (line.length() > 2) {
			if ((line.charAt(0) == tag) && (line.charAt(1) == ':')) {
				offset[0] = 2;
				okay = true;
			}
		}
		
		return okay;
	}
	
	protected static boolean match(String line, char tag1, char tag2, int offset[]) {
		boolean okay = false;
		
		if (line.length() > 3) {
			if ((line.charAt(0) == tag1) && (line.charAt(1) == tag2) && (line.charAt(2) == ':')) {
				offset[0] = 3;
				okay = true;
			}
		}
		
		return okay;
	}
	
	protected static boolean match(String line, char tag1, char tag2, char tag3, int offset[]) {
		boolean okay = false;
		
		if (line.length() > 4) {
			if ((line.charAt(0) == tag1) && (line.charAt(1) == tag2) && (line.charAt(2) == tag3) && (line.charAt(3) == ':')) {
				offset[0] = 4;
				okay = true;
			}
		}
		
		return okay;
	}
	
	/*********************************************************************************************/
	public abstract long primaryKey();
}
