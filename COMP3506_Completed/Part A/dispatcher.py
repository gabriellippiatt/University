
from dispatcher_base import DispatcherBase
from plane import *


class Dispatcher(DispatcherBase):
    """
        Implement all the necessary methods here
    """

    def __init__(self):
        super().__init__()
        self.list1 = PlaneList()
        self.PlaneCounter = 0

    def __len__(self):
        return self.PlaneCounter

    def add_plane(self, plane_number: str, time: str):
        NewNode = Node(Plane(plane_number, time))
        
        # First Insert
        if (self.PlaneCounter == 0):
            self.list1.headval = NewNode
            # print("first")

        # Second Insert
        elif (self.PlaneCounter == 1):
            # print("second")
            if (self.list1.headval.dataval.time > NewNode.dataval.time):
                NewNode.nextval = self.list1.headval
                self.list1.headval = NewNode
            elif (self.list1.headval.dataval.time < NewNode.dataval.time):
                self.list1.headval.nextval = NewNode
        # subsequent Insertion
        else :

            TempPrev = self.list1.headval
            Temp = TempPrev.nextval

            while (TempPrev):

                # Insert Before the head
                if (TempPrev == self.list1.headval and self.list1.headval.dataval > NewNode.dataval) :
                    self.list1.headval = NewNode
                    self.list1.headval.nextval = TempPrev
                    break
                # Insert between two values
                elif (Temp != None and NewNode.dataval < Temp.dataval and NewNode.dataval > TempPrev.dataval):
                    TempPrev.nextval = NewNode
                    NewNode.nextval = Temp
                    break
                # Insert last
                elif (NewNode.dataval > TempPrev.dataval and Temp == None) :
                    TempPrev.nextval = NewNode
                    break
                    
                # Next data set
                if (Temp is not None) :
                    TempPrev = TempPrev.nextval
                    Temp = TempPrev.nextval
                else :
                    break

        self.PlaneCounter = self.PlaneCounter + 1

    def allocate_landing_slot(self, current_time: str):
        TempPrev = self.list1.headval
        Temp = TempPrev.nextval

        # Convert Given time HR:MIN to Minutes
        CurrentTimeArray = current_time.split(":")
        CurrentMinutes = int(CurrentTimeArray[0])*60 + int(CurrentTimeArray[1])
        TempPrevArray = self.list1.headval.dataval.time.split(":")
        TempMinutes = int(TempPrevArray[0])*60 + int(TempPrevArray[1])

        # Delete Head if less than 5 minutes from current time
        if ((TempMinutes - CurrentMinutes) <= 5) :
            self.list1.headval = Temp
            self.PlaneCounter = self.PlaneCounter - 1
            return TempPrev.dataval.plane_number
        
    def emergency_landing(self, plane_number: str):
        TempPrev = self.list1.headval
        Temp = TempPrev.nextval

        while (TempPrev):
            # Delete Head
            if (TempPrev.dataval.plane_number == plane_number) :
                self.list1.headval = Temp
                break
            # Delete Middle
            elif (Temp.dataval.plane_number == plane_number and Temp != None) :
                TempPrev.nextval = Temp.nextval
                break
            # Delete Tail
            elif (Temp.dataval.plane_number == plane_number and Temp.nextval == None):
                TempPrev.nextval = None
                break
            if (Temp is not None) :
                TempPrev = TempPrev.nextval
                Temp = TempPrev.nextval
            else :
                break

        self.PlaneCounter = self.PlaneCounter - 1

    def is_present(self, plane_number: str):
        temp = self.list1.headval
        bool = False
        while (temp and (not bool)):
            temp = temp.nextval
            if (temp != None and temp.dataval.plane_number == plane_number):
                return True
        return bool