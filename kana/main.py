import curses, random, os, time, math

from utils import *
from kana  import kana, r

class LearnKana:

    def __init__(self):
        self.console_columns, self.console_rows    = 0, 0
        self.display_width,   self.display_height  = 0, 6
        self.padding_sides,   self.padding_top     = 3, 5

        self.hiragana,   self.katakana,\
        self.diacritics, self.digraphs,\
        self.yoon_digraphs =\
        True, True, True, True, True

        self.cpm,             self.wpm             = 0, 0

        self.typed_chars  = 0
        self.start_time   = 0
        self.elapsed_time = 0

        self.characters = []
        self.offset = 0

        self.current    = 0

        self.correct_chars = 0
        self.incorrect_chars = 0

        self.correct = True
        self.dt = 0

        self.state = 'typing'
        self.method = 'random'
        self.race_time = 60

        self.mode = 'race'

        self.romanji = True

        self.input_field = ''

        self.scr = curses.initscr()
        init_curses()

        rows, columns = os.popen('stty size', 'r').read().split()
        self.console_rows, self.console_columns = int(rows), int(columns)
        self.display_width = self.console_columns - 2 * self.padding_sides

    def __del__(self):
        curses.endwin()

    def init_typing(self):
        if self.mode == 'race':
            self.time_left = self.race_time * 1000
            self.generate_characters((self.display_height - 1) * int(self.display_width / 2))

    def mainloop(self):
        self.dt = time.time()
        self.init_typing()
        while self.state != 'exiting':
            self.update()
            self.draw()
            self.event()

    def end_game(self):
        pass

    def update(self):
        self.check_for_window_resize()
        if self.state == 'typing':
            if self.mode == 'race':
                self.dt = time.time() - self.dt
                self.time_left -= self.dt

                if self.time_left < 0:
                    self.end_game()
            elif self.mode == 'marathon':
                self.dt = time.time() - self.dt

            if self.current == int(self.display_width / 2):
                self.offset += int(self.display_width / 2)
                self.generate_characters(int(self.display_width / 2))
                self.current = 0

    def draw(self):
        if self.state == 'typing':
            statusbar = 'cpm: {} wpm: {}\ncorrect: {} incorrect: {} time left:{}'.\
                format(
                str(self.cpm),
                str(self.wpm),
                str(self.correct_chars),
                str(self.incorrect_chars),
                str(self.time_left))

            self.scr.refresh()
            self.print_input_window()
            self.print_characters()
            print_msg(self.scr, self.console_rows -  2, statusbar)

    def event(self):
        key_press = self.scr.getch()
        if self.state == 'typing':
            if key_press == 127: # backspace
                self.input_field = self.input_field[:-1]
            elif key_press == ord('q'):
                self.state = 'exiting'
            else:
                self.input_field += chr(key_press)

            if self.input_field:
                self.correct = self.input_field in self.characters[self.current][0]
            else:
                self.correct = True

            if self.input_field == self.characters[self.current][0]:
                self.correct_chars += 1
                self.typed_chars += 1
                self.current += 1
                self.input_field = ''

    def generate_characters(self, num):
        # TODO add other methods to generate characters
        if self.method == 'random':
            for _ in range(num):
                self.characters.append([random.choice(r),
                    random.choice([not self.hiragana, self.katakana])])

    def check_for_window_resize(self):
        rows, columns = os.popen('stty size', 'r').read().split()
        rows, columns = int(rows), int(columns)
        if self.console_rows != rows or self.console_columns != columns:
            self.console_columns = columns
            self.console_rows    = rows

            self.display_width = columns - 2 * self.padding_sides
            d = (self.display_height - 1) * int(self.display_width / 2)

            if len(self.characters) < d:
                self.generate_characters(d - len(self.characters))

            self.scr.clear()
            self.scr.refresh()

    def print_input_window(self):
        win = curses.newwin(3, 10, 12, int(self.console_columns / 2) - 5)
        win.move(0, 0)
        win.bkgd(curses.color_pair(1))
        print_msg(win, 1, self.input_field, 'center')
        win.refresh()

    def print_characters(self):
        win = curses.newwin(self.display_height, self.display_width, self.padding_top, self.padding_sides)
        pad = curses.newwin(self.display_height + 1, self.display_width + 2, self.padding_top - 1, self.padding_sides - 1)

        win.move(0, 0)
        pad.move(0, 0)
        pad.bkgd(curses.color_pair(1))
        win.bkgd(curses.color_pair(1))

        d = (self.display_height - 1) * int(self.display_width / 2)
        display_characters = self.characters[self.offset : self.offset + d]

        already_highlighted = False
        for character in display_characters:
            if display_characters.index(character) == self.current and not already_highlighted:
                win.addstr(kana(character), curses.color_pair(2))
                already_highlighted = True
            else:
                win.addstr(kana(character))

        pad.refresh()
        win.refresh()

lk = LearnKana()
lk.mainloop()

