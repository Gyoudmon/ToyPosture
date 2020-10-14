package gydm.wargrey;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import javax.swing.JFrame;

import java.io.IOException;
import java.io.FileNotFoundException;

/*************************************************************************************************/
public class DBMS {
	private static JFrame self;

	public static void main(String[] args) {
		try {
			File rootdir = DBMS.selectRootDirectory();
			File stonedir = DBMS.mkdir(rootdir, "stone");
			File self_gui = DBMS.buildPath(stonedir, "frame");
		
			if (self_gui.isFile()) {
				DBMS.self = DBMS.readFrame(self_gui);
				System.out.println("JFrame has been loaded from " + self_gui.getAbsolutePath());
			} else {
				DBMS.self = new JFrame("Java DBMS");
				DBMS.self.setLocationRelativeTo(null);
				DBMS.self.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				DBMS.self.setExtendedState(DBMS.self.getExtendedState() | JFrame.MAXIMIZED_BOTH);
				
				DBMS.writeFrame(self, self_gui);
				System.out.println("New constructed JFrame has been write to " + self_gui.getAbsolutePath());
			}
		} catch (Exception e) {
			DBMS.self = null;
			System.err.print(e.getMessage());
		}
		
		if (DBMS.self != null) {
			self.setVisible(true);
		}
	}

	/*********************************************************************************************/
	private static JFrame readFrame(File src) throws IOException, FileNotFoundException, ClassNotFoundException {
		FileInputStream stdin = new FileInputStream(src);
		ObjectInputStream objin = new ObjectInputStream(stdin);
		JFrame bean = (JFrame)objin.readObject();
		
		objin.close();
		stdin.close();
		
		return bean;
	}
	
	private static void writeFrame(JFrame bean, File dest) throws IOException {
		FileOutputStream stdout = new FileOutputStream(dest);
		ObjectOutputStream objout = new ObjectOutputStream(stdout);

		objout.writeObject(bean);
		
		objout.close();
		stdout.close();
	}
	
	/*********************************************************************************************/
	private static File selectRootDirectory() {
		String tempdir = System.getProperty("java.io.tmpdir");
		File rootdir = new File(tempdir + File.separator + "gydm.wargrey.dbms");
		
		if (!rootdir.isDirectory()) {
			rootdir.mkdirs();
		}
		
		return rootdir;
	}
	
	private static File mkdir(File rootdir, String name) {
		File dir = new File(rootdir.getAbsolutePath() + File.separator + name);
		
		if (!dir.isDirectory()) {
			dir.mkdirs();
		}
		
		return dir;
	}
	
	private static File buildPath(File dir, String name) {
		return DBMS.buildPath(dir, name, ".gui");
	}
	
	private static File buildPath(File dir, String name, String ext) {
		return new File(dir.getAbsolutePath() + File.separator + name + ext);
	}
}
