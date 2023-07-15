#!/bin/bash

# Fonction pour vérifier si une dépendance est déjà installée
function check_dependency {
    if command -v $1 >/dev/null 2>&1; then
        echo "La dépendance $1 est déjà installée."
        return 0
    else
        return 1
    fi
}

# Installation des bibliothèques nécessaires pour le code du serveur

# Installation de SFML
if ! check_dependency "sfml-config"; then
    # Installation des bibliothèques spécifiques à Fedora
    if [ -f /etc/fedora-release ]; then
        sudo dnf update
        sudo dnf groupinstall -y "Development Tools"
        cd utilities && ./sfml_fedora.bash
        cd ..
    fi

    # Installation des bibliothèques spécifiques à Ubuntu
    if [ -f /etc/lsb-release ]; then
        sudo apt-get update
        sudo apt-get install -y build-essential
        sudo apt-get install -y libsfml-dev
    fi
fi

# Compilation du code du serveur
cd .. && make

echo "Installation terminée."
