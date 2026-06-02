# gamebutwithraylib


Should I consider dirty recting
-> significantly decreases total iteration  
-> per pixelchange, (min, max)*2 (four if-statements!!!) +figuring out where to store (probably needs some kind of accessing too)
-> figuring some kind of acces
-> will probably only yield any returns when using larger subchunks

-> should I profile or just implement both with compiler settings : )





Dirty recting is def worth it because of situations where only the upper layer has to move? That way I can still keep significantly large chunks? Should I dump the subchunk system?




Anyway there should be rigidbodies and physics objects and the game world. How do I make them interact? I probably should just make things from the world into physics objects dynamically. Yeah it's gonna be way too complicated if I don't. But say I want to implement chains or something? I guess they should just not follow the grid ideally. I could always snap it to the grid after.
