#!/usr/bin/env bash
#-----------------------------------------------------------------------------
#
# This file is part of Clevit.
#
# Clevit is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Clevit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Clevit.  If not, see <http://www.gnu.org/licenses/>.
#
#-----------------------------------------------------------------------------
# Author: ZeroDot1 https://github.com/ZeroDot1
# Contact: zerodot1@bk.ru
# Author Website: https://goo.gl/k3nTZn
# Version: 1.2
# Date Time: 10.09.2017 13:37
# Credits: Thanks to Colin Duquesnoy https://github.com/ColinDuquesnoy
#-----------------------------------------------------------------------------
cd
clear
tput setaf 2
echo '##############################################################################'
echo '# Script to beta test Clevit with Archlinux/AntergOS/Manjaro                   #'
echo '# ************************************************************************** #'
echo '# https://github.com/TigaxMT/Clevit/tree/master                                #'
echo '##############################################################################'
sleep 2s
read -rsp $'Delete old Clevit BETA build [$HOME/Clevit-master]. Press any key or wait 5 seconds to continue...\n' -n 1 -t 5;
rm -rf Clevit-master
read -rsp $'Clevit Testbuild for Arch Linux. This action can take a few moments to complete. Press any key or wait 5 seconds to continue...\n' -n 1 -t 5;
echo 'Create Working Folder [./Clevit-master]'
mkdir Clevit-master && pushd Clevit-master
echo 'Download [TigaxMT/Clevit/archive/master.zip] . . . . .'
tput sgr0
wget --verbose --referer=https://github.com/ -t 20 --no-check-certificate https://github.com/TigaxMT/Clevit/archive/master.zip
tput setaf 2
echo 'Unzip [master.zip to $HOME/Clevit-master/Clevit-master] and Make [This action can take a few moments to complete]'
unzip master.zip
pushd Clevit-master
echo 'Compile . . . .'
sleep 1s
tput sgr0
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib
make
tput setaf 2
popd
popd
echo 'Run Clevit . . . have fun!'
echo 'If you find a bug while testing Clevit, please report it.'
echo 'https://github.com/TigaxMT/Clevit/issues'
sleep 2s
Clevit-master/Clevit-master/Clevit
