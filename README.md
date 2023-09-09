# Botnet-Development-Project-Ethical-Exploration

In this C programming project, my team, consisting of Michelle Dorcas, Mariam Miclauri and myself, has developed a simplified botnet, comprising a controller and a zombie component.
Our primary objective is to gain insights into vulnerabilities within code and configurations while emphasizing 
our commitment to the ethical use of these skills.

## Overview

This project explores the development of a simplified botnet using C programming. 
The botnet consists of two main components: the controller and the zombie. 
The goal of this project is to gain a deeper understanding of system programming, network communication 
and security vulnerabilities while emphasizing ethical use and responsible conduct.

## Components

- **Controller**:
  The controller is responsible for remotely connecting to the zombie(s) and opening a bash session without requiring authentication. It acts as the command center for the botnet.

- **Zombie**:
  The zombie represents the compromised computer within the botnet. It listens for commands from the controller and executes them on the host system, allowing remote control.
  
- **Labo**:
  This component launches two zombies on the same machine, creating a test environment to validate controller and zombie implementations. Zombies run on random ports selected from a list of 10 hardcoded ports. When the labo program terminates ('Ctrl+D'), it signals all launched zombies to terminate.

## Usage

1. Clone this repository to your local machine.

2. Execute `make` command to build the project using the provided makefile.

3. Run the `zombie` executable on the target machine(s) to act as the compromised host.

4. Run the `controller` executable on your machine to control the zombie(s) remotely.

## Ethical Considerations

This project is solely intended for educational purposes and to better understand potential security vulnerabilities. 
It is essential to emphasize ethical use and respect all legal and ethical boundaries when experimenting with these concepts. 
Do not use this code or knowledge for any malicious purposes.

## Disclaimer

The creators of this project disclaim any responsibility for any misuse, illegal activities, or harm caused by the use of this code. 
Use it responsibly and within the boundaries of the law.

## Credits
We would like to extend our heartfelt thanks to the following individuals who provided valuable guidance, support, and inspiration throughout the development of this project:

- **Professor Anthony Legrand** - for their valuable guidance, mentorship, expertise, and insights that significantly influenced the project's direction.

- **Professor Oliver Choquet** - for their invaluable assistance in improving our code.

- **Professor Jose Vander Meulen** - for their unwavering support and valuable insights played a crucial role in the development of our project.
  
- **Professor Xavier Gillard** - for their insightful feedback, valuable assistance, and the brilliant idea of the botnet project. This project proved to be highly successful and fruitful, allowing us to acquire significant knowledge. It was an academic endeavor that we found both highly educational and incredibly enjoyable !
  
- **Lucas** and **Thomas** - For their assistance in addressing challenges and providing valuable perspectives.

We are grateful for the support and contributions of everyone involved in this project.

---

**Note:** This project is intended for educational purposes only, and the developers do not endorse or encourage any illegal or unethical activities.
