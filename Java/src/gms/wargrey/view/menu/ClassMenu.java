package gms.wargrey.view.menu;

import gms.wargrey.view.AbstractMenu;
import gms.wargrey.view.IMenuListener;
import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public class ClassMenu extends AbstractMenu {
	@Override
	public void on_menu_char(IMenuListener master, MenuType self, int ch) {
		switch (ch) {
		case 0: master.onMenuSwitch(self, MenuType.TopLevel); break;
		case 1: master.onMenuTask(self, MenuTask.CreateClass); break;
		case 2: master.onMenuTask(self, MenuTask.DeleteClass); break;
		}
	}
	
	@Override
	protected String[] getMenuItems() {
		return this.items;
	}

	/*********************************************************************************************/
	private String items [] = {
			"Return",
	        "Create Class", "Delete Class"
	};
}
