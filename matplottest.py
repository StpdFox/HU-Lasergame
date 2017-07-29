import matplotlib
import os
import os.path
import numpy as np
import matplotlib.pyplot as plt

def readPlayerHealths():
    player_healths = list()
    #Todo:
    # read all 10 (possible) files and get the player health from those files
    # return an array
    for i in range (0, 10):
        path_str = "player_"
        path_str += str(i)
        path_str += ".stats"
        print(path_str)
        if os.path.isfile(path_str):
            file = open(path_str, "r")
            file.readline()
            phealth_line = file.readline() #get second line because that line contains player health
            if phealth_line.split(":")[0] == "Player_Health":
                player_healths.append(int(phealth_line.split(":")[1]))
    return player_healths

# Show health of each player
players_health = readPlayerHealths()
print(players_health)
# for i in range (0, 100, 10):
#     print(i)
#     players_health.append(i)
player_ids = [0, 1, 3]
max_range = 100 # 100 hp max
n = len(players_health)
X = np.asarray(player_ids)
Y1 = np.asarray(players_health)
print(X)
print(Y1)
plt.gcf().canvas.set_window_title("Players Health")
plt.xticks(X, player_ids)
plt.bar(X, Y1, facecolor='#9999ff', edgecolor='white')

for x,y in zip(X,Y1):
    plt.text(x, y, '%.0f' % y, ha='center', va= 'bottom')

plt.ylim(0,100)  # range of y depth
plt.show()