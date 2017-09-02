#!/usr/bin/env bash
#-----------------------------------------------------------------------------
#
# This file is part of TPad.
#
# TPad is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# TPad is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TPad.  If not, see <http://www.gnu.org/licenses/>.
#
#-----------------------------------------------------------------------------
# Author: ZeroDot1 https://github.com/ZeroDot1
# Contact: zerodot1@bk.ru
# Author Website: https://goo.gl/k3nTZn
# Version: 1.1
# Date Time: 01.09.2017 12:47
# Credits: Thanks to Colin Duquesnoy https://github.com/ColinDuquesnoy
#-----------------------------------------------------------------------------
clear
tput setaf 2
echo '##############################################################################'
echo '# Script to beta test TPad with Archlinux/AntergOS/Manjaro                   #'
echo '# ************************************************************************** #'
echo '# https://github.com/TigaxMT/TPad/tree/master                                #'
echo '##############################################################################'
sleep 2s
read -rsp $'Delete old TPad BETA build [$HOME/TPad-master]. Press any key or wait 5 seconds to continue...\n' -n 1 -t 5;
rm -rf TPad-master
read -rsp $'TPad Testbuild for Arch Linux. This action can take a few moments to complete. Press any key or wait 5 seconds to continue...\n' -n 1 -t 5;
echo 'Create Working Folder [./TPad-master]'
mkdir TPad-master && pushd TPad-master
echo 'Download [TigaxMT/TPad/archive/master.zip] . . . . .'
tput sgr0
wget --verbose --referer=https://github.com/ -t 20 --no-check-certificate https://github.com/TigaxMT/TPad/archive/master.zip
tput setaf 2
echo 'Unzip [master.zip to $HOME/TPad-master/TPad-master] and Make [This action can take a few moments to complete]'
unzip master.zip
pushd TPad-master
echo 'Compile . . . .'
sleep 1s
tput sgr0
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib
make
tput setaf 2
popd
popd
echo 'Run TPad . . . have fun!'
echo 'If you find a bug while testing TPad, please report it.'
echo 'https://github.com/TigaxMT/TPad/issues'
sleep 2s
TPad-master/TPad-master/TPad
