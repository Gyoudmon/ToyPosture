package gms.wargrey.model;

import gms.wargrey.model.entity.*;

public interface IModelListener {
	public void on_class_create(long clsId, ClassEntity cls);
	public void on_class_delete(long clsId, ClassEntity cls);
}
