package gui;

import java.awt.GridLayout;
import java.util.Hashtable;

import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import controller.CONSTANTS;
import controller.Controller;

public class Sensors extends JPanel implements ChangeListener {

	private static final long serialVersionUID = 1L;
	private JSlider accSlider;
	private JSlider decSlider;
	
	private Controller ctrl;
	
	public Sensors (Controller ctrl) {
		this.ctrl = ctrl;
		
		GridLayout layout = new GridLayout(2, 0, 5, 5);
		this.setBorder(new TitledBorder(new EtchedBorder(), "Sensors"));
		this.setLayout(layout);
		
		// Labels (0, 50 and 100)
		Hashtable<Integer, JLabel> labelTable = new Hashtable<Integer, JLabel>();
		labelTable.put(new Integer(0),   new JLabel("0"));
		labelTable.put(new Integer(50),  new JLabel("50"));
		labelTable.put(new Integer(100), new JLabel("100"));
		
		JPanel accStuff = new JPanel();
		accSlider = new JSlider(SwingConstants.HORIZONTAL, 0, 100, 0);
		accSlider.setMajorTickSpacing(10);
		accSlider.setMinorTickSpacing(1);
		accSlider.setPaintTicks(true);
		accSlider.setPaintLabels(true);
		accSlider.setLabelTable(labelTable);
		accSlider.addChangeListener((ChangeListener) this);
		
		accStuff.add(new JLabel("Acceleration:"));
		accStuff.add(accSlider);
		this.add(accStuff);
		
		
		JPanel decStuff = new JPanel();
		decSlider = new JSlider(SwingConstants.HORIZONTAL, 0, 100, 0);
		decSlider.setMajorTickSpacing(10);
		decSlider.setMinorTickSpacing(1);
		decSlider.setPaintTicks(true);
		decSlider.setPaintLabels(true);
		decSlider.setLabelTable(labelTable);
		decSlider.addChangeListener((ChangeListener) this);
		
		decStuff.add(new JLabel("Breaking:       "));
		decStuff.add(decSlider);
		this.add(decStuff);
	}
	
	public void stateChanged(ChangeEvent e) {
	    JSlider source = (JSlider)e.getSource();
	    if (!source.getValueIsAdjusting()) {
	    	if (source == accSlider)
	    		ctrl.sendMessage(CONSTANTS.ACC, source.getValue());
	    	else if (source == decSlider)
	    		ctrl.sendMessage(CONSTANTS.DEC, source.getValue());
	    }
	}

}
