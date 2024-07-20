import os
from pathlib import Path

PathTwoLevelsUp = Path(__file__).parents[2]

print(PathTwoLevelsUp)


for PathObject in PathTwoLevelsUp.rglob('*'):

    print(PathObject.name)