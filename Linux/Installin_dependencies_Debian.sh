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
# Author Website: https://goo.gl/ZuWQB2
# Version: 1.0
# Date Time: 16.10.2017 15:03
#-----------------------------------------------------------------------------
cd
clear
tput setaf 2
echo 'Installing dependencies on Ubuntu , Linux Mint, Debian'
sudo apt-get install build-essential make cmake qtdeclarative5-dev qml-module-qtquick-controls qt5-default openssl
exit 0