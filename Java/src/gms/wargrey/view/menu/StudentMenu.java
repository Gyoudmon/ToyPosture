package gms.wargrey.view.menu;

import gms.wargrey.view.AbstractMenu;
import gms.wargrey.view.IMenuListener;
import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public class StudentMenu extends AbstractMenu {
	@Override
	public void on_menu_char(IMenuListener master, MenuType self, int ch) {
		switch (ch) {
		case 0: master.onMenuSwitch(self, MenuType.TopLevel); break;
		case 1: master.onMenuTask(self, MenuTask.CreateStudent); break;
		case 2: master.onMenuTask(self, MenuTask.UpdateStudent); break;
		case 3: master.onMenuTask(self, MenuTask.DeleteStudent); break;
		}
	}
	
	@Override
	protected String[] getMenuItems() {
		return this.items;
	}

	/*********************************************************************************************/
	private String items [] = {
			"Return",
	        "Create Student", "Update Student", "Delete Student"
	};
}
