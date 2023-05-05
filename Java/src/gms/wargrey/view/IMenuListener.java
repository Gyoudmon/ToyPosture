package gms.wargrey.view;

import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public interface IMenuListener {
	public void on_menu_switch(MenuType self, MenuType to);
	public void on_menu_task(MenuType self, MenuTask task);
}
