package gms.wargrey.view.menu;

import gms.wargrey.view.AbstractMenu;
import gms.wargrey.view.IMenuListener;
import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public class GradeMenu extends AbstractMenu {
	@Override
	public void on_menu_char(IMenuListener master, MenuType self, int ch) {
		switch (ch) {
		case 0: master.on_menu_switch(self, MenuType.TopLevel); break;
		case 1: master.on_menu_task(self, MenuTask.CreateStudent); break;
		case 2: master.on_menu_task(self, MenuTask.UpdateStudent); break;
		case 3: master.on_menu_task(self, MenuTask.DeleteStudent); break;
		case 4: master.on_menu_task(self, MenuTask.BindClass); break;
		case 5: master.on_menu_task(self, MenuTask.ClearStudent); break;
		}
	}
	
	@Override
	protected String[] getMenuItems() {
		return this.items;
	}

	/*********************************************************************************************/
	private String items [] = {
			"Return to Toplevel",
	        "Create Grade", "Update Grade", "Delete Grade",
	        "Clear Detached Grades"
	};
}
