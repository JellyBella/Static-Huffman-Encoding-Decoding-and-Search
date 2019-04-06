#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TREE_HT 100

struct MinHeapNode {
    unsigned char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

struct MinHeapNode* newNode(char data, unsigned freq){
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct MinHeap* createMinHeap(unsigned capacity){
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap-> capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx){
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]-> freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]-> freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap){
    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){
    minHeap->size++;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap){
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}


int isLeaf(struct MinHeapNode* root){
    return !(root->left) && !(root->right);
}


struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size){
    struct MinHeap* minHeap = createMinHeap(size);
    int i;
    for (i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size){
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

char *codearray[256]={NULL};//2D array for huffman code list, index as ascii code is mapping to the frequency of that code
void makeCodes(struct MinHeapNode* root, int arr[], int top){
    if (root->left) {
        arr[top] = 0;
        makeCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        makeCodes(root->right, arr, top + 1);
    }
    if (isLeaf(root)) {
        codearray[(int)root->data] = (char*)malloc((top+1)*sizeof(char));//one more byte for code length
        int i;
        codearray[(int)root->data][0]=(char)top;//index 0 for code length
        for(i = 0;i<top;i++){
            codearray[(int)root->data][i+1]=(char)arr[i];
        }
    }
}

void HuffmanCodes(char data[], int freq[], int size){
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    makeCodes(root, arr, top);
}

void add_bit(char* bitarray, int k){
    int i = k/8;
    int j = k%8;
    bitarray[i] = ((0x80 >> j) | bitarray[i]);
}

int encode(char *file, char *file1){
    int characters[256] = {0};
	FILE* fp = fopen(file,"rb");
    if(fp == NULL) {
        perror("Error in opening file");
        return(-1);
     }
    fseek(fp, 0, SEEK_END);
    unsigned int flength = ftell(fp); // flength is the length of whole text string
    fseek(fp, 0, SEEK_SET);
    unsigned char *string = malloc(flength*sizeof(char));//string stores the total string of original text file
    fread(string, flength, sizeof(char), fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char c;
	int i,j=0;
    while(1){
		if(feof(fp)) break;
		c = fgetc(fp);
        characters[(int)c]++;
    }
	fclose(fp);
    int validlength = 0;//length of array ch and freq
    for(i = 0; i < 256; i++){
        if(characters[i] != 0){
            validlength++;
        }
    }

    if(!validlength){//empty file
        FILE* fp1 = fopen(file1,"wb");
        fwrite(characters,sizeof(int),256,fp1);
        fclose(fp1);
        return 0;
    }
    char *ch = malloc(validlength+1*sizeof(char));//char array that being passed to HuffmanCodes function
    int *freq= malloc((validlength+1)*sizeof(int));//frequency arry being passed to HuffmanCodes function 
    for(i = 0; i < 256; i++){
        if(characters[i] != 0){
            ch[j]=(char)i;
            freq[j] = characters[i];
            j++;
        }
    }
    ch[validlength]='\0';
    freq[validlength]='\0';
    HuffmanCodes(ch, freq, validlength);

    int *huffmancode = calloc(1,sizeof(int));//index 0 for length of total encoded huffman code
    int curri,codelen,newlen;
    //encoding huffman code and store into array
    for(i=0;i<flength;i++){
        curri = huffmancode[0];//current index
        codelen = (int)codearray[(int)string[i]][0];
        newlen = codelen+curri;
        huffmancode = (int*)realloc(huffmancode,sizeof(int)*(newlen+1));//newlen not include the index0 itself
        huffmancode[0] = newlen;
        for(j=1;j<=codelen;j++){
            huffmancode[curri+j]=(int)codearray[(int)string[i]][j];
        }
    }
    //characters[0] = huffmancode[0];
    FILE* fp1 = fopen(file1,"wb");
    fwrite(characters,sizeof(int),256,fp1);
    int buffersize = huffmancode[0]/8;
    if(huffmancode[0]>8*buffersize) buffersize+=1;//round up
    char *buffer = calloc(buffersize,sizeof(char));
    for(i=1;i<=huffmancode[0];i++){//transfer huffman code array to bits
        if(huffmancode[i]) add_bit(buffer, i-1);
    }
    fwrite(buffer,sizeof(char),buffersize,fp1);//write into file
    fclose(fp1);
    free(buffer);
    return 0;
}

int decode(char* file1, char* file2){
    int i,j;
    FILE* fp2 = fopen(file1,"rb");
    fseek(fp2, 0, SEEK_END);    // Go to end
    unsigned int outputlength = ftell(fp2); // outputlength in byte
    //empty other than header in file
    if(outputlength==1024){
        FILE * fp4 = fopen(file2,"wb");
        fwrite(0,0,0,fp4);
        fclose(fp4);
        return 0;
    }
    fseek(fp2, 0, SEEK_SET);
    int *header = malloc(256*sizeof(int));
    int encodedfilelen = outputlength-1024;//in byte
    char *encodedfile = malloc(encodedfilelen*sizeof(char));
    fread(header, sizeof(int),256, fp2);
    fread(encodedfile, sizeof(char), encodedfilelen, fp2);
    fclose(fp2);
    //int validcodelen = header[0];
    int num=0;
    for(i=0;i<256;i++){
        if(header[i]){
            num++;//length of array ch1 and freq1\uff0cwhich will be passed to HuffmanCodes function to build huffman tree
        }
    }
    char *ch1 = malloc((num+1)*sizeof(char));
    int *freq1 = malloc(sizeof(int)*(num+1));
    j=0;
    int charnum = 0;
    for(i = 0;i < 256 ; i++){//produce ch1 nad freq1 array
        if(header[i]){
            charnum+=header[i];
            ch1[j] = (char)i;
            freq1[j] = header[i];
            j++;
        }
    }
    ch1[num]='\0';
    freq1[num]='\0';
    HuffmanCodes(ch1,freq1,num);

    char flag=0;
    int* outputhuffman = calloc(encodedfilelen*8,sizeof(int));
    int n = 0;
    for(i = 0;i<encodedfilelen;i++){//transfer bits to byte element in array
        for(j=0;j<8;j++){
            flag = (encodedfile[i] & (1 << (7 - j) ));
            if(flag){
                outputhuffman[n]=1;
            }
            else {
                outputhuffman[n]=0;
            }
            n++;
        }
    }
    char *buffer1 = calloc(charnum,sizeof(char));
    int len,fail,index = 0;
    flag =0;
    n=0;
    while(n<charnum){//4 loops to decode given strings into chars
		len = 0;
        while(1){
	    	len++;
            for(i=0;i<256;i++){
                if(!codearray[i])continue;
                if(codearray[i][0]==len){
                    flag = 1;
                    fail = 0;
                    for(j=0;j<len;j++){
                        if(outputhuffman[index+j] != codearray[i][j+1]){
                            fail = 1;
                            break;
                        }
                    }
                    if(!fail){
                        buffer1[n]=(char)i;
                        n++;
                        break;
                    }
                }
            }
            if(flag&&!fail){
                flag = 0;
                fail = 1;
                break;}
        }
        index += len;
    }
    FILE * fp4 = fopen(file2,"wb");
    fwrite(buffer1,sizeof(char),charnum,fp4);
    fclose(fp4);
    free(buffer1);
    return 0;
}

int search(char* s, char* f){
    int i,j;
    FILE* fp2 = fopen(f,"rb");
    fseek(fp2, 0, SEEK_END);    // Go to end
    unsigned int outputlength = ftell(fp2); // outputlength in byte
    //empty other than header
    if(outputlength==1024){
        printf("0");
        return 0;
    }
    fseek(fp2, 0, SEEK_SET);
    int *header = malloc(256*sizeof(int));
    int encodedfilelen = outputlength-1024;//in byte
    char *encodedfile = malloc(encodedfilelen*sizeof(char));
    fread(header, sizeof(int),256, fp2);
    fread(encodedfile, sizeof(char), encodedfilelen, fp2);
    fclose(fp2);
    //int validcodelen = header[0];
    int num=0;
    for(i=0;i<256;i++){//length of array ch1 and freq1
        if(header[i]){
            num++;
        }
    }
    char *ch1 = malloc((num+1)*sizeof(char));
    int *freq1 = malloc(sizeof(int)*(num+1));
    j=0;
    int charnum = 0;
    for(i = 0;i < 256 ; i++){//produce ch1 and freq1
        if(header[i]&&i){
            charnum+=header[i];
            ch1[j] = (char)i;
            freq1[j] = header[i];
            j++;
        }
    }
    ch1[num]='\0';
    freq1[num]='\0';
    HuffmanCodes(ch1,freq1,num);

    char flag=0;
    int* outputhuffman = calloc(encodedfilelen*8,sizeof(int));
    int n = 0;
    for(i = 0;i<encodedfilelen;i++){//change bit into byte
        for(j=0;j<8;j++){
            flag = (encodedfile[i] & (1 << (7 - j) ));
            if(flag){
                outputhuffman[n]=1;
            }
            else {
                outputhuffman[n]=0;
            }
            n++;
        }
    }

    char *buffer1 = calloc(charnum,sizeof(char));
    int len,fail,index = 0;
    flag =0;
    n=0;
	len = 0;
    while(n<charnum){//decode
		len=0;
        while(1){
			len++;
            for(i=0;i<256;i++){
                if(!codearray[i])continue;
                if(codearray[i][0]==len){
                    flag = 1;
                    fail = 0;
                    for(j=0;j<len;j++){
                        if(outputhuffman[index+j] != codearray[i][j+1]){
                            fail = 1;
                            break;
                        }
                    }
                    if(!fail){
                        buffer1[n]=(char)i;
                        n++;
                        break;
                    }
                }
            }
            if(flag&&!fail){
                flag = 0;
                fail = 1;
                break;}
        }
        index += len;
    }
    n=0;
    int slen = strlen(s);
    for(i=0;i<charnum;i++){//search
        if(!strncmp(&s[0],&buffer1[i],1)){
			fail = 0;
            for(j=1;j<slen;j++){
                if(i+j>charnum)break;
                if(strncmp(&s[j],&buffer1[i+j],1)){
                    fail = 1;
                    break;
                }
            }
            if(!fail){n++;}
        }
    }
    printf("%d\n",n);
	return 0;
}
int main(int argc, char* argv[])
{
    if(!strcmp(argv[1],"-e"))encode(argv[2],argv[3]);
    else if(!strcmp(argv[1],"-d"))decode(argv[2],argv[3]);
    else if(!strcmp(argv[1],"-s"))search(argv[2],argv[3]);
    return 0;
}