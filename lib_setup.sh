#!/bin/sh

#	CHECK IF BREW IS INSTALLED
which -s brew
if [[ $? != 0 ]] ; then
    echo Install Homebrew pls (check github or this shell script)
    #ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
else
    brew update
fi

#	CHECK IF pkg-config and readline are installed

if brew list | grep pkg-config ; then
	echo "OK!"
else
	echo "pkg-config not found, please run:\nbrew install pkg-config"
	exit
fi

if brew list | grep readline ; then
	echo "OK!"
else
	echo "readline not found, please run:\nbrew install readline"
	exit
fi

printenv PKG_CONFIG_PATH
if [[ $? != 0 ]] ; then
    echo "Please set the environment variable"
	RL_PATH=$(brew ls readline | grep readline.pc | sed 's|/readline.pc||g')
	echo "Run: export PKG_CONFIG_PATH=$RL_PATH"
	exit
else
    echo "Environment variable OK"
fi

echo "The Makefile should compile correctly now."