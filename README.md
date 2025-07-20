# __FLAPPY BIRD__

Простая реализация игры Flappy Bird на предоставленном движке.

Для сборки на Linux понадобятся пакеты: _g++, cmake, libx11-dev_. Ваша операционная система должна поддерживать графический режим.

Для сборки:

```bash
mkdir build && cd build && cmake .. && make
```
После в папке _build_ появится исполняемый файл _game_. Для начала игры нужно его запусть.

# Правила игры

В данной игре нужно пролетать птичкой между дырками в трубах, при этом не задевая края экрана. Задача набрать как можно больше очков.

_SPACE_ - прыжок

_esc_ - выход

_ENTER_ - чтобы перезапуститься после проигрыша

Стрелочками можно контролировать ширину дырки и расстояние между трубами

_RIGHT/LEFT_ - _увеличить/уменьшить_ расстояние между трубами 

_UP/DOWN_ - _увеличить/уменьшить_ ширину дырки

-----

A simple implementation of the Flappy Bird game on the provided engine.

To build on Linux, you will need the following packages: _g++, cmake, libx11-dev_. Your operating system must support graphics mode.

For the assembly:

```bash
mkdir build && cd build && cmake .. && make
```
After that, the _game_ executable file will appear in the _build_ folder. To start the game, you need to launch it.

# The rules of the game

In this game, you need to fly a bird between the holes in the pipes, without touching the edges of the screen. The goal is to score as many points as possible.

_SPACE_ - jump

_esc_ - exit

_ENTER_ - to restart after a loss

The arrows can control the width of the hole and the distance between the pipes.

_RIGHT/LEFT_ - _ increase/decrease the distance between the pipes 

_UP/DOWN_ - _ increase/decrease the width of the hole
