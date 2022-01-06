# Bare metal blink on STM32 blue pill

### Prerequisites
- Assuming windows 10 with Docker desktop installed and WSL2 enabled. A Mac or Linux machine will work also.
- make sure the stlink driver is installed, open device manager to check. (I use a clone st-link V2). 
Flasing over a serial link is also possible, but not recommended because there is no debugging over a serial link.
- vscode is used as ide, with tasks configured in .vscode/tasks.json. 
If you want keybindings, install them as a user setting (File->Preferences->Keyboard shortcuts select json file)

WSL2 (preferable Debian), with the following packages installed:
- sudo apt-get install git
- sudo apt-get install gcc-arm-none-eabi
- sudo apt-get install build-essential
- sudo apt-get install stlink-tools

### Checking the dev enviroment
To check the arm dev envirement, open a terminal into the .devcontainer folder. 
Rebuild the arm dev enviroment with ```docker build -t armdev . ``` and run it in the background by ```docker run -it -d armdev```
Check if container is running by ```docker ps```, and terminal into it by using the container id: ```docker exec -it <id> bash```