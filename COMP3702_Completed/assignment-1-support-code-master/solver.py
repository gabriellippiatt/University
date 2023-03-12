import sys
from laser_tank import LaserTankMap
from queue import *

"""
Template file for you to implement your solution to Assignment 1.

COMP3702 2020 Assignment 1 Support Code

References:
1- Tutorial 2 Code (Regarding BFS and UCS)
2- Manhattan Distance Calculation (https://www.geeksforgeeks.org/sum-manhattan-distances-pairs-points/)
3- Hash Function (https://www.programiz.com/python-programming/methods/built-in/hash)
"""


class State:
    def __init__(self):
        self.children = []
        self.parent = None
        self.previous_action = ""
        self.cost = 0
        self.priority = 0
        self.game = None
        self.flag_position_x = 0
        self.flag_position_y = 0

    def get_child(self, action):
        grid_copy = [row[:] for row in self.game.grid_data]
        new_game = LaserTankMap(self.game.x_size, self.game.y_size, grid_copy, self.game.player_x, self.game.player_y, self.game.player_heading)

        if new_game.apply_move(action) == self.game.SUCCESS:
            new_state = State()
            new_state.parent = self
            new_state.game = new_game
            new_state.previous_action = action
            if action == self.game.MOVE_FORWARD:
                new_state.cost = self.cost + 1
            if action == self.game.TURN_LEFT:
                new_state.cost = self.cost + 3
            if action == self.game.TURN_RIGHT:
                new_state.cost = self.cost + 3
            if action == self.game.SHOOT_LASER:
                new_state.cost = self.cost + 2
            new_state.priority = self.cost + self.manhattan_distance()
            return new_state

    # Reference 2
    def manhattan_distance(self):
        grid_copy = [row[:] for row in self.game.grid_data]
        new_game = LaserTankMap(self.game.x_size, self.game.y_size, grid_copy, self.game.player_x, self.game.player_y, self.game.player_heading)
        distance = 0
        distance += abs(new_game.player_x - self.flag_position_x) + abs(new_game.player_y - self.flag_position_y)
        return distance

    # override equality - needed to check when 2 states are equal
    def __eq__(self, obj):
        """self == obj."""

        # checking the grid data each time is slow
        return self.game.player_x == obj.game.player_x and self.game.player_y == obj.game.player_y and self.game.player_heading == obj.game.player_heading and self.game.grid_data == obj.game.grid_data

    # override less than function for A*
    def __lt__(self, obj):

        # prioritises according to cost and heuristic
        return self.priority < obj.priority

    # hash function (Reference 3)
    def __hash__(self):

        return hash((self.game.player_x, self.game.player_y, self.game.player_heading) + tuple(map(tuple, self.game.grid_data)))


def write_output_file(filename, actions):
    """
    Write a list of actions to an output file. You should use this method to write your output file.
    :param filename: name of output file
    :param actions: list of actions where is action is in LaserTankMap.MOVES
    """
    f = open(filename, 'w')
    for i in range(len(actions)):
        f.write(str(actions[i]))
        if i < len(actions) - 1:
            f.write(',')
    f.write('\n')
    f.close()


def main(arglist):
    input_file = arglist[0]
    output_file = arglist[1]

    # Read the input testcase file
    game_map = LaserTankMap.process_input_file(input_file)

    init_state = State()
    init_state.game = game_map
    found = False

    for i in range(game_map.y_size):
        row = init_state.game.grid_data[i]
        for j in range(game_map.x_size):
            if row[j] == init_state.game.FLAG_SYMBOL:
                found = True
                init_state.flag_position_x = i
                init_state.flag_position_x = j
        if found:
            break

    visited = set()
    pq = PriorityQueue()
    pq.put(init_state)

    while pq:
        state = pq.get()

        if state.game.is_finished():
            # do nothing for now
            break
        for action in state.game.MOVES:
            child = state.get_child(action)
            if child and child not in visited:
                pq.put(child)
                visited.add(child)

    actions = []
    # initial node has no parent
    while state.parent:
        actions.append(state.previous_action)
        state = state.parent
    # reverse it to get the correct order
    actions.reverse()
    print ("Number of actions:", len(actions))
    print (actions)

    write_output_file(output_file, actions)


if __name__ == '__main__':
    main(sys.argv[1:])