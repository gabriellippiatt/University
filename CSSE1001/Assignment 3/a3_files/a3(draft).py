import tkinter as tk
import math


from tkinter import messagebox
from advanced_view import TowerView
from model import TowerGame
from tower import SimpleTower, MissileTower, PulseTower, AbstractTower
from enemy import SimpleEnemy, AbstractEnemy
from utilities import Stepper
from view import GameView
from level import AbstractLevel
from utilities import rectangles_intersect, get_delta_through_centre
from range_ import CircularRange

BACKGROUND_COLOUR = "#4a2f48"

__author__ = "Gabriel Lippiatt"
__copyright__ = "Copyright 2018, The University of Queensland"
__date__ = "01/06/2018"

class EnergyEnemy(AbstractEnemy):
    """Basic type of enemy"""
    name = "New Enemy"
    colour = 'green'  # Green

    points = 20

    def __init__(self, grid_size=(.2, .2), grid_speed=15/60, health=30):
        super().__init__(grid_size, grid_speed, health)

    def damage(self, damage, type_):
        """Inflict damage on the enemy

        Parameters:
            damage (int): The amount of damage to inflict
            type_ (str): The type of damage to do i.e. projectile, explosive
        """

        if type_ == "energy":
            self.health -= damage
        if self.health < 0:
            self.health = 0

    def step(self, data):
        """Move the enemy forward a single time-step

        Parameters:
            grid (GridCoordinateTranslator): Grid the enemy is currently on
            path (Path): The path the enemy is following

        Returns:
            bool: True iff the new location of the enemy is within the grid
        """
        grid = data.grid
        path = data.path

        # Repeatedly move toward next cell centre as much as possible
        movement = self.grid_speed
        while movement > 0:
            cell_offset = grid.pixel_to_cell_offset(self.position)

            # Assuming cell_offset is along an axis!
            offset_length = abs(cell_offset[0] + cell_offset[1])

            if offset_length == 0:
                partial_movement = movement
            else:
                partial_movement = min(offset_length, movement)

            cell_position = grid.pixel_to_cell(self.position)
            delta = path.get_best_delta(cell_position)

            # Ensures enemy will move to the centre before moving toward delta
            dx, dy = get_delta_through_centre(cell_offset, delta)

            speed = partial_movement * self.cell_size
            self.move_by((speed * dx, speed * dy))
            self.position = tuple(int(i) for i in self.position)

            movement -= partial_movement

        intersects = rectangles_intersect(*self.get_bounding_box(), (0, 0), grid.pixels)
        return intersects or grid.pixel_to_cell(self.position) in path.deltas

class EnergyTower(AbstractTower):
    """A simple tower with short range that rotates towards enemies and shoots projectiles at them"""
    name = 'New Tower'
    colour = 'purple'  # Purple

    range = CircularRange(2)
    cool_down_steps = 0

    base_cost = 30
    level_cost = 15

    rotation_threshold = (1 / 6) * math.pi

    def __init__(self, cell_size: int, grid_size=(.9, .9), rotation=math.pi * .25, base_damage=5, level: int = 1):
        super().__init__(cell_size, grid_size, rotation, base_damage, level)

    def step(self, data):
        """Rotates toward 'target' and attacks if possible"""
        self.cool_down.step()

        target = self.get_unit_in_range(data.enemies)

        if target is None:
            return

        angle = angle_between(self.position, target.position)
        partial_angle = rotate_toward(self.rotation, angle, self.rotation_threshold)
        self.rotation = partial_angle

        if partial_angle == angle:
            target.damage(self.get_damage(), 'energy')

# Could be moved to a separate file, perhaps levels/simple.py, and imported
class MyLevel(AbstractLevel):
    """A simple game level containing examples of how to generate a wave"""
    waves = 20

    def get_wave(self, wave):
        """Returns enemies in the 'wave_n'th wave

        Parameters:
            wave_n (int): The nth wave

        Return:
            list[tuple[int, AbstractEnemy]]: A list of (step, enemy) pairs in the
                                             wave, sorted by step in ascending order 
        """
        enemies = []

        if wave == 1:
            # A hardcoded singleton list of (step, enemy) pairs

            enemies = [(10, EnergyEnemy())]
        elif wave == 2:
            # A hardcoded list of multiple (step, enemy) pairs

            enemies = [(10, SimpleEnemy()), (15, SimpleEnemy()), (30, SimpleEnemy())]
        elif 3 <= wave < 10:
            # List of (step, enemy) pairs spread across an interval of time (steps)

            steps = int(40 * (wave ** .5))  # The number of steps to spread the enemies across
            count = wave * 2  # The number of enemies to spread across the (time) steps

            for step in self.generate_intervals(steps, count):
                enemies.append((step, SimpleEnemy()))

        elif wave == 10:
            # Generate sub waves
            sub_waves = [
                # (steps, number of enemies, enemy constructor, args, kwargs)
                (50, 10, SimpleEnemy, (), {}),  # 10 enemies over 50 steps
                (100, None, None, None, None),  # then nothing for 100 steps
                (50, 10, SimpleEnemy, (), {})  # then another 10 enemies over 50 steps
            ]

            enemies = self.generate_sub_waves(sub_waves)

        else:  # 11 <= wave <= 20
            # Now it's going to get hectic

            sub_waves = [
                (
                    int(13 * wave),  # total steps
                    int(25 * wave ** (wave / 50)),  # number of enemies
                    SimpleEnemy,  # enemy constructor
                    (),  # positional arguments to provide to enemy constructor
                    {},  # keyword arguments to provide to enemy constructor
                ),
                # ...
            ]
            enemies = self.generate_sub_waves(sub_waves)

        return enemies


class TowerGameApp(Stepper):
    """Top-level GUI application for a simple tower defence game"""

    # All private attributes for ease of reading
    _current_tower = None
    _paused = False
    _won = None

    _level = None
    _wave = None
    _score = None
    _coins = None
    _lives = None

    _master = None
    _game = None
    _view = None

    def __init__(self, master: tk.Tk, delay: int = 20):
        """Construct a tower defence game in a root window

        Parameters:
            master (tk.Tk): Window to place the game into
        """
        master.title("TOWERS OF GONDOR")
        self._master = master

        super().__init__(master, delay=delay)

        self._game = game = TowerGame()

        self._right_frame = RightFrame(master)
        self._right_frame.pack(side = tk.RIGHT)

        self.setup_menu()

        # create a game view and draw grid borders
        self._view = view = GameView(master, size=game.grid.cells,
                                     cell_size=game.grid.cell_size,
                                     bg='light goldenrod')
        view.pack(side=tk.LEFT, expand=True)

        # Task 1.3 (Status Bar): instantiate status bar
        # ...
        self._statusbar = StatusBar(master)
        self._statusbar.pack(side=tk.RIGHT, anchor = tk.N)
        self._statusbar.HUD()

        towers = [
            SimpleTower,
            MissileTower,
            PulseTower,
            EnergyTower
        ]

        shop = tk.Frame()
        shop.pack(side = tk.RIGHT, anchor= tk.W)

        # Create views for each tower & store to update if availability changes
        self._tower_views = []
        for tower_class in towers:
            tower = tower_class(self._game.grid.cell_size // 2)

            shop_view = ShopTowerView(shop, tower,  # bg=BACKGROUND_COLOUR, highlight="#4b3b4a",
                                      click_command=lambda class_=tower_class: self.select_tower(class_))
            shop_view.pack()
            self._tower_views.append((tower, shop_view))  # Can use to check if tower is affordable when refreshing view

        # Task 1.5 (Play Controls): instantiate widgets here'{:

        self._player_controls = PlayerControls(master, towergameapp=self)
        self._player_controls.pack(side=tk.BOTTOM)

        # bind game events
        game.on("enemy_death", self._handle_death)
        game.on("enemy_escape", self._handle_escape)
        game.on("cleared", self._handle_wave_clear)

        # Task 1.2 (Tower Placement): bind mouse events to canvas here
        # ...
        view.bind("<Button-1>", self._left_click)
        view.bind("<Button-3>", self._right_click)
        view.bind("<Motion>", self._move)
        view.bind("<Leave>", self._mouse_leave)

        # Level
        self._level = MyLevel()

        self.select_tower(EnergyTower)

        view.draw_borders(game.grid.get_border_coordinates())

        # Get ready for the game
        self._setup_game()


        # Task 1.5 (Play Controls): remove this line                         *WHY WOULD YOU REMOVE THIS LINE?*
        self.start()

    def setup_menu(self):
        # Task 1.4: construct file menu here
        # ...

        menubar = tk.Menu(self._master)
        self._master.config(menu=menubar)

        menubar.add_command(label="New Game", command=self._new_game)
        menubar.add_command(label="Exit", command=self._exit)

    def _toggle_paused(self, paused=None):
        """Toggles or sets the paused state

        Parameters:
            paused (bool): Toggles/pauses/unpauses if None/True/False, respectively
        """
        if paused is None:
            paused = not self._paused

        # Task 1.5 (Play Controls): Reconfigure the pause button here
        # ...

        if paused == True:
            self._player_controls._play_button.config(text = "Unpause")
        else :
            self._player_controls._play_button.config(text="Pause")


        if paused:
            self.pause()
        else:
            self.start()

        self._paused = paused

    def _setup_game(self):
        """Sets up game"""
        self._wave = 0
        self._score = 0
        self._coins = 7000
        self._lives = 20

        self._won = False

        # Task 1.3 (Status Bar): Update status here
        # ...
        self._statusbar.wave_status(self._wave)
        self._statusbar.score_status(self._score)
        self._statusbar.coins_status(self._coins)
        self._statusbar.lives_status(self._lives)

        # Task 1.5 (Play Controls): Re-enable the play controls here (if they were ever disabled)
        # ...
        self._player_controls._next_wave_button.config(state=tk.NORMAL)
        self._player_controls._play_button.config(state=tk.NORMAL)
        self._game.reset()

    # Task 1.4 (File Menu): Complete menu item handlers here (including docstrings!)
    #
    def _new_game(self):

        self._setup_game()
        self.start()


    def _exit(self):

        self._toggle_paused(paused=True)
        exit_message = messagebox.askyesno("Exit", "Are you sure you want to exit?")
        if  exit_message == True:
            self._master.quit()
        else:
            self._toggle_paused(paused=False)

    def refresh_view(self):
        """Refreshes the game view"""
        if self._step_number % 2 == 0:
            self._view.draw_enemies(self._game.enemies)
        self._view.draw_towers(self._game.towers)
        self._view.draw_obstacles(self._game.obstacles)

    def _step(self):
        """
        Perform a step every interval

        Triggers a game step and updates the view

        Returns:
            (bool) True if the game is still running
        """
        self._game.step()
        self.refresh_view()

        return not self._won

    # Task 1.2 (Tower Placement): Complete event handlers here (including docstrings!)
    # Event handlers: _move, _mouse_leave, _left_click
    def _move(self, event):
        """
        Handles the mouse moving over the game view canvas

        Parameter:
            event (tk.Event): Tkinter mouse event
        """
        if self._current_tower.get_value() > self._coins:
            self._view.delete('shadow', 'path', 'range')
            return

        # move the shadow tower to mouse position
        position = event.x, event.y
        self._current_tower.position = position

        legal, grid_path = self._game.attempt_placement(position)

        # find the best path and covert positions to pixel positions
        path = [self._game.grid.cell_to_pixel_centre(position)
                for position in grid_path.get_shortest()]

        self._view.draw_path(path)
        self._view.draw_preview(self._current_tower, legal)

    def _mouse_leave(self, event):
        """..."""
        # Task 1.2 (Tower placement): Delete the preview
        # Hint: Relevant canvas items are tagged with: 'path', 'range', 'shadow'
        #       See tk.Canvas.delete (delete all with tag)
        self._view.delete('shadow', 'path', 'range')

    def _left_click(self, event):
        """..."""
        # retrieve position to place tower
        if self._current_tower is None:
            return

        self._position_left = event.x, event.y
        self._cell_position_left = self._game.grid.pixel_to_cell(self._position_left)

            # Task 1.2 (Tower placement): Attempt to place the tower being previewed
        if self._coins >= self._current_tower.get_value():
            if self._game.place(self._cell_position_left, tower_type=self._current_tower.__class__):
                self._coins -= self._current_tower.get_value()
        self._view.delete('shadow', 'path', 'range')

    def _right_click(self, event):
        """..."""
        # retrieve position to place tower

        self._position_right = event.x, event.y
        self._cell_position_right = self._game.grid.pixel_to_cell(self._position_right)
        sold_tower = self._game.towers[self._position_right]
        try :
            self._game.remove(self._cell_position_right)
            self._coins += int(sold_tower.get_value()*0.8)
        except Exception:
            pass

    def next_wave(self):
        """Sends the next wave of enemies against the player"""
        if self._wave == self._level.get_max_wave():
            return

        self._wave += 1

        # Task 1.3 (Status Bar): Update the current wave display here
        # ...
        self._statusbar.wave_status(self._wave)

        # Task 1.5 (Play Controls): Disable the add wave button here (if this is the last wave)
        # ...

        # Generate wave and enqueue
        wave = self._level.get_wave(self._wave)
        for step, enemy in wave:
            enemy.set_cell_size(self._game.grid.cell_size)

        self._game.queue_wave(wave)

    def select_tower(self, tower):
        """
        Set 'tower' as the current tower

        Parameters:
            tower (AbstractTower): The new tower type
        """
        self._current_tower = tower(self._game.grid.cell_size)

    def _handle_death(self, enemies):
        """
        Handles enemies dying

        Parameters:
            enemies (list<AbstractEnemy>): The enemies which died in a step
        """
        bonus = len(enemies) ** .5
        for enemy in enemies:
            self._coins += enemy.points
            self._score += int(enemy.points * bonus)

        # Task 1.3 (Status Bar): Update coins & score displays here
        # ...
        self._statusbar.score_status(self._score)
        self._statusbar.coins_status(self._coins)

    def _handle_escape(self, enemies):
        """
        Handles enemies escaping (not being killed before moving through the grid

        Parameters:
            enemies (list<AbstractEnemy>): The enemies which escaped in a step
        """
        self._lives -= len(enemies)
        if self._lives < 0:
            self._lives = 0

        # Task 1.3 (Status Bar): Update lives display here
        # ...
        self._statusbar.lives_status(self._lives)

        # Handle game over
        if self._lives == 0:
            self._handle_game_over(won=False)

    def _handle_wave_clear(self):
        """Handles an entire wave being cleared (all enemies killed)"""
        if self._wave == self._level.get_max_wave():
            self._handle_game_over(won=True)

    def _handle_game_over(self, won=False):
        """Handles game over
        
        Parameter:
            won (bool): If True, signals the game was won (otherwise lost)
        """
        self._won = won
        self.stop()

        # Task 1.4 (Dialogs): show game over dialog here
        # ...
        if won == True:
            self._game_over_win = messagebox._show("GAME OVER", "YOU WIN! PLAY AGAIN!")
        else:
            self._game_over_loss = messagebox._show("GAME OVER", "YOU LOSE! TRY AGAIN!")

        self._player_controls._next_wave_button.config(state=tk.DISABLED)
        self._player_controls._play_button.config(state=tk.DISABLED)

class RightFrame(tk.Frame):

    def __init__(self, master):
        self._master = master
        super().__init__(master)
        #create right side frame

        self._abc = tk.Label(self, text="T", font='Helvetica 9 bold', bg="blue", width=40)
        self._abc.pack(side=tk.TOP)



class StatusBar(tk.Frame):

    def __init__(self, master):
        self._master = master
        super().__init__(master)

    def HUD(self):

        self._header = tk.Label(self, text="SURVIVE THE ONSLAUGHT", font='Helvetica 9 bold', bg="goldenrod", width=40)
        self._header.pack(side=tk.TOP)

        self._wave_status = tk.Label(self, text="Wave: 1/20", bg="white", width = 40)
        self._wave_status.pack(side=tk.TOP)

        self._score_status = tk.Label(self, text="Score: 0", bg="white", width=40)
        self._score_status.pack(side=tk.TOP)

        coin_icon = tk.PhotoImage(file='images/coins.gif')
        coin_icon_label = tk.Label(self, image=coin_icon, bg="white")
        coin_icon_label.image = coin_icon
        coin_icon_label.pack(side=tk.LEFT, anchor=tk.N)

        self._coins_status = tk.Label(self, text="60 coins", bg="white",  width=14, height=2)
        self._coins_status.pack(side=tk.LEFT, anchor = tk.N, expand= True, fill = tk.X)

        heart_icon = tk.PhotoImage(file='images/heart.gif')
        heart_icon_label = tk.Label(self, image=heart_icon, bg="white")
        heart_icon_label.image = heart_icon
        heart_icon_label.pack(side=tk.LEFT, anchor=tk.N)

        self._lives_status = tk.Label(self, text="20 lives", bg="white",  width=14, height=2)
        self._lives_status.pack(side=tk.LEFT, anchor = tk.N, expand= True, fill = tk.X)

    def wave_status(self, _wave):

        self._wave_status.config(text='Wave: ' + str(_wave) + '/20')

    def score_status(self, _score):

        self._score_status.config(text='Score: ' + str(_score))

    def coins_status(self, _coins):

        self._coins_status.config(text=str(_coins) + ' coins')

    def lives_status(self, _lives):

        self._lives_status.config(text=str(_lives) + ' lives')

class ShopTowerView(tk.Frame):

    def __init__(self, master, tower, click_command):
        super().__init__(master)

        #self._right_frame = RightFrame(master)

        canvas = tk.Canvas(master, height=30)
        canvas.pack()


        #self._master = master
        #self._tower = tower
        #self._click_command = click_command


        tower.position = (tower.cell_size // 2, tower.cell_size // 2)  # Position in centre
        tower.rotation = 3 * math.pi / 2  # Point up
        TowerView.draw(canvas, tower)


    # Don't forget to import TowerView from tower_view.py!

class PlayerControls(tk.Frame) :

    def __init__(self, master, towergameapp):

        super().__init__(master)


        self._start_message = tk.Label(self, text="Press the Next Wave button to start the game", font='Helvetica 9 bold', bg="goldenrod", width=40)
        self._start_message.pack(side=tk.TOP)
        self._next_wave_button = tk.Button(self, text="Next Wave", bg="white", command = towergameapp.next_wave)
        self._next_wave_button.pack(padx = 1, ipady= 8, ipadx= 10, side=tk.LEFT, fill = tk.BOTH, expand = True)
        self._play_button = tk.Button(self, text="Pause", bg="white", command = towergameapp._toggle_paused)
        self._play_button.pack(padx = 1, ipady= 8, ipadx= 10, side=tk.RIGHT, fill = tk.BOTH, expand = True)


def main() :
        root = tk.Tk()
        # Gets the requested values of the height and width.
        windowWidth = root.winfo_reqwidth()
        windowHeight = root.winfo_reqheight()

        # Gets both half the screen width/height and window width/height
        positionRight = int(root.winfo_screenwidth() / 3 - windowWidth / 2)
        positionDown = int(root.winfo_screenheight() / 3 - windowHeight / 2)

        # Positions the window in the center of the page.
        root.geometry("+{}+{}".format(positionRight, positionDown))

        root.config(bg = "dark goldenrod")
        app = TowerGameApp(root)
        root.mainloop()

if __name__ == "__main__" :
    main()