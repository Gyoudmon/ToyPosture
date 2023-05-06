package gms.wargrey.view;

import gms.wargrey.view.MenuTask;
import gms.wargrey.view.MenuType;

public interface IMenuListener {
	public void onMenuSwitch(MenuType self, MenuType to);
	public void onMenuTask(MenuType self, MenuTask task);
}
