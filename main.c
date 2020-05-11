#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM_OF_WORDS 100
#define MAX_NUM_OF_LETTERS 10

///Tree-related functions
typedef struct Node
{
    char* word;
    struct Node* left;
    struct Node* right;
} Node;

Node * createNode(char* key)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->word = (char*)malloc(strlen(key) + 1);
    strcpy(newNode->word, key);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insertNode(Node* root, char* key)
//it inserts the words such as they would be sorted ascendingly.
{
    if (root == NULL)
        return createNode(key);
    if (strcasecmp(key, root->word) < 0)
        root->left = insertNode(root->left, key);
    else if (strcasecmp(key, root->word) > 0)
        root->right = insertNode(root->right, key);
    return root;
    /*
    *we do not handle the case where the key is equal to
    *any node as BSTs do not allow duplication
    */
}

Node* searchTree(Node* root, char* key)
{
    if(root == NULL)
        return NULL;
    else if(strcasecmp(key, root->word) == 0) //word and key are identical
        return root;
    else if(strcasecmp(key, root->word) > 0)
        return searchTree(root->right, key);
    else
        return searchTree(root->left, key);
}

Node* findMin(Node* root)
{
    if(root == NULL)
        return NULL;
    while(root->left != NULL)
        root = root->left;
    return root;
}

Node* findMax(Node* root)
{
    if(root == NULL)
        return NULL;
    while(root->right != NULL)
        root = root->right;
    return root;
}

Node* getSuccessor(Node* root, char* key)   //according to inorder
{
    Node* current = searchTree(root, key);
    if(current == NULL)
        return NULL;
    if(current->right != NULL)  //Case 1: Node has right subtree
        return findMin(current->right);
    else     //Case 2: No right subtree
    {
        Node* successor = NULL;
        Node* ancestor = root;
        while(ancestor != current)
        {
            if(strcasecmp(current->word, ancestor->word) < 0)
            {
                successor = ancestor;
                ancestor = ancestor->left;
            }
            else
                ancestor = ancestor->right;
        }
        return successor;
    }
}

Node* getPredecessor(Node* root, char* key)
{
    Node* current = searchTree(root, key);
    if (current == NULL)
        return NULL;
    if (current->left != NULL)
        return findMax(current->left);
    else
    {
        Node* predecessor = NULL;
        Node* ancestor = root;
        while(ancestor != current)
        {
            if(strcasecmp(current->word, ancestor->word) > 0)
            {
                predecessor = ancestor;
                ancestor = ancestor->right;
            }
            else
                ancestor = ancestor->left;
        }
        return predecessor;
    }
}

void printInorder(Node *root)
{
    if(root == NULL)
        return;

    printInorder(root->left);
    printf("%s ", root->word);
    printInorder(root->right);
}

int max(int x ,int y )
{
    return x>y?x:y;
}

int getHeight(Node* root)
{
    if(root==NULL)
        return -1;
    else
        return 1 + max(getHeight(root->left), getHeight(root->right));

}

int getNumberOfNodes(Node *root){
    if (root)
        return 1 + getNumberOfNodes(root->left) + getNumberOfNodes(root->right);
    return 0;
}

///File related functions
Node* loadFile(char* fileName)
{
    char word[50];
    Node* root = NULL;
    FILE* filePointer;

    filePointer = fopen(fileName, "r");

    if(filePointer != NULL)
    {
        printf("...............................\n");
        printf("Dictionary loaded successfully!\n");
        printf("...............................\n");

        while (fscanf(filePointer, "%[^\n]\n", word) != EOF)
            root = insertNode(root, word);
    }
    else
    {
        printf("UNEXPECTED ERROR!");
        exit(-1);
    }
    return root;
}

///Functions that have to do with the user's input
char** tokenizeSentence(char* sentence)
{
    char** words = malloc(MAX_NUM_OF_WORDS * MAX_NUM_OF_LETTERS * sizeof(char));
    int i = 0;
    words[i] = strtok(sentence, " ,.\n");
    while (words[i])
        words[++i] = strtok(NULL, " ,.\n");
    return words;

}

int main()
{
    char sentence[100];
    Node* root = NULL;
    root = loadFile("EN-US-Dictionary.txt");
    printf("Size = %d\n", getNumberOfNodes(root));
    printf("...............................\n");
    printf("Height = %d\n", getHeight(root));
    printf("...............................\n");


    printf("Enter a sentence:\n");
    fgets(sentence, 100, stdin);
    char** words = NULL;
    words = tokenizeSentence(sentence);
    int i = 0;
    while (words[i])
    {
        printf("%s\n", words[i++]);
    }


    return 0;
}
