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
	@Override public void onMenuSwitch(MenuType self, MenuType to) {
		if (this.menus.containsKey(to)) {
			this.theMenu = to;
			this.theTask = MenuTask.Idle;
		} else {
			System.err.printf("no such menu: %s\n", to.toString());
		}
	}

	@Override public void onMenuTask(MenuType self, MenuTask task) {
		this.theTask = task;
		
		try {
			switch (task) {
			/* BEGIN Entity Manipulation */
			case CreateClass:
				this.model.createClassFromUser(
					this.readUsrLine("please input the class record: ",
										ClassEntity.prompt()));
				break;
			case DeleteClass:
				this.model.deleteClassByUser(
					this.readUsrLine("please input the class identity: "));
				break;
			case CreateDiscipline:
				this.model.createDisciplineFromUser(
					this.readUsrLine("please input the discipline record: ",
										DisciplineEntity.prompt()));
				break;
			case UpdateDiscipline:
				this.model.updateDisciplineFromUser(
					this.readUsrLine("please input the discipline info: ",
										DisciplineEntity.update_prompt()));
				break;
			case DeleteDiscipline:
				this.model.deleteDisciplineByUser(
					this.readUsrLine("please input the discipline code: "));
				break;
			case CreateStudent:
				this.model.createStudentFromUser(
					this.readUsrLine("please input the student record: ",
										StudentEntity.prompt()));
				break;
			case UpdateStudent:
				this.model.updateStudentFromUser(
					this.readUsrLine("please input the student info: ",
										StudentEntity.update_prompt()));
				break;
			case DeleteStudent:
				this.model.deleteStudentByUser(
					this.readUsrLine("please input the student No.: "));
				break;
			/* END Entity Manipulation */
			case CreateGrade: case DeleteGrade: case UpdateGrade: case ShowReport: this.on_grade_task(); break;
			case ImportData: this.model.importFromFile(gmsin); break;
			case ExportData: this.model.exportToFile(gmsout); break;
			default: /* do nothing */ break;
			}
		} catch (Exception e) {
			System.err.println(e.getClass().getName() + ": " + e.getMessage());
		}
	}
	
	public void on_grade_task() {
		int sNo = this.theSNo;
		int ts = this.theTimestamp;
		
		this.displayBriefStudentSet("> Existed Students", 8);
		
		if (sNo <= 0) {
			sNo = this.readIntegerFromUser("please choose a student by No.: ");
		} else {
			sNo = this.readIntegerFromUser(String.format("Current student is %d, press ENTER to continue or choose another one: ", sNo), sNo); 
		}
		

		this.displayGradeTimestamps("> Registered Timestamps", 8);
		
		if (ts <= 0) {
			ts = this.readIntegerFromUser("please input the timestamp(YYYYMMDD): ");
		} else {
			ts = this.readIntegerFromUser(String.format("Current timestamp is %d, press ENTER to continue or choose another one: ", ts), ts); 
		}
		
		this.theSNo = sNo;
		this.theTimestamp = ts;
		
		if (this.theTask == MenuTask.ShowReport) {
			this.displayGradeReport(this.theSNo, this.theTimestamp);
		} else {	
			int disCode = this.readIntegerFromUser("please input the discipline code: ");
		
			if (this.theTask == MenuTask.DeleteGrade) {
				this.model.deleteStudentScoresByUser(sNo, disCode, ts);
			} else {
				String line = this.readUsrLine("please input the points, separated by SPACE: ");
			
				if (this.theTask == MenuTask.CreateGrade) {
					this.model.registerStudentScoresFromUser(sNo, disCode, ts, line);
				} else {
					this.model.updateStudentScoresFromUser(sNo, disCode, ts, line);
					this.displayGradeReport(this.theSNo, this.theTimestamp);
				}
			}
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
	
	@Override public void onGradeCreated(int sNo, GradeEntity grade) {
		System.out.printf("! found a new grade(%d@%d) for student(%d)\n",
				grade.getDisciplineCode(), grade.getTimestamp(), sNo);
	}

	@Override public void onGradeUpdated(int sNo, GradeEntity grade) {
		int disCode = grade.getDisciplineCode();
		int ts = grade.getTimestamp();
		
		System.out.printf("! Student(%d) gets a new point(%f) for grade(%d, %d)\n",
				sNo, this.model.getStudentScore(sNo, disCode, ts), disCode, ts);
	}
	
	@Override public void onGradeDeleted(int sNo, GradeEntity grade) {
		System.out.printf("! Grade(%d@%d) for student(%d) disappeared\n",
				grade.getDisciplineCode(), grade.getTimestamp(), sNo);
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
			this.onMenuTask(this.theMenu, MenuTask.ImportData);
		}
		
		while (this.theTask != MenuTask.Exit) {
			AbstractMenu m = this.menus.get(this.theMenu);
			
			System.out.println("************************************");
			m.display();
			System.out.println("************************************");
			
			if (this.theMenu != MenuType.TopLevel) {
				switch (this.theMenu) {
				case ClassManagement: this.displayClassSet("> Existed Classes:", 8); break;
				case DisciplineManagement: this.displayDisciplineSet("> Existed Discipline:", 8); break;
				case StudentManagement: this.displayStudentSet("> Existed Students", 16); break;
				case GradeManagement: this.displayGradeTimestamps("> Registered Timestamps", 8); break;
				default: /* do nothing */ break;
				}
			
				System.out.println("....................................");
			}
			
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
	
	private int readIntegerFromUser(String prompt) {
		return Integer.parseUnsignedInt(this.readUsrLine(prompt, ""));
	}
	
	private int readIntegerFromUser(String prompt, int fallback) {
		try {
			return readIntegerFromUser(prompt);
		} catch (NumberFormatException nfe) {
			return fallback;
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
	private void displayClassSet(String prompt, int column) {
		ClassEntity [] classes = this.model.getAllClasses();
		int lastIdx = classes.length - 1;
		
		if (lastIdx >= 0) {
			System.out.println(prompt);
			
			for (int idx = 0; idx <= lastIdx; idx ++) {
				System.out.print(classes[idx].hashCode());
				this.displaySeparator(idx, column, lastIdx);
			}
		}
	}
	
	private void displayDisciplineSet(String prompt, int column) {
		DisciplineEntity [] disciplines = this.model.getAllDisciplines();
		int lastIdx = disciplines.length - 1;
		
		if (lastIdx >= 0) {
			System.out.println(prompt);
			
			for (int idx = 0; idx <= lastIdx; idx ++) {
				System.out.printf("%s[%d]",
					disciplines[idx].getType().name(),
					disciplines[idx].hashCode());
				this.displaySeparator(idx, column, lastIdx);
			}
		}
	}
	
	private void displayBriefStudentSet(String prompt, int column) {
		StudentEntity [] students = this.model.getAllStudents();
		int lastIdx = students.length - 1;
		
		if (lastIdx >= 0) {
			System.out.println(prompt);
			
			for (int idx = 0; idx < students.length; idx ++) {
				System.out.printf("%s[%d]",
						students[idx].getNickName(),
						students[idx].hashCode());			
				this.displaySeparator(idx, column, lastIdx);
			}
		}
	}
	
	private void displayGradeTimestamps(String prompt, int column) {
		TreeSet<Integer> tss = this.model.getAllGradeTimeStamps();
		int lastIdx = tss.size() - 1;
		
		if (lastIdx >= 0) {
			int idx = 0;
			
			System.out.println(prompt);
			
			for (Integer ts : tss) {
				System.out.printf("%d", ts);			
				this.displaySeparator(idx++, column, lastIdx);
			}
		}
	}
	
	private void displayStudentSet(String prompt, int row) {
		StudentEntity [] students = this.model.getAllStudents();
		
		if (students.length >= 0) {
			System.out.println(prompt);
			
			for (int idx = 0; idx < students.length; idx ++) {
				System.out.printf("%s[%d]@%d, %s, %d\n",
						students[idx].getNickName(), students[idx].hashCode(),
						students[idx].getClassIdentity(),
						students[idx].getGender(), students[idx].getAge());
				
				if ((idx + 1) % row == 0) {
					this.readUsrLine("Press ENTER to continue");
				}
			}
		}
	}
	
	private void displayGradeReport(int sNo, int timestamp) {
		DisciplineEntity [] disciplines = this.model.getAllDisciplines();
		String name = this.model.getStudentName(sNo);
		double total = 0;
		
		System.out.println("============== GRADE REPORT ==============");
		this.displayReportLine(String.format("%s[%d]", name, sNo), String.valueOf(timestamp), 42);
		System.out.println("------------------------------------------");
		
		for (DisciplineEntity dis : disciplines) {
			double points = this.model.getStudentScore(sNo, dis.hashCode(), timestamp);
			
			if (points >= 0.0) {
				total += points;
				this.displayReportLine(dis.getType().name(), String.valueOf(points), 42);
			} else {
				this.displayReportLine(dis.getType().name(), "-", 42);
			}
		}
		
		System.out.println("------------------------------------------");
		this.displayReportLine("TOTAL", String.valueOf(total), 42);
		System.out.println("==========================================");
	}
	
	private void displaySeparator(int idx, int column, int lastIdx) {
		if (((idx + 1) % column == 0) || (idx == lastIdx)) {
			System.out.print("\n");
		} else {
			System.out.print(' ');
		}
	}
	
	private void displayReportLine(String left, String right, int column) {
		System.out.print(left);
		System.out.print(" ".repeat(column - left.length() - right.length()));
		System.out.println(right);
	}
	
	/*********************************************************************************************/
	private EnumMap<MenuType, AbstractMenu> menus;
	private MenuTask theTask = MenuTask.Idle;
	private MenuType theMenu = MenuType.TopLevel;
	private GMSModel model = null;
	
	private int theSNo = 0;
	private int theTimestamp = 0;
	
	private Scanner stdin = null;
	private String gmsin = "";
	private String gmsout = "";
}
