package gms.wargrey.model;

import gms.wargrey.model.entity.*;

import java.io.File;
import java.io.PrintStream;

import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.EnumMap;

public class GMSModel {
	public GMSModel(IModelListener master) {
		this.master = master;

		this.classes = new TreeMap<>();
		this.disciplines = new TreeMap<>();
		
		this.disCodes = new EnumMap<>(DisciplineType.class);
	}

	/*********************************************************************************************/
	public void importFromFile(String gmsin) {
		File gmsPath = new File(gmsin);
		
		if (gmsPath.exists() && gmsPath.isFile()) {
			try {
				Scanner parser = new Scanner(gmsPath);
				int [] offset = { 0 };
			
				while (parser.hasNext()) {
					try {
						String line = parser.nextLine();
					
						if (ClassEntity.match(line, offset)) {
							this.registerClass(new ClassEntity(line, offset[0]));
						} else if (DisciplineEntity.match(line, offset)) {
							this.registerDiscipline(new DisciplineEntity(line, offset[0]));
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
    		this.master.onDisciplineDeleted(disCode, dis);
    	} else {
    		throw new NoSuchElementException(String.format("Invalid discipline code(%d) or not found", disCode));
    	}	
    }

	/*********************************************************************************************/
    private void exportToPrintStream(PrintStream gmsout) {
		for (ClassEntity cls : this.classes.values()) {
			gmsout.println(cls.toString());
		}
		
		for (DisciplineEntity dis : this.disciplines.values()) {
			gmsout.println(dis.toString());
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
	
	/*********************************************************************************************/
	private TreeMap<Integer, ClassEntity> classes;
	private TreeMap<Integer, DisciplineEntity> disciplines;

	/*********************************************************************************************/
	private IModelListener master;
	private EnumMap<DisciplineType, Integer> disCodes;
}
