#!/usr/bin/env python3

import glob
import os
import sys

def getMetapostFilePaths(directoryPath):
    metapostFilePaths = glob.glob(directoryPath + '/*.mp')
    directoryContents = os.listdir(directoryPath)
    for item in directoryContents:
        itemAbsPath = os.path.abspath(item)
        if os.path.isdir(itemAbsPath):
            os.chdir(itemAbsPath)
            metapostFilePaths += getMetapostFilePaths(itemAbsPath)
            os.chdir(os.pardir)
    return metapostFilePaths

def getFileDirectoryPath(absFilePath):
    i = len(absFilePath) - 1
    while absFilePath[i] != os.sep:
        i -= 1
    return absFilePath[:i]

def getFileName(filePath):
    i = len(filePath) - 1
    while filePath[i] != '/' and i >= 0:
        i -= 1
    j = len(filePath) - 1
    while filePath[j] != '.':
        j -= 1
    return filePath[i:j]

def getInputFilePaths(metapostFilePath):
    metapostFile = open(metapostFilePath, 'r')
    currentDirectoryPath = os.path.abspath(os.curdir)
    os.chdir(getFileDirectoryPath(metapostFilePath))
    inputFilePaths = []
    while True:
        line = metapostFile.readline()
        if not line:
            break
        if line[:5] == 'input':
            inputFilePath = line[6:len(line) - 1]
            if inputFilePath[len(inputFilePath) - 1] == ';':
                inputFilePath = inputFilePath[:len(inputFilePath) - 1]
            inputFilePaths.append(os.path.abspath(inputFilePath) + '.mp')
    os.chdir(currentDirectoryPath)
    metapostFile.close()
    return inputFilePaths

def buildDependencyGraph(directoryPath):
    metapostFilePaths = getMetapostFilePaths(directoryPath)
    mapping = []
    for metapostFilePath in metapostFilePaths:
        mapping.append((metapostFilePath, getInputFilePaths(metapostFilePath)))
    return dict(mapping)

def buildReversedDependencyGraph(directoryPath):
    dependencyGraph = buildDependencyGraph(directoryPath)
    keys = list(dependencyGraph.keys())
    mapping = []
    for key in keys:
        mapping.append((key, []))
    reversedGraph = dict(mapping)
    for key in dependencyGraph:
        for metapostFilePath in dependencyGraph[key]:
            reversedGraph[metapostFilePath].append(key)
    return reversedGraph

def compileRecursively(metapostFilePath, errorOccured):
    if not errorOccured:
        visited[metapostFilePath] = True
        if not metapostFilePath in forbiddenFilePaths:
            directoryPath = getFileDirectoryPath(metapostFilePath)
            os.chdir(directoryPath)
            if os.system('mpost --halt-on-error ' + metapostFilePath + ' > /dev/null 2>&1') > 0:
                logFile = open(metapostFilePath[:len(metapostFilePath) - 2] + 'log', 'r')
                while True:
                    line = logFile.readline()
                    if not line:
                        break
                    sys.stderr.write(line)
                errorOccured = True
            newEpsFilePaths = glob.glob('*.eps')
            for i in range(len(newEpsFilePaths)):
                newEpsFilePaths[i] = directoryPath + os.sep + newEpsFilePaths[i]
                epsFilePaths.add(newEpsFilePaths[i])
        neighbours = graph[metapostFilePath]
        for neighbour in neighbours:
            if compileRecursively(neighbour, errorOccured):
                errorOccured = True
    return errorOccured

workingDirectoryPath = sys.argv[1]
metapostFilePaths = sys.argv[2:]
graph = buildReversedDependencyGraph(workingDirectoryPath)

keys = list(graph.keys())
mapping = []
for key in keys:
    mapping.append((key, False))
visited = dict(mapping)

forbiddenFilePaths = set()
forbiddenFile = open(workingDirectoryPath + '/.mptopdf.py.forb', 'r')
while True:
    line = forbiddenFile.readline()
    if not line:
        break
    forbiddenFilePaths.add(line[:len(line) - 1])

epsFilePaths = set()

for metapostFilePath in metapostFilePaths:
    if compileRecursively(metapostFilePath, False):
        sys.exit(0)

compilationProgramDirectoryPath = sys.argv[1] + '/.mptopdf'
if not os.path.isdir(compilationProgramDirectoryPath):
    os.mkdir(compilationProgramDirectoryPath)

for epsFilePath in epsFilePaths:
    texFilePath = compilationProgramDirectoryPath + getFileName(epsFilePath) + '.tex'
    texFile = open(texFilePath, 'w')
    texFile.write('\documentclass{standalone}\n')
    texFile.write('\\usepackage{graphicx}\n')
    texFile.write('\DeclareGraphicsRule{*}{mps}{*}{}\n')
    texFile.write('\\begin{document}%\n')
    texFile.write('\includegraphics{' + epsFilePath + '}\n')
    texFile.write('\end{document}\n')
    texFile.close()

    os.chdir(compilationProgramDirectoryPath)
    os.system('pdflatex ' + texFilePath + ' > /dev/null 2>&1')
    
    pdfFilePath = texFilePath[:len(texFilePath) - 4] + '.pdf'
    sys.stdout.write(os.path.abspath(pdfFilePath) + '\n')
    sys.stdout.write(os.path.abspath(pdfFilePath) + '\n')
