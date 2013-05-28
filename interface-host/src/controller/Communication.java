package controller;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.concurrent.Semaphore;

public class Communication implements SerialPortEventListener {
	final int BUFFER_SIZE = 10;
	
	private Controller ctrl;
	
	private String[] buffer;
	
	private Semaphore mutex;
	private Semaphore empty;
	private Semaphore full;

	private int in;
	private int out;
	
	// Pour indiquer si des donnees sont pretes a etres recues
	private Boolean flag;
	
	private final Object verrou = new Object();
	
	public Communication (Controller ctrl) {
		this.ctrl = ctrl;
		
		buffer = new String[BUFFER_SIZE];
		
		mutex = new Semaphore(1);
		empty  = new Semaphore(0);
		full = new Semaphore(BUFFER_SIZE);
		
		in = 0;
		out = 0;
		
		flag = false;
	}
	       
	// Connexion au port serie
    public void connect (String portName) throws Exception {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
         
        if (portIdentifier.isCurrentlyOwned()) {
            System.out.println("Error: Port " + portName + " is busy");
        }
        else {
            CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);
            
            if (commPort instanceof SerialPort) {
                SerialPort serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200,
                							   SerialPort.DATABITS_8,
                							   SerialPort.STOPBITS_1,
                							   SerialPort.PARITY_NONE);
                
                final InputStream in = serialPort.getInputStream();
                final OutputStream out = serialPort.getOutputStream();
                
                //pour recevoir une info quand les donnees sont pretes
                serialPort.notifyOnDataAvailable(true);
                // ajout d'un ecouteur sur "serialPort"
                serialPort.addEventListener(this);
                
                // Tache receptionant les donnees
                Thread readerTask = new Thread(new Runnable() { 
                	public void run() { 
                		byte[] buffer = new byte[1024];
                		int len = -1;
						String newStr = null;
                		try {
                			while(true){
                				// n'essayer de recevoir la reponse que si les donnees sont pretes
                				if(flag && (len = in.read(buffer)) > -1){
									newStr = new String(buffer,0,len);
									ctrl.receiveResponse(newStr);
									synchronized(verrou) {
										flag = false;
									}
                				}
								try {
									Thread.sleep(90);
								} catch (InterruptedException e) {
									e.printStackTrace();
								}
                			}
                		}
                		catch (IOException e) {
                			e.printStackTrace();
                		} 
                	}
                });
                
                // Tache emettant les donnees
                Thread writerTask = new Thread(new Runnable() {
                	public void run () {
                        try {                
                            while (true) {
                                out.write(extract().getBytes());
                            }                
                        }
                        catch (IOException e) {
                            e.printStackTrace();
                        }            
                    }
                });
                
                readerTask.start();
                writerTask.start();

            }
            else {
                System.out.println("Error: Only serial ports");
            }
        }     
    }
    
    // Ajoute des donnees au tampon
    private void add (String newElement) {
    	try {
			full.acquire();
			
			mutex.acquire();
				buffer[in] = newElement;
				in = (in+1) % BUFFER_SIZE;
			mutex.release();
			
			empty.release();
    	
    	} 
    	catch (InterruptedException e) {
			e.printStackTrace();
		}
    	
    }
    
    // Supprime des donnees du tampon
    private String extract() {
    	String extractedElement = new String("ERROR");
    	
    	try {
			empty.acquire();
    	
			mutex.acquire();
            	extractedElement = buffer[out];
            	out = (out+1) % BUFFER_SIZE;
            mutex.release();
        
            full.release();
    	} 
    	catch (InterruptedException e) {
			e.printStackTrace();
		}
    	
    	return extractedElement;  	
    }
    
    public void send(String command) {
    	add(command);
    }

	// Surcharge de la methode due a l'implementation de SerialPortEventListener.
    // Methode lancee automatiquement lorsque des donnees sont pretes a etres recues.
    // Change la valeur de l'indicateur "flag" pour que le thread de reception se rende
    // compte qu'il peut lire les donnees du buffer.
	public void serialEvent(SerialPortEvent arg0) {
		synchronized(verrou) {
			flag = true;
			}
	}
}