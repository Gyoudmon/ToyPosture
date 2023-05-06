package gms.wargrey;

import gms.wargrey.view.*;
import gms.wargrey.view.menu.*;

import gms.wargrey.model.*;
import gms.wargrey.model.entity.*;

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
			System.err.println(e.getMessage() + ": " + e.getMessage());
		}
	}
	
	/*********************************************************************************************/
	@Override public void on_menu_switch(MenuType self, MenuType to) {
		if (this.menus.containsKey(to)) {
			this.theMenu = to;
			this.theTask = MenuTask.Idle;
		} else {
			System.err.printf("no such menu: %s\n", to.toString());
		}
	}

	@Override public void on_menu_task(MenuType self, MenuTask task) {
		this.theTask = task;
		
		try {
			switch (task) {
			case CreateClass: this.model.createClassFromUser(this.readUsrLine("please input the class record: ", ClassEntity.prompt())); break;
			case DeleteClass: this.model.deleteClassByUser(this.readUsrLine("please input the class identity: ")); break;
			case CreateDiscipline: this.model.createDisciplineFromUser(this.readUsrLine("please input the discipline record: ", DisciplineEntity.prompt())); break;
			case UpdateDiscipline: this.model.updateDisciplineFromUser(this.readUsrLine("please input the discipline info: ", DisciplineEntity.update_prompt())); break;
			case DeleteDiscipline: this.model.deleteDisciplineByUser(this.readUsrLine("please input the discipline code: ")); break;
			case CreateStudent: this.model.createStudentFromUser(this.readUsrLine("please input the student record: ", StudentEntity.prompt())); break;
			case UpdateStudent: this.model.updateStudentFromUser(this.readUsrLine("please input the student info: ", StudentEntity.update_prompt())); break;
			case DeleteStudent: this.model.deleteStudentByUser(this.readUsrLine("please input the student No.: ")); break;
			case ClearStudent: break;
			case CreateGrade: break;
			case DeleteGrade: break;
			case UpdateGrade: break;
			case ClearGrade: break;
			case ImportData: this.model.importFromFile(gmsin); break;
			case ExportData: this.model.exportToFile(gmsout); break;
			default: /* do nothing */ break;
			}
		} catch (Exception e) {
			System.err.println(e.getClass().getName() + ": " + e.getMessage());
		}
	}
	
	/*********************************************************************************************/
	@Override public void onClassCreated(int clsId, ClassEntity cls) {
		System.out.printf("! found a new class %d\n", clsId);
	}

	@Override public void onClassDeleted(int clsId, ClassEntity cls) {
		System.out.printf("! Class(%d) disappeared\n", clsId);
	}
	
	@Override public void onDisciplineCreated(int disCode, DisciplineEntity dis) {
		System.out.printf("! found a new discipline %s(%d)\n", dis.getType(), disCode);
	}

	@Override public void onDisciplineUpdated(int disCode, DisciplineEntity dis) {
		System.out.printf("! Discipline(%s) changed its credit to %d\n", dis.getType(), dis.getCredit());
	}
	
	@Override public void onDisciplineDeleted(int disCode, DisciplineEntity dis) {
		System.out.printf("! Discipline(%s) disappeared\n", dis.getType());
	}
	
	@Override public void onStudentCreated(int sNo, StudentEntity stu) {
		System.out.printf("! found a new student %s(%d)\n", stu.getNickName(), sNo);
	}

	@Override public void onStudentUpdated(int sNo, StudentEntity stu) {
		System.out.printf("! Student(%s) changed %s (class, age) to (%d, %d)\n",
				stu.getNickName(),
				stu.getGender() == StudentGender.Girl ? "her" : "his",
				stu.getClassIdentity(), stu.getAge());
	}
	
	@Override public void onStudentDeleted(int sNo, StudentEntity stu) {
		System.out.printf("! Student(%s) disappeared\n", stu.getNickName());
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
		
		while (this.theTask != MenuTask.Exit) {
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
				System.out.print("$ record format: ");
				System.out.println(format);
			}
			
			System.out.print(prompt);
			line = this.stdin.nextLine().trim();
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
