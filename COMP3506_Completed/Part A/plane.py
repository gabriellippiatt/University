from plane_base import PlaneBase

class Node:
   def __init__(self, dataval=None):
      self.dataval = dataval
      self.nextval = None

class PlaneList:
   def __init__(self):
      self.headval = None
      self.tailval = None

class Plane(PlaneBase):
    """
        Implement all the necessary methods of PlaneBase here
    """
    def __init__(self, plane_number, time):
        self.plane_number = plane_number
        self.time = time


    def __lt__(self, other):
        if not isinstance(other, PlaneBase):
            # Don't attempt to compare against unrelated types
            return NotImplemented

        if (self.time < other.time) or (self.time == other.time and self.plane_number < other.plane_number):
            return True
        else:
            return False

