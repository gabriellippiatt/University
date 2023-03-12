from laser_tank import LaserTankMap, DotDict
import random
import time

"""
Template file for you to implement your solution to Assignment 4. You should 
implement your solution by filling in the
following method stubs:
    train_q_learning()
    train_sarsa()
    get_policy()

You may add to the __init__ method if required, and can add additional helper 
methods and classes if you wish.

To ensure your code is handled correctly by the autograder, you should avoid 
using any try-except blocks in your
implementation of the above methods (as this can interfere with our time-out 
handling).

COMP3702 2020 Assignment 4 Support Code
"""


class Solver:

    def __init__(self):
        """
        Initialise solver without a Q-value table.
        """

        #
        # TODO
        # You may add code here if you wish (e.g. define constants used by
        # both methods).
        #
        # The allowed time for this method is 1 second.
        #

        self.policy = []
        self.q_values = {}
        self.alpha = 0.05
        self.epsilon = 0.8
        self.depth = 75
        self.states = list()
        self.obstacles = list()

    def remove_obstacles(self, simulator):
        for x in range(simulator.x_size):
            for y in range(simulator.y_size):
                for heading in simulator.DIRECTIONS:
                    simulator.player_x = x
                    simulator.player_y = y
                    simulator.player_heading = heading
                    state = hash(simulator)
                    if not simulator.cell_is_blocked(y, x):
                        state = (x, y, heading)
                        self.states.append(state)
                    else:
                        self.obstacles.append(state)

    def get_action(self, simulator):
        random_value = random.uniform(0, 1)
        if random_value >= self.epsilon:
            random_index = random.randint(0, 3)
            return simulator.MOVES[random_index]
        else:
            return self.get_policy(simulator)

    def train_q_learning(self, simulator):
        """
        Train the agent using Q-learning, building up a table of Q-values.
        :param simulator: A simulator for collecting episode data (
        LaserTankMap instance)
        """

        # Q(s, a) table
        # suggested format: key = hash(state), value = dict(mapping actions
        # to values)

        # self.q_values = {state: {} for state in self.all_possible_states}

        # store the computed Q-values
        # self.q_values = q_values

        #
        # TODO
        # Write your Q-Learning implementation here.
        #
        # When this method is called, you are allowed up to [
        # state.time_limit] seconds of compute time. You should
        # continue training until the time limit is reached.
        #

        self.remove_obstacles(simulator)

        start_time = time.time()

        while True:
            if time.time() - start_time > simulator.time_limit:
                break

            simulator.reset_to_start()
            x, y, heading = random.choice(self.states)
            simulator.player_x = x
            simulator.player_y = y
            simulator.player_heading = heading

            random_state = hash(simulator)

            if random_state not in self.q_values:
                self.q_values.update({random_state: {'f': 0.0, 'l': 0.0,
                                                     'r': 0.0, 's': 0.0}})

            i = 0
            is_done = False

            while i <= self.depth and not is_done:

                action = self.get_action(simulator)

                current_state = hash(simulator)
                reward, is_done = simulator.apply_move(action)
                next_state = hash(simulator)

                # Check if next state is in dict
                if next_state not in self.q_values:
                    self.q_values.update({next_state: {'f': 0.0, 'l': 0.0,
                                                       'r': 0.0, 's': 0.0}})

                self.q_values[current_state][action] = self.q_values[current_state][action] + \
                    self.alpha * (reward + simulator.gamma * (max(self.q_values[next_state].values())) -
                                  self.q_values[current_state][action])


    def train_sarsa(self, simulator):
        """
        Train the agent using SARSA, building up a table of Q-values.
        :param simulator: A simulator for collecting episode data (LaserTankMap instance)
        """

        # Q(s, a) table
        # suggested format: key = hash(state), value = dict(mapping actions to values)

        #
        # TODO
        # Write your SARSA implementation here.
        #
        # When this method is called, you are allowed up to [state.time_limit] seconds of compute time. You should
        # continue training until the time limit is reached.
        #

        # store the computed Q-values
        self.remove_obstacles(simulator)

        start_time = time.time()

        while True:
            if time.time() - start_time > simulator.time_limit:
                break

            simulator.reset_to_start()
            x, y, heading = random.choice(self.states)
            simulator.player_x = x
            simulator.player_y = y
            simulator.player_heading = heading

            random_state = hash(simulator)

            if random_state not in self.q_values:
                self.q_values.update({random_state: {'f': 0.0, 'l': 0.0,
                                                     'r': 0.0, 's': 0.0}})

            action = self.get_action(simulator)

            i = 0
            is_done = False

            while i <= self.depth and not is_done:

                state_key = hash(simulator)
                reward, is_done = simulator.apply_move(action)
                next_state_key = hash(simulator)

                # Check if next state is in dict
                if next_state_key not in self.q_values:
                    self.q_values.update({next_state_key: {'f': 0.0, 'l': 0.0,
                                                       'r': 0.0, 's': 0.0}})

                next_action = self.get_action(simulator)

                self.q_values[state_key][action] \
                    = self.q_values[state_key][action] + \
                      self.alpha * (reward + simulator.gamma
                                    * self.q_values[next_state_key][next_action]
                                    - self.q_values[state_key][action])

                action = next_action

    def get_policy(self, state):
        """
        Get the policy for this state (i.e. the action that should be performed at this state).
        :param state: a LaserTankMap instance
        :return: pi(s) [an element of LaserTankMap.MOVES]
        """

        #
        # TODO
        # Write code to return the optimal action to be performed at this state based on the stored Q-values.
        #
        # You can assume that either train_q_learning( ) or train_sarsa( ) has been called before this
        # method is called.
        #
        # When this method is called, you are allowed up to 1 second of compute time.
        #

        policy_state_key = hash(state)
        return max(self.q_values[policy_state_key], key=self.q_values[policy_state_key].get)