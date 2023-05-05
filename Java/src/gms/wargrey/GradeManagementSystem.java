package gms.wargrey;

import gms.wargrey.view.*;
import gms.wargrey.view.menu.*;

import java.util.*;

public class GradeManagementSystem implements IMenuListener {
	public static void main(String[] args) {
		GradeManagementSystem self = new GradeManagementSystem();
		
		try {
			self.stdin = new Scanner(System.in);
			self.loadMenus();
			self.bigBang();
		} catch (Exception e) {
			System.err.println(e.getMessage());
		}
	}
	
	/*********************************************************************************************/
	@Override
	public void on_menu_switch(MenuType self, MenuType to) {
		if (this.menus.containsKey(to)) {
			this.theMenu = to;
		} else {
			System.err.printf("no such menu: %s\n", to.toString());
		}
	}

	@Override
	public void on_menu_task(MenuType self, MenuTask task) {
		if (this.theTask != task) {
			this.theTask = task;
		}
	}

	/*********************************************************************************************/
	private void loadMenus() {
		this.menus = new EnumMap<>(MenuType.class);

		this.menus.put(MenuType.TopLevel, new ToplevelMenu());
		this.menus.put(MenuType.ClassManagement, new ClassMenu());
		this.menus.put(MenuType.DisciplineManagement, new DisciplineMenu());
		this.menus.put(MenuType.StudentManagement, new StudentMenu());
		this.menus.put(MenuType.GradeManagement, new GradeMenu());
	}
	
	private void bigBang() {
		while(this.theTask != MenuTask.Exit) {
			AbstractMenu m = this.menus.get(this.theMenu);
			
			m.display();
			
			int todo = this.readNumericalCharFromUser("Please input a number, followed by an ENTER: ");
			if ((todo >= 0) && (todo < m.count())) {
				m.on_menu_char(this, this.theMenu, todo);
			} else {
				System.err.println("invalid input");
			}
		}
		
		System.out.println("Bye!");
	}
	
	private int readNumericalCharFromUser(String prompt) {
		String line = this.readLineFromUser(prompt);
		char ch = (line.length() >= 1) ? line.charAt(0) : '\0';
			
		if ((ch >= '0') && (ch <= '9')) {
			return ch - '0';
		} else {
			return -1;
		}
	}
	
	private String readLineFromUser(String prompt) {
		String line = "";
		
		try {
			System.out.print(prompt);
			line = this.stdin.nextLine();
		} catch (Exception e) {}
		
		return line;
	}
	
	/*********************************************************************************************/
	private EnumMap<MenuType, AbstractMenu> menus;
	private MenuTask theTask = MenuTask.Idle;
	private MenuType theMenu = MenuType.TopLevel;
	private Scanner stdin;
}
