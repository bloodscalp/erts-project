import gui.*;

import java.awt.*;
import javax.swing.*;

import controller.*;


public class Tampomat extends JFrame {

	private static final long serialVersionUID = 1L;
	private Commands commands;
	private Sensors  sensors;
	private Outputs  outputs;

	private Controller ctrl;

	public Tampomat(String port) {	
		outputs  = new Outputs();
		getContentPane().add(outputs, BorderLayout.EAST);

		ctrl = new Controller(outputs, port);

		commands = new Commands(ctrl);
		getContentPane().add(commands, BorderLayout.WEST);

		sensors  = new Sensors(ctrl);
		getContentPane().add(sensors, BorderLayout.CENTER);

		// Parametres pour la fenetre
		this.pack();
		setTitle("Tampomat");
		setVisible(true);
		setResizable(false);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main (String[] args) {
		String port = new String("");
		
		if (args.length == 0) {
			System.out.println("Argument [portCOM] manquant, e.g. COM1 !");
			System.out.println("Rappel : java Tempomat [portCOM]");
			System.exit(1);
		}
		else if (args.length > 1) {
			System.out.println("Trop d'arguments !");
			System.out.println("Rappel : java Tempomat [portCOM]");
			System.exit(1);
		}
		else {
			port = args[0];
		}

		new Tampomat(port);
	}
}
