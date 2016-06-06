FROM base/archlinux

RUN pacman --noconfirm -Syy
RUN pacman --noconfirm -S git
RUN pacman --noconfirm -S python
RUN pacman --noconfirm -S gcc
RUN pacman --noconfirm -S clang

