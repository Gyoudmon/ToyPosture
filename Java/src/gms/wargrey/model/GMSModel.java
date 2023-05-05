package gms.wargrey.model;

import gms.wargrey.model.*;
import gms.wargrey.model.entity.*;

import java.io.File;
import java.io.FileInputStream;
import java.io.PrintStream;

import java.util.Scanner;
import java.util.TreeMap;

public class GMSModel {
	public GMSModel(IModelListener master) {
		this.master = master;

		this.classes = new TreeMap<Long, ClassEntity>();
	}

	/*********************************************************************************************/
	public void import_from_file(String gmsin) {
		File gmsPath = new File(gmsin);
		
		if (gmsPath.exists() && gmsPath.isFile()) {
			try {
				Scanner parser = new Scanner(new FileInputStream(gmsPath));
				int [] offset = { 0 };
			
				while (parser.hasNext()) {
					try {
						String line = parser.nextLine();
					
						if (ClassEntity.match(line, offset)) {
							this.register_class(new ClassEntity(line, offset[0]));
						}
					} catch (Exception e) {
						System.err.println(e.getMessage());
					}
				}

				parser.close();
			} catch (Exception e) {
				System.err.println(e.getMessage());
			}
		}
	}
	
	public void export_to_file(String gmsout) throws Exception {
		if (!gmsout.isBlank()) {
			File gmsPath = new File(gmsout);
			PrintStream ps = new PrintStream(gmsPath);
			
			this.export_to_print_stream(ps);
			ps.close();
		} else {
			this.export_to_print_stream(System.out);
		}
	}
	
	/*********************************************************************************************/
	public void createClassFromUser(String line) throws Exception {
		this.register_class(new ClassEntity(line, 0));
	}
	
    public void deleteClassByUser(String line) throws Exception {
    	long clsId = Integer.parseInt(line);
    	
    	if (this.classes.containsKey(clsId)) {
    		ClassEntity cls = this.classes.remove(clsId);
    		this.master.on_class_delete(clsId, cls);
    	} else {
    		throw new Exception(String.format("Invalid class(%d) or not found", clsId));
    	}
    	
    }

	/*********************************************************************************************/
    private void export_to_print_stream(PrintStream gmsout) throws Exception {
		for (ClassEntity cls : this.classes.values()) {
			gmsout.println(cls.toString());
		}
	}
    
    private void register_class(ClassEntity cls) throws Exception {
		long clsId = cls.primaryKey();
		
		if (!this.classes.containsKey(clsId)) {
			this.classes.put(clsId, cls);
			this.master.on_class_create(clsId, cls);
		} else {
			throw new Exception(String.format("Class(%d) has already been existed", clsId));
		}
	}
	
	/*********************************************************************************************/
	private TreeMap<Long, ClassEntity> classes;

	/*********************************************************************************************/
	private IModelListener master;
}
