import re
import sys

lines = sys.argv[1].split(";")
lines = list(map(lambda x: x.strip(), lines))

colors = {
    "debug": "red",
    "filter": "green",
    "device": "blue",
    "recording": "grey"
}
filters = []
connections = []
for line in lines:
    filterMatch = re.search(
        '([a-zA-Z0-9]+)::.+ +([a-zA-Z0-9]+){(([[a-zA-Z0-9"/:.]+,? *)*)}', line)
    if filterMatch is not None:
        escaped = filterMatch.group(3).replace('"', '\\"')
        print(filterMatch.group(1), filterMatch.group(2))
        filters.append((filterMatch.group(2), escaped, filterMatch.group(1)))

for line in lines:
    connectionMatch = re.search(
        '([a-zA-Z0-9]+)\.([a-zA-Z0-9]+)\(\) *>> *([a-zA-Z0-9]+)\.([a-zA-Z0-9]+)\(\)', line)
    if connectionMatch is not None:
        outName = connectionMatch.group(2).replace("get", "").replace("Channel", "").replace("Out", "")
        inName = connectionMatch.group(4).replace("get", "").replace("Channel", "").replace("In", "")
        connections.append((connectionMatch.group(1),
                            connectionMatch.group(3),
                            outName, inName))

text = 'digraph G {\n'
text += '\tfontname = "Helvetica"\n'
text += '\tfontsize = 8\n'
text += '\tnodesep = "1"\n'
text += '\tsplines=ortho\n'
text += '\tnode [\n'
text += '\t\tfontname = "Helvetica"\n'
text += '\t\tfontsize = 8\n'
text += '\t\tshape = "record"\n'
text += '\t]\n'
text += '\tedge [\n'
text += '\t\tfontname = "Helvetica"\n'
text += '\t\tfontsize = 6\n'
text += '\t\tlabeldistance = 1.2\n'
text += '\t]\n\n'

for filt in filters:
    text += '\n\t' + filt[0] + ' [\n'
    if filt[1]:
        text += '\t\tlabel = "{' + filt[0] + '|' + filt[1] + '\\l}"\n'
    else:
        text += '\t\tlabel = "' + filt[0] + '"\n'
    text += '\t\tcolor = "' + colors[filt[2]] + '"\n'
    text += '\t]\n'

for connection in connections:
    text += '\n\t' + connection[0] + ' -> ' + connection[1] + ' [\n'
    if connection[2] and connection[3] and connection[2] != connection[3]:
        text += '\t\txlabel = "' + connection[2] + '->' + connection[3] + '"\n'
    elif connection[2]:
        text += '\t\txlabel = "' + connection[2] + '"\n'
    elif connection[3]:
        text += '\t\txlabel = "' + connection[3] + '"\n'
    text += '\t]\n'

text += '}'

print(text)
f = open("tmp.dot", "w")
f.write(text)
