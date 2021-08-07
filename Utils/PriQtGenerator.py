import os
import sys


header = "HEADERS +="
source = "SOURCES +="


def findFile(path):
    global header
    global source
    for root, dirs, files in os.walk(path):
        for file in files:
            filePath = os.path.join(root, file)
            filePath = os.path.relpath(filePath)
            filePath = filePath.replace('\\', '/')
            filePath = "$$PWD/" + filePath
            if filePath.endswith(".h") or filePath.endswith(".hpp"):
                header += " \\\n    "
                header += filePath
            elif filePath.endswith(".c") or filePath.endswith(".cpp"):
                source += " \\\n    "
                source += filePath


argv = sys.argv
if len(argv) < 3:
    print("--------------")
    exit()
path = os.path.abspath(argv[1])
priPath = os.path.abspath(argv[2])
findFile(path)
with open(priPath, 'w') as wf:
    wf.write(header)
    wf.write("\n\n")
    wf.write(source)
    wf.write("\n")
