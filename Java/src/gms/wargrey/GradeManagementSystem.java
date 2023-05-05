package gms.wargrey;

import gms.wargrey.view.*;
import gms.wargrey.view.menu.*;
import gms.wargrey.model.*;
import gms.wargrey.model.entity.ClassEntity;

import java.util.*;


/*************************************************************************************************/
public class GradeManagementSystem implements IMenuListener, IModelListener {
	public static void main(String[] args) {
		GradeManagementSystem self = new GradeManagementSystem();
		
		if (args.length > 0) {
			self.gmsin = args[0];
			self.gmsout = (args.length > 1) ? args[1] : self.gmsin;
		}
		
		try {
			self.stdin = new Scanner(System.in);
			self.model = new GMSModel(self);
			
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
			this.theTask = MenuTask.Idle;
		} else {
			System.err.printf("no such menu: %s\n", to.toString());
		}
	}

	@Override
	public void on_menu_task(MenuType self, MenuTask task) {
		this.theTask = task;
		
		try {
			switch (task) {
			case ImportData: this.model.import_from_file(gmsin); break;
			case ExportData: this.model.export_to_file(gmsout); break;
			case CreateClass: this.model.createClassFromUser(this.readUsrLine("please input the class record: ", ClassEntity.prompt())); break;
			case DeleteClass: this.model.deleteClassByUser(this.readUsrLine("please input the class ID: ")); break;
			case CreateDiscipline: break;
			case UpdateDiscipline: break;
			case DeleteDiscipline: break;
			case CreateStudent: break;
			case UpdateStudent: break;
			case BindClass: break;
			case DeleteStudent: break;
			case ClearStudent: break;
			case CreateGrade: break;
			case DeleteGrade: break;
			case UpdateGrade: break;
			case ClearGrade: break;
			default: /* do nothing */ break;
			}
		} catch (Exception e) {
			System.err.println(e.getMessage());
		}
	}
	
	/*********************************************************************************************/
	@Override
	public void on_class_create(long clsId, ClassEntity cls) {
		System.out.printf("! found a new class %d\n", clsId);
	}

	@Override
	public void on_class_delete(long clsId, ClassEntity cls) {
		System.out.printf("! Class(%d) disappeared\n", clsId);
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
		if (!this.gmsin.isBlank()) {
			this.on_menu_task(this.theMenu, MenuTask.ImportData);
		}
		
		while(this.theTask != MenuTask.Exit) {
			AbstractMenu m = this.menus.get(this.theMenu);
			
			System.out.println("***********************************");
			m.display();
			System.out.println("***********************************");
			
			int todo = this.readNumericalCharFromUser("Please input a single digit: ");
			if ((todo >= 0) && (todo < m.count())) {
				m.on_menu_char(this, this.theMenu, todo);
			} else {
				System.err.println("invalid input");
			}
		}
		
		System.out.println("Bye!");
	}
	
	/*********************************************************************************************/
	private int readNumericalCharFromUser(String prompt) {
		String line = this.readUsrLine(prompt);
		char ch = (line.length() >= 1) ? line.charAt(0) : '\0';

		if ((ch >= '0') && (ch <= '9')) {
			return ch - '0';
		} else {
			return -1;
		}
	}
	
	private String readUsrLine(String prompt) {
		return this.readUsrLine(prompt, "");
	}
	
	private String readUsrLine(String prompt, String format) {
		String line = "";
		
		try {
			if (!format.isBlank()) {
				System.out.print("format: ");
				System.out.println(format);
			}
			
			System.out.print(prompt);
			line = this.stdin.nextLine();
		} catch (Exception e) {}
		
		return line;
	}
	
	/*********************************************************************************************/
	private EnumMap<MenuType, AbstractMenu> menus;
	private MenuTask theTask = MenuTask.Idle;
	private MenuType theMenu = MenuType.TopLevel;
	private GMSModel model = null;
	
	private Scanner stdin = null;
	private String gmsin = "";
	private String gmsout = "";
}
