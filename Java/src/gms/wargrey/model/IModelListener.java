package gms.wargrey.model;

import gms.wargrey.model.entity.*;

public interface IModelListener {
	public void onClassCreated(int clsId, ClassEntity cls);
	public void onClassDeleted(int clsId, ClassEntity cls);
	
	public void onDisciplineCreated(int disCode, DisciplineEntity dis);
	public void onDisciplineUpdated(int disCode, DisciplineEntity dis);
	public void onDisciplineDeleted(int disCode, DisciplineEntity dis);

	public void onStudentCreated(int sNo, StudentEntity stu);
	public void onStudentUpdated(int sNo, StudentEntity stu);
	public void onStudentDeleted(int sNo, StudentEntity stu);
	
	public void onGradeCreated(int sNo, GradeEntity stu);
	public void onGradeUpdated(int sNo, GradeEntity stu);
	public void onGradeDeleted(int sNo, GradeEntity stu);
}
