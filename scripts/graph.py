import subprocess
import re

result = subprocess.run(['sh', 'scripts/graph.sh'], stdout=subprocess.PIPE)
lines = result.stdout.decode("utf-8").split("\n")
lines = list(map(lambda x: x.strip(), lines))
filters = []
connections = []
for line in lines:
    filterMatch = re.search('[a-zA-Z0-9:<>]+ +([a-zA-Z0-9]+){(([[a-zA-Z0-9"/:\.]+,?\ *)*)};', line)
    if filterMatch is not None:
        escaped = filterMatch.group(2).replace('"', '\\"')
        filters.append((filterMatch.group(1), escaped))

for line in lines:
    connectionMatch = re.search('([a-zA-Z0-9]+)\.([a-zA-Z0-9]+)\(\) *>> *([a-zA-Z0-9]+)\.([a-zA-Z0-9]+)\(\)', line)
    if connectionMatch is not None:
        connections.append((connectionMatch.group(1), connectionMatch.group(3),
                            connectionMatch.group(2), connectionMatch.group(4)))

text = 'digraph G {\n'
text += '\tfontname = "Helvetica"\n'
text += '\tfontsize = 8\n'
text += '\trankdir="LR";\n'
text += '\tnodesep = "0.6"\n'
text += '\tsplines=ortho\n'
text += '\tnode [\n'
text += '\t\tfontname = "Helvetica"\n'
text += '\t\tfontsize = 8\n'
text += '\t\tshape = "record"\n'
text += '\t]\n'
text += '\tedge [\n'
text += '\t\tfontname = "Helvetica"\n'
text += '\t\tfontsize = 8\n'
text += '\t\tlabeldistance = 1.8\n'
text += '\t]\n\n'

for filt in filters:
    text += '\n\t' + filt[0] + ' [\n'
    if filt[1]:
        text += '\t\tlabel = "' + filt[0] + '(' + filt[1] + ')"\n'
    else:
        text += '\t\tlabel = "' + filt[0] + '"\n'
    text += '\t]\n'

for connection in connections:
    text += '\n\t' + connection[0] + ' -> ' + connection[1] + ' [\n'
    text += '\t\t' + 'taillabel = "' + connection[2] + '"\n'
    text += '\t\t' + 'headlabel = "' + connection[3] + '"\n'
    text += '\t]\n'

text += '}'

print(text)
f = open("tmp.dot", "w")
f.write(text)
