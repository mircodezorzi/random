import curses

def init_curses():
    curses.noecho()
    curses.cbreak()
    curses.curs_set(0)
    if curses.has_colors():
        curses.start_color()

    curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_WHITE)
    curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_GREEN)
    curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_RED)

def print_msg(screen, line, msgs, align='left'):
    height, width = screen.getmaxyx()
    width = width - 1

    msgs = msgs.split('\n')

    if height < line + len(msgs):
        return 1

    screen.addstr(line, 0, ' ' * width * len(msgs))
    screen.move(line, 0)

    for msg in msgs:
        padding = ''
        new_line = '\n' * (msgs.index(msg) != len(msgs) - 1)
        if align == 'center':
            padding = ' ' * int((width - len(msg[:width])) / 2 + 1)
        if align == 'right':
            padding = ' ' * (width - len(msg[:width]))
        screen.addstr(padding + msg[:width] + new_line)
