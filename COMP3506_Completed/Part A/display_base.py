from plane import Plane

"""
    Sorting must be performed based on time
    If two or more planes have the same time -> sort by the plane_number
    
    Example:

        plane 1: "ABC1233", "9:24"
        plane 2: "ENC3453", "8:23"
        => plane 1 is after plane 2

        plane 1: "ABC1233", "9:24"
        plane 2: "BAA1113", "9:24"
        => plane 1 is before plane 2

        plane 1: "ABC1233", "9:24"
        plane 2: "ABC1234", "9:24"
        => plane 1 is before plane 2
"""


class DisplayRandomBase:

    def __init__(self, lines):
        """
        Read the planes from csv file and store them as Plane objects
        in self.data array

        :param lines: list of lines containing planes in CSV format, no header
        """

        self.data = []
        for line in lines:
            parts = line.split(',')
            new_plane = Plane(parts[0], parts[1])
            self.data.append(new_plane)

    def sort(self):
        """
        Sort the planes from self.data

        :return: the sorted array of planes
        """
        raise NotImplementedError('must be implemented by subclass')


class DisplayPartiallySortedBase:
    def __init__(self, schedule_lines, extra_plane_lines):
        """
        Read the schedule of planes and the extra planes and store them as
        Plane objects in self.schedule and self.extra_planes arrays

        :param schedule_lines, extra_plane_lines: lists of lines containing
                planes in CSV format, no header.
                schedule is sorted, extra planes is unsorted
        """

        self.schedule = []
        self.extra_planes = []

        for line in schedule_lines:
            parts = line.split(',')
            new_plane = Plane(parts[0], parts[1])
            self.schedule.append(new_plane)

        for line in extra_plane_lines:
            parts = line.split(',')
            new_plane = Plane(parts[0], parts[1])
            self.extra_planes.append(new_plane)

    def sort(self):
        """
        Combine the planes from self.schedule and self.extra_planes,
        so that the resulting array is sorted

        :return: the combined sorted array of planes
        """
        raise NotImplementedError('must be implemented by subclass')

