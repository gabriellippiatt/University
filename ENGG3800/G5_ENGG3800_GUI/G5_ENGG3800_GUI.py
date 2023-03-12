# Bellow lists all of the importations that are made in order to appropriatley 
# and accuratley run the code
import tkinter as tk
import time
import serial.tools.list_ports
from tkinter import *
from tkinter.font import BOLD, Font   
from tkinter.ttk import *

# initiate the COM ports in order to connect to the device
ports = serial.tools.list_ports.comports()
portsList = ['Select Comport']
for port in ports:
    portsList.append(str(port))
    print(str(port))

# initiate the Tkinter Frame and set the size of the window to the 
# exact size desired
root = tk.Tk()
root.resizable(width=False, height=False)
# set the screen position to open at approx the middle of the screen
root.geometry("+550+50")


# This class will hold all of the codes functionality and styling
class mainGUI():
    def __init__(self, master):
        self.master = master
        self.master.title("ENGG3800-Team05") # set the title of the Tkinter Gui
        self.brightness = 3  # initiate the first brightness level before communication with device begins
        self.holdOn = False  # False for hold off, and True for Hold on
        self.modeVal = 0  # 0-AC, 1 dc, 2 current, 3 resistance, 4 continuity
        self.modestring = "---:"   # Initiate the mode variable string to be sent to the device.
        self.resetstring = "---:"  # Initiate the reset variable string to be sent to the device. 'Rest' is sent when reset button is pushed
        self.brightnessstring = "---:"  # Initiate the Brightness variable string to be sent to the device.
        self.holdstring = "---:"  # Initiate the Hold variable string to be sent to the device.
        self.continuitystring = "---:"  # Initiate the continuity variable string to be sent to the device.
        self.textstring = "---:\r"  # Initiate the string message variable string to be sent to the device.
        self.connected = False  # set the connected variable to False to begine. False for not connected and True when connected
        self.flag = False  # initiate flag variable used throughout code for functionality purposes
        self.buttonflag = False  # initiate the button flag to be true when a button is pushed and False when not being pushed
        self.serialInstance = None  # initiate the variable used for pyserial connecction
        self.progress = None  # initiate the progress variable that is used to maintain the progress bars values when brightness is minipulated
        self.portStr = None  # initiate the string used to store the users desired Com port for connection
        root.update_idletasks()
    
    # All of the varying values and strings that require to be constantly updated and displayed are initiated below using String Var()
    # to al for no static displayment to take place.
    root.units = tk.StringVar()
    root.modename = tk.StringVar()
    root.currentVal = tk.StringVar()
    root.string = tk.StringVar()
    root.brightness = tk.StringVar()
    root.modeState = tk.StringVar()
    root.acmin = tk.StringVar()
    root.acMax = tk.StringVar()
    root.holdval = tk.StringVar()
    root.holdstate = tk.StringVar()

    # initiating the values required for the below sting variables as requried for initial dispaly before Uart communication
    # updates the variable values
    root.modename.set(str("Voltage DC"))
    root.units.set(str("Volts"))
    root.holdval.set(str("0"))
    root.holdstate.set(str("HOLD OFF"))
    root.currentVal.set("00") 
    root.acmin.set("00")
    root.acMax.set("00")

    # send large packet of information in one string from GUI to Device
    def sendString(self):
        if self.connected:
            msg = str(self.modestring)+str(self.resetstring)+str(self.brightnessstring)+str(self.holdstring)+str(self.continuitystring)+str(self.textstring)
            self.serialInstance.write(msg.encode())
            print(msg)

    # Function is used to toggle between the different modes
    # When called will move from one mode to the next updating all of the variables required for displaying that modes values
    def modeToggle(self):
        # flag used to set when this function s being called by a function and not by a button on the GUI
        if self.flag is False:
            self.buttonflag = True
        
        if self.modeVal == 0:
            if self.flag is False:
                self.modestring = "Mode 1:"
            self.modeVal += 1
            root.units.set(str("Volts Rms"))
            root.modename.set(str("Voltage AC"))

        elif self.modeVal == 1:
            if self.flag is False:
                self.modestring = "Mode 2:"
            self.modeVal = 2
            root.units.set(str("ohms \u03A9"))
            root.modename.set(str("Resistance"))

        elif self.modeVal == 2:
            if self.flag is False:
                self.modestring = "Mode 3:"
            self.modeVal = 3
            root.units.set(str("ohms \u03A9"))
            root.modename.set(str("Continuity"))

        elif self.modeVal == 3:
            if self.flag is False:
                self.modestring = "Mode 0:"
            self.modeVal = 0
            root.units.set(str("Volts"))
            root.modename.set(str("Voltage DC"))
        # update the GUI window / task
        root.update_idletasks()

    # Function is used to reset the values of the min and Max variable. Function is called when the Reset Button gets pushed
    def reset(self):
        #flag used to set when this function s being called by a function and not by a button on the GUI
        if self.flag is False:
            self.buttonflag = True
        #reset Values and set string to cummunicate with the device
        root.acmin.set("0")
        root.acMax.set("15")
        self.resetstring = "Reset:"

    # Function is used to increase the brightness variable and update the progress bar to corrrectly ilustrate to the user the
    # brightness level of the device. Function is called when the '+' button is pushed by the user
    def barUP(self):
        # flag used to set when this function s being called by a function and not by a button on the GUI
        if self.flag is False:
            self.buttonflag = True
        # check if the brightness is at its maximum level. If not do not change the brightness
        if self.brightness < 5:
            self.brightness += 1
            if self.flag is False:
                self.brightnessstring = "Up:"
        if self.brightness == 0:
            self.progress['value'] = 0
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 1:
            self.progress['value'] = 20
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 2:
            self.progress['value'] = 40
            #update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 3:
            self.progress['value'] = 60
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 4:
            self.progress['value'] = 80
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 5:
            self.progress['value'] = 100
            # update the GUI window / task
            root.update_idletasks()

    # Function is used to decrease the brightness variable and update the progress bar to corrrectly ilustrate to the user the
    # brightness level of the device. Function is called when the '-' button is pushed by the user
    def barDOWN(self):
        # flag used to set when this function s being called by a function and not by a button on the GUI
        if self.flag is False:
            self.buttonflag = True
        # check if the brightness is at its maximum level. If not do not change the brightness
        if self.brightness > 0:
            self.brightness -= 1
            if self.flag is False:
                self.brightnessstring = "Down:"
        if self.brightness == 0:
            self.progress['value'] = 0
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 1:
            self.progress['value'] = 20
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 2:
            self.progress['value'] = 40
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 3:
            self.progress['value'] = 60
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 4:
            self.progress['value'] = 80
            # update the GUI window / task
            root.update_idletasks()
        elif self.brightness == 5:
            self.progress['value'] = 100
            # update the GUI window / task
            root.update_idletasks()
  
    # Function keeps track of the progress bar and updates its progress as required
    def progressbar(self, frame):
        self.progress = Progressbar(frame, orient=HORIZONTAL, length=120, mode='determinate')
        if self.brightness == 3:
            self.progress['value'] = 60
        self.progress.pack(pady=75, side=BOTTOM)

# Function is used to toggle between the two different Hold modes. send 'Hold On' to the device to being the hold
# and 'Hold Off' to end the Hold functionality
    def holdToggle(self):
        # flag used to set when this function s being called by a function and not by a button on the GUI
        if self.flag is False:
            self.buttonflag = True
        #check what state the HoldOn variable is in toggle it to the next state
        if self.holdOn:
            if self.flag is False:
                self.holdstring = "Hold Off:"
            self.holdOn = False
            root.holdstate.set(str("HOLD OFF"))
        else:
            if self.flag is False:
                self.holdstring = "Hold On:"
            self.holdOn = True
            root.holdstate.set(str("HOLD ON"))

# Main function is used to construct all of the GUI windows and widgets. This function also holds the Main functionality 
# of the Gui.
    def main(self):
        # initiate the Gui Canvase and create the bootom layer thst is to be used as the backgrounf of the gui
        canvas = tk.Canvas(root, height=700, width=400, bg="blue")
        canvas.pack()
        # create the next layer of the gui where all text and variables willl be displayed
        frame = tk.Frame(root, bg="yellow")
        frame.place(relwidth=0.96, relheight=0.96, relx=0.02,rely=0.02)

        # build the progress bar and place it within the frame as created above
        self.progressbar(frame)

        # Create label for the progress bar
        brightness = tk.Label(frame, text="BRIGHTNESS:", background="yellow", font=("Arial", 15))
        brightness.place(relx=0.33, rely=0.90)

        # Create and Place the title of the Multimetier user Interface
        title = tk.Label(frame, text="DIGITAL MULTIMETER", background="yellow", font=("Arial", 25, BOLD))
        title.pack()

        # Create and Place the label for the current value being measured aswell as the frame in which to show the value in
        current = tk.Label(frame, text="CURRENT VALUE:", background="yellow", font=("Arial", 15))
        current.pack()
        display = tk.Frame(frame, bg = "white", borderwidth=2, relief=SOLID)
        display.place(relwidth=0.40, relheight=0.2, relx=0.3,rely=0.1)

        # Display the Current value inside of the box constructed above and set its units
        currentvalue = tk.Label(display, textvariable=root.currentVal, background="white", font=("Arial", 25))
        currentvalue.pack(pady=10, padx=10)
        currentvalueunit = tk.Label(display, textvariable=root.units, background="white", font=("Arial", 15))
        currentvalueunit.pack()
        
        # Create and Place the label for the MIN value being measured aswell as the frame in which to show the value in
        min = tk.Label(frame, text="MIN VALUE:", background="yellow", font=("Arial", 15))
        min.place(relx=0.1, rely=0.31)
        minidisplaymin = tk.Frame(frame, bg="white", borderwidth=2, relief=SOLID)
        minidisplaymin.place(relwidth=0.20, relheight=0.15, relx=0.1, rely=0.35)

        # Display the MIN value inside of the box constructed above and set its units
        minvalue = tk.Label(minidisplaymin, textvariable=root.acmin, background="white", font=("Arial", 15))
        minvalue.pack(pady=10)
        minvalueunit = tk.Label(minidisplaymin, textvariable=root.units, background="white", font=("Arial", 10))
        minvalueunit.pack()

        # Create and Place the label for the MAX value being measured aswell as the frame in which to show the value in
        max = tk.Label(frame, text="MAX VALUE:", background="yellow", font=("Arial", 15))
        max.place(relx=0.6, rely=0.31)
        minidisplaymax = tk.Frame(frame, bg="white", borderwidth=2, relief=SOLID)
        minidisplaymax.place(relwidth=0.20, relheight=0.15, relx=0.7,rely=0.35)
        
        # Display the MAX value inside of the box constructed above and set its units
        maxvalue = tk.Label(minidisplaymax, textvariable=root.acMax, background="white", font=("Arial",15))
        maxvalue.pack(pady=10)
        maxvalueunit = tk.Label(minidisplaymax, textvariable=root.units, background="white", font=("Arial", 10))
        maxvalueunit.pack()

        # Construct Reset button
        resetbutton = tk.Button(frame, text="RESET", bg = "red", relief=SOLID, font=("Arial", 15), command = self.reset)
        resetbutton.place(relwidth=0.20, relheight=0.05, relx=0.4,rely=0.35)

        # Display the mde that the multimeter is functioning in to the user
        mode = tk.Label(frame, text="MODE:", background="yellow", font=("Arial", 15))
        mode.place(relx=0.43, rely=0.41)
        modedisplay = tk.Label(frame, textvariable=root.modename, bg = "white", relief=SOLID, font=("Arial", 15))
        modedisplay.place(relwidth=0.30, relheight=0.05, relx=0.35,rely=0.45)
                
        # Construct the Increse Brightness button
        increasebutton = tk.Button(frame, text ="+", bg = "orange", relief=SOLID, font=("Arial", 15), command = self.barUP)
        increasebutton.place(relwidth=0.30, relheight=0.05, relx=0.55,rely=0.94)

        # Construct the Decrease Brightness button
        decreasebutton = tk.Button(frame, text ="-", bg = "orange", relief=SOLID, font=("Arial", 15), command = self.barDOWN)
        decreasebutton.place(relwidth=0.30, relheight=0.05, relx=0.15,rely=0.94)

        # Construct the Mode aToggling Button
        modebutton = tk.Button(frame, text ="MODE", bg = "orange", relief=SOLID, font=("Arial", 15), command = self.modeToggle)
        modebutton.place(relwidth=0.30, relheight=0.05, relx=0.15,rely=0.55)

        # Construct the Hold Toggling Button
        holdbutton = tk.Button(frame, textvariable= root.holdstate, bg = "orange", relief=SOLID, font=("Arial", 15), command = self.holdToggle)
        holdbutton.place(relwidth=0.30, relheight=0.05, relx=0.55,rely=0.55)

        # Enter text Box Label
        textLabel = Label(frame, text="ENTER TEXT:", background="yellow", font=("Arial", 15))
        textLabel.place(relx=0.33,rely=0.65)
        # create message box to take string input from user
        entry = tk.Entry (root, relief=SOLID, font=("Arial", 15)) 
        entry.insert(END, 'Insert Text Here')
        entry.pack()
        canvas.create_window(120, 500, window=entry, height=40, width=150)
        
        #Code Below is used for the construction of the COM port connection menu
        #COM port select
        port = "None"  # Default drop down menu if no comports are
        for port in serial.tools.list_ports.comports():
            port = port.device
        self.comms_var = tk.StringVar()
        self.comOption = tk.OptionMenu(frame, self.comms_var, port).place(relx=0.28,rely=0.78)

        #function is used to obtain the desired port that is selected from the drop box and stor the name of that
        #port in a string to be used for connection
        def portname():
            #get string value
            if portsList is not None:
                desiredPort = self.comms_var.get()
                #if port is within rage set as desied port
                for port in range(0, len(portsList)):
                    if portsList[port].startswith(str(desiredPort)):
                        self.portStr = str(desiredPort)

        #Construct Button for the connectin of the com port                
        self.select_port_button = tk.Button(frame, text="Select COM Port", command=portname)
        self.select_port_button.place(relx=0.50,rely=0.78)

        #below text is used in order to construct a slider to be used for continuity threshold functionality
        # slider current value
        current_value = tk.DoubleVar()

        #function used to get the current slider position and convert it to 
        #the continuity threshold value as required
        def get_current_value():
            self.continuitystring = str(round(int(current_value.get()),3))+":"
            return round(int(current_value.get())*0.1,3)

        #function required for when the slider change
        def slider_changed(event):
            value_label.configure(text=get_current_value())


        # label for the slider
        slider_label = tk.Label(root, text='Continuity:', background='yellow')

        #place the label for the slider
        slider_label.place(relx=0.79,rely=0.10)

        #  slider
        slider = tk.Scale( root, from_=0, to=200, orient='vertical', command=slider_changed, background = 'yellow', 
            fg='black', troughcolor='grey', variable=current_value, showvalue = 0, length=120)

        #set the initial value of the slider to be at 2.0 ohms and place the slider
        slider.set(20)
        slider.place(relx=0.85,rely=0.13)

        # current value label
        current_value_label = tk.Label(root, text='Threshold:', background='yellow')
        current_value_label.place(relx=0.70,rely=0.16)

        # value label
        value_label = tk.Label(root, text=get_current_value(), background='yellow')
        value_label.place(relx=0.78,rely=0.19)

        #function used to get the string message from the user and stor it in a variable to then be displayed to the 
        def getMessage ():  
            text = entry.get()
            self.textstring = text + ":\r"

            

        # Construcrt Text Entry Button, upon being pressed will send the string to the user
        textbutton = tk.Button(text='Send Message', command=getMessage, relief=SOLID, font=("Arial", 15))
        textbutton.place(relx=0.53,rely=0.68)

        #main while loop to hold codes core functionality and connect to the device
        while True:
            #check if the desired port has been selected by the user
            if not self.portStr == None:
                #use try and except functionality to cpnnect to the device and detect when a disconnection occurs
                try:
                    #if the port has not been connected to connect to the desired port with the desired baudrate
                    if(self.serialInstance == None):
                        self.serialInstance = serial.Serial(self.portStr, baudrate = 9600)
                        print("Connecting")
                        #set flag for being connected
                        self.connected = True
                        continue
                    #if connected register the strings sent from the device and sl=plit them into the different values 
                    #as required
                    if self.connected:
                        #read a line of string from the device
                        serialPacket = self.serialInstance.readline()
                        root.string.set(serialPacket)
                        #split string up into array for each value sent
                        bigstring = root.string.get()
                        stringArray = bigstring.split(':')
                        #Check if the correct string length and if any unwanted noise has been sent through the string
                        #ignore if they are detected 
                        if len(stringArray) >= 7:
                            # set the first value to the text required for the measured value
                            root.currentVal.set(stringArray[1])
                            #set the Max value
                            root.acmin.set(stringArray[2])
                            #set the min value
                            root.acMax.set(stringArray[3])
                            #set the brightness value
                            root.brightness.set(stringArray[4])
                            #set the mode state variable
                            root.modeState.set(stringArray[5])
                            #code to designate if the hold button is pressed
                            root.holdval.set(stringArray[6])
                            #following code is used to keep the gui up to date with the device by updating all of the values 
                            #on the gui when they differ from the device
                            if self.buttonflag == False:
                                #If the brightness on the gui is larger than that of the device decrease the GUI's brightness
                                if self.brightness > int(float(root.brightness.get())):
                                    self.flag = True
                                    self.barDOWN()
                                    self.flag = False
                                # If the brightness on the gui is smaller than that of the device increasethe GUI's brightness
                                elif self.brightness < round(int(root.brightness.get())):
                                    self.flag = True
                                    self.barUP()
                                    self.flag = False
                
                                # if the modes are not equal on the device and gui the gui will move through all modes until they match
                                if not self.modeVal == int(root.modeState.get()):
                                    self.flag = True
                                    self.modeToggle()
                                    self.flag = False

                                #if the hold values on the device and gui do not match the hold will be toggled to keep them the same
                                if int(float(root.holdval.get())) == 0:
                                    if self.holdOn == True: 
                                        self.flag = True
                                        self.holdToggle()
                                        self.flag = False
                                if int(float(root.holdval.get())) == 1:
                                    if self.holdOn == False:
                                        self.flag = True
                                        self.holdToggle()
                                        self.flag = False
                            self.buttonflag = False

                            #send the string as required from the GUI to the device and reset all string values back to their inital states
                            self.sendString()
                            self.modestring = "---:"
                            self.resetstring = "---:"
                            self.brightnessstring = "---:"
                            self.holdstring = "---:"
                            self.textstring = "---:\r"
                            #set the mode value (to be done)
                except:

                    #Disconnect of USB->UART occured
                    #If this disconnection occurs close the Uart link and set the required values to nothing in order to be reconnected at a later date when required.
                    if not self.serialInstance == None:
                        self.serialInstance.close()
                        self.connected = False
                        self.serialInstance = None
                        print("disconected")
            #Update the GUI
            root.update()

#direct code to create the Main Gui and run throught the main function
if __name__ == "__main__":
    a = mainGUI(root)
    a.main()

root.mainloop()
