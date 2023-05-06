package gms.wargrey.model.entity;

import gms.wargrey.model.GMSText;
import gms.wargrey.model.GMSEntity;

import java.util.ArrayList;

public final class GradeEntity extends GMSEntity {
	public static boolean match(String line, int offset[]) {
		return GMSText.match(line, GMSText.STUDENT_TAG, GMSText.DISCIPLINE_TAG, GMSText.TIMESTAMP_TAG, offset);
	}
	
	public static String prompt() {
		return "sNo:nat, disCode:nat, timestamp:nat, score:nat ...";
	}

	/*********************************************************************************************/
	public GradeEntity(int sNo, int disCode, int timestamp) {
		this.sNo = sNo;
		this.disCode = disCode;
		this.timestamp = timestamp;
		
		this.points = new ArrayList<>();
	}

	public GradeEntity(String line, int offset) {
		String record = (offset > 0) ? line.substring(offset) : line;
		String [] tokens = record.split(GMSText.DELIMITER);

		this.sNo = Integer.parseUnsignedInt(tokens[0].trim());
		if (this.sNo == 0) throw new IllegalArgumentException("invalid student No.");
		
		this.disCode = Integer.parseUnsignedInt(tokens[1].trim());
		if (this.disCode == 0) throw new IllegalArgumentException("invalid discipline code");
		
		this.timestamp = Integer.parseUnsignedInt(tokens[2].trim());
		if (this.timestamp == 0) throw new IllegalArgumentException("invalid timestamp");
		
		this.points = new ArrayList<>();
		this.extractScores(tokens[3], 0);
	}
	
	public void extractScores(String line, int offset) {
		String pts = (offset > 0) ? line.substring(offset) : line;
		String [] points = pts.split("\\s+");

		this.points.clear();
		
		for (String pt : points) {
			double s = (Double.parseDouble(pt));
			
			if ((s >= 0.0) && (s <= 150.0)) {
				this.points.add(s);
			} else {
				throw new IllegalArgumentException(String.format("invalid score: %f", s));
			}
		}
 	}
	
	/*********************************************************************************************/
	public int getStudentNo() {
		return this.sNo;
	}

	public int getDisciplineCode() {
		return this.disCode;
	}
	
	public int getTimestamp() {
		return this.timestamp;
	}
	
	public double getTotalScore() {
		double score = 0.0;
		
		for (double pt : this.points) {
			score += pt;
		}
		
		return score;
	}
	
	/*********************************************************************************************/
	@Override public int hashCode() {
		int compositeKey[] = { this.sNo, this.disCode, this.timestamp };
		
		return compositeKey.hashCode();
	}
	
	@Override public String toString() {
		StringBuffer s = new StringBuffer();
		
		if (!this.points.isEmpty()) {
			s.append(this.points.get(0));
			
			for (int idx = 1; idx < this.points.size(); idx ++) {
				s.append(' ');
				s.append(this.points.get(idx));
			}
		}
		
		return String.format("%c%c%c:%d,%d,%d,%s",
				GMSText.STUDENT_TAG, GMSText.DISCIPLINE_TAG, GMSText.TIMESTAMP_TAG,
				this.sNo, this.disCode, this.timestamp, s.toString());
	}

	/*********************************************************************************************/
	private int sNo;
	private int disCode;
	private int timestamp;
	private ArrayList<Double> points;
}
