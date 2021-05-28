'''
    BT18CSE028 Aditya Dhane
'''
import sys
import threading
import time
from queue import Queue

''' Global Variables '''
no_of_iterations = 4                # number of interations of routing algorithm           
no_of_routers = 0                   # number of total routers
names = list()                      # list of names of all routers

network = dict()                    # dict consisting of dicts for all router r
                                        # dict consisting of dict for each neighbor n of r
                                            # weight of link between n and r and shared queue object of n
queues = dict()                     # list of shared queues
table = dict()                      # dict used for displaying info


''' Fetching data from File and Initizing global variables '''
file = open(sys.argv[1])

# fetching no of routers and their names
no_of_routers = int(file.readline())
names = file.readline()[:-1].split(' ')

output = Queue(no_of_routers)       # Queue for output strings
for router in names:
    table[router] = dict()
    network[router] = dict()
    queues[router] = Queue()

# Default values for table
for src in names:
    for dest in names:
        table[src][dest] = {'cost':'-','link':'-'}

# Reading links between routers
while True:
    link = file.readline()[:-1].split(' ')
    if len(link) == 1:
        break
    r1 = link[0]                                # router1
    r2 = link[1]                                # router2
    wt = int(link[2])                           # weight of link

    # For r1 -> neighbour r2 -> weight and input queue
    network[r1][r2] = {'weight': wt,
                       'queue': queues[r2]}
    # For r2 -> neighbour r1 -> weight and input queue
    network[r2][r1] = {'weight': wt,
                       'queue': queues[r1]}
file.close()


''' Thread Target Function 
    Arguments: myname -> name of current router
               myq    -> Shared queue for current router
               nabrs  -> dict containing weight of link and shared queue of all neighbors of current router
'''
def start_router(myname, myq, nabrs):
    no_of_nabrs = len(nabrs)                    # number of neighbors

    # Initializing distance vector
    # mydv dictionary has 2 keys cost and link with values as dictionaries
        # Value of cost key contains dictionary with  destinations as keys and cost to that destinations as value
        # Value of link key contains dictionary with  destinations as keys and first router in path to that destinations as value
    mydv = {'cost': {myname: 0},                # cost from itself to itself = 0, link = itself
            'link': {myname: myname}}
    for name, prop in nabrs.items():            # for each neighbour cost = weight, link = neighbour
        mydv['cost'][name] = prop['weight']
        mydv['link'][name] = name

    updated = []
    iteration = 0

    while iteration<=no_of_iterations:
        # Send distance vector to all neighbors
        for prop in nabrs.values():
            prop['queue'].put((myname, mydv['cost'].copy()))

        # Sending data to main thread for displaying
        output.put((myname,mydv.copy()))

        time.sleep(2)
        iteration += 1

        # Update distance vector
        # loop over each neighbour
        for i in range(no_of_nabrs):
            # get neighbour name and distance vector                                                
            name, dv = myq.get()             
            # get weight of link between that neighbour                                       
            wt = nabrs[name]['weight']

            # loop over each entry in distance vector                                               
            for dest, cost in dv.items():                
                # if found new minimum path then update it's own distance vector                           
                if (dest not in mydv['cost']) or (mydv['cost'][dest] > cost+wt):     
                    mydv['cost'][dest] = cost+wt
                    mydv['link'][dest] = name
                    # Add to list of updated paths
                    updated.append(dest)


''' Initializing router threads '''
for router in names:
    thread = threading.Thread(target=start_router, args=(router, queues[router], network[router]))
    thread.start()


''' Printing Output '''
for i in range(no_of_iterations+1):

    print('\nIteration '+str(i)+':')

    # For each router update table dictionary
    for j in range(no_of_routers):
        name, dv = output.get()
        for link in dv['cost']:     
            if table[name][link]['cost'] == dv['cost'][link]:       # if entry not updated
                table[name][link]['link'] = dv['link'][link]
            else:                                                   # if entry updated
                table[name][link]['link'] = dv['link'][link] + '*'      # append *
            table[name][link]['cost'] = dv['cost'][link]
    
    # Print table dictionary as real Table
    print('\t',end=' ')
    for r in names:
        print(r,end='\t')
    print('\n\t---------------------------------------------')
    for r1 in table:
        print(r1+' | ',end='\t')
        for r2 in table[r1]:
            print(str(table[r1][r2]['cost']) +' '+table[r1][r2]['link'],end='\t')
        print()
    print()