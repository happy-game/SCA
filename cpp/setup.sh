#!/bin/bash

dependencies=("eigen" "boost")
source_url=("https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz" "https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.gz")
# ask for install with package manager or from source
echo "Do you want to install from source or with a package manager? S for source, P for package manager"
read installType

# install with package manager
if [ "$installType" == "P" ]; then
    echo "Installing with package manager"
    # check distro
    if [ -f /etc/debian_version ]; then
        echo "Debian based distro detected"
        # install dependencies
        for i in "${dependencies[@]}"
        do
            sudo apt-get install $i
        done
    elif [ -f /etc/redhat-release ]; then
        echo "Redhat based distro detected"
        # install dependencies
        for i in "${dependencies[@]}"
        do
            sudo yum install $i
        done
    elif [ -f /etc/arch-release ]; then
        echo "Arch based distro detected"
        # install dependencies
        for i in "${dependencies[@]}"
        do
            sudo pacman -S $i
        done
    else
        echo "Unsupported distro"
        exit 1
    fi
# install from source
elif [ "$installType" == "S" ]; then
    echo "Installing from source"
    # check if dependencies are installed
    for i in "${dependencies[@]}"
    do
        if [${source_url[$i]} == "eigen3"]; then
            if [ ! -d "/usr/include/eigen3" ]; then
                echo "Eigen3 not installed"
                echo "Installing Eigen3"
                wget ${source_url[$i]}
                tar -xzf eigen-3.4.0.tar.gz
                mv eigen-3.4.0 eigen3
            fi
        elif [${source_url[$i]} == "boost"]; then
            if [ ! -d "/usr/include/boost" ]; then
                echo "Boost not installed"
                echo "Installing Boost"
                wget ${source_url[$i]}
                tar -xzf boost_1_83_0.tar.gz
                mv boost_1_83_0 boost
            fi
        fi
    done
fi