package gui;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.*;

import controller.*;

public class Commands extends JPanel implements ActionListener {

	private static final long serialVersionUID = 1L;
	private JButton onButton;
	private JButton offButton;
	private JButton resButton;
	private JButton setButton;
	
	private Controller ctrl;
	
	public Commands (Controller ctrl) {
		this.ctrl = ctrl;
		
		GridLayout layout = new GridLayout(2, 2, 5, 5);

		this.setBorder(new TitledBorder(new EtchedBorder(), "Commands"));
		this.setLayout(layout);
		
		onButton  = new JButton(CONSTANTS.STATUS_ON);
		this.add(onButton);
		
		offButton = new JButton(CONSTANTS.STATUS_OFF);
		this.add(offButton);
		
		resButton = new JButton("RES");
		this.add(resButton);
		
		setButton = new JButton("SET");
		this.add(setButton);
		
		// Ecouteurs
        onButton .addActionListener((ActionListener) this);
        offButton.addActionListener((ActionListener) this);
        resButton.addActionListener((ActionListener) this);
        setButton.addActionListener((ActionListener) this);
    }
    
    // Actions
    public void actionPerformed (ActionEvent e) {
        JButton b  = (JButton)e.getSource();
        
        if      (b == onButton)
        	ctrl.sendMessage(CONSTANTS.cmdON, 0);
        else if (b == offButton)
        	ctrl.sendMessage(CONSTANTS.cmdOFF, 0);
        else if (b == resButton)
        	ctrl.sendMessage(CONSTANTS.cmdRES, 0);
        else if (b == setButton)
        	ctrl.sendMessage(CONSTANTS.cmdSET, 0);
    }
}
