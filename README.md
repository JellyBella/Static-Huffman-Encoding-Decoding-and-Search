# Web-Data-Compression
Design of Huffman tree:
1. MinHeapNode is the basic structure of huffman node, with data, frequency and points to left and right node. MinHeap stores MinHeapNode array, size and capacity. 

2. Create a leaf node for each unique character and build a min heap of all leaf nodes (Min Heap is used as a priority queue. The value of frequency field is used to compare two nodes in min heap. Initially, the least frequent character is at root)

3. Extract two nodes with the minimum frequency from the min heap. Create a new internal node with frequency equal to the sum of the two nodes frequencies. Make the first extracted node as its left child and the other extracted node as its right child. Add this node to the min heap.

4. Repeat step#3 until the heap contains only one node. The remaining node is the root node and the tree is complete.

5. Make huffman code list by traversing huffman tree we built from step#4, record 0 if have left child, 0 for right child, stop when reaches leaf node and stores record to 2D array, which index is mapping by ascii of data, points to NULL if no char is found under index as the ascii code of that char and the 1st element(index 0) of each code list is length. And traversing is done by recursion. 


The header:
1024 bytes are evenly separated to 256 integars, the first integar stores valid bits(validcodelen) following header while the others are for frequencies of characters with ascii 1 to 255. Since char with ascii 0 can't be typed through keyboard so won't be exist in test case, I simply discarded its frequency.
 
 
How search works:
1. First rebuild huffman tree by frequencies provided in header, make huffman codes for each char appeared and stored them into a 2D array(1st dimension is mapping by ascii number, point to NULL if frequency is 0, index 0 stores huffman code length of the char. Being refered to huffman code lists later).

2. Process the given code after header and use an array to store them bit by bit after transferring. Decode the given code array from file to original string by 4 loops, 1st loop starts from index 0 till reaching validcodelen, 2nd loop is for code matching in different length ranging from 1, 3rd loop is to search matching code in huffman code list by comparing the length(stored at index 0) first then following code, which is implemented by 4th loop. Decoded char will be stored into buffer one by one. After all being done, we get original string.  

3. Search from the start of original string (being processed from #3) till the end, if the 1st char matches with index 0 of string pattern, keep comparing the following strings until reaching pattern end, count++ when all match and print count in the end.
