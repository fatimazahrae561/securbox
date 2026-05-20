# 🔐 SecureBox - Système de surveillance de répertoire sous Linux

## 📌 Description

SecureBox est une application système développée en langage C sous Linux permettant de surveiller en temps réel un répertoire donné.

Elle détecte les événements liés aux fichiers et dossiers, les journalise, analyse les comportements suspects et génère des alertes.

Le projet utilise une architecture **multi-threads avec synchronisation POSIX**.

---

## ⚙️ Fonctionnalités

### 📁 Surveillance en temps réel
- Création de fichiers
- Suppression de fichiers
- Modification de fichiers
- Renommage / déplacement
- Création / suppression de dossiers

---

### 📊 Analyse de sécurité
- Détection de fichiers cachés
- Détection d’extensions interdites
- Détection de suppressions massives
- Détection de modifications rapides

---

### 📝 Journalisation
Chaque événement est enregistré dans un fichier log avec :

- Date
- Heure
- Type d’événement
- Ressource concernée
- Chemin complet
- Niveau de gravité
- PID du processus

---

### 📊 Statistiques
Le système affiche :

- Nombre total d’événements
- Créations
- Suppressions
- Modifications
- Renommages
- Alertes
- Durée d’exécution

---

### 🧵 Architecture multi-threads

- Thread de surveillance (inotify)
- Thread d’analyse
- Thread de journalisation
- Thread principal

---

### 🔄 Communication inter-processus

- Files de messages (queues)
- Mutex
- Variables de condition

---

### 🛑 Arrêt propre

Le système gère correctement :

- SIGINT (Ctrl+C)
- SIGTERM

Lors de l’arrêt :

- arrêt des threads
- vidage des queues
- fermeture des fichiers
- affichage des statistiques finales

---

## 📂 Structure du projet
securebox/
│
├── main.c
├── monitor.c / monitor.h
├── analyzer.c / analyzer.h
├── logger.c / logger.h
├── stats.c / stats.h
├── signals.c / signals.h
├── queue.c / queue.h
├── log_queue.c / log_queue.h
├── event.h
├── Makefile
│
└── logs/
└── securebox.log  
