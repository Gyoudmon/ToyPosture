package gydm.wargrey;

import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;

import java.awt.Component;

import javax.swing.JTextArea;
import javax.swing.JButton;

/*************************************************************************************************/
public class Interaction {
	public Interaction(){
		this.children = new Component[2][];
	}
	
	/*********************************************************************************************/	
	public int getUIRowCount() {
		return this.children.length;
	}
	
	public Component[] getChildren(int index, boolean [] needSeparator) {
		Component[] c = null;
		
		if (index < this.children.length) {
			c = this.children[index];
			
			if (c == null) {
				if (needSeparator.length > 0) {
					needSeparator[0] = false;
				}
				
				switch (index) {
				case 0:
					this.query = new JTextArea();
					this.result = new JTextArea();
					this.children[index] = new Component[] { this.query, this.result };
					c = this.children[index];
					
					if (needSeparator.length > 0) {
						needSeparator[0] = true;
					}
					break;
				case 1:
					this.clear = new JButton("Clear");
					this.submit = new JButton("Submit");
					this.children[index] = new Component[] { this.clear, this.submit };
					c = this.children[index];
					break;
				}
			}
		}
		
		return c;
	}
	
	/*********************************************************************************************/
	public JMenuBar getMenuBar() {
		if (this.menubar == null) {
			JMenu dbmenu = new JMenu("Database");
			JMenuItem connecting = new JMenuItem("Connect to...");
		
			this.menubar = new JMenuBar();
			this.menubar.add(dbmenu);
			dbmenu.add(connecting);
		}
		
		return this.menubar;
	}
	
	/*********************************************************************************************/
	private JMenuBar menubar = null;
	private Component[][] children = null;
	
	private JTextArea query;
	private JTextArea result;
	
	private JButton clear;
	private JButton submit;
}
