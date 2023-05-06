package gms.wargrey.view.menu;

import gms.wargrey.view.AbstractMenu;
import gms.wargrey.view.IMenuListener;
import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public class GradeMenu extends AbstractMenu {
	@Override
	public void on_menu_char(IMenuListener master, MenuType self, int ch) {
		switch (ch) {
		case 0: master.onMenuSwitch(self, MenuType.TopLevel); break;
		case 1: master.onMenuTask(self, MenuTask.CreateGrade); break;
		case 2: master.onMenuTask(self, MenuTask.UpdateGrade); break;
		case 3: master.onMenuTask(self, MenuTask.DeleteGrade); break;
		case 4: master.onMenuTask(self, MenuTask.ShowReport); break;
		}
	}
	
	@Override
	protected String[] getMenuItems() {
		return this.items;
	}

	/*********************************************************************************************/
	private String items [] = {
			"Return",
	        "Create Grade", "Update Grade", "Delete Grade",
	        "Show Report"
	};
}
