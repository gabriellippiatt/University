

class AirportBase:
    """
    An airport is a collection of terminals and shuttles between terminals.
    """
    def __init__(self, capacity):
        """
        Creates a new AirportBase instance with the given capacity.

        :param capacity: the capacity of the airport shuttles (the same for all shuttles) (int)
        """
        self.capacity = capacity

    class TerminalBase:
        """
        Represents a terminal in the airport.
        """
        def __init__(self, id: str, waiting_time: int):
            """
            Creates a new TerminalBase instance with the given terminal ID
            and waiting time.

            :param id: terminal ID (str)
            :param waiting_time: waiting time for the terminal, in minutes (int)
            """
            self.id = id
            self.waiting_time = waiting_time

        def __repr__(self):
            return f"Terminal{{id={self.id}, wait={self.waiting_time}}}"

    class ShuttleBase:
        """
        Represents an airport shuttle that travels between two terminals.
        """
        def __init__(self, origin, destination, time):
            """
            Creates a new ShuttleBase instance, travelling from origin to destination
            and requiring 'time' minutes to travel.

            :param origin: origin terminal
            :param destination: destination terminal
            :param time: time required to travel, in minutes (int)
            """
            self.source = origin
            self.destination = destination
            self.time = time

        def __repr__(self):
            return f"Shuttle{{origin={self.source}, dest={self.destination}, time={self.time}}}"

    def opposite(self, shuttle, terminal):
        """
        Given a terminal and a shuttle, returns the other terminal that the shuttle travels between.

        :param shuttle: shuttle to look for opposite terminal on
        :param terminal: terminal to find opposite of
        :return: opposite terminal or None if the shuttle is not incident to the given terminal
        """
        raise NotImplementedError('must be implemented')

    def insert_terminal(self, terminal):
        """
        Adds the given terminal to the airport, and returns the added terminal.

        :param terminal: terminal to add
        :return: terminal that was added
        """

        raise NotImplementedError('must be implemented')

    def insert_shuttle(self, origin, destination, time):
        """
        Creates and returns a new shuttle connecting origin to destination.
        All shuttles are bidirectional.

        :param origin: origin terminal of shuttle
        :param destination: destination terminal of shuttle
        :param time: time it takes to go from origin to destination, in minutes
        :return: newly created shuttle
        """
        raise NotImplementedError('must be implemented')

    def remove_terminal(self, terminal):
        """
        Removes the given terminal and all of its incident shuttles from the airport.
        All shuttles going to/from the given terminal should be removed.

        :param terminal: terminal to remove
        :return: True if removed successfully,
                 False otherwise (if the terminal was not in the airport)
        """
        raise NotImplementedError('must be implemented')

    def remove_shuttle(self, shuttle):
        """
        Removes the shuttle from the airport.

        :param shuttle:
        :return: True if removed successfully,
                 False otherwise
        """
        raise NotImplementedError('must be implemented')

    def outgoing_shuttles(self, terminal):
        """
        Returns a list of all shuttles incident to the given terminal.

        :param terminal: terminal to find incident shuttles of
        :return: list of incident shuttles
        """
        raise NotImplementedError('must be implemented')

    def find_shortest_path(self, origin, destination):
        """
        Returns the shortest path between the given origin and destination
        terminals. The shortest path is the path that requires the least number
        of shuttles.

        The returned Path consists of a list of terminals in the path, and the
        total time spent travelling along the path. The first element of the
        Path's terminal list should be the given origin terminal, and the last
        element should be the given destination terminal. Any intermediate
        terminals in the path should appear in the list in the order travelled.

        :param origin: the starting terminal
        :param destination: the destination terminal
        :return: list of terminals in path, total time taken in path
                 or
                 None,None if destination is not reachable from origin
        """
        raise NotImplementedError('must be implemented')

    def find_fastest_path(self, origin, destination):
        """
        Returns the fastest path between the given origin and destination
        terminals. The fastest path has the lowest total time spent travelling
        and waiting.

        The returned Path consists of a list of terminals in the path, and the
        total time spent travelling along the path. The first element of the
        Path's terminal list should be the given origin terminal, and the last
        element should be the given destination terminal. Any intermediate
        terminals in the path should appear in the list in the order travelled.

        :param origin: the starting terminal
        :param destination: the destination terminal
        :return: list of terminals, total time taken in path
                 or
                 None,None if destination is not reachable from origin
        """
        raise NotImplementedError('must be implemented')

