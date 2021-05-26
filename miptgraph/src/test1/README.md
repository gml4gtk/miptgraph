# miptgraph test1

GNU GPL Free C++ directed graph layout library

This are example test programs using the library, run make to generate them  
Also: make clean..

test1.cpp create graph and layout and generate svg image data  
test2.cpp create multiple graph at once and run layout  
test3.cpp create graph with a cycle and run layout  
test4.cpp create graph with nodes but no edges
test5.cpp create graph and layout twice and generate svg image data  

Added features:
graph Setid(int number) to set uniq id number of graph for use  

Example ouput of test1.cpp is this:  
Layout test started..  
Dumping graph  
Nodes:  
Node 0:  
  In  edges:  
  Out edges: 0->1 0->3 0->10  
Node 1:  
  In  edges: 0->1  
  Out edges: 1->5 1->7 1->6 1->5 1->15 1->16  
Node 2:  
  In  edges: 6->2 5->2 11->2  
  Out edges:  
Node 3:  
  In  edges: 0->3  
  Out edges: 3->12  
Node 4:  
  In  edges: 9->4 13->4 14->4  
  Out edges:  
Node 5:  
  In  edges: 1->5 1->5  
  Out edges: 5->8 5->2  
Node 6:  
  In  edges: 1->6  
  Out edges: 6->9 6->2  
Node 7:  
  In  edges: 1->7  
  Out edges: 7->14  
Node 8:  
  In  edges: 5->8 15->8  
  Out edges:  
Node 9:  
  In  edges: 6->9 16->9  
  Out edges: 9->4  
Node 10:  
  In  edges: 0->10  
  Out edges: 10->11  
Node 11:  
  In  edges: 10->11  
  Out edges: 11->2  
Node 12:  
  In  edges: 3->12  
  Out edges: 12->13  
Node 13:  
  In  edges: 12->13  
  Out edges: 13->4  
Node 14:  
  In  edges: 7->14  
  Out edges: 14->4  
Node 15:  
  In  edges: 1->15  
  Out edges: 15->8  
Node 16:  
  In  edges: 1->16  
  Out edges: 16->9  
Edges:  
Edge 0: 0-->1 rev= 0  
Edge 2: 0-->3 rev= 0  
Edge 3: 1-->5 rev= 0  
Edge 4: 6-->2 rev= 1  
Edge 5: 5-->2 rev= 1  
Edge 8: 9-->4 rev= 1  
Edge 9: 5-->8 rev= 0  
Edge 10: 6-->9 rev= 0  
Edge 12: 1-->7 rev= 0  
Edge 13: 1-->6 rev= 0  
Edge 14: 1-->5 rev= 0  
Edge 16: 0-->10 rev= 0  
Edge 17: 10-->11 rev= 0  
Edge 18: 11-->2 rev= 0  
Edge 19: 3-->12 rev= 0  
Edge 20: 12-->13 rev= 0  
Edge 21: 13-->4 rev= 0  
Edge 22: 7-->14 rev= 1  
Edge 23: 14-->4 rev= 1  
Edge 24: 1-->15 rev= 0  
Edge 25: 15-->8 rev= 0  
Edge 26: 1-->16 rev= 0  
Edge 27: 16-->9 rev= 0  
maxrank = 4  
Layout test passed!  

