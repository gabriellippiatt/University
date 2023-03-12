import tkinter as tk
import serial.tools.list_ports
import time
from tkinter import *
from tkinter.font import BOLD, Font   
from tkinter.ttk import *


ports = serial.tools.list_ports.comports()


portsList = ['Select Comport']

for port in ports:
    portsList.append(str(port))
    print(str(port))

root = tk.Tk()
root.resizable(width=False, height=False)

class MyFirstGUI():
    def __init__(self, master):
        self.master = master
        self.master.title("testing reconnection")
        self.brightness = 3
        self.currentVal = 12
        self.holdOn = False
        self.modeStatus = 0
        self.modeVal = 0 # 0 ac, 1 dc, 2 current, 3 resistance, 4 continuity
        self.modestring = "---:"
        self.resetstring = "---:"
        self.brightnessstring = "---:"
        self.holdstring = "---:"
        self.continuitystring = "---:"
        self.textstring = "---:\r"
        self.connected =False
        self.flag = False
        self.buttonflag = False
        self.serialInstance = None
        self.progress = None
        self.portStr = None

        root.update_idletasks()
    
    root.units = tk.StringVar()
    root.modename = tk.StringVar()
    root.text = tk.StringVar()
    root.string = tk.StringVar()
    root.brightness = tk.StringVar()
    root.modeState = tk.StringVar()
    root.acmin = tk.StringVar()
    root.acMax = tk.StringVar()
    root.holdval = tk.StringVar()
    root.holdstate = tk.StringVar()

    root.modename.set(str("Voltage DC"))
    root.units.set(str("Volts"))
    root.holdval.set(str("0"))
    root.holdstate.set(str("HOLD OFF"))

    # send large packet of information in on string
    def sendString(self):
        if self.connected:
            msg = str(self.modestring)+str(self.resetstring)+str(self.brightnessstring)+str(self.holdstring)+str(self.continuitystring)+str(self.textstring)
            self.serialInstance.write(msg.encode())
            print(msg)

    def modeToggle (self):
        if self.flag == False:
            self.buttonflag = True
        
        if self.modeVal == 0:
            if self.flag == False:
                self.modestring = "Mode 1:"
            self.modeVal += 1
            root.units.set(str("Volts Rms"))
            root.modename.set(str("Voltage AC"))

        elif self.modeVal == 1:
            if self.flag == False:
                self.modestring = "Mode 2:"
            self.modeVal = 2
            root.units.set(str("ohms \u03A9"))
            root.modename.set(str("Resistance"))

        elif self.modeVal == 2:
            if self.flag == False:
                self.modestring = "Mode 3:"
            self.modeVal = 3
            root.units.set(str("ohms \u03A9"))
            root.modename.set(str("Continuity"))

        elif self.modeVal == 3:
            if self.flag == False:
                self.modestring = "Mode 0:"
            self.modeVal = 0
            root.units.set(str("Volts"))
            root.modename.set(str("Voltage DC"))
            
        root.update_idletasks()
    
    def reset(self):
        if self.flag == False:
            self.buttonflag = True
        self.currentVal = 0
        root.acmin.set("0")
        root.acMax.set("15")
        self.resetstring = "Reset:"


    def barUP(self):
        if self.flag == False:
            self.buttonflag = True

        if self.brightness < 6 :
            self.brightness += 1
            if self.flag == False:
                self.brightnessstring = "Up:"

        if self.brightness == 0:
            self.progress['value'] = 0
            root.update_idletasks()
        elif self.brightness == 1:
            self.progress['value'] = 20
            root.update_idletasks()
        elif self.brightness == 2:
            self.progress['value'] = 40
            root.update_idletasks()
        elif self.brightness == 3:
            self.progress['value'] = 60
            root.update_idletasks()
        elif self.brightness == 4:
            self.progress['value'] = 80
            root.update_idletasks()
        elif self.brightness == 5:
            self.progress['value'] = 100
            root.update_idletasks()

    def barDOWN(self):
        if self.flag == False:
            self.buttonflag = True

        if self.brightness > 0 :
            self.brightness -= 1
            if self.flag == False:
                self.brightnessstring = "Down:"

        if self.brightness == 0:
            self.progress['value'] = 0
            root.update_idletasks()
        elif self.brightness == 1:
            self.progress['value'] = 20
            root.update_idletasks()
        elif self.brightness == 2:
            self.progress['value'] = 40
            root.update_idletasks()
        elif self.brightness == 3:
            self.progress['value'] = 60
            root.update_idletasks()
        elif self.brightness == 4:
            self.progress['value'] = 80
            root.update_idletasks()
        elif self.brightness == 5:
            self.progress['value'] = 100
            root.update_idletasks()
  
    def progressbar(self, frame):
        self.progress = Progressbar(frame, orient = HORIZONTAL, length = 120, mode = 'determinate')
        if self.brightness == 3:
            self.progress['value'] = 60
        self.progress.pack(pady= 75, side=BOTTOM)


    def holdToggle (self):
        if self.flag == False:
            self.buttonflag = True
        if self.holdOn:
            if self.flag == False:
                self.holdstring = "Hold Off:"
            self.holdOn=False
            root.holdstate.set(str("HOLD OFF"))
        else:
            if self.flag == False:
                self.holdstring = "Hold On:"
            self.holdOn=True
            root.holdstate.set(str("HOLD ON"))


    def main(self):

        canvas = tk.Canvas(root, height = 700, width = 400, bg= "blue")
        canvas.pack()

        frame = tk.Frame(root, bg = "yellow")
        frame.place(relwidth=0.96, relheight=0.96, relx=0.02,rely=0.02)

        self.progressbar(frame)

        title = Label(frame, text="DIGITAL MULTIMETER", background="yellow", font=("Arial", 25, BOLD))
        title.pack()
        current = Label(frame, text="CURRENT VALUE:", background="yellow", font=("Arial", 15))
        current.pack()

        display = tk.Frame(frame, bg = "white", borderwidth=2, relief=SOLID)
        display.place(relwidth=0.40, relheight=0.2, relx=0.3,rely=0.1)


        root.text.set("25") 
        currentvalue = Label(display, textvariable=root.text, background="white", font=("Arial", 25))
        currentvalue.pack(pady=10, padx = 10)
        currentvalueunit = Label(display, textvariable=root.units, background="white", font=("Arial", 15))
        currentvalueunit.pack()
        
        min = Label(frame, text="MIN VALUE:", background="yellow", font=("Arial", 15))
        min.place(relx=0.1,rely=0.31)

        minidisplaymin = tk.Frame(frame, bg = "white", borderwidth=2, relief=SOLID)
        minidisplaymin.place(relwidth=0.20, relheight=0.15, relx=0.1,rely=0.35)

        # set up min value label
        root.acmin.set("03")
        maxvalue = Label(minidisplaymin, textvariable=root.acmin, background="white", font=("Arial", 15))
        maxvalue.pack(pady=10)

        #input min value units
        minvalueunit = Label(minidisplaymin, textvariable=root.units, background="white", font=("Arial", 10))
        minvalueunit.pack()

        #set up max value box and title
        max = Label(frame, text="MAX VALUE:", background="yellow", font=("Arial", 15))
        max.place(relx=0.6,rely=0.31)
        minidisplaymax = tk.Frame(frame, bg = "white", borderwidth=2, relief=SOLID)
        minidisplaymax.place(relwidth=0.20, relheight=0.15, relx=0.7,rely=0.35)
        
        # set up max value label
        root.acMax.set("56")
        minvalue = Label(minidisplaymax, textvariable=root.acMax, background="white", font=("Arial",15))
        minvalue.pack(pady=10)
        #input the max value units
        maxvalueunit = Label(minidisplaymax, textvariable=root.units, background="white", font=("Arial", 10))
        maxvalueunit.pack()

        # Reset Button
        resetbutton = tk.Button(frame, text ="RESET", bg = "red", relief=SOLID, font=("Arial", 15), command = self.reset)
        resetbutton.place(relwidth=0.20, relheight=0.05, relx=0.4,rely=0.35)

        # mode display
        mode = Label(frame, text="MODE:", background="yellow", font=("Arial", 15))
        mode.place(relx=0.43, rely=0.41)
        modedisplay = tk.Label(frame, textvariable=root.modename, bg = "white", relief=SOLID, font=("Arial", 15))
        modedisplay.place(relwidth=0.30, relheight=0.05, relx=0.35,rely=0.45)
                
        # Increse Brightness
        increasebutton = tk.Button(frame, text ="+", bg = "orange", relief=SOLID, font=("Arial", 15), command = self.barUP)
        increasebutton.place(relwidth=0.30, relheight=0.05, relx=0.55,rely=0.94)

        # Decrease Brightness
        decreasebutton = tk.Button(frame, text ="-", bg = "orange", relief=SOLID, font=("Arial", 15), command = self.barDOWN)
        decreasebutton.place(relwidth=0.30, relheight=0.05, relx=0.15,rely=0.94)

        # Mode
        modebutton = tk.Button(frame, text ="MODE", bg = "orange", relief=SOLID, font=("Arial", 15), command = self.modeToggle)
        modebutton.place(relwidth=0.30, relheight=0.05, relx=0.15,rely=0.55)

        # Hold
        holdbutton = tk.Button(frame, textvariable= root.holdstate, bg = "orange", relief=SOLID, font=("Arial", 15), command = self.holdToggle)
        holdbutton.place(relwidth=0.30, relheight=0.05, relx=0.55,rely=0.55)


        # Progress bar widget
        brightness = Label(frame, text="BRIGHTNESS:", background="yellow", font=("Arial", 15))
        brightness.place(relx=0.33,rely=0.90)

        # Enter text
        brightness = Label(frame, text="ENTER TEXT:", background="yellow", font=("Arial", 15))
        brightness.place(relx=0.33,rely=0.65)
        entry = tk.Entry (root, relief=SOLID, font=("Arial", 15)) 
        entry.insert(END, 'Insert Text Here')
        entry.pack()
        canvas.create_window(120, 500, window=entry, height=40, width=150)

        # brightness = Label(minidisplaymax, textvariable=root.brightness, background="white", font=("Arial", 50))
        # brightness.pack()
        # brightness.place(relx=0.40,rely=0.80)
        
        # comp port select
        port = "None"  # Default drop down menu if no comports are

        for port in serial.tools.list_ports.comports():
            port = port.device

        self.comms_var = tk.StringVar()
        self.comOption = tk.OptionMenu(frame, self.comms_var, port).place(relx=0.28,rely=0.78)

        def portname():
            if portsList is not None:
                desiredPort = self.comms_var.get()
                # print(desiredPort)

                for port in range(0, len(portsList)):
                    if portsList[port].startswith(str(desiredPort)):
                        self.portStr = str(desiredPort)
        
        def connect():
            if not self.portStr == None:
                try:
                    if(self.serialInstance == None):
                        self.serialInstance = serial.Serial(self.portStr, baudrate = 9600)
                        print("Reconnecting")
                        self.connected = True

                    print("Writing Data...")
                except:
                    #Disconnect of USB->UART occured
                    self.serialInstance.close()
                    self.connected = False
                    print("disconected")
                    
                    

        self.select_port_button = tk.Button(frame, text="Select COM Port", command=portname)
        self.select_port_button.place(relx=0.50,rely=0.78)

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
        slider_label = tk.Label(
            root,
            text='Continuity:',
            background='yellow'
        )

        slider_label.place(relx=0.79,rely=0.10)

        #  slider
        slider = tk.Scale(
            root,
            from_=0,
            to=200,
            orient='vertical',  # vertical
            command=slider_changed,
            background = 'yellow',
            fg='black',
            troughcolor='grey',
            variable=current_value,
            showvalue = 0,
            length=130
        )

        slider.set(20)

        slider.place(relx=0.85,rely=0.13)

        # current value label
        current_value_label = tk.Label(
            root,
            text='Threshold:',
            background='yellow'
        )

        current_value_label.place(relx=0.70,rely=0.16)

        # value label
        value_label = tk.Label(
            root,
            text=get_current_value(),
            background='yellow'
        )

        value_label.place(relx=0.78,rely=0.19)

        def getMessage ():  
            text = entry.get()
            self.textstring = text + ":\r"

            

        # Text Entry Button
        textbutton = tk.Button(text='Send Message', command=getMessage, relief=SOLID, font=("Arial", 15))
        textbutton.place(relx=0.53,rely=0.68)


        while True:
            if not self.portStr == None:
                try:
                    if(self.serialInstance == None):
                        self.serialInstance = serial.Serial(self.portStr, baudrate = 9600)
                        print("Connecting")
                        self.connected = True

                    # print("Writing Data...")
                    if self.connected:
                    #if self.serialInstance.in_waiting:
                        serialPacket = self.serialInstance.readline()
                        root.string.set(serialPacket)
                        #split string up into array for each value sent
                        bigstring = root.string.get()
                        stringArray = bigstring.split(':')
                        if len(stringArray) >= 7:
                            # set the first value to the text required for the measured value
                            root.acmin.set(stringArray[2])
                            #set the Max value
                            root.text.set(stringArray[1])
                            #set the min value
                            root.acMax.set(stringArray[3])
                            #set the brightness value
                            root.brightness.set(stringArray[4])
                            #code to designate if the hold button is pressed
                            root.holdval.set(stringArray[6])
                            
                            if self.buttonflag == False:
                                if self.brightness > int(float(root.brightness.get())):
                                    self.flag = True
                                    self.barDOWN()
                                    self.flag = False
                                    
                                elif self.brightness < round(int(root.brightness.get())):
                                    self.flag = True
                                    self.barUP()
                                    self.flag = False

                                root.modeState.set(stringArray[5])
                
                                # print("modeVal:"+str(self.modeVal))
                                # print("device mode:"+root.modeState.get())
                                if not self.modeVal == int(root.modeState.get()):
                                    self.flag = True
                                    self.modeToggle()
                                    self.flag = False
                                
                                # print("self:"+str(self.holdOn))
                                # print("dev:"+root.holdval.get())
                                if int(float(root.holdval.get())) == 0:
                                    if self.holdOn == True: 
                                        # self.flag = True
                                        self.holdToggle()
                                        # self.flag = False
                                if int(float(root.holdval.get())) == 1:
                                    if self.holdOn == False:
                                        # self.flag = True
                                        self.holdToggle()
                                        # self.flag = False

                                
                            self.buttonflag = False

                            self.sendString()
                            self.modestring = "---:"
                            self.resetstring = "---:"
                            self.brightnessstring = "---:"
                            self.holdstring = "---:"
                            self.textstring = "---:\r"
                            
                            #set the mode value (to be done)
                except:
                    #Disconnect of USB->UART occured
                    if not self.serialInstance == None:
                        self.serialInstance.close()
                        self.connected = False
                        self.serialInstance = None
                        print("disconected")
                
            root.update()


if __name__ == "__main__":
    
    a = MyFirstGUI(root)
    a.main()

root.mainloop()