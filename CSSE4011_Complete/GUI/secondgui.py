import tkinter as tk
from knn2 import gabe

# Main class for the GUI
class p2gui(object):
    def __init__(self, master):
        self._master = master
        self._master.title("CSSE4011 Prac 2")
        self._master.resizable(0, 0)
        self.offset = 30
        self.RSSI = [-66,-50,-63,-62,-51,-57,-27,-44,-57,-51,-61,-61]
        self.USonic = [1, 1]
        self.map = {
            "1": (67,333),
            "2": (133,333),
            "3": (200,333),
            "4": (267,333),
            "5": (333,333),
            "6": (67,267),
            "7": (133,267),
            "8": (200,267),
            "9": (267,267),
            "10": (333,267),
            "11": (67,200),
            "12": (133,200),
            "13": (200,200),
            "14": (267,200),
            "15": (333,200),
            "16": (67,133),
            "17": (133,133),
            "18": (200,133),
            "19": (267,133),
            "20": (333,133),
            "21": (67,67),
            "22": (133,67),
            "23": (200,67),
            "24": (267,67),
            "25": (333,67),
            "A": (000,400),
            "B": (133,400),
            "C": (266,400),
            "D": (400,400),
            "E": (400,266),
            "F": (400,133),
            "G": (400,000),
            "H": (266,000),
            "I": (133,000),
            "J": (000,000),
            "K": (000,133),
            "L": (000,266),
        }
        
        self.c = tk.Canvas(master=self._master, height=600+(2*self.offset), width=400+(2*self.offset), bg='white')
        self.main()

    # Creates the background grid
    def create_grid(self):
        
        w = 400 # Get current width of canvas
        h = 400 # Get current height of canvas
        self.c.delete('grid_line') # Will only remove the grid_line

        # Creates all vertical lines at intevals of 100
        for i in range(0+self.offset, w+self.offset, 50):
            self.c.create_line([(i, 0+self.offset), (i, h+self.offset)], tag='grid_line')

        # Creates all horizontal lines at intevals of 100
        for i in range(0+self.offset, h+self.offset, 50):
            self.c.create_line([(0+self.offset, i), (w+self.offset, i)], tag='grid_line')
        
        # Draw the border
        self.c.create_line([(0+self.offset, 0+self.offset), (0+self.offset, h+self.offset)], tag='grid_line')
        self.c.create_line([(h+self.offset, 0+self.offset), (h+self.offset, h+self.offset)], tag='grid_line')
        self.c.create_line([(0+self.offset, 0+self.offset), (0+self.offset, w+self.offset)], tag='grid_line')
        self.c.create_line([(0+self.offset, w+self.offset), (w+self.offset, w+self.offset)], tag='grid_line')

    # Draws the dot at the given coordinate location
    def draw_dot(self,x,y):
        a = int(x)+self.offset
        b = 400-int(y)+self.offset
        self.c.delete('dotty') # Will only remove the grid_line
        self.c.create_oval((a-20),(b-20),(a+20),(b+20), activefill = "red", fill="red", width=2,tag='dotty')
        self.c.create_text(a+3, b+3, font="Times 20 bold", fill="white", text= ":)", angle=270, tag='dotty')
    
    # (This can be deleted) The inputs and button for entering a coordinate
    # def draw_requirements(self):
    #     self.c.create_text(50+self.offset, 450+self.offset, font="Times 20 bold", text= "X:")
    #     self.c.create_text(150+self.offset, 450+self.offset, font="Times 20 bold", text= "Y:")
    #     entry1 = tk.Entry(root, font="Times 20", bd=4)
    #     self.c.create_window(100+self.offset, 450+self.offset, width=50, window=entry1)
    #     entry2 = tk.Entry(root, font="Times 20", bd=4)
    #     self.c.create_window(200+self.offset, 450+self.offset, width=50, window=entry2)
    #     btn = tk.Button(root,font="Times 20 bold", text='Draw', command= lambda : self.draw_dot(entry1.get(),entry2.get()))
    #     btn.place(x=250+self.offset, y=425+self.offset)

    # def show_entry_fields(self):
    #     print("x: %d, y: %d\n" % (int(self.entry1.get()),int(self.entry2.get())))

    # Draws the nodes
    def draw_nodes(self):
        r = 4
        w = 5
        nodeOffset = 10
        self.c.create_oval((0-r+self.offset),(0-r+self.offset),(0+r+self.offset),(0+r+self.offset),fill="black", width=w)
        self.c.create_text((0-nodeOffset+self.offset),(0-nodeOffset+self.offset), text= "3")
        self.c.create_oval((400-r+self.offset),(0-r+self.offset),(400+r+self.offset),(0+r+self.offset),fill="black", width=w)
        self.c.create_text((400-nodeOffset+self.offset),(0-nodeOffset+self.offset), text= "2")
        self.c.create_oval((400-r+self.offset),(400-r+self.offset),(400+r+self.offset),(400+r+self.offset),fill="black", width=w)
        self.c.create_text((400-nodeOffset+self.offset),(400+nodeOffset+self.offset), text= "4")
        self.c.create_oval((0-r+self.offset),(400-r+self.offset),(0+r+self.offset),(400+r+self.offset),fill="black", width=w)
        self.c.create_text((0-nodeOffset+self.offset),(400+nodeOffset+self.offset), text= "1")

    def draw_node_labels(self):
        labelOffset = 10
        self.c.create_text((0-labelOffset+self.offset),(400-400-labelOffset+self.offset), text= "A")
        self.c.create_text((133-labelOffset+self.offset),(400-400-labelOffset+self.offset), text= "B")
        self.c.create_text((266-labelOffset+self.offset),(400-400-labelOffset+self.offset), text= "C")
        self.c.create_text((400-labelOffset+self.offset),(400-400-labelOffset+self.offset), text= "D")
        self.c.create_text((400-labelOffset+self.offset),(400-266+self.offset), text= "E")
        self.c.create_text((400-labelOffset+self.offset),(400-400-133+self.offset), text= "F")
        self.c.create_text((400-labelOffset+self.offset),(400-0+labelOffset+self.offset), text= "G")
        self.c.create_text((266-labelOffset+self.offset),(400-0+labelOffset+self.offset), text= "H")
        self.c.create_text((133-labelOffset+self.offset),(400-0+labelOffset+self.offset), text= "I")
        self.c.create_text((0-labelOffset+self.offset),(400-0+labelOffset+self.offset), text= "J")
        self.c.create_text((0-labelOffset+self.offset),(400-133+self.offset), text= "K")
        self.c.create_text((0-labelOffset+self.offset),(400-266+self.offset), text= "L")

    # draws the cute little axis of the graph
    def draw_axis(self):
        w = 400 # Get current width of canvas
        h = 400 # Get current height of canvas
        axisOffset = 20
        self.c.create_line([(0+self.offset-axisOffset, h+self.offset+axisOffset), (50+self.offset-axisOffset, h+self.offset+axisOffset)], tag='axis_line',arrow=tk.LAST)
        self.c.create_line([(0+self.offset-axisOffset, h+self.offset+axisOffset), (0+self.offset-axisOffset, h-50+self.offset+axisOffset)], tag='axis_line',arrow=tk.LAST)
        self.c.create_text(0+self.offset-axisOffset, h-50+self.offset+axisOffset-5, text= "Y")
        self.c.create_text(50+self.offset-axisOffset+5, h+self.offset+axisOffset, text= "X")

    # Displays the RSSI and Ultra Sonic values given in the array in the init function
    # Will require a function to update these text boxes when the values are update/changed
    def display_measurements(self):
        p = 50
        j = 50
        k = 50

        self.c.delete('rssi_values')
        for i, x in enumerate(self.RSSI):
            if i < 6:
                self.c.create_text(50+self.offset, p+450+self.offset, font="Times 12 bold", text= ("RSSI " + str(i+1) + ": " + str(x)),anchor=tk.W, tag='rssi_values')
                p = p + 20
            else :
                self.c.create_text(150+self.offset, j+450+self.offset, font="Times 12 bold", text= ("RSSI " + str(i+1) + ": " + str(x)),anchor=tk.W, tag='rssi_values')
                j = j + 20

        for y in self.USonic:
            self.c.create_text(250+self.offset, k+450+self.offset, font="Times 12 bold", text= ("Ultra Sonic " + str(y) + ": " + str(self.USonic[y-1])),anchor=tk.W, tag='rssi_values')
            k = k + 20

    def main(self):
        # self.draw_requirements()
        self.draw_nodes()
        self.draw_axis()
        self.display_measurements()
        self.create_grid()
        self.draw_node_labels()
        self.c.pack(fill=tk.BOTH, expand=True)
        buttcheek = gabe(self.RSSI)
        print(buttcheek)
        for key in self.map:
            if key == buttcheek:
                gabex = self.map[key][0]
                gabey = self.map[key][1]
                # print(gabex)
                # print(gabey)
                #self.draw_dot(gabex,gabey)
        
if __name__ == "__main__":
    root = tk.Tk()
    a = p2gui(root)
    root.mainloop()