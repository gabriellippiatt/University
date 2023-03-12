from laser_tank import LaserTankMap, DotDict
import time


"""
Template file for you to implement your solution to Assignment 3. You should implement your solution by filling in the
following method stubs:
    run_value_iteration()
    run_policy_iteration()
    get_offline_value()
    get_offline_policy()
    get_mcts_policy()
    
You may add to the __init__ method if required, and can add additional helper methods and classes if you wish.

To ensure your code is handled correctly by the autograder, you should avoid using any try-except blocks in your
implementation of the above methods (as this can interfere with our time-out handling).

COMP3702 2020 Assignment 3 Support Code
"""

ACTIONS = ['f', 'l', 'r', 'fr', 'fl', 's', 'tl', 'tr']

class Solver:

    def __init__(self, game_map):
        self.game_map = game_map

        #
        # TODO
        # Write any environment preprocessing code you need here (e.g. storing teleport locations).
        #
        # You may also add any instance variables (e.g. root node of MCTS tree) here.
        #
        # The allowed time for this method is 1 second, so your Value Iteration or Policy Iteration implementation
        # should be in the methods below, not here.
        #

        # self.values = dict()
        self.values = [[[0 for _ in LaserTankMap.DIRECTIONS]
                    for __ in range(1, self.game_map.y_size)]
                   for ___ in range(1, self.game_map.x_size)]

        self.policy = None
        self.states = []
        self.rewards = []
        self.discount = self.game_map.gamma
        self.MoveF = 'f'
        self.MoveL = 'l'
        self.MoveR = 'r'
        self.MoveFR = 'fr'
        self.MoveFL = 'fl'
        self.MoveS = 's'
        self.TurnL = 'tl'
        self.TurnR = 'tr'
        self.converged = False

    def get_states(self):
        print(type(self.game_map))
        print("ysize", self.game_map.y_size)
        print("xsize", self.game_map.x_size)

        for i in range(self.game_map.y_size):
            for j in range(self.game_map.x_size):
                if not self.game_map.cell_is_blocked(i, j) and \
                        not self.game_map.cell_is_game_over(i, j) and \
                        not self.is_game_finished(j, i):
                    for heading in self.game_map.DIRECTIONS:
                        state = (j, i , heading)
                        self.states.append(state)
                if self.game_map.cell_is_game_over(i, j) or \
                        self.is_game_finished(j, i):
                    for heading in self.game_map.DIRECTIONS:
                        reward_state = (j, i , heading)
                        self.rewards.append(reward_state)
        return self.states

    def is_game_finished(self, x, y):
        if x == self.game_map.flag_x and y == self.game_map.flag_y:
            return True
        else:
            return False

    def attempt_action(self, state, action):

        # return the new state if it is valid otherwise return original state

        # rewrite apply move to make it deterministic
        attempt_x, attempt_y, attempt_heading = state
        new_state = state

        if action == self.MoveF:
            if attempt_heading == self.game_map.UP:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x):
                    new_state = attempt_x, attempt_y - 1, attempt_heading

            elif attempt_heading == self.game_map.DOWN:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x):
                    new_state = attempt_x, attempt_y + 1, attempt_heading

            elif attempt_heading == self.game_map.RIGHT:
                if not self.game_map.cell_is_blocked(attempt_y, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y, attempt_heading

            elif attempt_heading == self.game_map.LEFT:
                if not self.game_map.cell_is_blocked(attempt_y, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y, attempt_heading

        elif action == self.MoveL:
            if attempt_heading == self.game_map.UP:
                if not self.game_map.cell_is_blocked(attempt_y, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y, attempt_heading

            elif attempt_heading == self.game_map.DOWN:
                if not self.game_map.cell_is_blocked(attempt_y, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y, attempt_heading

            elif attempt_heading == self.game_map.RIGHT:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x):
                    new_state = attempt_x, attempt_y - 1, attempt_heading

            elif attempt_heading == self.game_map.LEFT:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x):
                    new_state = attempt_x, attempt_y + 1, attempt_heading

        elif action == self.MoveR:
            if attempt_heading == self.game_map.UP:
                if not self.game_map.cell_is_blocked(attempt_y, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y, attempt_heading

            elif attempt_heading == self.game_map.DOWN:
                if not self.game_map.cell_is_blocked(attempt_y, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y, attempt_heading

            elif attempt_heading == self.game_map.RIGHT:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x):
                    new_state = attempt_x, attempt_y + 1, attempt_heading

            elif attempt_heading == self.game_map.LEFT:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x):
                    new_state = attempt_x, attempt_y - 1, attempt_heading

        elif action == self.MoveFR:
            if attempt_heading == self.game_map.UP:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y - 1, attempt_heading

            elif attempt_heading == self.game_map.DOWN:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y + 1, attempt_heading

            elif attempt_heading == self.game_map.RIGHT:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y + 1, attempt_heading

            elif attempt_heading == self.game_map.LEFT:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y - 1, attempt_heading

        elif action == self.MoveFL:
            if attempt_heading == self.game_map.UP:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y - 1, attempt_heading

            elif attempt_heading == self.game_map.DOWN:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y + 1, attempt_heading

            elif attempt_heading == self.game_map.RIGHT:
                if not self.game_map.cell_is_blocked(attempt_y - 1, attempt_x + 1) and \
                        not self.game_map.cell_is_game_over(attempt_y - 1, attempt_x + 1) and \
                        not self.is_game_finished(attempt_y - 1, attempt_x + 1):
                    new_state = attempt_x + 1, attempt_y - 1, attempt_heading

            elif attempt_heading == self.game_map.LEFT:
                if not self.game_map.cell_is_blocked(attempt_y + 1, attempt_x - 1) and \
                        not self.game_map.cell_is_game_over(attempt_y + 1, attempt_x - 1) and \
                        not self.is_game_finished(attempt_y + 1, attempt_x - 1):
                    new_state = attempt_x - 1, attempt_y + 1, attempt_heading

        elif action == self.MoveS:
            new_state = state

        elif action == self.TurnL:
            if attempt_heading == self.game_map.UP:
                new_state = attempt_x, attempt_y, self.game_map.LEFT
            elif attempt_heading == self.game_map.DOWN:
                new_state = attempt_x, attempt_y, self.game_map.RIGHT
            elif attempt_heading == self.game_map.LEFT:
                new_state = attempt_x, attempt_y, self.game_map.DOWN
            elif attempt_heading == self.game_map.RIGHT:
                new_state = attempt_x, attempt_y, self.game_map.UP

        elif action == self.TurnR:
            if attempt_heading == self.game_map.UP:
                new_state = attempt_x, attempt_y, self.game_map.RIGHT
            elif attempt_heading == self.game_map.DOWN:
                new_state = attempt_x, attempt_y, self.game_map.LEFT
            elif attempt_heading == self.game_map.LEFT:
                new_state = attempt_x, attempt_y, self.game_map.UP
            elif attempt_heading == self.game_map.RIGHT:
                new_state = attempt_x, attempt_y, self.game_map.DOWN

        return new_state

    def action_probability(self, action):

        if action == self.game_map.MOVE_FORWARD:
            act_dict = {self.MoveF: self.game_map.t_success_prob,
                        self.MoveL: (1 - self.game_map.t_success_prob)/5,
                        self.MoveR: (1 - self.game_map.t_success_prob)/5,
                        self.MoveFR: (1 - self.game_map.t_success_prob) / 5,
                        self.MoveFL: (1 - self.game_map.t_success_prob) / 5,
                        self.MoveS: (1 - self.game_map.t_success_prob)/5,
                        self.TurnL: 0,
                        self.TurnR: 0}
        elif action == self.game_map.TURN_LEFT:
            act_dict = {self.MoveF: 0,
                        self.MoveL: 0,
                        self.MoveR: 0,
                        self.MoveFR: 0,
                        self.MoveFL: 0,
                        self.MoveS: 0,
                        self.TurnL: 1.0,
                        self.TurnR: 0}
        elif action == self.game_map.TURN_RIGHT:
            act_dict = {self.MoveF: 0,
                        self.MoveL: 0,
                        self.MoveR: 0,
                        self.MoveFR: 0,
                        self.MoveFL: 0,
                        self.MoveS: 0,
                        self.TurnL: 0,
                        self.TurnR: 1.0}
        else:
            act_dict = {self.MoveF: 0,
                        self.MoveL: 0,
                        self.MoveR: 0,
                        self.MoveFR: 0,
                        self.MoveFL: 0,
                        self.MoveS: 0,
                        self.TurnL: 0,
                        self.TurnR: 0}

        return act_dict

    def action_reward(self, state):
        attempt_x, attempt_y, attempt_heading = state
        if self.game_map.cell_is_blocked(attempt_y, attempt_x):
            return self.game_map.collision_cost
        elif self.game_map.cell_is_game_over(attempt_y, attempt_x):
            return self.game_map.game_over_cost
        elif attempt_x == self.game_map.flag_x and attempt_y == self.game_map.flag_y:
            return self.game_map.goal_reward
        else:
            return self.game_map.move_cost

    def next_iteration(self):
        values = [[[0 for _ in LaserTankMap.DIRECTIONS]
                    for __ in range(1, self.game_map.y_size)]
                   for ___ in range(1, self.game_map.x_size)]

        policy = [[[0 for _ in LaserTankMap.DIRECTIONS]
                   for __ in range(1, self.game_map.y_size)]
                  for ___ in range(1, self.game_map.x_size)]
        # new_values = dict()
        for s in self.get_states():
            # Keep track of maximum value
            action_values = list()
            for a in self.game_map.MOVES:
                total = 0
                for act, prob in self.action_probability(a).items():
                    # # Apply action
                    # print("act", act)
                    # print("prob", prob)
                    # print("a", a)
                    # print("s", s)
                    s_next = self.attempt_action(s, act)
                    total += prob * (self.action_reward(s) + (self.game_map.gamma * self.values[s_next[0]][s_next[1]][s_next[2]]))
                action_values.append(total)
            # Update state value with best action
        values[s_next[0]][s_next[1]][s_next[2]] = max(action_values)

        # Check convergence
        differences = [abs(self.values[s[0]][s[1]][s[2]] - values[s[0]][s[1]][s[2]]) for s in self.get_states()]
        if max(differences) < self.game_map.epsilon:
            self.converged = True

        # Update values
        self.values = values

    def run_value_iteration(self):
        """
        Build a value table and a policy table using value iteration, and store inside self.values and self.policy.
        """
        # make into dictionary

        # # self.values = [[[0 for _ in LaserTankMap.DIRECTIONS]
        # #            for __ in range(1, self.game_map.y_size - 1)]
        # #           for ___ in range(1, self.game_map.x_size - 1)]
        # policy = [[[-1 for _ in LaserTankMap.DIRECTIONS]
        #            for __ in range(1, self.game_map.y_size - 1)]
        #           for ___ in range(1, self.game_map.x_size - 1)]

        # for state in self.get_states():
        #     # print(state)
        #     self.values[state] = 0.0

        start = time.time()
        count = 0
        while not self.converged:
            count += 1
            print("num", count)
            self.next_iteration()
            end = time.time()
            if end - start >= self.game_map.time_limit:
                break

        #
        # TODO
        # Write your Value Iteration implementation here.
        #
        # When this method is called, you are allowed up to [state.time_limit] seconds of compute time. You should stop
        # iterating either when max_delta < epsilon, or when the time limit is reached, whichever occurs first.
        #

        # store the computed values and policy
        # self.values = values
        # self.policy = policy

    def run_policy_iteration(self):
        """
        Build a value table and a policy table using policy iteration, and store inside self.values and self.policy.
        """
        values = [[[0 for _ in LaserTankMap.DIRECTIONS]
                   for __ in range(1, self.game_map.y_size - 1)]
                  for ___ in range(1, self.game_map.x_size - 1)]
        policy = [[[-1 for _ in LaserTankMap.DIRECTIONS]
                   for __ in range(1, self.game_map.y_size - 1)]
                  for ___ in range(1, self.game_map.x_size - 1)]

        #
        # TODO
        # Write your Policy Iteration implementation here.
        #
        # When this method is called, you are allowed up to [state.time_limit] seconds of compute time. You should stop
        # iterating either when max_delta < epsilon, or when the time limit is reached, whichever occurs first.
        #

        # store the computed values and policy
        self.values = values
        self.policy = policy

    def get_offline_value(self, state):
        """
        Get the value of this state.
        :param state: a LaserTankMap instance
        :return: V(s) [a floating point number]
        """

        #
        # TODO
        # Write code to return the value of this state based on the stored self.values
        #
        # You can assume that either run_value_iteration( ) or run_policy_iteration( ) has been called before this
        # method is called.
        #
        # When this method is called, you are allowed up to 1 second of compute time.
        #

        return self.values[state]

    def get_offline_policy(self, state):
        """
        Get the policy for this state (i.e. the action that should be performed at this state).
        :param state: a LaserTankMap instance
        :return: pi(s) [an element of LaserTankMap.MOVES]
        """

        #
        # TODO
        # Write code to return the optimal action to be performed at this state based on the stored self.policy
        #
        # You can assume that either run_value_iteration( ) or run_policy_iteration( ) has been called before this
        # method is called.
        #
        # When this method is called, you are allowed up to 1 second of compute time.
        #

        return self.policy

    def get_mcts_policy(self, state):
        """
        Choose an action to be performed using online MCTS.
        :param state: a LaserTankMap instance
        :return: pi(s) [an element of LaserTankMap.MOVES]
        """

        #
        # TODO
        # Write your Monte-Carlo Tree Search implementation here.
        #
        # Each time this method is called, you are allowed up to [state.time_limit] seconds of compute time - make sure
        # you stop searching before this time limit is reached.
        #

        pass


# if __name__ == '__main__':
#     main(sys.argv[1:])

map1 = LaserTankMap.process_input_file("testcases/vi_t0.txt")
solv = Solver(map1)
solv.run_value_iteration()





