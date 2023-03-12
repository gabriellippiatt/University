
"""
    General information about the priority:
    The priority must be given to the planes with the earliest estimated time.
    If two or more planes have the same estimated time ->
        the priority must be given to a plane with the plane_number,
        that comes first in alphabetic order

    Example:

        plane 1: "ABC1233", "9:24"
        plane 2: "ENC3453", "8:23"
        => the priority must be given to the plane 2

        plane 1: "ABC1233", "9:24"
        plane 2: "BAA1113", "9:24"
        => the priority must be given to the plane 1

        plane 1: "ABC1233", "9:24"
        plane 2: "ABC1234", "9:24"
        => the priority must be given to the plane 1
"""


class DispatcherBase:

    def __init__(self):
        """
        Initialize the data structure to store planes
        """
        # raise NotImplementedError('must be implemented by subclass')

    def __len__(self):
        """
        Compute the number of the planes in the system

        :return: the number of the planes in the system
        """
        raise NotImplementedError('must be implemented by subclass')

    def is_empty(self):
        """
        Return True if there are no planes in the system
        """
        return len(self) == 0

    def add_plane(self, plane_number: str, time: str):
        """
        Add a plane to the system.
        The complexity must be O(n)

        :param plane_number: string with 3 letters, followed by 4 numbers.
                             Example: "ABC1243", "ENC3455"
        :param time: string, represents time in 24h format.
                     Example: "9:24", "15:32"
        :return:
        """
        raise NotImplementedError('must be implemented by subclass')

    def allocate_landing_slot(self, current_time: str):
        """
        Allocate the landing slot to the next plane in line if it is already waiting
        or if it arrives no later than 5 minutes from the current time.
        Remove the plane that has been granted a landing slot and return its number
        Otherwise return None

        The complexity must be O(1)

        :param current_time:string, represents the current time in 24h format.
                            Example: "9:24", "15:32"
        :return: Plane number or None
        """
        raise NotImplementedError('must be implemented by subclass')

    def emergency_landing(self, plane_number: str):
        """
        Find and remove a plane by its number.
        The complexity must be O(n)

        :param plane_number: string with 3 letters, followed by 4 numbers.
                             Example: "ABC1236", "ENC3455"
        :return: Plane number or None
        """
        raise NotImplementedError('must be implemented by subclass')

    def is_present(self, plane_number: str):
        """
        Returns True if the plane is in the system, otherwise return False

        :param plane_number: string with 3 letters, followed by 4 numbers.
                             Example: "ABC1235", "ENC3454"
        :return: True/False
        """
        raise NotImplementedError('must be implemented by subclass')
