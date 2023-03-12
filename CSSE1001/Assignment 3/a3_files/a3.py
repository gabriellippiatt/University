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
from utilities import rectangles_intersect, get_delta_through_centre, rotate_toward, angle_between
from range_ import CircularRange

BACKGROUND_COLOUR = "light goldenrod"

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

        # instantiates the file menu
        self.setup_menu()

        # create a game view and draw grid borders
        self._view = view = GameView(master, size=game.grid.cells,
                                     cell_size=game.grid.cell_size,
                                     bg='light goldenrod')
        view.pack(side=tk.LEFT, expand=True)

        # Instantiate status bar
        self._statusbar = StatusBar(master)
        self._statusbar.pack(side=tk.TOP, anchor = tk.N)
        self._statusbar.HUD()

        # Create a list of towers for the shop
        towers = [
            SimpleTower,
            EnergyTower,
            MissileTower,
            PulseTower
        ]
        # Create a frame for the Shop View
        shop = tk.Frame(bg = "light goldenrod")
        shop.pack(fill=tk.X)

        # Create views for each tower & store to update if availability changes
        self._tower_views = []
        for tower_class in towers:
            tower = tower_class(self._game.grid.cell_size // 1.3)

            shop_view = ShopTowerView(shop, tower,
                                      click_command=lambda class_=tower_class: self.select_tower(class_))
            shop_view.pack(expand = True, fill = tk.X)

            self._tower_views.append((tower, shop_view)) # Can use to check if tower is affordable when refreshing view

        # Instantiate player controls
        self._player_controls = PlayerControls(master, towergameapp=self)
        self._player_controls.pack(side=tk.BOTTOM)

        # bind game events
        game.on("enemy_death", self._handle_death)
        game.on("enemy_escape", self._handle_escape)
        game.on("cleared", self._handle_wave_clear)

        # bind mouse events to canvas
        view.bind("<Button-1>", self._left_click)
        view.bind("<Button-3>", self._right_click)
        view.bind("<Motion>", self._move)
        view.bind("<Leave>", self._mouse_leave)

        # Level
        self._level = MyLevel()

        # selects SimpleTower as the default selection
        self.select_tower(SimpleTower)

        view.draw_borders(game.grid.get_border_coordinates())

        # Get ready for the game
        self._setup_game()
        self.start()

    def setup_menu(self):
        """"Creates a File menu at the top of the window"""
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

        # Reconfigures the pause button.
        if paused==True:
            self._player_controls._play_button.config(text = "Unpause")
        else:
            self._player_controls._play_button.config(text="Pause")

        # pauses or unpauses/starts the game
        if paused:
            self.pause()
        else:
            self.start()

        self._paused = paused

    def _setup_game(self):
        """Sets up game"""
        self._wave = 0
        self._score = 0
        self._coins = 50
        self._lives = 20

        self._won = False

        # Update status
        self._statusbar.wave_status(self._wave)
        self._statusbar.score_status(self._score)
        self._statusbar.coins_status(self._coins)
        self._statusbar.lives_status(self._lives)

        # Re-enable the play controls here (if they were ever disabled)
        self._player_controls._next_wave_button.config(state=tk.NORMAL)
        self._player_controls._play_button.config(state=tk.NORMAL)
        self._game.reset()

    def _new_game(self):
        """"Restarts the Game"""

        self._setup_game()
        self.start()

    def _exit(self):
        """Asks the user weather or not they want tp exit the game"""

        self._toggle_paused(paused=True)
        exit_message = messagebox.askyesno("Exit", "Are you sure you want to exit?")
        if exit_message == True:
            self._master.destroy()
        else:
            self._toggle_paused(paused=False)

    def refresh_view(self):
        """Refreshes the game view"""
        if self._step_number % 2 == 0:
            self._view.draw_enemies(self._game.enemies)
        self._view.draw_towers(self._game.towers)
        self._view.draw_obstacles(self._game.obstacles)

        # Checks if coins is more than the towers costs' in shop, if so change coin label colour, else change to black
        for i, tower in enumerate(self._tower_views):
            self._tower_views[i][1].set_available(self._coins >= self._tower_views[i][0].get_value())

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
        """
        Handles the mouse leaving the game view canvas

        Parameter:
            event (tk.Event): Tkinter mouse event
        """
        # Task 1.2 (Tower placement): Delete the preview
        # Hint: Relevant canvas items are tagged with: 'path', 'range', 'shadow'
        #       See tk.Canvas.delete (delete all with tag)
        self._view.delete('shadow', 'path', 'range')

    def _left_click(self, event):
        """
        Handles the mouse left clicking on the game view canvas

        Parameter:
            event (tk.Event): Tkinter mouse event
        """
        # retrieve position to place tower
        if self._current_tower is None:
            return

        self._position_left = event.x, event.y
        self._cell_position_left = self._game.grid.pixel_to_cell(self._position_left)

        # Attempt to place the tower being previewed
        if self._coins >= self._current_tower.get_value():
            if self._game.place(self._cell_position_left, tower_type=self._current_tower.__class__):
                self._coins -= self._current_tower.get_value()
        self._view.delete('shadow', 'path', 'range')

        # refresh the view so the towers can be seen when the game is paused
        self.refresh_view()

    def _right_click(self, event):
        """
        Handles the mouse right clicking on the game view canvas

        Parameter:
            event (tk.Event): Tkinter mouse event
        """
        # retrieve position to place tower
        self._position_right = event.x, event.y
        self._cell_position_right = self._game.grid.pixel_to_cell(self._position_right)
        sold_tower = self._game.towers[self._cell_position_right]

        # sell the tower for 80% the original value
        self._game.remove(self._cell_position_right)
        self._coins += int(sold_tower.get_value()*0.8)

        # refresh the view so the towers will be removed when the game is paused
        self.refresh_view()

    def next_wave(self):
        """Sends the next wave of enemies against the player"""
        if self._wave == self._level.get_max_wave():
            return

        self._wave += 1

        # Update the current wave display
        self._statusbar.wave_status(self._wave)

        # Disable the add wave button (if this is the last wave)
        if self._wave == 20:
            self._player_controls._next_wave_button.config(state=tk.DISABLED)

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

        # Update coins & score displays
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

        # Update lives display
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

        # show game over dialog
        if won == True:
            self._game_over_win = messagebox._show("GAME OVER", "YOU WIN! PLAY AGAIN!")
        else:
            self._game_over_loss = messagebox._show("GAME OVER", "YOU LOSE! TRY AGAIN!")

        # disables the buttons when the game is over
        self._player_controls._next_wave_button.config(state=tk.DISABLED)
        self._player_controls._play_button.config(state=tk.DISABLED)

class StatusBar(tk.Frame):
    """A frame that tracks the status of waves, score, coins and lives"""

    def __init__(self, master):
        self._master = master
        super().__init__(master)

    def HUD(self):
        """Holds all the labels and images"""
        self._header = tk.Label(self, text="SURVIVE THE ONSLAUGHT", font='Helvetica 9 bold', bg="goldenrod", width=40)
        self._header.pack(side=tk.TOP)

        self._wave_status = tk.Label(self, text=None, bg="white", width = 40)
        self._wave_status.pack(side=tk.TOP)

        self._score_status = tk.Label(self, text="Score: 0", bg="white", width=40)
        self._score_status.pack(side=tk.TOP)

        # imports and image of coins
        coin_icon = tk.PhotoImage(file='images/coins.gif')
        coin_icon_label = tk.Label(self, image=coin_icon, bg="white")
        coin_icon_label.image = coin_icon
        coin_icon_label.pack(side=tk.LEFT, anchor=tk.N)

        self._coins_status = tk.Label(self, text="60 coins", bg="white",  width=14, height=2)
        self._coins_status.pack(side=tk.LEFT, anchor = tk.N, expand= True, fill = tk.X)

        # imports an image of a heart
        heart_icon = tk.PhotoImage(file='images/heart.gif')
        heart_icon_label = tk.Label(self, image=heart_icon, bg="white")
        heart_icon_label.image = heart_icon
        heart_icon_label.pack(side=tk.LEFT, anchor=tk.N)

        self._lives_status = tk.Label(self, text="20 lives", bg="white",  width=14, height=2)
        self._lives_status.pack(side=tk.LEFT, anchor = tk.N, expand= True, fill = tk.X)

    def wave_status(self, _wave):
        """Configures the Wave label to display the current wave"""
        self._wave_status.config(text='Wave: ' + str(_wave) + '/20')

    def score_status(self, _score):
        """Configures the Score label to display the current score"""
        self._score_status.config(text='Score: ' + str(_score))

    def coins_status(self, _coins):
        """Configures the Coins label to display the current coins"""
        self._coins_status.config(text=str(_coins) + ' coins')

    def lives_status(self, _lives):
        """Configures the Lives label to display the current lives"""
        self._lives_status.config(text=str(_lives) + ' lives')

class ShopTowerView(tk.Frame):
    """Creates a frame where the player can buy different towers"""
    def __init__(self, master, tower, click_command):
        super().__init__(master)

        # creates a canvas for the towers you can buy
        self._canvas = tk.Canvas(master, height=44, width = 282, bg="light goldenrod")
        self._canvas.pack(fill=tk.BOTH)

        self._master = master
        self._tower = tower
        self._click_command = click_command

        self._tower.position = (tower.cell_size // 2, tower.cell_size // 2)  # Position in centre
        self._tower.rotation = 3 * math.pi / 2  # Point up
        TowerView.draw(self._canvas, tower)

        self._tower_name = self._canvas.create_text(148,15,text=tower.name) # displays the tower name
        self._tower_cost = self._canvas.create_text(148,30, text=str(tower.base_cost)+" coins") # displayes the cost of the tower

    def set_available(self, available):
        '''Changes colour of coinlabel text if tower is affordable

        Parameter:
            availabe (bool): If tower can be purchased
        '''
        if available:
            # Set coinlabel text colour to black
            self._canvas.itemconfigure(self._tower_cost, fill='black')
        else:
            # Set coinlabel text colour to red
            self._canvas.itemconfigure(self._tower_cost, fill='red')
        pass


class PlayerControls(tk.Frame) :
    """Creates a frame that allows the user to pause the game or call another wave"""

    def __init__(self, master, towergameapp):

        super().__init__(master)

        self._start_message = tk.Label(self, text="Press the Next Wave button to start the game",
                                                                    font='Helvetica 9 bold', bg="goldenrod", width=40)
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