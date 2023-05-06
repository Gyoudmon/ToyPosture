package gms.wargrey.view;

public abstract class AbstractMenu {
	public abstract void on_menu_char(IMenuListener master, MenuType self, int ch);
	
	protected abstract String [] getMenuItems();
	
	/*********************************************************************************************/
	public int count() {
		return this.getMenuItems().length;
	}
	
	public void display() {
		String [] items = this.getMenuItems();
		
		for (int idx = 0; idx < items.length; idx ++) {
			System.out.printf("%d. %s\n", idx, items[idx]);
		}
	}
}
