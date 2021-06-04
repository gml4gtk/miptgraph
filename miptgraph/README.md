# miptgraph

GNU GPL Free C++ directed graph layout library

This is a GNU GPL Free stand-alone C++ Graph Layout Library based on GNU GPL Version 3 Free Mipt-Vis tool from Moscow Institute of Physics and Technology( MIPT)  
This project is another reincarnation of the Mipt-Vis project.  

To compile:  
./autogen.sh  
./configure  
make  

To clean: make clean  
make distclean  
make cleaner  

To indent sources: make indent  

Version 1: initial with minimal InitCoordinates() routine to be improved  
Version 2: in development

The directory src/test2 has the gtk+ gui program miptgml for use with gml graph data  
The directory src/test1 and test2 has example programs how to use this graph layout library.  
The source is documented for use with the doxygen program generating html documentation.  
The dmake script can be usd to get a log file of the make process  
This library has own routines for hierarchial graph layout and does not depend on graphviz or other libs  
This library has a c interface to use it with normal c source code  
The Graph and Layout directory does not depend on qt5 library or other libraries  
To compile this C++ with emscripten or otherwise to wasm webassembly should be possible to run it in the browser  
The user interface program must copy some graph data to/from this library to make sure it does not depend on qt5  
Because multiple graphs can be layouted independent at once it can be used to layout bigger graph with subgraphs  
There are almost no C++ libraries for graph layout with sugiyama algorithm but other GPL Free ones are ogdf, graphlhet and igraph  

Most so called graph layout programs are only spring embedder layout or drawing programs.  

Other software implementing sugiyama barycenter graph algorithm are  
C, GPL, gml4gtk graph viewer with GTK+ gui ![gml4gtk](https://github.com/gml4gtk/gml4gtk)  
C, GPL, single c file directed graph layout ![singlefilegraph](https://github.com/gml4gtk/singlefilegraph)  
C, GPL, minimize crossings with science papers ![crossings-minimazation](https://github.com/mfms-ncsu/Crossing-Minimization)  
C, GPL, unmaintined graphed editor ![graphed](https://www3.cs.stonybrook.edu/~algorith/implement/graphed/implement.shtml)  
C, GPL, ginger graph editor with matrix barycenter, offline  
C, GPL, rdis for Linux GTK+ gui ![rdis](https://github.com/endeav0r/rdis)  
C, GPL, GTK+ graph viewer with folding ![tuxsee](https://notabug.org/mooigraph/tuxsee)  
C, GPL, xsurgeon or surgeonsrc toolkit ![surgeon](http://www.cs.loyola.edu/~kbg/surgeonsrc/surgeon.tar.gz)  
C++, gpl, graphlet graph viewer ![graphlet](https://github.com/GunterMueller/UNI_PASSAU_FMI_Graph_Drawing)  
C++, GPL, big graph library ![ogdf](https://ogdf.uos.de/)  
C++, GPL, graph lib with python, R interface ![igraph](https://igraph.org/)  
C++, MIT, bgl graph library using boost ![bgl](https://github.com/lokimx88/BGL-sugiyama)  
C++, GPL, mipt graph visualizer ![mipt-vis](https://github.com/MIPT-ILab-Compilers/MIPT-Vis)  
C++, GPL, mipt graph library ![icdv](https://github.com/MIPT-ILab-Compilers/ICDV)  
javascript, MIT, dagre lib ![dagre](https://github.com/dagrejs/dagre)  
javascript, sugiyama drawing ![sigiyma-drawing](https://github.com/Platane/sugiyama-graph-drawing)  
javascript, some theory info and javascript ![theory](https://github.com/simonegasperoni/sugiyama-drawings)  
javascript, sugiyama drawing lib ![sugiyama-javascript](https://github.com/aravindanve/hierarchical-graph)  
javascript, GPL, sugiyama in web browser ![dGraph](https://github.com/speich/dGraph)  
javascript, sugiyama javascript ![sugiyama-javascript](https://github.com/subhero24/sugiyama)  
javascript, blog ![blog](https://blog.disy.net/sugiyama-method/)  
javascript, MIT, mxgraph javascript ![mxgraph-js](https://github.com/jgraph/mxgraph-js)  
java, sugiyama in java ![itaka-digrap](https://github.com/beckchr/ithaka-digraph)  
java, sugiyama, ![sugiyma-java](https://github.com/jfschaefer/sugiyamalayout)  
java, graph framework ![gvf](http://gvf.sourceforge.net/)  
Python, minimize edge crossings ![mincrossings](https://github.com/GeneSchroer/MinCrossings)  
Python, GPL, python drawing ![python-sugiyama](https://github.com/KadaB/sugiyama)  
Python, GPL, python graph ![granndalf](https://github.com/bdcht/grandalf)  
Python, BSD, maybe grakel ![grakel](https://github.com/ysig/GraKeL)  
Python, reduce edge crossings ![datavis](https://github.com/terana/data-visualisation)  
Python, mipt graph layout course ![miptcourse](https://github.com/pekashy/data-viz)  
elm, sugiyama drawing ![elm](https://github.com/stil4m/sugiyama)  
typescript, sugiyama drawing ![sugiyama-typescript](https://github.com/Michaeladze/sugiyama)  
typescript, sugiyama, ![sugiyama-typescript](https://github.com/Michaeladze/graph)  
typescript, MIT, d3 dag ![d3-dag](https://github.com/erikbrinkman/d3-dag)  


![screenshot](./screenshot.png)
![screenshot2](./screenshot2.png)
![screenschotmiptgml](./src/test2/screenshot.png)

![gplv3](./gplv3-with-text-84x42.png)


SPDX-License-Identifier: GPL-3.0+  
License-Filename: LICENSE  

<span>🇳🇱</span>

