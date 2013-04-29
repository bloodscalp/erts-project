package gui;

import java.awt.BorderLayout;
import java.awt.GridLayout;

import javax.swing.*;
import javax.swing.border.*;

import controller.*;

public class Outputs extends JPanel {
	
	private static final long serialVersionUID = 1L;
	private JTextField speedField;
	private JTextField throttleField;
	private JTextField statusField;
	
	public Outputs () {		
		GridLayout layout = new GridLayout(3, 1, 5, 5);
		
		this.setBorder(new TitledBorder(new EtchedBorder(), "Outputs"));
		this.setLayout(layout);
		
		speedField = new JTextField("0", 4);
		speedField.setEditable(false);
		JPanel speedStuff = new JPanel();
		speedStuff.add(new JLabel("Speed:    "), BorderLayout.NORTH);
		speedStuff.add(speedField, BorderLayout.SOUTH);
		this.add(speedStuff, BorderLayout.NORTH);
		
		throttleField = new JTextField("0", 4);
		throttleField.setEditable(false);
		JPanel throttleStuff = new JPanel();
		throttleStuff.add(new JLabel("Throttle: "), BorderLayout.NORTH);
		throttleStuff.add(throttleField, BorderLayout.SOUTH);
		this.add(throttleStuff, BorderLayout.CENTER);
		
		statusField = new JTextField("OFF",4);
		statusField.setEditable(false);
		JPanel statusStuff = new JPanel();
		statusStuff.add(new JLabel("Status:   "), BorderLayout.NORTH);
		statusStuff.add(statusField, BorderLayout.SOUTH);
		this.add(statusStuff, BorderLayout.SOUTH);
	}
	
	/* la valeur doit etre comprise entre 0 et 250 (sujet a modification...) */
	public void setSpeed (int speed) {
		/*
		if (speed < 0)
			speed = 0;
		else if (speed > 250)
			speed = 250;
		*/
		speedField.setText(((Integer)speed).toString());
	}
	
	/* la valeur doit etre comprise entre 0 et 100 (sujet a modification...) */
	public void setThorttle (int thorttle) {
		/*
		if (thorttle < 0)
			thorttle = 0;
		else if (thorttle > 100)
			thorttle = 100;
		*/
		throttleField.setText(((Integer)thorttle).toString());
	}
	
	public void setStatus (String status) {
		if (status.equalsIgnoreCase(CONSTANTS.STATUS_ON))
			statusField.setText(CONSTANTS.STATUS_ON);
		else if (status.equalsIgnoreCase(CONSTANTS.STATUS_OFF))
			statusField.setText(CONSTANTS.STATUS_OFF);
		else if (status.equalsIgnoreCase(CONSTANTS.STATUS_STDBY))
			statusField.setText(CONSTANTS.STATUS_STDBY);
		else if (status.equalsIgnoreCase(CONSTANTS.STATUS_INT))
			statusField.setText(CONSTANTS.STATUS_INT);
	}
}


