
from os import remove
from sys import path
from priority_queue import HeapPriorityQueue
from airport_base import AirportBase


class Airport(AirportBase):
    def __init__(self, capacity):
        self.capacity = capacity
        self.graph = {}

    """
        Implement all the necessary methods of the Airport here
    """

    class Shuttle(AirportBase.ShuttleBase):
        def __init__(self, origin, destination, time):
            super().__init__(origin, destination, time)
            self.capacity = 0
        """
            Implement all the necessary methods of the Shuttle here
        """
    class Terminal(AirportBase.TerminalBase):
        def __init__(self, id: str, waiting_time: int):
            super().__init__(id, waiting_time)
        """
            Implement all the necessary methods of the Terminal here
        """
    def opposite(self, shuttle, terminal):
        """
        Given a terminal and a shuttle, returns the other terminal that the shuttle travels between.

        :param shuttle: shuttle to look for opposite terminal on
        :param terminal: terminal to find opposite of
        :return: opposite terminal or None if the shuttle is not incident to the given terminal
        """
        if shuttle.source == terminal:
            return shuttle.destination
        elif shuttle.destination == terminal :
            return shuttle.source
        else :
            return None

    def insert_terminal(self, terminal):
        """
        Adds the given terminal to the airport, and returns the added terminal.

        :param terminal: terminal to add
        :return: terminal that was added
        """
        
        if terminal not in self.graph :
            self.graph[terminal] = {}

        return terminal

    def insert_shuttle(self, origin, destination, time):
        """
        Creates and returns a new shuttle connecting origin to destination.
        All shuttles are bidirectional.

        :param origin: origin terminal of shuttle
        :param destination: destination terminal of shuttle
        :param time: time it takes to go from origin to destination, in minutes
        :return: newly created shuttle
        """
        shuttle = Airport.Shuttle(origin, destination, time)
        self.graph[origin][shuttle] = 0
        self.graph[destination][shuttle] = 0
    
        return shuttle


    def remove_terminal(self, terminal):
        """
        Removes the given terminal and all of its incident shuttles from the airport.
        All shuttles going to/from the given terminal should be removed.

        :param terminal: terminal to remove
        :return: True if removed successfully,
                 False otherwise (if the terminal was not in the airport)
        """
        if terminal in self.graph:
            shuttles = self.outgoing_shuttles(terminal)
            for i in shuttles:
                self.remove_shuttle(i)
            self.graph.pop(terminal)
            return True
        else:
            return False

    def remove_shuttle(self, shuttle):
        """
        Removes the shuttle from the airport.

        :param shuttle:
        :return: True if removed successfully,
                 False otherwise
        """
        sourceTerminal = shuttle.source
        destinationTerminal = shuttle.destination
        forwardShuttle = shuttle
        backwardShuttle = Airport.Shuttle(shuttle.destination, shuttle.source, shuttle.time)

        if sourceTerminal in self.graph and destinationTerminal in self.graph :
            if forwardShuttle in self.graph[sourceTerminal]:
                self.graph[sourceTerminal].pop(forwardShuttle)
                self.graph[destinationTerminal].pop(forwardShuttle)
                return True
            elif backwardShuttle in self.graph[sourceTerminal]:
                self.graph[sourceTerminal].pop(backwardShuttle)
                self.graph[destinationTerminal].pop(backwardShuttle)
                return True
            else:
                return False
        else :
            return False

    def outgoing_shuttles(self, terminal):
        """
        Returns a list of all shuttles incident to the given terminal.

        :param terminal: terminal to find incident shuttles of
        :return: list of incident shuttles
        """
        outgoingkeys = self.graph[terminal]
        outgoing = list(outgoingkeys).copy()
        return outgoing

    def get_neighbours(self, terminal):
        """
        Gets the neighbours of the given terminal
        """
        neighbours = []
        for i in self.outgoing_shuttles(terminal):
            if i.source not in neighbours and i.source is not terminal:
                neighbours.append(i.source)
            if i.destination not in neighbours and i.destination is not terminal:
                neighbours.append(i.destination)

        return neighbours

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
        visited = []
        terminal_queue = HeapPriorityQueue()
        terminal_queue.add(0, (origin, [], []))
        priority = 0
        cost = 0
        desired_shuttle = None
        while not terminal_queue.is_empty():
            k,(v,p,s) = terminal_queue.remove_min()
            visited.append(v)

            if v is destination :
                p.append(v)
                for i in p:
                    cost += i.waiting_time
                for j in s:
                    j.capacity += 1
                    cost += j.time
                    if j.capacity >= self.capacity:
                        self.remove_shuttle(j)
                cost -= destination.waiting_time
                return p, cost
            else :
                for i in self.get_neighbours(v):
                    if i not in visited:
                        for j in self.outgoing_shuttles(i):
                            if (j.source == i and j.destination == v) or (j.destination == i and j.source == v):
                                desired_shuttle = j
                        terminal_queue.add(priority, (i, p + [v], s + [desired_shuttle]))
                priority += 1

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
        visited = []
        terminal_queue = HeapPriorityQueue()
        terminal_queue.add(0, (origin, [], []))
        priority = 0
        cost = 0
        desired_shuttle = None
        while not terminal_queue.is_empty():
            k,(v,p,s) = terminal_queue.remove_min()
            visited.append(v)
            if v is destination :
                p.append(v)
                for i in p:
                    cost += i.waiting_time
                for j in s:
                    j.capacity += 1
                    cost += j.time
                    if j.capacity >= self.capacity:
                        self.remove_shuttle(j)
                cost -= destination.waiting_time
                return p, cost
            else :
                for i in self.get_neighbours(v):
                    if i not in visited:
                        for j in self.outgoing_shuttles(i):
                            if (j.source == i and j.destination == v) or (j.destination == i and j.source == v):
                                desired_shuttle = j
                        terminal_queue.add((k + priority + i.waiting_time + desired_shuttle.time), (i, p + [v], s + [desired_shuttle]))


if __name__ == "__main__":

    """
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        REMOVE THE MAIN FUNCTION BEFORE SUBMITTING TO THE AUTOGRADER 
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        The following main function is provided for simple debugging only
    """

#     # g = Airport(capacity=3)
#     # terminalA = g.insert_terminal(g.Terminal("A", 1))
#     # terminalB = g.insert_terminal(g.Terminal("B", 3))
#     # terminalC = g.insert_terminal(g.Terminal("C", 4))
#     # terminalD = g.insert_terminal(g.Terminal("D", 2))
#     # terminalE = g.insert_terminal(g.Terminal("D", 2))

#     # shuttle1 = g.insert_shuttle(terminalA, terminalB, 2)
#     # shuttle2 = g.insert_shuttle(terminalA, terminalC, 5)
#     # shuttle3 = g.insert_shuttle(terminalA, terminalD, 18)
#     # shuttle4 = g.insert_shuttle(terminalB, terminalD, 8)
#     # shuttle5 = g.insert_shuttle(terminalC, terminalD, 15)

#     # # Opposite
#     # assert g.opposite(shuttle1, terminalA).id == 'B'
#     # assert g.opposite(shuttle1, terminalB).id == 'A'
#     # assert g.opposite(shuttle1, terminalE) == None


#     # # Outgoing Shuttles
#     # assert [s.time for s in g.outgoing_shuttles(terminalA)] == [2, 5, 18]

#     # # Remove Terminal
#     # g.remove_terminal(terminalC)
#     # assert [s.time for s in g.outgoing_shuttles(terminalA)] == [2, 18]

#     # # Shortest path
#     # g.find_shortest_path(terminalA, terminalD)
#     # # shortest_path, distance = g.find_shortest_path(terminalA, terminalD)
#     # # assert [t.id for t in shortest_path] == ['A', 'D']
#     # # assert distance == 19

#     # # # Fastest path
#     # # fastest_path, distance = g.find_fastest_path(terminalA, terminalD)
#     # # assert [t.id for t in fastest_path] == ['A', 'B', 'D']
#     # # assert distance == 14

#     # print("here")

#     #//////////////////////// Chadeen //////////////////////////////
#     # g = Airport(capacity=3)
#     # terminalA = g.insert_terminal(g.Terminal("A", 4))
#     # terminalB = g.insert_terminal(g.Terminal("B", 1))
#     # terminalC = g.insert_terminal(g.Terminal("C", 1))
#     # terminalD = g.insert_terminal(g.Terminal("D", 2))
#     # terminalE = g.insert_terminal(g.Terminal("E", 4))
#     # terminalF = g.insert_terminal(g.Terminal("F", 1))
#     # terminalStray = g.insert_terminal(g.Terminal("S", 1))

#     # shuttle1 = g.insert_shuttle(terminalA, terminalB, 3)
#     # shuttle2 = g.insert_shuttle(terminalA, terminalC, 2)
#     # shuttle3 = g.insert_shuttle(terminalA, terminalD, 18)
#     # shuttle4 = g.insert_shuttle(terminalA, terminalE, 2)
#     # shuttle5 = g.insert_shuttle(terminalB, terminalE, 7)
#     # shuttle6 = g.insert_shuttle(terminalB, terminalF, 1)
#     # shuttle7 = g.insert_shuttle(terminalC, terminalD, 15)
#     # shuttle8 = g.insert_shuttle(terminalD, terminalE, 4)
#     # shuttle9 = g.insert_shuttle(terminalE, terminalF, 22)

#     # print(g.opposite(shuttle1,terminalB)) # TA
#     # print(g.opposite(shuttle9, terminalB))  #NONE
#     # print(g.get_neighbours(terminalA))
#     # print(g.find_shortest_path(terminalA,terminalD)) #AD 22
#     # print(g.find_fastest_path(terminalA, terminalD)) #AED 14
#     # print(g.find_shortest_path(terminalA, terminalF)) # ABF 9
#     # print(g.find_fastest_path(terminalA, terminalF)) #ABF 9
#     # print(g.find_fastest_path(terminalA, terminalD)) #AED 14
#     # print(g.find_fastest_path(terminalA, terminalD)) #AED 14
#     # print(g.find_fastest_path(terminalA, terminalD)) #AD 22
#     # print(g.find_fastest_path(terminalA, terminalD)) #AD 22
#     # print(g.find_fastest_path(terminalA, terminalD)) #AC 22

#     # for i in g.outgoing_shuttles(terminalA):
#     #     print(i.time, end = ' ') # 3 2 18 2
#     # print("\n")
#     # g.remove_terminal(terminalB)
#     # for i in g.outgoing_shuttles(terminalA):
#     #     print(i.time, end = ' ') # 2 18 2

#     # g.find_shortest_path(terminalA, terminalB)


#     g = Airport(capacity=5)
#     terminalA = g.insert_terminal(g.Terminal("A", 1))
#     terminalB = g.insert_terminal(g.Terminal("B", 2))
#     terminalC = g.insert_terminal(g.Terminal("C", 3))
#     terminalD = g.insert_terminal(g.Terminal("D", 2))
#     terminalE = g.insert_terminal(g.Terminal("E", 1))
#     terminalF = g.insert_terminal(g.Terminal("F", 6))

#     shuttleAB = g.insert_shuttle(terminalA, terminalB, 1)
#     shuttleAC = g.insert_shuttle(terminalA, terminalC, 1)
#     shuttleAD = g.insert_shuttle(terminalA, terminalD, 1)
#     shuttleBD = g.insert_shuttle(terminalB, terminalD, 1)
#     shuttleBC = g.insert_shuttle(terminalB, terminalC, 3)
#     shuttleCD = g.insert_shuttle(terminalC, terminalD, 3)
#     shuttleCE = g.insert_shuttle(terminalE, terminalC, 3)
#     shuttleDE = g.insert_shuttle(terminalE, terminalD, 6)
#     shuttleDF = g.insert_shuttle(terminalF, terminalD, 5)
#     shuttleEF = g.insert_shuttle(terminalF, terminalE, 1)

#     # shortest_path, distance = g.find_shortest_path(terminalA, terminalF)
#     # # print(shortest_path, distance)
#     # assert [t.id for t in shortest_path] == ['A', 'D', 'F']
#     # assert distance == 9


#     fastest_path, distance = g.find_fastest_path(terminalA, terminalF)
#     print(fastest_path, distance)

#     # print("A to B")
#     # fastest_path, distance = g.find_fastest_path(terminalA, terminalB)
#     # print(fastest_path, distance)
#     # print("A to C")
#     # fastest_path, distance = g.find_fastest_path(terminalA, terminalC)
#     # print(fastest_path, distance)
#     # print("A to D")
#     # fastest_path, distance = g.find_fastest_path(terminalA, terminalC)
#     # print(fastest_path, distance)
#     # print("B to D")
#     # fastest_path, distance = g.find_fastest_path(terminalA, terminalC)
#     # print(fastest_path, distance)
#     # print("B to D")
#     # fastest_path, distance = g.find_fastest_path(terminalA, terminalC)
#     # print(fastest_path, distance)

    print('\n\n------')
    # g = Airport(capacity=5)
    # terminalA = g.insert_terminal(g.Terminal("A", 1))
    # terminalB = g.insert_terminal(g.Terminal("B", 1))
    # terminalC = g.insert_terminal(g.Terminal("C", 3))
    # terminalD = g.insert_terminal(g.Terminal("D", 1))
    # terminalE = g.insert_terminal(g.Terminal("E", 1))
    # terminalF = g.insert_terminal(g.Terminal("F", 6))

    # shuttleAB = g.insert_shuttle(terminalA, terminalB, 7)
    # shuttleAC = g.insert_shuttle(terminalA, terminalC, 7)
    # shuttleAD = g.insert_shuttle(terminalA, terminalD, 1)
    # shuttleBD = g.insert_shuttle(terminalB, terminalD, 1)
    # shuttleBC = g.insert_shuttle(terminalB, terminalC, 1)
    # shuttleCD = g.insert_shuttle(terminalC, terminalD, 7)
    # shuttleCE = g.insert_shuttle(terminalE, terminalC, 3)
    # shuttleDE = g.insert_shuttle(terminalE, terminalD, 6)
    # shuttleDF = g.insert_shuttle(terminalF, terminalD, 5)
    # shuttleEF = g.insert_shuttle(terminalF, terminalE, 1)

    # assert g.opposite(shuttleCE, terminalE) == terminalC
    # fastest_path, distance = g.find_fastest_path(terminalA, terminalC)
    # print(fastest_path, distance)
    # assert [t.id for t in fastest_path] == ['A', 'D', 'B', 'C']
    # assert distance == 6

    # shortest_path, distance = g.find_shortest_path(terminalA, terminalC)
    # print(shortest_path, distance)
    # assert [t.id for t in shortest_path] == ['A', 'C']
    # assert distance == 8

    g = Airport(capacity=5)
    terminalA = g.insert_terminal(g.Terminal("A", 1))
    terminalB = g.insert_terminal(g.Terminal("B", 10))
    terminalC = g.insert_terminal(g.Terminal("C", 2))
    terminalD = g.insert_terminal(g.Terminal("D", 2))

    shuttle1 = g.insert_shuttle(terminalA, terminalB, 2)
    shuttle2 = g.insert_shuttle(terminalA, terminalC, 10)
    shuttle3 = g.insert_shuttle(terminalB, terminalD, 8)
    shuttle4 = g.insert_shuttle(terminalC, terminalD, 8)
    print(g.find_fastest_path(terminalA, terminalB))
    print(g.find_fastest_path(terminalA, terminalC))