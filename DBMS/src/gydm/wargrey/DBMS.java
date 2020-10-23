package gydm.wargrey;

import gydm.wargrey.Interaction;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSeparator;

import java.util.Scanner;

import java.awt.Component;
import javax.swing.BoxLayout;

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
				Interaction it = new Interaction();
				int childrenCount = it.getUIRowCount();
				
				DBMS.self = new JFrame("Java DBMS");
				DBMS.self.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				
				DBMS.self.setLocationRelativeTo(null); // centralize
				DBMS.self.setExtendedState(DBMS.self.getExtendedState() | JFrame.MAXIMIZED_BOTH);
				
				DBMS.self.setJMenuBar(it.getMenuBar());
				
				if (childrenCount > 0) {
					JPanel content = new JPanel();
					BoxLayout rlayout = new BoxLayout(content, BoxLayout.X_AXIS);
					boolean separator[] = { false };
					
					content.setLayout(rlayout);
					
					for (int idx = 0; idx < childrenCount; idx ++) {
						Component children[] = it.getChildren(idx, separator);
						
						if (children != null) {
							JPanel subcontent = new JPanel();
							BoxLayout layout = new BoxLayout(subcontent, BoxLayout.Y_AXIS);
						
							subcontent.setLayout(layout);
							subcontent.setAlignmentY(JPanel.TOP_ALIGNMENT);
							content.add(new JSeparator(JSeparator.VERTICAL));
							content.add(subcontent);
							
							for (int jdx = 0; jdx < children.length; jdx ++) {
								if (separator[0]) {
									subcontent.add(new JSeparator(JSeparator.HORIZONTAL));
								}
								
								subcontent.add(children[jdx]);
							}
						}
					}
					
					DBMS.self.setContentPane(content);
				}
				
				// DBMS.writeFrame(self, self_gui);
				System.out.println("New constructed JFrame has been write to " + self_gui.getAbsolutePath());
			}
		} catch (Exception e) {
			DBMS.self = null;
			System.err.print(e.getMessage());
		}
		
		if (DBMS.self != null) {
			//DBMS.self.pack();
			//DBMS.self.setVisible(true);
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
