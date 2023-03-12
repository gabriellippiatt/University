import tkinter as tk
from turtle import color

offset = 30

# initialise global variables for RSSI and Ultra Sonic
RSSI = [1,2,3,4,5,6,7,8,9,10,11,12]
USonic = ["U1", "U2"]

root = tk.Tk()

def create_grid(event=None):
    
    w = 400 # Get current width of canvas
    h = 400 # Get current height of canvas
    c.delete('grid_line') # Will only remove the grid_line

    # Creates all vertical lines at intevals of 100
    for i in range(0+offset, w+offset, 50):
        c.create_line([(i, 0+offset), (i, h+offset)], tag='grid_line')

    # Creates all horizontal lines at intevals of 100
    for i in range(0+offset, h+offset, 50):
        c.create_line([(0+offset, i), (w+offset, i)], tag='grid_line')
    
    # Draw the border
    c.create_line([(0+offset, 0+offset), (0+offset, h+offset)], tag='grid_line')
    c.create_line([(h+offset, 0+offset), (h+offset, h+offset)], tag='grid_line')
    c.create_line([(0+offset, 0+offset), (0+offset, w+offset)], tag='grid_line')
    c.create_line([(0+offset, w+offset), (w+offset, w+offset)], tag='grid_line')

def draw_dot(x,y):
    a = int(x)+offset
    b = 400-int(y)+offset
    c.delete('dotty') # Will only remove the grid_line
    c.create_oval((a-20),(b-20),(a+20),(b+20), activefill = "red", fill="red", width=2,tag='dotty')
    c.create_text(a+3, b+3, font="Times 20 bold", fill="white", text= ":)", angle=270, tag='dotty')

def draw_requirements(event=None):
    c.create_text(50+offset, 450+offset, font="Times 20 bold", text= "X:")
    c.create_text(150+offset, 450+offset, font="Times 20 bold", text= "Y:")

def show_entry_fields():
    print("x: %d, y: %d\n" % (int(entry1.get()),int(entry2.get())))

def draw_nodes():
    r = 4
    w = 5
    nodeOffset = 10
    c.create_oval((0-r+offset),(0-r+offset),(0+r+offset),(0+r+offset),fill="black", width=w)
    c.create_text((0-nodeOffset+offset),(0-nodeOffset+offset), text= "3")
    c.create_oval((400-r+offset),(0-r+offset),(400+r+offset),(0+r+offset),fill="black", width=w)
    c.create_text((400-nodeOffset+offset),(0-nodeOffset+offset), text= "2")
    c.create_oval((400-r+offset),(400-r+offset),(400+r+offset),(400+r+offset),fill="black", width=w)
    c.create_text((400-nodeOffset+offset),(400-nodeOffset+offset), text= "4")
    c.create_oval((0-r+offset),(400-r+offset),(0+r+offset),(400+r+offset),fill="black", width=w)
    c.create_text((0-nodeOffset+offset),(400-nodeOffset+offset), text= "1")
    #c.create_rectangle((0-nodeOffset+offset),(400-nodeOffset+offset),(0-nodeOffset+offset)+5,(400-nodeOffset+offset)+10, fill="black", width=w)

def draw_axis():
    w = 400 # Get current width of canvas
    h = 400 # Get current height of canvas
    axisOffset = 20
    c.create_line([(0+offset-axisOffset, h+offset+axisOffset), (50+offset-axisOffset, h+offset+axisOffset)], tag='axis_line',arrow=tk.LAST)
    c.create_line([(0+offset-axisOffset, h+offset+axisOffset), (0+offset-axisOffset, h-50+offset+axisOffset)], tag='axis_line',arrow=tk.LAST)
    c.create_text(0+offset-axisOffset, h-50+offset+axisOffset-5, text= "Y")
    c.create_text(50+offset-axisOffset+5, h+offset+axisOffset, text= "X")

# update the rssi values on the GUI
# n -> the specific node
# val -> the rssi value
def display_measurements():
    
    
# label update instead of recalling the function
# main is the only thing that gets called and you can have a while loop for updating labels
# might need threading for updating

def main():
    draw_requirements()
    draw_nodes()
    draw_axis()
    display_measurements()
    entry1 = tk.Entry(root, font="Times 20", bd=4)
    c.create_window(100+offset, 450+offset, width=50, window=entry1)
    entry2 = tk.Entry(root, font="Times 20", bd=4)
    c.create_window(200+offset, 450+offset, width=50, window=entry2)
    btn = tk.Button(root,font="Times 20 bold", text='Draw', command= lambda : draw_dot(entry1.get(),entry2.get()))
    btn.place(x=250+offset, y=425+offset)
    root.mainloop()

c = tk.Canvas(root, height=600+(2*offset), width=400+(2*offset), bg='white')
c.pack(fill=tk.BOTH, expand=True)
c.bind('<Configure>', create_grid)

if __name__ == "__main__":
    main()