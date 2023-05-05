package gms.wargrey.model;

import gms.wargrey.model.entity.*;

public interface IModelListener {
	public void onClassCreated(int clsId, ClassEntity cls);
	public void onClassDeleted(int clsId, ClassEntity cls);
	
	public void onDisciplineCreated(int disCode, DisciplineEntity dis);
	public void onDisciplineUpdated(int disCode, DisciplineEntity dis);
	public void onDisciplineDeleted(int disCode, DisciplineEntity dis);
}
