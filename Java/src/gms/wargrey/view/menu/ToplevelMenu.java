package gms.wargrey.view.menu;

import gms.wargrey.view.AbstractMenu;
import gms.wargrey.view.IMenuListener;
import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public class ToplevelMenu extends AbstractMenu {
	@Override
	public void on_menu_char(IMenuListener master, MenuType self, int ch) {
		switch (ch) {
		case 0: master.on_menu_task(self, MenuTask.Exit); break;
		case 1: master.on_menu_task(self, MenuTask.ImportData); break;
		case 2: master.on_menu_task(self, MenuTask.ExportData); break;
		case 3: master.on_menu_switch(self, MenuType.ClassManagement); break;
		case 4: master.on_menu_switch(self, MenuType.DisciplineManagement); break;
		case 5: master.on_menu_switch(self, MenuType.StudentManagement); break;
		case 6: master.on_menu_switch(self, MenuType.GradeManagement); break;
		}
	}
	
	@Override
	protected String[] getMenuItems() {
		return this.items;
	}
	
	/*********************************************************************************************/
	private String items [] = {
			"Exit",
	        "Import Data", "Export Data",
	        "Class Management", "Discipline Management", "Student Management", "Grade Management"
	};
}
