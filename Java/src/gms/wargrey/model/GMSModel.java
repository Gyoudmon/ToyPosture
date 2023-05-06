package gms.wargrey.model;

import gms.wargrey.model.entity.*;

import java.io.File;
import java.io.PrintStream;

import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.EnumMap;

public class GMSModel {
	public GMSModel(IModelListener master) {
		this.master = master;

		this.classes = new TreeMap<>();
		this.disciplines = new TreeMap<>();
		this.students = new TreeMap<>();
		this.scores = new TreeMap<>();
		
		this.disCodes = new EnumMap<>(DisciplineType.class);
	}

	/*********************************************************************************************/
	public void importFromFile(String gmsin) {
		File gmsPath = new File(gmsin);
		
		if (gmsPath.exists() && gmsPath.isFile()) {
			try {
				Scanner parser = new Scanner(gmsPath);
				int [] offset = { 0 };
				
				this.clearAll();
			
				while (parser.hasNext()) {
					try {
						String line = parser.nextLine();
					
						if (ClassEntity.match(line, offset)) {
							this.registerClass(new ClassEntity(line, offset[0]));
						} else if (DisciplineEntity.match(line, offset)) {
							this.registerDiscipline(new DisciplineEntity(line, offset[0]));
						} else if (StudentEntity.match(line, offset)) {
							this.registerStudent(new StudentEntity(line, offset[0]));
						} else if (GradeEntity.match(line, offset)) {
							this.registerStudentScores(new GradeEntity(line, offset[0]));
						}
					} catch (Exception e) {
						System.err.println(e.getClass().getName() + ": " + e.getMessage());
					}
				}

				parser.close();
			} catch (Exception e) {
				System.err.println(e.getClass().getName() + ": " + e.getMessage());
			}
		}
	}
	
	public void exportToFile(String gmsout) throws Exception {
		if (!gmsout.isBlank()) {
			File gmsPath = new File(gmsout);
			PrintStream ps = new PrintStream(gmsPath);
			
			this.exportToPrintStream(ps);
			ps.close();
		} else {
			this.exportToPrintStream(System.out);
		}
	}
	
	/*********************************************************************************************/
	public void createClassFromUser(String line) {
		this.registerClass(new ClassEntity(line, 0));
	}
	
    public void deleteClassByUser(String line) {
    	int clsId = Integer.parseInt(line.trim());
    	
    	if (this.classes.containsKey(clsId)) {
    		ClassEntity cls = this.classes.remove(clsId);
    		this.master.onClassDeleted(clsId, cls);
    	} else {
    		throw new NoSuchElementException(String.format("Invalid class Identity(%d) or not found", clsId));
    	}	
    }
    
    public void createDisciplineFromUser(String line) {
		this.registerDiscipline(new DisciplineEntity(line, 0));
	}
    
    public void updateDisciplineFromUser(String line) {
    	String [] tokens = line.split(GMSText.DELIMITER);
    	int disCode = Integer.parseInt(tokens[0].trim());
    	
    	if (this.disciplines.containsKey(disCode)) {
    		DisciplineEntity dis = this.disciplines.get(disCode);
    		
    		if (dis.update(tokens[1], 0)) {
    			this.master.onDisciplineUpdated(disCode, dis);
    		}
    	} else {
    		throw new NoSuchElementException(String.format("Discipline(%d) not found", disCode));
    	}
	}
	
    public void deleteDisciplineByUser(String line) {
    	int disCode = Integer.parseInt(line.trim());
    	
    	if (this.disciplines.containsKey(disCode)) {
    		DisciplineEntity dis = this.disciplines.remove(disCode);
    		this.disCodes.remove(dis.getType());
    		this.master.onDisciplineDeleted(disCode, dis);
    	} else {
    		throw new NoSuchElementException(String.format("Invalid discipline code(%d) or not found", disCode));
    	}	
    }
    
    public void createStudentFromUser(String line) {
		this.registerStudent(new StudentEntity(line, 0));
	}
    
    public void updateStudentFromUser(String line) {
    	String [] tokens = line.split(GMSText.DELIMITER, 2);
    	int sNo = Integer.parseInt(tokens[0].trim());
    	
    	if (this.students.containsKey(sNo)) {
    		StudentEntity stu = this.students.get(sNo);
    		
    		if (stu.update(tokens[1], 0)) {
    			this.master.onStudentUpdated(sNo, stu);
    		}
    	} else {
    		throw new NoSuchElementException(String.format("Student(%d) not found", sNo));
    	}
	}
	
    public void deleteStudentByUser(String line) {
    	int sNo = Integer.parseInt(line.trim());
    	
    	if (this.students.containsKey(sNo)) {
    		StudentEntity stu = this.students.remove(sNo);
    		this.master.onStudentDeleted(sNo, stu);
    	} else {
    		throw new NoSuchElementException(String.format("Invalid student No(%d) or not found", sNo));
    	}	
    }
    
    public void registerStudentScoresFromUser(int sNo, int disCode, int ts, String line) {
    	GradeEntity grade = new GradeEntity(sNo, disCode, ts);

        grade.extractScores(line, 0);
        this.registerStudentScores(grade);
    }
    
    public void updateStudentScoresFromUser(int sNo, int disCode, int ts, String line) {
    	if (this.scores.containsKey(sNo)) {
    		TreeMap<Integer, TreeMap<Integer, GradeEntity>>  tsMap = this.scores.get(sNo);
    		
    		if (tsMap.containsKey(ts)) {
    			TreeMap<Integer, GradeEntity> disMap = tsMap.get(ts);
    			
    			if (disMap.containsKey(disCode)) {
    				GradeEntity grade = disMap.get(disCode);
    				
    				grade.extractScores(line, 0);
    				this.master.onGradeUpdated(sNo, grade);
    			} else {
    				throw new NoSuchElementException(String.format("Grade for student(%d@%d) not found", sNo, ts));
    			}
    		} else {
    			throw new NoSuchElementException(String.format("Grade for student(%d@%d) not found", sNo, ts));
    		}
    	} else {
    		throw new NoSuchElementException(String.format("Grade for student(%d) not found", sNo));
    	}
    }
    
    public void deleteStudentScoresByUser(int sNo, int disCode, int ts) {
    	if (this.scores.containsKey(sNo)) {
    		TreeMap<Integer, TreeMap<Integer, GradeEntity>>  tsMap = this.scores.get(sNo);
    		
    		if (tsMap.containsKey(ts)) {
    			TreeMap<Integer, GradeEntity> disMap = tsMap.get(ts);
    			
    			if (disMap.containsKey(disCode)) {
    				GradeEntity grade = disMap.remove(disCode);
    				this.master.onGradeDeleted(sNo, grade);
    			} else {
    				throw new NoSuchElementException(String.format("Grade for student(%d@%d) not found", sNo, ts));
    			}
    		} else {
    			throw new NoSuchElementException(String.format("Grade for student(%d@%d) not found", sNo, ts));
    		}
    	} else {
    		throw new NoSuchElementException(String.format("Grade for student(%d) not found", sNo));
    	}
    }
    
    /*********************************************************************************************/
    public ClassEntity [] getAllClasses() {
    	return this.classes.values().toArray(new ClassEntity[0]);
    }
    
    public DisciplineEntity [] getAllDisciplines() {
    	return this.disciplines.values().toArray(new DisciplineEntity[0]);
    }
    
    public StudentEntity [] getAllStudents() {
    	return this.students.values().toArray(new StudentEntity[0]);
    }
    
    public TreeSet<Integer> getAllGradeTimeStamps() {
    	TreeSet<Integer> tss = new TreeSet<>();

    	for (TreeMap<Integer, TreeMap<Integer, GradeEntity>> tsMap : this.scores.values()) {
    		for (Integer ts : tsMap.keySet()) {
    			tss.add(ts);
    		}
    	}
    	
    	return tss;
    }
    
    public String getStudentName(int sNo) {
    	String name = "";
    	
    	if (this.students.containsKey(sNo)) {
    		name = this.students.get(sNo).getNickName();
    	}
    	
    	return name;
    }
    
    public double getStudentScore(int sNo, int disCode, int ts) {
    	double score = -1.0;
    	
    	if (this.scores.containsKey(sNo)) {
    		TreeMap<Integer, TreeMap<Integer, GradeEntity>> tsMap = this.scores.get(sNo);
    		
    		if (tsMap.containsKey(ts)) {
    			TreeMap<Integer, GradeEntity> disMap = tsMap.get(ts);
    			
    			if (disMap.containsKey(disCode)) {
    				score = disMap.get(disCode).getTotalScore();
    			}
    		}
    	}
    	
    	return score;
    }
    

	/*********************************************************************************************/
    private void exportToPrintStream(PrintStream gmsout) {
		for (ClassEntity cls : this.classes.values()) {
			gmsout.println(cls.toString());
		}
		
		for (DisciplineEntity dis : this.disciplines.values()) {
			gmsout.println(dis.toString());
		}
		
		for (StudentEntity stu : this.students.values()) {
			gmsout.println(stu.toString());
		}
		
		for (TreeMap<Integer, TreeMap<Integer, GradeEntity>> tsMaps : this.scores.values()) {
			for (TreeMap<Integer, GradeEntity> disMaps : tsMaps.values()) {
				for (GradeEntity grade : disMaps.values()) {
					gmsout.println(grade.toString());
				}
			}
		}
	}
    
    private void registerClass(ClassEntity cls) {
		int clsId = cls.hashCode();
		
		if (!this.classes.containsKey(clsId)) {
			this.classes.put(clsId, cls);
			this.master.onClassCreated(clsId, cls);
		} else {
			throw new IllegalArgumentException(String.format("Class(%d) has already been existed", clsId));
		}
	}
    
    private void registerDiscipline(DisciplineEntity dis) {
		int disCode = dis.hashCode();
		DisciplineType disType = dis.getType();
		
		if (!this.disciplines.containsKey(disCode)) {
			if (!this.disCodes.containsKey(disType)) {
				this.disciplines.put(disCode, dis);
				this.disCodes.put(disType, disCode);
				this.master.onDisciplineCreated(disCode, dis);
			} else {
				throw new IllegalArgumentException(String.format("Discipline(%s) has already been existed", disType));
			}
		} else {
			throw new IllegalArgumentException(String.format("Discipline(%d) has already been existed", disCode));
		}
	}
    
    private void registerStudent(StudentEntity stu) {
		int sNo = stu.hashCode();
		
		if (!this.students.containsKey(sNo)) {
			this.students.put(sNo, stu);
			this.master.onStudentCreated(sNo, stu);
		} else {
			throw new IllegalArgumentException(String.format("Student(%d) has already been existed", sNo));
		}
	}
    
    private void registerStudentScores(GradeEntity grade) {
		int sNo = grade.getStudentNo();
		int disCode = grade.getDisciplineCode();
		int ts = grade.getTimestamp();
		
		if (!this.scores.containsKey(sNo)) {
			TreeMap<Integer, GradeEntity> disMap = new TreeMap<>();
			TreeMap<Integer, TreeMap<Integer, GradeEntity>> tsMap = new TreeMap<>();
			
			disMap.put(disCode, grade);
			tsMap.put(ts, disMap);
			
			this.scores.put(sNo, tsMap);
		} else {
			TreeMap<Integer, TreeMap<Integer, GradeEntity>> tsMap = this.scores.get(sNo);
			
			if (!tsMap.containsKey(ts)) {
				TreeMap<Integer, GradeEntity> disMap = new TreeMap<>();
				
				disMap.put(disCode, grade);
				tsMap.put(ts, disMap);
			} else {
				TreeMap<Integer, GradeEntity> disMap = tsMap.get(ts);
				
				if (!disMap.containsKey(disCode)) {
					disMap.put(disCode, grade);
				} else {
					throw new IllegalArgumentException(String.format("grade(%d, %d, %d) has already been registered",
							sNo, disCode, ts));
				}
			}
		}
		
		this.master.onGradeCreated(sNo, grade);
	}
    
    private void clearAll() {
    	this.classes.clear();
    	this.disciplines.clear();
    	this.students.clear();
    	this.scores.clear();
    	this.disCodes.clear();
    }
	
	/*********************************************************************************************/
	private TreeMap<Integer, ClassEntity> classes;
	private TreeMap<Integer, DisciplineEntity> disciplines;
	private TreeMap<Integer, StudentEntity> students;
	private TreeMap<Integer, TreeMap<Integer, TreeMap<Integer, GradeEntity>>> scores;

	/*********************************************************************************************/
	private IModelListener master;
	private EnumMap<DisciplineType, Integer> disCodes;
}
