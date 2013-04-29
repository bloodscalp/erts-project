package controller;

import gui.*;

public class Controller {
	private Communication comm;
	private Outputs       out;
	
	public Controller (Outputs out, String port) {
		this.out = out;
		
		comm = new Communication(this);
		try {
			comm.connect(port);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void sendMessage (String cmd, int parameter) {
		String param;
		
		System.out.print("Send: ");
		
		if (cmd.equalsIgnoreCase(CONSTANTS.ACC) ||
			cmd.equalsIgnoreCase(CONSTANTS.DEC)) {
			
			if (parameter >= 0 && parameter < 10 )
				param = new String("00"+parameter);
			else if ( parameter >= 10  && parameter < 100)
				param = new String("0"+parameter);
			else
				param = String.valueOf(parameter);
				
			System.out.println(cmd+param);
			comm.send(cmd+param);
		}
			
		else {
			System.out.println(cmd);
			comm.send(cmd);
		}
			
	}
	
	/* Analyse la reponse venue de la cible NTRT et affiche en consequance */
	public void receiveResponse (String resp) {
		String str;
		System.out.println("Receive: " + resp);
		switch (resp.charAt(0)) {
			case CONSTANTS.THROTTLE:
				str = resp.substring(1, resp.length()-2);
				out.setThorttle(Integer.parseInt(str));
				break;
			
			case CONSTANTS.SPEED:
				str = resp.substring(1, resp.length()-2);
				out.setSpeed(Integer.parseInt(str));
				break;
			
			case CONSTANTS.STATUS:
				switch (resp.charAt(1)) {
					case CONSTANTS.respON:
						out.setStatus(CONSTANTS.STATUS_ON);
						break;
					
					case CONSTANTS.respOFF:
						out.setStatus(CONSTANTS.STATUS_OFF);
						break;
						
					case CONSTANTS.respSTDBY:
						out.setStatus(CONSTANTS.STATUS_STDBY);
						break;
						
					case CONSTANTS.respINT:
						out.setStatus(CONSTANTS.STATUS_INT);
						break;
					
					default:
				}
				break;
				
			default:
		}
	}
}
