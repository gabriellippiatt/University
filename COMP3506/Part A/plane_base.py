
class PlaneBase:
    def __init__(self, plane_number, time):
        self.plane_number = plane_number
        self.time = time

    def __repr__(self):
        return '({0}, {1})'.format(self.plane_number, self.time)

    def __lt__(self, other):
        if not isinstance(other, PlaneBase):
            # Don't attempt to compare against unrelated types
            return NotImplemented
        raise NotImplementedError('must be implemented by subclass')